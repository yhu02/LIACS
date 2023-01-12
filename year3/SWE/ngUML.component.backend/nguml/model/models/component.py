""" 
The class definitions for all classes used by the component modeler.
"""

from django.db import models
from model.models.base import Node, Edge, Model
from django.utils.translation import gettext_lazy as _
from model.enums import PortType
from django.core.validators import MaxValueValidator, MinValueValidator

# Using the specification as detailed in
# https://www.omg.org/spec/UML/2.5.1/PDF
# Component-specific models


class Component(Node):
    """
    A class definition for a component. 
    """
    # node may be a parent component
    parent = models.ForeignKey(Node, null=True, blank=True, on_delete=models.CASCADE, related_name='+')
    name = models.CharField(max_length=255, null=True, blank=True)
    
    # This is necessary for the styling 
    node_type = models.CharField(max_length=255, null=True, blank=True)
    class_name = models.CharField(max_length=255, null=True, blank=True)
    background_color = models.CharField(max_length=255, null=True, blank=True, default='rgba(255,255,255,0.2)')
    width = models.IntegerField(null=False, default = 200)
    height = models.IntegerField(null=False, default = 200)
    border = models.CharField(max_length=255, null=False, blank=True)
    
    # This makes sure subcomponents can't go outside of parent
    extent = models.CharField(max_length=255, null=False, default='parent', blank=True)

    def __str__(self):
        if(self.name):
            return self.name
        else:
            return "unknown"

class Port(models.Model):
    """
    Class definition for ports.
    Not currently used.
    """
    type = models.CharField(
        max_length=255, choices=PortType.choices(), default=PortType.SimplePort)
    name = models.CharField(max_length=255, null=True, blank=True)

    # Set up a maximum of 6 anchor points in component to attach ports,
    # TO DO add full_clean method to trigger validators
    position = models.IntegerField(null=False, default=0, validators=[
        MaxValueValidator(6),
        MinValueValidator(0)
    ])
    component = models.ForeignKey(Component, on_delete=models.CASCADE)

    # For the interfaces
    provided = models.ManyToManyField('Interface', blank=True, related_name='provided')
    required = models.ManyToManyField('Interface', blank=True, related_name='required')

    class Meta:
        unique_together = ("component", "position")

    def __str__(self):
        if(self.name):
            return self.name
        else:
            return "unknown"

class Interface(Node):
    """
    Class definition for interfaces.
    Not currently used.
    """
    parent = models.ForeignKey(Node,null=True, blank=True, on_delete=models.CASCADE, related_name='+')
    name = models.CharField(max_length=255, null=True, blank=True)

    def __str__(self):
        if(self.name):
            return self.name
        else:
            return "unknown"
        
class ExternalView():
    node = models.ForeignKey(Component, on_delete=models.CASCADE)

class Relation(models.Model):
    """
    Class definition for relations.
    An instance can be a dependency or a relation.
    """
    # Source and target of the relation, determines the direction
    source = models.ForeignKey(Node, null=True, blank=True, related_name='source_node', on_delete=models.CASCADE)
    target = models.ForeignKey(Node, null=True, blank=True, related_name='target_node', on_delete=models.CASCADE)
    
    # The name of the relation
    label = models.CharField(max_length=255, null=True)
   
    # These things are all for styling of relation
    # This also makes it either a dependency or a relation
    type = models.CharField(max_length=255, null=False, default='step')
    animated = models.BooleanField()
    start_width = models.IntegerField(null=True, default=0)
    start_height = models.IntegerField(null=True, default=0)
    end_width = models.IntegerField(null=True, default=0)
    end_height = models.IntegerField(null=True, default=0)
    marker_end_type = models.CharField(max_length=255, null=True, blank=True)
    marker_start_type = models.CharField(max_length=255, null=True, blank=True)
    
    # A bool that is used to easily verify if it's a dependency or a relation
    dependency = models.BooleanField(null=True, blank=False)
    
    class Meta:
        constraints = [
            models.UniqueConstraint(
                fields=['source', 'target'], name='unique_source_target_combination'
            )
        ]
    
    def __str__(self):
        if(self.label):
            return self.label
        else:
            return "unknown"