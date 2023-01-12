import os
from ..models import Classifier, Class, Enumerator, Property, Application
from django.core import management
from shutil import copyfile
from shutil import rmtree
from .PropertyGenerator import PropertyGenerator


def index(classifier, properties):
    index_string = (
        'def index(request):\n'
        + '    properties = []\n'
        + '    objects = '
        + classifier.name
        + '.objects.all()\n'
        + '    for object in objects:\n'
        + '        object.properties = []\n'
        + '        object.properties.reverse()\n'
    )
    index_string += (
        '    context = {\'application\': __get_application(request), \'properties\': properties, \'objects\': objects, \'object_name\': \''
        + classifier.name
        + '\', \'object_name_lower\': \''
        + classifier.name.lower()
        + '\'}\n'
        + '    return render(request, \'view_list.html\', context)'
    )

    return index_string


def add(classifier, properties):
    add_string = 'def add(request):\n' + '    properties = []\n'
    add_string += (
        '    if request.method == \'GET\':\n'
        '        properties.reverse()\n'
        '        add_objects = []\n'
        '        add_context = {\n'
        '             \'application\': __get_application(request),\n'
        '             \'properties\': properties,\n'
        '             \'object_name\': \'' + classifier.name + '\',\n'
        '             \'object_name_lower\': \'' + classifier.name.lower() + '\'\n'
        '        }\n'
        '        return render(request, \'add_object.html\', add_context)\n\n'
    )
    add_string += '    ' + classifier.name.lower() + ' = ' + classifier.name + '('
    add_string += (
        ')\n    '
        + classifier.name.lower()
        + '.save()\n\n    return redirect(\'/\' + __get_application(request).name + \'/'
        + classifier.name.lower()
        + '\')'
    )
    return add_string


def delete(classifier):
    delete_string = 'def delete(request, id):\n    '
    delete_string += (
        classifier.name.lower() + ' = ' + classifier.name + '.objects.get(id=id)\n'
        '    ' + classifier.name.lower() + '.delete()\n'
        '    return redirect(\'/\' + __get_application(request).name + \'/' + classifier.name.lower() + '\')'
    )
    return delete_string


def edit(classifier, properties):
    edit_string = 'def edit(request, id):\n' + '    properties = []\n'
    edit_string += '    ' + classifier.name.lower() + ' = ' + classifier.name + '.objects.get(id=id)\n'
    edit_string += (
        '    if request.method == \'GET\':\n'
        '        properties.reverse()\n'
        '        edit_objects = []\n'
        '        edit_context = {\n'
        '             \'application\': __get_application(request),\n'
        '             \'object_id\': id,\n'
        '             \'properties\': properties,\n'
        '             \'object_name\': \'' + classifier.name + '\',\n'
        '             \'object_name_lower\': \'' + classifier.name.lower() + '\'\n'
        '        }\n'
        '        return render(request, \'edit_object.html\', edit_context)\n\n'
    )
    edit_string += (
        '    '
        + classifier.name.lower()
        + '.save()\n\n    return redirect(\'/\' + __get_application(request).name + \'/'
        + classifier.name.lower()
        + '\')'
    )
    return edit_string


def standard_imports():
    imports = (
        'from django.shortcuts import render\n'
        + 'from django.shortcuts import redirect\n'
        + 'from runtime.models import *\n'
        + 'from model.models import Application\n'
    )
    return imports


def application_getter_function():
    return (
        'def __get_application(request):\n'
        + '    application_name_lower = request.build_absolute_uri().split(\'/\')[3]\n'
        + '    application = Application.objects.get(name=application_name_lower)\n'
        + '    return application\n'
    )


def generate_crud_views(classifier):
    views_file = open('runtime/views/' + classifier.name.lower() + '.py', 'w+')
    views_file.write(standard_imports() + '\n\n')
    views_file.write(index(classifier, Property.objects.filter(classifier=classifier.id)) + '\n\n\n')
    views_file.write(add(classifier, Property.objects.filter(classifier=classifier.id)) + '\n\n\n')
    views_file.write(edit(classifier, Property.objects.filter(classifier=classifier.id)) + '\n\n\n')
    views_file.write(delete(classifier) + '\n\n\n')
    views_file.write(application_getter_function())
    views_file.close()


def add_class_definition(classifier):
    model_file = open('runtime/models.py', 'a+')
    if 'class ' + classifier.name + '(' not in model_file.read():
        if classifier.subtypeOf != '':
            basemodel = classifier.subtypeOf
        else:
            basemodel = "models.Model"
        model_file.write('\n\nclass ' + classifier.name + '(' + basemodel + '):\n    pass\n')
        model_file.close()
        return True
    else:
        return False


def add_enumerator_definition(classifier):
    enum_file = open('runtime/enums.py', 'w+')
    if 'import enum' not in enum_file.read():
        enum_file.write('import enum\n\n')
    if 'class ' + classifier.name + '(' not in enum_file.read():
        enum_file.write(
            '\nclass '
            + classifier.name
            + '(enum.Enum):\n'
            + '    @classmethod\n'
            + '    def choices(type):\n'
            + '        return tuple((i.name, i.value) for i in types)\n'
        )
        enum_file.close()
        return True
    else:
        return False


def remove_from_models(classifier):
    f = open("runtime/models.py", "r")
    contents = f.readlines()
    copylist = []
    copy = True
    f.close()
    for line in contents:
        if copy:
            if line == 'class ' + classifier.name + '(models.Model):\n':
                copy = False
            else:
                copylist.append(line)
        elif line.startswith('class '):
            copy = True
            copylist.append(line)

    f = open("runtime/models.py", "w")
    f.write("".join(copylist))
    f.close()


def remove_views(classifier):
    os.remove('runtime/views/' + classifier.name.lower() + '.py')


def remove_view_import(classifier, application):
    f = open(application.name + '/views/__init__.py', "r")
    contents = f.readlines()
    f.close()
    for line in contents:
        if line == 'from . import ' + classifier.name.lower() + '\n':
            contents.remove(line)

    f = open(application.name + '/views/__init__.py', "w")
    f.write("".join(contents))
    f.close()


def remove_urls(classifier, application):
    f = open(application.name + '/urls.py', "r")
    contents = f.readlines()
    copylist = []
    copy = True
    f.close()
    for line in contents:
        if copy:
            if line.startswith('    path(\'' + classifier.name.lower()):
                copy = False
            else:
                copylist.append(line)
        elif not line.startswith('    path(\'' + classifier.name.lower()):
            copy = True
            copylist.append(line)

    f = open(application.name + '/urls.py', "w")
    f.write("".join(copylist))
    f.close()


class ClassifierGenerator(object):
    def __init__(self, classifier, subtypeof=""):
        self.classifier = classifier
        self.classifier.subtypeOf = subtypeof

    def generate(self, should_migrate=True):
        if isinstance(self.classifier, Class):
            need_migration = add_class_definition(self.classifier)
        elif isinstance(self.classifier, Enumerator):
            need_migration = add_enumerator_definition(self.classifier)
        else:
            raise NotImplementedError

        if need_migration & should_migrate:
            management.call_command('make_and_run_migrations')

        generate_crud_views(self.classifier)

    def link_application(self, application_id):
        application = Application.objects.get(id=application_id)
        copyfile(
            'runtime/views/' + self.classifier.name.lower() + '.py',
            application.name + '/views/' + self.classifier.name.lower() + '.py',
        )
        f = open(application.name + "/views/__init__.py", "a+")
        f.write('from . import ' + self.classifier.name.lower() + '\n')
        f.close()

        urls = (
            '    path(\''
            + self.classifier.name.lower()
            + '/\', views.'
            + self.classifier.name.lower()
            + '.index),\n'
            + '    path(\''
            + self.classifier.name.lower()
            + '/add\', views.'
            + self.classifier.name.lower()
            + '.add),\n'
            + '    path(\''
            + self.classifier.name.lower()
            + '/<id>\', views.'
            + self.classifier.name.lower()
            + '.edit),\n'
            + '    path(\''
            + self.classifier.name.lower()
            + '/<id>/delete\', views.'
            + self.classifier.name.lower()
            + '.delete),\n'
        )

        f = open(application.name + "/urls.py", "r")
        contents = f.readlines()
        index = contents.index(']\n')
        f.close()
        contents.insert(index, urls)

        f = open(application.name + "/urls.py", "w")
        contents = "".join(contents)
        f.write(contents)
        f.close()

        properties = Property.objects.filter(classifier_id=self.classifier.id)
        for prop in properties:
            prop.applications.add(application)
            PropertyGenerator(prop).link_to_application(application)

    def delete(self, should_migrate=True):
        remove_from_models(self.classifier)
        remove_views(self.classifier)
        for application in self.classifier.application_set.all():
            remove_view_import(self.classifier, application)
        for application in self.classifier.application_set.all():
            remove_urls(self.classifier, application)

        if should_migrate:
            management.call_command('make_and_run_migrations')

    def unlink_application(self, application):
        remove_view_import(self.classifier, application)
        remove_urls(self.classifier, application)
        os.remove(application.name + '/views/' + self.classifier.name.lower() + '.py')
