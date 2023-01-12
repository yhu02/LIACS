"""Bucket API"""
from extraction.models import Bucket
from rest_framework import viewsets, serializers, decorators
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from extraction.api.serializers import BucketSerializer


@extend_schema_view(
    list=extend_schema(
        summary="Get all buckets",
        description="Get all the data on all the buckets (name, description and version)",
    ),
    retrieve=extend_schema(summary="Get a bucket", description="Get a bucket"),
)
class BucketViewSet(viewsets.ReadOnlyModelViewSet):
    """
    A viewset for viewing and editing bucket instances.
    """

    serializer_class = BucketSerializer
    queryset = Bucket.objects.all()
    lookup_field = 'id'

    def get_queryset(self):
        return Bucket.objects.filter(requirement=self.kwargs['requirement_id'])
