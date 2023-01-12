from django.db import models
from .requirement import Requirement
from .bucket import Bucket
from .meta import NLPModel


class Job(models.Model):
    """
    Job are used to keep track of what we queued to the NLP:
    - Model
    - Unique UUID (given by Django-RQ)
    - Status
    - Origin (can be Requirement or Bucket)
    """

    model = models.CharField(
        max_length=20,
        choices=NLPModel.choices,
        default=NLPModel.CLASS,
    )
    bucket = models.ForeignKey(Bucket, blank=True, null=True, on_delete=models.CASCADE, related_name="jobs")
    requirement = models.ForeignKey(Requirement, blank=True, null=True, on_delete=models.CASCADE, related_name="jobs")
    uuid = models.UUIDField()

    def __str__(self):
        if self.requirement:
            return '{0} {1} ({2})'.format(self.requirement.name, self.type, self.pk)
        return '{0} {1} ({2})'.format(str(self.bucket), self.type, self.pk)
