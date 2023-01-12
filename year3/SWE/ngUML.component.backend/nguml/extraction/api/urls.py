from django.urls import path
from extraction.api.views import (
    ExtractionRootView,
    RequirementViewSet,
    BucketViewSet,
    ExtractionViewSet,
    JobViewSet,
)
from rest_framework_nested import routers

router = routers.SimpleRouter()
router.APIRootView = ExtractionRootView

router.register('job', JobViewSet, basename='job')
router.register('requirement', RequirementViewSet, basename='requirement')

buckets_router = routers.NestedSimpleRouter(router, r'requirement', lookup='requirement')
buckets_router.register(r'bucket', BucketViewSet, basename='requirement-buckets')

extractions_router = routers.NestedSimpleRouter(router, r'requirement', lookup='requirement')
extractions_router.register(r'extraction', ExtractionViewSet, basename='requirement-extractions')

app_name = 'extraction-api'
urlpatterns = router.urls + buckets_router.urls + extractions_router.urls
