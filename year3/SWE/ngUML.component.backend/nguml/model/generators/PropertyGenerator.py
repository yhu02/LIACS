import os
from ..models import Type, Class
from django.core import management


def add_property_to_index_view(property, application):
    f = open(application.name + "/views/" + property.classifier.name.lower() + ".py", "r")
    contents = f.readlines()
    index = contents.index('def index(request):\n') + 5
    value = (
        '        object.properties.append({\'name\': \''
        + property.name
        + '\', \'type\': \''
        + property.type
        + '\', \'value\': object.'
        + property.name
        + '})\n'
    )
    value2 = '    properties.append({\'name\': \'' + property.name + '\'})\n'
    f.close()
    index2 = [i for i, n in enumerate(contents) if n.startswith('    context =')][0] + 1
    contents.insert(index, value)
    contents.insert(index2, value2)

    f = open(application.name + "/views/" + property.classifier.name.lower() + ".py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def add_property_to_add_view(property, application):
    f = open(application.name + "/views/" + property.classifier.name.lower() + ".py", "r")
    contents = f.readlines()
    for line in contents:
        if line.startswith('    ' + property.classifier.name.lower() + ' = ' + property.classifier.name + '('):
            if line.startswith('    ' + property.classifier.name.lower() + ' = ' + property.classifier.name + '()'):
                line2 = line.replace(')', property.name + '=' + property.name + ')')
            else:
                line2 = line.replace(')', ', ' + property.name + '=' + property.name + ')')
            constructor_index = contents.index(line)
            contents.remove(line)
            contents.insert(constructor_index, line2)
    index = contents.index('def add(request):\n') + 2
    value = '    properties.append({\'type\': \'' + property.type + '\', \'name\': \'' + property.name + '\'})\n'
    index2 = contents.index('    ' + property.classifier.name.lower() + '.save()\n') - 1
    if property.type == 'bool':
        value2 = (
            '    if \''
            + property.name
            + '\' in request.POST:\n'
            + '        '
            + property.name
            + ' = True\n'
            + '    else:\n'
            + '        '
            + property.name
            + ' = False\n'
        )
    else:
        value2 = '    ' + property.name + ' = request.POST[\'' + property.name + '\']\n'
    f.close()
    contents.insert(index, value)
    contents.insert(index2 + 1, value2)

    f = open(application.name + "/views/" + property.classifier.name.lower() + ".py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def add_property_to_edit_view(property, application):
    value = (
        '    properties.append({\'type\': \''
        + property.type
        + '\', \'name\': \''
        + property.name
        + '\', \'value\': '
        + property.classifier.name.lower()
        + '.'
        + property.name
        + '})\n'
    )
    f = open(application.name + "/views/" + property.classifier.name.lower() + ".py", "r")
    contents = f.readlines()
    index = contents.index('def edit(request, id):\n') + 3
    index2 = [i for i, n in enumerate(contents) if n == '    ' + property.classifier.name.lower() + '.save()\n'][1]
    if property.type == 'bool':
        value2 = (
            '    if \''
            + property.name
            + '\' in request.POST:\n'
            + '        '
            + property.classifier.name.lower()
            + '.'
            + property.name
            + ' = True\n'
            + '    else:\n'
            + '        '
            + property.classifier.name.lower()
            + '.'
            + property.name
            + ' = False\n'
        )
    else:
        value2 = (
            '    '
            + property.classifier.name.lower()
            + '.'
            + property.name
            + ' = request.POST[\''
            + property.name
            + '\']\n'
        )
    f.close()
    contents.insert(index, value)
    contents.insert(index2 + 1, value2)

    f = open(application.name + "/views/" + property.classifier.name.lower() + ".py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def generate_property_declaration(name, type):
    declaration = '    ' + name + ' = '
    if type in Type._value2member_map_:
        if type == Type.String.value:
            return declaration + 'models.CharField(max_length=255, default=\'\')\n'
        elif type == Type.Int.value:
            if name == 'id':
                return declaration + 'models.IntegerField(primary_key=True, default=0)\n'
            else:
                return declaration + 'models.IntegerField(default=0)\n'
        elif type == Type.Bool.value:
            return declaration + 'models.BooleanField(default=False)\n'

    return declaration + 'models.CharField(max_length=255, default=\'\')\n'


def write_to_models_file(property, classifier):
    f = open("runtime/models.py", "r")
    contents = f.readlines()
    index = next(k for k, v in enumerate(contents) if classifier.name in v) + 1
    # index = contents.index('class ' + classifier.name + '(models.Model):\n') + 1
    value = generate_property_declaration(property.name, property.type)
    f.close()
    contents.insert(index, value)

    f = open("runtime/models.py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def delete_from_model(property):
    f = open("runtime/models.py", "r")
    contents = f.readlines()
    class_index = contents.index('class ' + property.classifier.name + '(models.Model):\n')
    start_contents = contents[:class_index]
    end_contents = contents[class_index:]
    prop_index = end_contents.index(generate_property_declaration(property.name, property.type))
    del end_contents[prop_index]
    contents = start_contents + end_contents
    f = open("runtime/models.py", "w")
    f.write("".join(contents))
    f.close()


def delete_from_view(property, application):
    classifier = property.classifier
    f = open(application.name + "/views/" + classifier.name.lower() + ".py", "r")
    contents = f.readlines()
    value = (
        '        object.properties.append({\'name\': \''
        + property.name
        + '\', \'type\': \''
        + property.type
        + '\', \'value\': object.'
        + property.name
        + '})\n'
    )
    contents.remove(value)
    value = '    properties.append({\'name\': \'' + property.name + '\'})\n'
    contents.remove(value)

    value = '    properties.append({\'type\': \'' + property.type + '\', \'name\': \'' + property.name + '\'})\n'
    contents.remove(value)
    if property.type == 'bool':
        value = (
            '    if \''
            + property.name
            + '\' in request.POST:\n'
            + '        '
            + property.name
            + ' = True\n'
            + '    else:\n'
            + '        '
            + property.name
            + ' = False\n'
        )
    else:
        value = '    ' + property.name + ' = request.POST[\'' + property.name + '\']\n'
    contents.remove(value)
    value = (
        '    properties.append({\'type\': \''
        + property.type
        + '\', \'name\': \''
        + property.name
        + '\', \'value\': '
        + property.classifier.name.lower()
        + '.'
        + property.name
        + '})\n'
    )
    contents.remove(value)
    if property.type == 'bool':
        value = (
            '    if \''
            + property.name
            + '\' in request.POST:\n'
            + '        '
            + property.classifier.name.lower()
            + '.'
            + property.name
            + ' = True\n'
            + '    else:\n'
            + '        '
            + property.classifier.name.lower()
            + '.'
            + property.name
            + ' = False\n'
        )
    else:
        value = (
            '    '
            + property.classifier.name.lower()
            + '.'
            + property.name
            + ' = request.POST[\''
            + property.name
            + '\']\n'
        )
    contents.remove(value)

    contructor_index = [
        i
        for i, n in enumerate(contents)
        if n.startswith('    ' + classifier.name.lower() + ' = ' + classifier.name + '(')
    ][0]
    contents[contructor_index] = contents[contructor_index].replace(property.name + '=' + property.name, '')
    if contents[contructor_index].__contains__(', )'):
        contents[contructor_index] = contents[contructor_index].replace(', )', ')')
    if contents[contructor_index].__contains__(', ,'):
        contents[contructor_index] = contents[contructor_index].replace(', ,', ',')
    if contents[contructor_index].__contains__('(,'):
        contents[contructor_index] = contents[contructor_index].replace('(,', '(')

    f = open(application.name + "/views/" + classifier.name.lower() + ".py", "w")
    f.write("".join(contents))
    f.close()


class PropertyGenerator:
    def __init__(self, property):
        self.property = property

    def generate(self, should_migrate=True):
        classifier = self.property.classifier
        write_to_models_file(self.property, classifier)

        if should_migrate:
            management.call_command('make_and_run_migrations')

    def link_to_application(self, application):
        add_property_to_index_view(self.property, application)
        add_property_to_add_view(self.property, application)
        add_property_to_edit_view(self.property, application)

    def unlink_from_application(self, application):
        delete_from_view(self.property, application)

    def delete(self, should_migrate=True):
        delete_from_model(self.property)
        for application in self.property.applications.all():
            delete_from_view(self.property, application)

        if should_migrate:
            management.call_command('make_and_run_migrations')
