"""System API"""
from model.models import System
from rest_framework import viewsets, serializers, decorators
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from model.api.serializers import SystemSerializer


@extend_schema_view(
    list=extend_schema(
        summary="Get all systems",
        description="Get all the data on all the systems (name, description and version)",
    ),
    create=extend_schema(
        summary="Add a new system",
        description="Add a new system (needs name, requirements text, version and description)",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo new system",
                value={
                    "name": "Test system",
                    "text": "This is the system text. A text about classes and activities.",
                    "version": "v1",
                    "description": "This system is a test system",
                },
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    ),
    retrieve=extend_schema(summary="Get a system", description="Get a system"),
    partial_update=extend_schema(
        summary="Partially update a system", description="Modify an existing system by patching some attributes"
    ),
    update=extend_schema(
        summary="Overwrite a system", description="Modify an existing system by overwriting all attributes"
    ),
    destroy=extend_schema(summary="Delete a system", description="Delete a system"),
)
class SystemViewSet(viewsets.ModelViewSet):
    """
    A viewset for viewing and editing system instances.
    """

    serializer_class = SystemSerializer
    lookup_field = 'id'

    def get_queryset(self):
        return System.objects.filter(project=self.kwargs['project_id'])
