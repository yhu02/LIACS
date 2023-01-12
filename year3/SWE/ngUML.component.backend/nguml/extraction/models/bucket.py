from django.db import models
from .requirement import Requirement


class Bucket(models.Model):
    """
    Buckets are used to categorize :
    - Unique name
    - Optional description
    """

    type = models.CharField(max_length=200, blank=True)
    requirement = models.ForeignKey(Requirement, on_delete=models.CASCADE, related_name="buckets")
    text = models.TextField()

    def __str__(self):
        return '{0} {1} ({2})'.format(self.requirement.name, self.type, self.pk)
