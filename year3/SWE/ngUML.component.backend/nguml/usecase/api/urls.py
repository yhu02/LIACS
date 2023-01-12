from rest_framework.routers import DefaultRouter
from usecase.api.views import UsecaseRootView

router = DefaultRouter()
router.APIRootView = UsecaseRootView

app_name = 'usecase-api'
urlpatterns = router.urls
