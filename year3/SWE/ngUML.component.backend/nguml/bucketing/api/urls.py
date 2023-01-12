from rest_framework.routers import DefaultRouter
from bucketing.api.views import BucketingRootView

router = DefaultRouter()
router.APIRootView = views.ExtractionRootView

app_name = 'bucketing-api'
urlpatterns = router.urls
