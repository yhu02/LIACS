from django.urls import path
from model.api.views import (
    ActivityViewSet,
    ClassViewSet,
    ModelRootView,
    ProjectViewSet,
    SystemViewSet,
    UsecaseViewSet,
    ComponentModelViewSet,
)
from rest_framework_nested import routers
from nguml.utils import NestedPatchedRouter

router = routers.SimpleRouter()
router.APIRootView = ModelRootView
router.register('project', ProjectViewSet, basename='project')

projects_router = routers.NestedSimpleRouter(router, r'project', lookup='project')
projects_router.register(r'system', SystemViewSet, basename='project-systems')


router.register(r'project/(?P<project_id>[0-9]+)/system/(?P<system_id>[0-9]+)', ComponentModelViewSet, basename='component-model')
app_name = 'model-api'

urlpatterns = (
    [
        path(
            'project/<int:project_id>/system/<int:system_id>/activity/',
            ActivityViewSet.as_view(
                {
                    'get': 'get',
                    'post': 'post',
                    'put': 'put',
                    'delete': 'delete',
                }
            ),
        ),
        path(
            'project/<int:project_id>/system/<int:system_id>/class/',
            ClassViewSet.as_view(
                {
                    'get': 'get',
                    'post': 'post',
                    'put': 'put',
                    'delete': 'delete',
                }
            ),
        ),
        path(
            'project/<int:project_id>/system/<int:system_id>/class/layout/',
            ClassViewSet.as_view(
                {
                    'post': 'layout',
                }
            ),
        ),
        path(
            'project/<int:project_id>/system/<int:system_id>/usecase/',
            UsecaseViewSet.as_view(
                {
                    'get': 'get',
                    'post': 'post',
                    'put': 'put',
                    'delete': 'delete',
                }
            ),
        )
        # TODO: IMPLEMENT RULES_API AS APP
        # path("rule/<project_id>/<system_id>", rules_api.index_without_rule_id),
        # path("rule/<project_id>/<system_id>/<rule_id>", rules_api.index),
    ]
    + projects_router.urls
    + router.urls
)
