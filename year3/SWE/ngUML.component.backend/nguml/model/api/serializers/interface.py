from rest_framework import serializers
from model.models import Interface

class InterfaceSerializer(serializers.ModelSerializer):
    
    class Meta:
        model =  Interface
        fields = '__all__'
        
