from rest_framework.routers import APIRootView
from .activity_model import ActivityViewSet
from .class_model import ClassViewSet
from .project import ProjectViewSet
from .system import SystemViewSet
from .usecase_model import UsecaseViewSet
from .component_model import ComponentModelViewSet


class ModelRootView(APIRootView):
    """
    Model API root view
    """

    def get_view_name(self):
        return 'Model'


#
# Model
#
