from rest_framework import serializers
from extraction.models import Job


class JobSerializer(serializers.ModelSerializer):
    class Meta:
        model = Job
        fields = '__all__'


class JobSpawnSerializer(serializers.ModelSerializer):
    class Meta:
        model = Job
        fields = ['requirement', 'bucket', 'model']
