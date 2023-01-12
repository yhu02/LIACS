from django.db import models
from .requirement import Requirement


class Extraction(models.Model):
    """
    Extractions are used to store NLP results for later introspection:
    - Unique name
    - Optional description
    """

    model = models.CharField(max_length=200, blank=True)
    requirement = models.ForeignKey(Requirement, on_delete=models.CASCADE, related_name="extractions")
    result = models.JSONField()

    def __str__(self):
        return '{0} {1} ({2})'.format(self.requirement.name, self.model, self.pk)
