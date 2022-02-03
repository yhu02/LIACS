from app import app
from app.routes import socketio

if __name__ == "__main__":
        socketio.run(app, debug=True)
