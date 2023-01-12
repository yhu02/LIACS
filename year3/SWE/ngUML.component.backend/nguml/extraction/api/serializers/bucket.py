from rest_framework import serializers
from extraction.models import Bucket


class BucketSerializer(serializers.ModelSerializer):
    class Meta:
        model = Bucket
        fields = '__all__'
