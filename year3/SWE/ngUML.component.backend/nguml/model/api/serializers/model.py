from rest_framework import serializers
from model.models import Model, System
from model.enums import *

class ModelSerializer(serializers.ModelSerializer):
    system = serializers.PrimaryKeyRelatedField(queryset=System.objects.all())
    type = serializers.ChoiceField(ModelTypes.choices())
    title = serializers.CharField()
    raw_text = serializers.CharField()
    time = serializers.DateTimeField()
    
    class Meta:
        model =  Model
        fields = '__all__'
        
    def __str__(self):
        return self.raw_text
