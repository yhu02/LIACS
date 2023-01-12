from django.core.management.base import BaseCommand
from django.core.management import call_command
from model.models import Classifier, Property, Class
from model.enums import Type
import os


def generate_class_declaration(name):
    return '\nclass ' + name + '(models.Model):\n'


def generate_attribute_declaration(name, type):
    declaration = '   ' + name + ' = '
    if type in Type._value2member_map_:
        if type == Type.String.value:
            return declaration + 'models.CharField(max_length=255)\n'
        elif type == Type.Int.value:
            if name == 'id':
                return declaration + 'models.IntegerField(primary_key=True)\n'
            else:
                return declaration + 'models.IntegerField()\n'
        elif type == Type.Bool.value:
            return declaration + 'models.BooleanField()\n'

    return declaration + 'models.CharField(max_length=255)\n'


def generate_new_class(classifier, model_file):
    model_file.write(generate_class_declaration(classifier.name))
    for attribute in Property.objects.filter(classifier=classifier.id):
        model_file.write(generate_attribute_declaration(attribute.name, attribute.type))


def generate_classifier(classifiers, model_file):
    file_content = model_file.read()
    if file_content == '':
        model_file.write('from django.db import models\n\n')
        file_content = 'from django.db import models\n\n'

    for classifier in classifiers:
        if generate_class_declaration(classifier.name) not in file_content:
            generate_new_class(classifier, model_file)


class Command(BaseCommand):
    def handle(self, *args, **options):
        classifiers = Classifier.objects.all()
        model_file = open('application/models.py', 'w+')
        generate_classifier(classifiers, model_file)
        model_file.close()
        call_command('generate_views')
        call_command('make_and_run_migrations')
