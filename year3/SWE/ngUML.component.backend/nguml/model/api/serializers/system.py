from rest_framework import serializers
from model.models import System


class SystemSerializer(serializers.ModelSerializer):
    diagrams = serializers.SerializerMethodField()

    class Meta:
        model = System
        fields = '__all__'

    def get_diagrams(self, obj):
        return {
            'activity': bool(obj.models.filter(type="activity")),
            'class': bool(obj.models.filter(type="class")),
            'usecase': bool(obj.models.filter(type="usecase")),
            'component': bool(obj.models.filter(type="component")),
        }
