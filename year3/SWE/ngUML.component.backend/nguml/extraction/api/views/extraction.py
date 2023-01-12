"""Extraction API"""
from extraction.models import Extraction
from rest_framework import viewsets, serializers, decorators
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from extraction.api.serializers import ExtractionSerializer


@extend_schema_view(
    list=extend_schema(
        summary="Get all extractions",
        description="Get all the data on all the extractions (name, description and version)",
    ),
    retrieve=extend_schema(summary="Get a extraction", description="Get a extraction"),
)
class ExtractionViewSet(viewsets.ReadOnlyModelViewSet):
    """
    A viewset for viewing and editing extraction instances.
    """

    serializer_class = ExtractionSerializer
    queryset = Extraction.objects.all()
    lookup_field = 'id'

    def get_queryset(self):
        return Extraction.objects.filter(requirement=self.kwargs['requirement_id'])
