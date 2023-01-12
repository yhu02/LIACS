from rest_framework import serializers
from model.models import Node, System, Model
from model.enums import *

class NodeSerializer(serializers.ModelSerializer):
    x = serializers.IntegerField()
    y = serializers.IntegerField()
    model = serializers.PrimaryKeyRelatedField(queryset=Model.objects.all())
    
    class Meta:
        model =  Node
        fields = '__all__'
        
    def __str__(self):
        return self.raw_text
