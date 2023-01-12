import os


def generate_operation_declaration(name, implementation):
    declaration = '    def ' + name + '(self):\n'
    for line in implementation.splitlines():
        declaration += '        ' + line + '\n'

    return declaration


def delete_from_model(operation):
    f = open("runtime/models.py", "r")
    contents = f.readlines()
    class_index = contents.index('class ' + operation.classifier.name + '(models.Model):\n')
    start_contents = contents[:class_index]
    end_contents = contents[class_index:]
    for line in generate_operation_declaration(operation.name, operation.implementation).splitlines():
        operation_index = end_contents.index(line + '\n')
        del end_contents[operation_index]
    contents = start_contents + end_contents
    f = open("runtime/models.py", "w")
    f.write("".join(contents))
    f.close()


def write_to_models_file(operation, classifier):
    f = open("runtime/models.py", "r")
    contents = f.readlines()
    index = contents.index('class ' + classifier.name + '(models.Model):\n') + 1
    value = generate_operation_declaration(operation.name, operation.implementation)
    f.close()
    contents.insert(index, value)

    f = open("runtime/models.py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


class OperationGenerator:
    def __init__(self, operation):
        self.operation = operation

    def generate(self):
        classifier = self.operation.classifier
        write_to_models_file(self.operation, classifier)

    def delete(self):
        delete_from_model(self.operation)
