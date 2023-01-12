from rest_framework.routers import DefaultRouter
from classes.api.views import ClassesRootView

router = DefaultRouter()
router.APIRootView = ClassesRootView

app_name = 'classes-api'
urlpatterns = router.urls
