from rest_framework.routers import DefaultRouter
from activity.api.views import ActivityRootView

router = DefaultRouter()
router.APIRootView = ActivityRootView

app_name = 'activity-api'
urlpatterns = router.urls
