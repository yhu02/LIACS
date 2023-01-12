from rest_framework import serializers


class IdSerializer(serializers.Serializer):
    id = serializers.IntegerField(read_only=True)
