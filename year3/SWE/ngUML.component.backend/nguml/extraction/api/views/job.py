"""Job API"""
from extraction.models import Job
from rest_framework import viewsets, serializers, decorators, response
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from extraction.api.serializers import JobSerializer, JobSpawnSerializer


@extend_schema_view(
    list=extend_schema(
        summary="Get all jobs",
        description="Get all the jobs texts",
    ),
    retrieve=extend_schema(summary="Get a job", description="Get a job"),
)
class JobViewSet(viewsets.ReadOnlyModelViewSet):
    """
    A viewset for viewing and editing job instances.
    """

    serializer_class = JobSerializer
    queryset = Job.objects.all()
    lookup_field = 'id'

    @extend_schema(summary="Spawn a NLP Pipeline", description="Spawns a new NLP pipeline to process requirements")
    @decorators.action(detail=False, methods=['POST'], serializer_class=JobSpawnSerializer)
    def spawn(self, request, **kwargs):
        serializer = JobSpawnSerializer(data=request.data)
        if not serializer.is_valid():
            return serializer.Response(serializer.errors)

        requirement = None
        if id := serializer.data.get('requirement'):
            from extraction.models import Requirement

            requirement = Requirement.objects.get(pk=id)

        if id := serializer.data.get('bucket'):
            from extraction.models import Bucket

            requirement = Bucket.objects.get(pk=id)

        if not requirement:
            return response.Response('NO MODEL OR BUCKET PROVIDED!')

        from extraction.utils import enqueue_nlp

        return response.Response(enqueue_nlp(serializer.data.get('model'), requirement.text))
