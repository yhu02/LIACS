from django.db import models
from model.models import System


class Rule(models.Model):
    system = models.ForeignKey(System, on_delete=models.CASCADE, blank=True, null=True)
    original_input = models.TextField()
