import os
from ..models import *
from django.core import management


def write_to_class_model(classifier, declaration_string):
    f = open("runtime/models.py", "r")
    contents = f.readlines()
    index = contents.index('class ' + classifier.name + '(models.Model):\n') + 1
    f.close()
    contents.insert(index, declaration_string)

    f = open("runtime/models.py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def add_to_index_view(classifier_from_name, classifier_to_name):
    f = open("runtime/views/" + classifier_from_name.lower() + ".py", "r")
    contents = f.readlines()
    index = contents.index('def index(request):\n') + 5
    value = (
        '        object.properties.append({\'name\': \''
        + classifier_to_name
        + '\', \'type\': \'association\', \'value\': object.'
        + classifier_to_name.lower()
        + '.__str__'
        + '})\n'
    )
    value2 = '    properties.append({\'name\': \'' + classifier_to_name + '\'})\n'
    f.close()
    index2 = [i for i, n in enumerate(contents) if n.startswith('    context =')][0] + 1
    contents.insert(index, value)
    contents.insert(index2, value2)

    f = open("runtime/views/" + classifier_from_name.lower() + ".py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def add_to_add_view(classifier_from_name, classifier_to_name):
    f = open("runtime/views/" + classifier_from_name.lower() + ".py", "r")
    contents = f.readlines()
    for line in contents:
        if line.startswith('    ' + classifier_from_name.lower() + ' = ' + classifier_from_name + '('):
            if line.startswith('    ' + classifier_from_name.lower() + ' = ' + classifier_from_name + '()'):
                line2 = line.replace(')', classifier_to_name.lower() + '=' + classifier_to_name.lower() + ')')
            else:
                line2 = line.replace(')', ', ' + classifier_to_name.lower() + '=' + classifier_to_name.lower() + ')')
            constructor_index = contents.index(line)
            contents.remove(line)
            contents.insert(constructor_index, line2)
        if line.startswith('        add_objects = ['):
            replacement = (
                line
                + '        add_objects.append({\'name\': \''
                + classifier_to_name.lower()
                + '\', \'values\': '
                + classifier_to_name
                + '.objects.all()})\n'
            )
            repl_index = contents.index(line)
            contents.remove(line)
            contents.insert(repl_index, replacement)
        if line.startswith('        add_context = {'):
            replacement = line + '             \'objects\': add_objects,\n'
            repl_index = contents.index(line)
            contents.remove(line)
            contents.insert(repl_index, replacement)
    index = contents.index('def add(request):\n') + 2
    value = '    properties.append({\'type\': \'association\', \'name\': \'' + classifier_to_name.lower() + '\'})\n'
    index2 = contents.index('    ' + classifier_from_name.lower() + '.save()\n') - 1
    value2 = '    ' + classifier_to_name.lower() + '_id = request.POST[\'' + classifier_to_name.lower() + '\']\n'
    value2 += (
        '    '
        + classifier_to_name.lower()
        + ' = '
        + classifier_to_name
        + '.objects.get(id='
        + classifier_to_name.lower()
        + '_id)\n'
    )
    f.close()
    contents.insert(index, value)
    contents.insert(index2 + 1, value2)

    f = open("runtime/views/" + classifier_from_name.lower() + ".py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def add_to_edit_view(classifier_from_name, classifier_to_name):
    value = (
        '    properties.append({\'type\': \'association\', \'name\': \''
        + classifier_to_name.lower()
        + '\', \'value\': '
        + classifier_from_name.lower()
        + '.'
        + classifier_to_name.lower()
        + '.__str__})\n'
    )
    f = open("runtime/views/" + classifier_from_name.lower() + ".py", "r")
    contents = f.readlines()
    for line in contents:
        if line.startswith('        edit_objects = ['):
            replacement = (
                line
                + '        edit_objects.append({\'name\': \''
                + classifier_to_name.lower()
                + '\', \'values\': '
                + classifier_to_name
                + '.objects.all()})\n'
            )
            repl_index = contents.index(line)
            contents.remove(line)
            contents.insert(repl_index, replacement)
        if line.startswith('        edit_context = {'):
            replacement = line + '             \'objects\': edit_objects,\n'
            repl_index = contents.index(line)
            contents.remove(line)
            contents.insert(repl_index, replacement)
    index = contents.index('def edit(request, id):\n') + 3
    index2 = [i for i, n in enumerate(contents) if n == '    ' + classifier_from_name.lower() + '.save()\n'][1]
    value2 = (
        '    '
        + classifier_from_name.lower()
        + '.'
        + classifier_to_name.lower()
        + ' = '
        + classifier_to_name
        + '.objects.get(id=request.POST[\''
        + classifier_to_name.lower()
        + '\'])\n'
    )
    f.close()
    contents.insert(index, value)
    contents.insert(index2 + 1, value2)

    f = open("runtime/views/" + classifier_from_name.lower() + ".py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def add_to_views(relationship):
    if isinstance(relationship, Association) or isinstance(relationship, Composition):
        if (relationship.multiplicity_to == '1') and (relationship.multiplicity_from == '*'):
            classifier_to_name = relationship.classifier_to.name
            classifier_from_name = relationship.classifier_from.name
            add_to_index_view(classifier_from_name, classifier_to_name)
            add_to_add_view(classifier_from_name, classifier_to_name)
            add_to_edit_view(classifier_from_name, classifier_to_name)
        if (relationship.multiplicity_to == '*') and (relationship.multiplicity_from == '1'):
            classifier_to_name = relationship.classifier_from.name
            classifier_from_name = relationship.classifier_to.name
            add_to_index_view(classifier_from_name, classifier_to_name)
            add_to_add_view(classifier_from_name, classifier_to_name)
            add_to_edit_view(classifier_from_name, classifier_to_name)


# 注意这段代码
def add_to_models(relationship):
    if isinstance(relationship, Association) or isinstance(relationship, Composition):
        name = relationship.name
        multiplicity_from = relationship.multiplicity_from
        multiplicity_to = relationship.multiplicity_to
        end_to = relationship.end_to.lower()
        end_from = relationship.end_from.lower()

        if (multiplicity_to == '1') and (multiplicity_from == '*'):
            classifier = relationship.classifier_from
            declaration_string = (
                '    '
                + end_to
                + ' = models.ForeignKey(\''
                + relationship.classifier_to.name
                + '\', on_delete=models.CASCADE, null=True, related_name=\''
                + name.lower().replace(' ', '_')
                + '\')\n'
            )

            write_to_class_model(classifier, declaration_string)

        if (multiplicity_to == '*') and (multiplicity_from == '1'):
            classifier = relationship.classifier_to
            declaration_string = (
                '    '
                + end_from
                + ' = models.ForeignKey(\''
                + relationship.classifier_from.name
                + '\', on_delete=models.CASCADE, null=True, related_name=\''
                + name.lower().replace(' ', '_')
                + '\')\n'
            )

            write_to_class_model(classifier, declaration_string)


class RelationshipGenerator:
    def __init__(self, relationship):
        self.relationship = relationship

    def generate(self, should_migrate=True):
        add_to_models(self.relationship)
        add_to_views(self.relationship)

        if should_migrate:
            management.call_command('make_and_run_migrations')
