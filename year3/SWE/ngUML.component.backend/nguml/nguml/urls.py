"""backend URL Configuration.
"""
from django.conf import settings
from django.conf.urls import include
from django.urls import path, re_path
from django.views.decorators.csrf import csrf_exempt
from django.views.static import serve
from django.views.generic import TemplateView
from schema_graph.views import Schema
from nguml.admin import admin_site
from nguml.api.views import APIRootView, StatusView
from drf_spectacular.views import SpectacularAPIView, SpectacularRedocView, SpectacularSwaggerView

#
# Django Admin URLs
#
_patterns = [
    # Admin
    path("admin/background-tasks/", include("django_rq.urls")),
    path("admin/", admin_site.urls),
]

#
# Default API URLs
#
_patterns += [
    path('api/', APIRootView.as_view(), name='api-root'),
    path('api/status/', StatusView.as_view(), name='api-status'),
]

#
# Debug toolbar, if necessary
#
if settings.DEBUG:
    import debug_toolbar

    _patterns += [
        path("__debug__/", include(debug_toolbar.urls)),
    ]

#
# Django Applications that have API URLs
#
for app in settings.NGUML_APPLICATIONS:
    if app in ['extraction', 'model']:
        _patterns += [path("{0}/".format(app), include("{0}.urls".format(app)))]
        _patterns += [path("api/{0}/".format(app), include("{0}.api.urls".format(app)))]

#
# Schema exploration URL
#
_patterns += (
    [path("dev/database/", Schema.as_view())] if settings.DEBUG else []
)  # Only add if project is in development mode

#
# OpenAPI / DRF Spectacular
#
_patterns += [
    path('api/schema/', SpectacularAPIView.as_view(), name='schema'),
]

_patterns += (
    [
        path('dev/schema/', SpectacularAPIView.as_view(), name='dev-schema'),
        path('dev/schema/swagger-ui/', SpectacularSwaggerView.as_view(url_name='schema'), name='swagger-ui'),
        path('dev/schema/redoc/', SpectacularRedocView.as_view(url_name='schema'), name='redoc'),
    ]
    if settings.DEBUG
    else []
)  # Only add if project is in development mode

# Prepend BASE_PATH
urlpatterns = [path("{}".format(settings.BASE_PATH), include(_patterns))]

handler500 = "nguml.views.server_error"
