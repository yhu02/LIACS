from django.core.management.base import BaseCommand
from model.models import Classifier, Relationship, Property, Operation, OperationParameter
from model.enums import Type


def append_views_init(name):
    init_file = open('application/views/__init__.py', 'a+')
    init_file.write('\nfrom . import ' + name.lower() + '\n')
    init_file.close()


def append_urls(classifier):
    urls = (
        '    path(\''
        + classifier.name.lower()
        + '/\', views.'
        + classifier.name.lower()
        + '.index),\n'
        + '    path(\''
        + classifier.name.lower()
        + '/add\', views.'
        + classifier.name.lower()
        + '.add),\n'
        + '    path(\''
        + classifier.name.lower()
        + '/<id>\', views.'
        + classifier.name.lower()
        + '.edit),\n'
        + '    path(\''
        + classifier.name.lower()
        + '/<id>/delete\', views.'
        + classifier.name.lower()
        + '.delete),\n]\n'
    )
    urls_file = open('application/urls.py', 'r+')
    content = urls_file.read()
    index = content.index(']')
    urls_file.seek(index)
    urls_file.write(urls)
    urls_file.close()


def index(classifier, properties):
    index_string = (
        'def index(request):\n'
        + '    objects = '
        + classifier.name
        + '.objects.all()\n'
        + '    for object in objects:\n'
        + '        object.properties = []\n'
    )
    property_string = '    properties = []\n'
    for prop in properties:
        index_string += (
            '        object.properties.append({\'name\': \''
            + prop.name
            + '\', \'type\': \''
            + prop.type
            + '\', \'value\': object.'
            + prop.name
            + '})\n'
        )
        property_string += '    properties.append({\'name\': \'' + prop.name + '\'})\n'
    index_string += property_string
    index_string += (
        '    context = {\'properties\': properties, \'objects\': objects, \'object_name\': \''
        + classifier.name
        + '\', \'object_name_lower\': \''
        + classifier.name.lower()
        + '\'}\n'
        + '    return render(request, \'view_list.html\', context)'
    )

    return index_string


def add(classifier, properties):
    add_string = 'def add(request):\n' + '    properties = []\n'
    for prop in properties:
        add_string += '    properties.append({\'type\': \'' + prop.type + '\', \'name\': \'' + prop.name + '\'})\n'
    add_string += (
        '    if request.method == \'GET\':\n'
        '        context = {\n'
        '             \'properties\': properties,\n'
        '             \'object_name\': \'' + classifier.name + '\',\n'
        '             \'object_name_lower\': \'' + classifier.name.lower() + '\'\n'
        '        }\n'
        '        return render(request, \'add_object.html\', context)\n\n'
    )
    for prop in properties:
        if prop.type == 'bool':
            add_string += (
                '    if \''
                + prop.name
                + '\' in request.POST:\n'
                + '        '
                + prop.name
                + ' = True\n'
                + '    else:\n'
                + '        '
                + prop.name
                + ' = False\n'
            )
        else:
            add_string += '    ' + prop.name + ' = request.POST[\'' + prop.name + '\']\n'
    add_string += '    ' + classifier.name.lower() + ' = ' + classifier.name + '('
    for i in range(len(properties)):
        add_string += properties[i].name + '=' + properties[i].name
        if i < len(properties) - 1:
            add_string += ', '
    add_string += (
        ')\n    '
        + classifier.name.lower()
        + '.save() \n\n    return redirect(\'/application/'
        + classifier.name.lower()
        + '\')'
    )
    return add_string


def delete(classifier):
    delete_string = 'def delete(request, id):\n    '
    delete_string += (
        classifier.name.lower() + ' = ' + classifier.name + '.objects.get(id=id)\n'
        '    ' + classifier.name.lower() + '.delete()\n'
        '    return redirect(\'/application/' + classifier.name.lower() + '\')'
    )
    return delete_string


def edit(classifier, properties):
    edit_string = 'def edit(request, id):\n' + '    attributes = []\n'
    edit_string += '    ' + classifier.name.lower() + ' = ' + classifier.name + '.objects.get(id=id)\n'
    for prop in properties:
        edit_string += (
            '    attributes.append({\'type\': \''
            + prop.type
            + '\', \'name\': \''
            + prop.name
            + '\', \'value\': '
            + classifier.name.lower()
            + '.'
            + prop.name
            + '})\n'
        )
    edit_string += (
        '    if request.method == \'GET\':\n'
        '        context = {\n'
        '             \'object_id\': id,\n'
        '             \'attributes\': attributes,\n'
        '             \'object_name\': \'' + classifier.name + '\',\n'
        '             \'object_name_lower\': \'' + classifier.name.lower() + '\'\n'
        '        }\n'
        '        return render(request, \'edit_object.html\', context)\n\n'
    )
    for prop in properties:
        if prop.type == 'bool':
            edit_string += (
                '    if \''
                + prop.name
                + '\' in request.POST:\n'
                + '        '
                + prop.name
                + ' = True\n'
                + '    else:\n'
                + '        '
                + prop.name
                + ' = False\n'
            )
        else:
            edit_string += '    ' + prop.name + ' = request.POST[\'' + prop.name + '\']\n'
        edit_string += '    ' + classifier.name.lower() + '.' + prop.name + ' = ' + prop.name + '\n'
    edit_string += (
        '    '
        + classifier.name.lower()
        + '.save() \n\n    return redirect(\'/application/'
        + classifier.name.lower()
        + '\')'
    )
    return edit_string


def standard_imports():
    imports = (
        'from django.shortcuts import render\n' + 'from django.shortcuts import redirect\n' + 'from ..models import *\n'
    )
    return imports


def generate_crud_views(classifier):
    views_file = open('runtime/views/' + classifier.name.lower() + '.py', 'w+')
    views_file.write(standard_imports() + '\n\n')
    views_file.write(index(classifier, Property.objects.filter(classifier=classifier.id)) + '\n\n\n')
    views_file.write(add(classifier, Property.objects.filter(classifier=classifier.id)) + '\n\n\n')
    views_file.write(edit(classifier, Property.objects.filter(classifier=classifier.id)) + '\n\n\n')
    views_file.write(delete(classifier) + '\n')
    views_file.close()


class Command(BaseCommand):
    def handle(self, *args, **options):
        classifiers = Classifier.objects.all()
        for classifier in classifiers:
            generate_crud_views(classifier)
            append_urls(classifier)
            append_views_init(classifier.name)
