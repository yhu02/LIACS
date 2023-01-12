from django.db import models
from model.enums import Type, ModelTypes
from django.db.models.signals import post_save
import logging
from django.dispatch import receiver


class Project(models.Model):
    name = models.CharField(max_length=255)
    text = models.TextField()
    description = models.TextField()
    version = models.CharField(max_length=255)
    time = models.DateTimeField(auto_now=True)


class Document(models.Model):
    file = models.FileField(upload_to="data/%Y-%m-%d")
    content = models.TextField()
    metadata = models.BinaryField()
    project = models.ForeignKey(Project, on_delete=models.CASCADE)

    def __str__(self):
        return self.content


class AvailableEntities(models.Model):
    document = models.ForeignKey(Document, on_delete=models.CASCADE)
    entity = models.CharField(max_length=255)
    entity_numbers = models.TextField()


class System(models.Model):
    project = models.ForeignKey(Project, on_delete=models.CASCADE, related_name='systems')
    name = models.CharField(max_length=255)
    description = models.CharField(max_length=255)
    version = models.CharField(max_length=255)
    application_generated = models.BooleanField(default=False)
    time = models.DateTimeField(auto_now=True)


class SelectedEntities(models.Model):
    system = models.ForeignKey(System, on_delete=models.CASCADE)
    entity = models.ForeignKey(AvailableEntities, on_delete=models.CASCADE)


class Model(models.Model):
    system = models.ForeignKey(System, on_delete=models.CASCADE, blank=True, null=True, related_name='models')
    type = models.CharField(max_length=255, choices=ModelTypes.choices(), default=ModelTypes.Component)
    title = models.CharField(max_length=255)
    raw_text = models.TextField()
    time = models.DateTimeField(auto_now=True)

    class Meta:
        unique_together = ("system", "type")

    def __str__(self):
        return self.raw_text

# Database trigger for the chatbot
@receiver(post_save, sender=Model)
def chatbot_trigger(sender, **kwargs):
    logging.warning("YIHAAA")

class ModelLayout(models.Model):
    model = models.ForeignKey(Model, on_delete=models.CASCADE)
    layout_settings = models.CharField(max_length=255)  # String for now, definitve shape tbd
    type = models.CharField(max_length=255)  # type being original or set by user or set by specific algorithm etc.
    time = models.DateTimeField(auto_now=True)

class ModelChanges(models.Model):
    model = models.ForeignKey(Model, on_delete=models.CASCADE)
    changes = models.JSONField()
    time = models.DateTimeField(auto_now=True)

class Node(models.Model):
    
    x = models.BigIntegerField(default=0)
    y = models.BigIntegerField(default=0)
    model = models.ForeignKey(Model, on_delete=models.CASCADE, related_name='nodes')
    
    @property
    def position(self):
        return {
            'x': self.x,
            'y': self.y
        }

    @property
    def type(self):
        if nt := self.get('node_type'):
            return nt
        return 'DefaultNode'

class Edge(models.Model):
    model = models.ForeignKey(Model, on_delete=models.CASCADE, related_name='edges')
    source = models.ForeignKey(Node, on_delete=models.SET_NULL, null=True, default=None, related_name='edges_out')
    destination = models.ForeignKey(Node, on_delete=models.SET_NULL, null=True, default=None, related_name='edges_in')
