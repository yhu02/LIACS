from rest_framework import serializers
from model.models import Port

class PortSerializer(serializers.ModelSerializer):

    class Meta:
        model =  Port
        fields = '__all__'
        
