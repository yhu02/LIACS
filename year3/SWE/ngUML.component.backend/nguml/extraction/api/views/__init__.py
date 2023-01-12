from rest_framework.routers import APIRootView
from .requirement import RequirementViewSet
from .bucket import BucketViewSet
from .extraction import ExtractionViewSet
from .job import JobViewSet


class ExtractionRootView(APIRootView):
    """
    Extraction API root view
    """

    def get_view_name(self):
        return 'Extraction'


#
# Extraction
#
