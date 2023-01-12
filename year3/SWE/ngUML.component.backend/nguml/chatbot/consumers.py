import json

from channels.generic.websocket import WebsocketConsumer
from django.urls import re_path


class ChatConsumer(WebsocketConsumer):
    def connect(self):
        self.accept()

    def disconnect(self, close_code):
        pass

    # this function receives messages on an opened websocket stream and calls chatbot_main to get teh appropriate engine
    # and sends that back through the socket
    def receive(self, text_data):
        from chatbot.utils import chatbot_main

        text_data_json = json.loads(text_data)
        message = text_data_json['message']
        answer = chatbot_main(message)
        self.send(text_data=json.dumps(answer))


websocket_urlpatterns = [
    re_path(r'ws/chatbot/(?P<session_id>\w+)/$', ChatConsumer.as_asgi()),
]
