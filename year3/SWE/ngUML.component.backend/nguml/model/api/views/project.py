"""Project API"""
from model.models import Project
from rest_framework import viewsets, serializers, decorators
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from model.api.serializers import ProjectSerializer


@extend_schema_view(
    list=extend_schema(
        summary="Get all projects",
        description="Get all the data on all the projects (name, description and version)",
    ),
    create=extend_schema(
        summary="Add a new project",
        description="Add a new project (needs name, requirements text, version and description)",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo new project",
                value={
                    "name": "Test project",
                    "text": "This is the project text. A text about classes and activities.",
                    "version": "v1",
                    "description": "This project is a test project",
                },
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    ),
    retrieve=extend_schema(summary="Get a project", description="Get a project"),
    partial_update=extend_schema(
        summary="Partially update a project", description="Modify an existing project by patching some attributes"
    ),
    update=extend_schema(
        summary="Overwrite a project", description="Modify an existing project by overwriting all attributes"
    ),
    destroy=extend_schema(summary="Delete a project", description="Delete a project"),
)
class ProjectViewSet(viewsets.ModelViewSet):
    """
    A viewset for viewing and editing project instances.
    """

    serializer_class = ProjectSerializer
    queryset = Project.objects.all()
    lookup_field = 'id'
