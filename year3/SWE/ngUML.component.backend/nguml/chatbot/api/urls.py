from rest_framework.routers import DefaultRouter
from chatbot.api.views import ChatbotRootView

router = DefaultRouter()
router.APIRootView = ChatbotRootView
app_name = 'chatbot-api'
urlpatterns = router.urls
