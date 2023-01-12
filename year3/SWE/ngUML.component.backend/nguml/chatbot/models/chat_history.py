from django.db import models

# Create your models here.

# Model fot the chat history of the chatbot
class Chat_history(models.Model):
    user_message = models.TextField()
    answer = models.TextField()
    pattern_tag = models.TextField()
    time = models.TimeField(auto_now_add=True)
