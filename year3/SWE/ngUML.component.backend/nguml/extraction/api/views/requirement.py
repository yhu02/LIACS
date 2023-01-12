"""Requirement API"""
from extraction.models import Requirement
from rest_framework import viewsets, serializers, decorators
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from extraction.api.serializers import RequirementSerializer


@extend_schema_view(
    list=extend_schema(
        summary="Get all requirements",
        description="Get all the requirements texts",
    ),
    create=extend_schema(
        summary="Add a new requirement",
        description="Add a new requirement",
    ),
    retrieve=extend_schema(summary="Get a requirement", description="Get a requirement"),
    partial_update=extend_schema(
        summary="Partially update a requirement",
        description="Modify an existing requirement by patching some attributes",
    ),
    update=extend_schema(
        summary="Overwrite a requirement", description="Modify an existing requirement by overwriting all attributes"
    ),
    destroy=extend_schema(summary="Delete a requirement", description="Delete a requirement"),
)
class RequirementViewSet(viewsets.ModelViewSet):
    """
    A viewset for viewing and editing requirement instances.
    """

    serializer_class = RequirementSerializer
    queryset = Requirement.objects.all()
    lookup_field = 'id'
