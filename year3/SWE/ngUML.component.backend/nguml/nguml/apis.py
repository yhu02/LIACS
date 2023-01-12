from django.urls import path
from model.views import apis as model_api
from rules import api as rules_api
from drf_spectacular.views import SpectacularAPIView, SpectacularSwaggerView

urlpatterns = [
    path("project/", model_api.project.index_without_project_id),
    path("project/<project_id>", model_api.project.index),
    path("system/<project_id>", model_api.system.index_without_id),
    path("system/<project_id>/<system_id>", model_api.system.index),
    path("model/class/<project_id>/<system_id>", model_api.class_model.index),
    path("model/activity/<project_id>/<system_id>", model_api.activity_model.index),
    path('model/usecase/<project_id>/<system_id>', model_api.usecase_model.index),
    path("rule/<project_id>/<system_id>", rules_api.index_without_rule_id),
    path("rule/<project_id>/<system_id>/<rule_id>", rules_api.index),
    path(
        "explorer/",
        SpectacularSwaggerView.as_view(template_name="api_view.html", url_name="schema"),
        name="swagger-ui",
    ),
    path('openapi/', SpectacularAPIView.as_view(), name='schema'),
]
