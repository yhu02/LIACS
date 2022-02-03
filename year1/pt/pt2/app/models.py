from app import db, login
from werkzeug.security import generate_password_hash, check_password_hash
from flask_login import UserMixin
from dataclasses import dataclass
from datetime import datetime

#Members in group association table
members = db.Table('association',
    db.Column('user_id', db.Integer, db.ForeignKey('user.id')),
    db.Column('group_id', db.Integer, db.ForeignKey('group.id'))
)
#Rest of the code is straightforward
@dataclass
class User(db.Model, UserMixin):
    __tablename__ = 'user'

    id: int
    username:str
    email:str
    password_hash:str

    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(64), index=True, unique=True)
    email = db.Column(db.String(120), index=True, unique=True)
    password_hash = db.Column(db.String(128))
    groups = db.relationship(
        "Group",
        secondary=members,
        backref=db.backref('users', lazy=True))

    def set_password(self, password):
        self.password_hash = generate_password_hash(password)

    def check_password(self, password):
        return check_password_hash(self.password_hash, password)

@dataclass
class Post(db.Model):
    __tablename__ = 'post'

    id:int
    body:str
    timestamp:datetime
    to_id:int
    from_id:str
    group_id:str
    from_username:str

    id = db.Column(db.Integer, primary_key=True)
    body = db.Column(db.String(140))
    timestamp = db.Column(db.DateTime, index=True, default=datetime.utcnow)
    from_id =  db.Column(db.Integer)
    from_username = db.Column(db.String(64))
    to_id =  db.Column(db.Integer)
    group_id = db.Column(db.Integer)

@dataclass
class Group(db.Model):
    __tablename__ = 'group'

    id:int
    group_name:str
    
    id = db.Column(db.Integer, primary_key=True)
    group_name = db.Column(db.String(140), unique=True)

@login.user_loader
def load_user(id):
    return User.query.get(int(id))