from rest_framework import serializers
from model.models import Node, Port, Component, Model

class ComponentSerializer(serializers.ModelSerializer):
    parent = serializers.PrimaryKeyRelatedField(queryset=Node.objects.all())
    name = serializers.CharField()
    x = serializers.IntegerField()
    y = serializers.IntegerField()
    #model_id = serializers.PrimaryKeyRelatedField(Model, related_name='model_id', on_delete=models.CASCADE)

    class Meta:
        model =  Component
        fields = '__all__'
        
    def __str__(self):
        return self.name

    def get_nodes(self, obj):
        return obj.nodes.all()

    def get_edges(self, obj):
        return obj.edges.all()