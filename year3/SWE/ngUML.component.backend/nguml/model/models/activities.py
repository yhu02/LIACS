from django.db import models
from model.enums import Type, ModelTypes
from model.models.base import Model
from django.db.models.signals import post_save
import logging
from django.dispatch import receiver

#################################################
#               Activity Model                  #
#################################################

# TODO
# - implement Operation
# - implement CallOperationAction


class Activity(models.Model):
    activity_model = models.ForeignKey(Model, on_delete=models.CASCADE, blank=True, null=True)
    name = models.CharField(max_length=255, unique=False)
    precondition = models.CharField(max_length=255, unique=False)
    postcondition = models.CharField(max_length=255, unique=False)
    isReadOnly = models.BooleanField(default=False)
    isSingleExecution = models.BooleanField(default=False)
    callBehaviorAction = models.ForeignKey(
        "Action",
        on_delete=models.CASCADE,
        related_name="callBehaviorAction",
        blank=True,
        null=True,
    )


class ActivityNode(models.Model):
    """ActivityNode to model the Activity Node."""

    name = models.CharField(max_length=255, unique=False)
    description = models.CharField(max_length=255, unique=False, null=True, blank=True)
    xpos = models.IntegerField(null=True, blank=True)
    ypos = models.IntegerField(null=True, blank=True)
    activity = models.ForeignKey(Activity, on_delete=models.CASCADE, related_name="activitynode_activity")


class ActivityEdge(models.Model):
    guard = models.CharField(max_length=255, unique=False, null=True, blank=True)
    weight = models.CharField(max_length=255, unique=False, null=True, blank=True)

    # In the class diagram there are multiple incoming and outgoing activities
    incoming_node = models.ForeignKey(ActivityNode, on_delete=models.CASCADE, related_name="incoming_node")
    outgoing_node = models.ForeignKey(ActivityNode, on_delete=models.CASCADE, related_name="outgoing_node")
    activity = models.ForeignKey(Activity, on_delete=models.CASCADE, related_name="activityedge_activity")


class ControlNode(ActivityNode):
    # Not instantiable
    pass


class InitialNode(ControlNode):
    def parentId(self):
        """Return the parent id of the node."""
        return self.controlnode_ptr_id


class FinalNode(ControlNode):
    # Not instantiable
    pass


class FlowFinalNode(FinalNode):
    def parentId(self):
        """Return the parent id of the node."""
        return self.finalnode_ptr_id


class ActivityFinalNode(FinalNode):
    def parentId(self):
        """Return the parent id of the node."""

        return self.finalnode_ptr_id


class ForkNode(ControlNode):
    def parentId(self):
        """Return the parent id of the node."""
        return self.controlnode_ptr_id


class MergeNode(ControlNode):
    def parentId(self):
        """Return the parent id of the node."""
        return self.controlnode_ptr_id


class JoinNode(ControlNode):
    joinSpec = models.CharField(max_length=255, unique=False)
    isCombineDuplicate = models.BooleanField(default=True)

    def parentId(self):
        """Return the parent id of the node."""
        return self.controlnode_ptr_id


class DecisionNode(ControlNode):
    # Should be Behavior needs to be implemented
    decisionInput = models.CharField(max_length=255, unique=False)
    # Should be objectFlow needs to be implemented
    decisionInputFlow = models.CharField(max_length=255, unique=False)

    def parentId(self):
        """Return the parent id of the node."""
        return self.controlnode_ptr_id


class ExecutableNode(ActivityNode):
    pass


class Action(ExecutableNode):
    localPrecondition = models.CharField(max_length=255, unique=False)
    localPostcondition = models.CharField(max_length=255, unique=False)
    body = models.CharField(max_length=255, unique=False)

    def parentId(self):
        """Return the parent id of the node."""
        return self.executablenode_ptr_id


class OperationAction(models.Model):
    """Class to model the Operation class."""

    # Called OperationAction, as Operation already existed in the class model
    name = models.CharField(max_length=255, unique=False)
    type = models.CharField(max_length=20, choices=Type.choices(), default=None)
    implementation = models.TextField(default="raise NotImplementedError")
    callOperationAction = models.ForeignKey(
        Action, on_delete=models.CASCADE, related_name="action", blank=True, null=True
    )
