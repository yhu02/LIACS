from rest_framework.routers import APIRootView


class ChatbotRootView(APIRootView):
    """
    Chatbot API root view
    """

    def get_view_name(self):
        return 'Chatbot'


#
# Chatbot
#
