
from app import app, db
from flask import render_template, flash, redirect, url_for, request, session, Flask, jsonify
from flask_login import current_user, login_user, logout_user, login_required
from app.models import User, Post, Group
from app.forms import LoginForm, RegistrationForm
from sqlalchemy import desc, or_, and_
from flask_socketio import SocketIO, emit, join_room, disconnect
import json

async_mode = None

socketio = SocketIO(app, async_mode=async_mode)

session_id_to = None       #Last clicked socketid
session_id_from = None     #Last known socketid
active_users = {}          #Map active user ids to username
session_user_map = {}      #Map user id to socket id

@app.route('/')

@app.route('/home', methods=['GET'])
def home():
    return render_template('home.html', title='Home')

#Login page
@app.route('/login', methods=['GET', 'POST'])
def login():
    form = LoginForm()
    
    if form.validate_on_submit():
        user = User.query.filter_by(username=form.username.data).first()
        if user is None or not user.check_password(form.password.data):
            flash('Invalid username or password')
            return redirect(url_for('login'))
        if user.id in active_users:
            flash('User already logged in')
            return redirect(url_for('login'))
        login_user(user)
        return redirect(url_for('chat'))
    return render_template('login.html', title='Sign In', form=form)


@app.route("/logout")
@login_required
def logout():
    logout_user()
    return redirect(url_for('login'))

#Register page
@app.route('/register', methods=['GET', 'POST'])
def register():
    form = RegistrationForm()
    if form.validate_on_submit():
        user = User(username=form.username.data, email=form.email.data)
        user.set_password(form.password.data)
        db.session.add(user)
        db.session.commit()
        flash('Congratulations, you are now a registered user!')
        return redirect(url_for('login'))
    return render_template('register.html', title='Register', form=form)

@app.route('/account', methods=['GET'])
@login_required
def account():
    return render_template('account.html', title='Account')

@app.shell_context_processor
def make_shell_context():
    return {'db': db, 'User': User, 'Post': Post}

@app.route('/chat', methods=['GET','POST'])
@login_required
def chat():
    return render_template('chat.html', async_mode=socketio.async_mode)


#On ping
@socketio.event
def my_ping():
    user = User.query.filter_by(id=session['_user_id']).first()
    groups = user.groups
    active_groups = {}

    #List all groups that the user is in
    for group in user.groups:
        members = []
        for user in group.users:
            members.append(user.username)
        active_groups[group.group_name] = members

    emit('my_pong', {'active_users':active_users, 'active_groups':active_groups})

#On connect socket
@socketio.event
def connect():
    if current_user.id in active_users:
        print('User already logged in')
        return redirect(url_for('login'))
        
    active_users[current_user.id] = current_user.username #Add user to active users

    user = User.query.filter_by(username=current_user.username).first()
    groups = user.groups
    active_groups = []

    #List all previously made groups that the user is in
    for i in user.groups:
        active_groups.append(i.group_name)

    emit('connecting', {'current_user':current_user.username, 'current_user_id':current_user.id, 'active_groups':active_groups})


#On disconnect socket
@socketio.event
def disconnect():
    
    #Remove user from active user list
    left_user=User.query.filter_by(id=session['_user_id']).first()
    del active_users[left_user.id]
    del session_user_map[session['_user_id']]

    #Close chats of other people in open chat with disconnected user
    emit('chat_close', {'left_user':left_user.username}, room=session_id_to)
    emit('chat_close', {'left_user':left_user.username}, room=session_id_from)

#Send message to chat, insert into database and push to other users and 
@socketio.event
def add_message(msg):

    jsonData = msg['message']
    # if no user selected
    if(jsonData['to'] == None):
        return

    class_name = jsonData['class_name']
    from_user = jsonData['from_user']
    to   = jsonData['to']
    body = jsonData['body']
    from_id = User.query.filter_by(username=from_user).first().id
    
    if(class_name == 'user'):

        to_id = User.query.filter_by(username=to).first().id

        post = Post(body=body, to_id=to_id, from_id=from_id, from_username=from_user)
        
        db.session.add(post)
        db.session.commit()

        message = jsonify(Post.query.order_by(desc(Post.timestamp)).limit(1).all()).json

        session_id_from = session_user_map[str(message[0]['from_id'])]
        session_id_to = session_user_map[str(message[0]['to_id'])]
        
        #Push to both users
        emit('push_message', {'messages':message, 'from_user':from_user, 'to':to, 'class_name':class_name}, room=session_id_to)
        emit('push_message', {'messages':message, 'from_user':from_user, 'to':to, 'class_name':class_name}, room=session_id_from)
    elif(class_name == 'group'):

        to_id = Group.query.filter_by(group_name=to).first().id
        
        post = Post(body=body, group_id=to_id, from_username=from_user, from_id=from_id)
        
        db.session.add(post)
        db.session.commit()

        message = jsonify(Post.query.order_by(desc(Post.timestamp)).limit(1).all()).json
        #Push to room
        emit('push_message', {'messages':message, 'from_user':from_user, 'to':to, 'class_name':class_name}, room=to)

#Return chat history
@socketio.event
def get_chat(msg):
    global session_id_to
    global session_id_from

    jsonData = msg['data']
    class_name = jsonData['class_name']
    from_user = jsonData['from_user']
    to  = jsonData['to']
    
    if(class_name == 'user'):

        to_id = User.query.filter_by(username=jsonData['to']).first().id
        from_id = User.query.filter_by(username=jsonData['from_user']).first().id
        #Assign last clicked user needed to close chat of other user
        session_id_to = session_user_map[str(to_id)]
        session_id_from = session_user_map[str(from_id)]
        
        chat_history = Post.query.filter(or_(and_(Post.to_id==to_id, Post.from_id==from_id),and_(Post.to_id==from_id, Post.from_id==to_id))).all()

    elif(class_name == 'group'):
 
        to_id = Group.query.filter_by(group_name=jsonData['to']).first().id
        chat_history = Post.query.filter(Post.group_id == to_id).all() 
      
    #Format data
    chat_history_json_list = jsonify(chat_history).json 
    chat_history_json = json.dumps(chat_history_json_list) 

    emit('push_message', {'messages':chat_history_json_list, 'from_user':from_user, 'to':to, 'class_name':class_name})

#Map user id to socket id
@socketio.event
def session_map(msg):
    session_user_map[session['_user_id']] = msg['socket_id']

#Create group and insert into database
@socketio.event
def create_group(msg):
    group_members = msg['group_members']
    group_name    = msg['group_name']

    group = Group(group_name=group_name)

    db.session.add(group)
    db.session.commit()

    #Let every member join the room
    for i in group_members:
        user = User.query.filter_by(username=i).first()

        socket_id = session_user_map[str(user.id)]
        
        group.users.append(user)
        db.session.commit()
        
        emit('join_request', {'group_name':group_name}, room=socket_id)

#Join room
@socketio.event
def join(message):
    join_room(message['room'])