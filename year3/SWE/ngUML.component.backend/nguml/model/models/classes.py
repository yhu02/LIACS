from django.db import models
from model.enums import Type, ModelTypes
from django.db.models.signals import post_save
from model.models.base import Model
import logging
from django.dispatch import receiver


#################################################
#                 Class Model                   #
#################################################


class Classifier(models.Model):
    class_model = models.ForeignKey(Model, on_delete=models.CASCADE)
    name = models.CharField(max_length=255)
    is_abstract = models.BooleanField(default=False)
    xpos = models.IntegerField(null=True, blank=True)
    ypos = models.IntegerField(null=True, blank=True)

    class Meta:
        unique_together = ("name", "class_model")

    def __str__(self):
        return self.name


class Category(models.Model):
    name = models.CharField(max_length=255)


class Application(models.Model):
    name = models.CharField(max_length=255, unique=True)
    classifiers = models.ManyToManyField(Classifier)
    categories = models.ManyToManyField(Category)


class Page(Application):
    type = models.CharField(max_length=255)
    query = models.CharField(max_length=255)
    create = models.BooleanField(default=False)
    category = models.ManyToManyField(Category)
    data_paths = models.TextField()


class Section(Application):
    classes = models.CharField(max_length=255)
    sorting = models.CharField(max_length=255)
    content = models.TextField()
    linked_page = models.ForeignKey(Page, on_delete=models.CASCADE, related_name="linked_page")


class Class(Classifier):
    pass


class Enumerator(Classifier):
    pass


class Property(models.Model):
    name = models.CharField(max_length=255)
    type = models.CharField(max_length=20, choices=Type.choices())
    inherited = models.BooleanField(default=False)
    applications = models.ManyToManyField(Application)
    classifier = models.ForeignKey(Classifier, on_delete=models.CASCADE, related_name="classifier")

    def __str__(self):
        return self.name


class Operation(models.Model):
    name = models.CharField(max_length=255)
    type = models.CharField(max_length=20, choices=Type.choices())
    implementation = models.TextField(default="raise NotImplementedError")
    classifier = models.ForeignKey(Classifier, on_delete=models.CASCADE, related_name="operation_classifier")

    def __str__(self):
        return self.name


class OperationParameter(models.Model):
    name = models.CharField(max_length=255)
    type = models.CharField(max_length=20, choices=Type.choices())
    operation = models.ForeignKey(Operation, on_delete=models.CASCADE, related_name="operation")

    def __str__(self):
        return self.name


class Relationship(models.Model):
    class_model = models.ForeignKey(Model, on_delete=models.CASCADE)
    name = models.CharField(max_length=255, default="")
    classifier_to = models.ForeignKey(Classifier, on_delete=models.CASCADE, related_name="classifier_to")
    classifier_from = models.ForeignKey(Classifier, on_delete=models.CASCADE, related_name="classifier_from")

    def __str__(self):
        return self.name


class Association(Relationship):
    multiplicity_to = models.CharField(max_length=255)
    multiplicity_from = models.CharField(max_length=255)
    end_from = models.CharField(max_length=255, default="")
    end_to = models.CharField(max_length=255, default="")

    def __str__(self):
        return self.name


class DirectedRelationship(Relationship):
    class Meta:
        abstract = True


class Generalization(DirectedRelationship):
    pass


class Composition(Relationship):
    multiplicity_to = models.CharField(max_length=255)
    multiplicity_from = models.CharField(max_length=255)
    end_from = models.CharField(max_length=255, default="")
    end_to = models.CharField(max_length=255, default="")

    def __str__(self):
        return self.name
