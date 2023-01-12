from rest_framework import serializers
from extraction.models import Extraction


class ExtractionSerializer(serializers.ModelSerializer):
    class Meta:
        model = Extraction
        fields = '__all__'
