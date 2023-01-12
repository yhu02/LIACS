from django.db import models
from model.enums import Type, ModelTypes
from django.db.models.signals import post_save
from model.models.base import Model
import logging
from django.dispatch import receiver

#################################################
#               UseCase Model                  #
#################################################


class UseCaseClassifier(models.Model):
    usecase_model = models.ForeignKey(Model, on_delete=models.CASCADE, blank=True, null=True)
    name = models.CharField(max_length=255, unique=False)
    xpos = models.IntegerField(null=True, blank=True)
    ypos = models.IntegerField(null=True, blank=True)
    isReadOnly = models.BooleanField(default=False)
    isSingleExecution = models.BooleanField(default=False)


class UseCase(UseCaseClassifier):
    def parentId(self):
        """Return the parent id of the node."""
        return self.usecaseclassifier_ptr_id


class Precondition(models.Model):
    name = models.CharField(max_length=255)
    type = models.CharField(max_length=20, choices=Type.choices())
    usecase = models.ForeignKey(UseCase, on_delete=models.CASCADE, related_name="usecase_precondition")

    def __str__(self):
        return self.name


class Postcondition(models.Model):
    name = models.CharField(max_length=255)
    type = models.CharField(max_length=20, choices=Type.choices())
    usecase = models.ForeignKey(UseCase, on_delete=models.CASCADE, related_name="usecase_postcondition")

    def __str__(self):
        return self.name


class Trigger(models.Model):
    name = models.CharField(max_length=255)
    type = models.CharField(max_length=20, choices=Type.choices())
    usecase = models.ForeignKey(UseCase, on_delete=models.CASCADE, related_name="usecase_trigger")

    def __str__(self):
        return self.name


class Scenario(models.Model):
    name = models.CharField(max_length=255)
    type = models.CharField(max_length=20, choices=Type.choices())
    usecase = models.ForeignKey(UseCase, on_delete=models.CASCADE, related_name="usecase_scenario")

    def __str__(self):
        return self.name


class Actor(UseCaseClassifier):
    def parentId(self):
        """Return the parent id of the node."""
        return self.usecaseclassifier_ptr_id


class UseCaseSystem(models.Model):
    name = models.CharField(max_length=255, unique=False)
    description = models.CharField(max_length=255, unique=False, null=True, blank=True)
    xpos = models.IntegerField(null=True, blank=True)
    ypos = models.IntegerField(null=True, blank=True)
    usecase = models.ForeignKey(Model, on_delete=models.CASCADE, related_name="UseCaseSystem_Usecase")


class UseCaseRelationship(models.Model):
    from_classifier = models.ForeignKey(UseCaseClassifier, on_delete=models.CASCADE, related_name="incoming_classifier")
    to_classifier = models.ForeignKey(UseCaseClassifier, on_delete=models.CASCADE, related_name="outgoing_classifier")
    usecase = models.ForeignKey(Model, on_delete=models.CASCADE, related_name="relationship_usecase")


class Interaction(UseCaseRelationship):
    pass


class Extension(UseCaseRelationship):
    pass


class Inclusion(UseCaseRelationship):
    pass
