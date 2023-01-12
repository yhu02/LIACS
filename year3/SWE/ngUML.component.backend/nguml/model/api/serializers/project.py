from rest_framework import serializers
from model.models import Project
from .system import SystemSerializer


class ProjectSerializer(serializers.ModelSerializer):
    systems = SystemSerializer(many=True, read_only=True)

    class Meta:
        model = Project
        fields = '__all__'
