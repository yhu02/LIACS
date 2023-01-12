from model.generators.PropertyGenerator import generate_property_declaration
import os.path


def read_from_shared_file(name):
    f = open("runtime/" + name + ".py", "r")
    text = f.read()
    f.close()

    return text


def write_to_shared_file(name, text):
    f = open("runtime/" + name + ".py", "w+")
    f.write(text)
    f.close()


def get_validator_function_reference(pk):
    return "rule_" + str(pk)


def get_validator_function_definition(pk, validator):
    return "\n\ndef rule_" + str(pk) + "(value):\n" + validator


def add_keyword(property, keyword, value):
    '''Adds validator reference to models.py text for (each) rule for @classifier and @property'''
    # Open and read models.py
    text = read_from_shared_file("models")

    # Search classifier and split text into 2
    class_string = 'class ' + property.classifier.name + '(models.Model):\n'
    class_index = text.find(class_string) + len(class_string)
    base_text = text[:class_index]
    classifier_text = text[class_index:]

    # Generate property declaration and find index of the beginning and the end of the prop in text
    old_property = generate_property_declaration(property.name, property.type)[:-2]
    property_index = classifier_text.find(old_property) + len(old_property)
    target_text = classifier_text[property_index:]
    classifier_text = classifier_text[:property_index]
    end_of_property_index = target_text.find('\n')

    # Add the reference
    new_value = keyword + "=" + str(value)
    if keyword in target_text[:end_of_property_index]:
        keywordstart = target_text.find(keyword)
        keywordend = min(target_text[keywordstart:].find(","), end_of_property_index) - 1
        old_value = target_text[keywordstart:keywordend]
        target_text = target_text.replace(old_value, new_value, 1)
    else:
        target_text = target_text.replace(")", ", " + new_value + ")", 1)

    # Save text to models.py
    write_to_shared_file("models", base_text + classifier_text + target_text)


def add_validator_reference(property, pk):
    '''Adds validator reference to models.py text for (each) rule for @classifier and @property'''

    # Open and read models.py
    text = read_from_shared_file("models")

    # Ensure that the import statement in models.py is correct
    import_statement = "from runtime.validators import *\n"
    if import_statement not in text:
        text = import_statement + text

    # Search classifier and split text into 2
    class_string = 'class ' + property.classifier.name + '(models.Model):\n'
    class_index = text.find(class_string) + len(class_string)
    base_text = text[:class_index]
    classifier_text = text[class_index:]

    # Generate property declaration and find index of the beginning and the end of the prop in text
    old_property = generate_property_declaration(property.name, property.type)[:-2]
    property_index = classifier_text.find(old_property) + len(old_property)
    target_text = classifier_text[property_index:]
    classifier_text = classifier_text[:property_index]
    end_of_property_index = target_text.find('\n')

    # Add the reference
    reference = get_validator_function_reference(pk) + ", ]"
    if "validators" in target_text[:end_of_property_index]:
        target_text = target_text.replace("]", " " + reference, 1)
    else:
        target_text = target_text.replace(")", ", validators = [" + reference + ")", 1)

    # Save text to models.py
    write_to_shared_file("models", base_text + classifier_text + target_text)


def read_validators_py():
    # Create validators.py if it does not already exist
    if not os.path.isfile("runtime/validators.py"):
        write_to_shared_file("validators", "")

    # Read texts
    text = read_from_shared_file("validators")

    # Add import statement to validators.py if it does not already exist
    import_statement = "from django.core.exceptions import ValidationError"
    if import_statement not in text:
        text = import_statement + "\n" + text

    return text


def add_import_statement(import_statement):
    text = read_validators_py()

    if import_statement not in text:
        text = import_statement + "\n" + text

    write_to_shared_file("validators", text)


def add_validator_function(pk, validator):
    text = read_validators_py()

    # Add rule and create function
    validator_function = get_validator_function_definition(pk, validator)
    if validator_function not in text:
        text += validator_function

    write_to_shared_file("validators", text)


def add_validator(property, pk, validator):
    add_validator_reference(property, pk)
    add_validator_function(pk, validator)


def remove_validator_reference(pk):
    '''Removes "rule_pk" from models.py'''
    text = read_from_shared_file("models")
    text = text.replace(get_validator_function_reference(pk) + ", ", '', 1)
    write_to_shared_file("models", text)


def remove_validator_function(rule):
    text = read_from_shared_file("validators")
    validator_function = get_validator_function_definition(rule.pk, rule.get_validator())
    text = text.replace(validator_function, '')
    write_to_shared_file("validators", text)


def remove_validator(rule):
    remove_validator_reference(rule.pk)
    remove_validator_function(rule)


def get_standard_if_statement(conditionalExpression, rule):
    return (
        "\tif " + conditionalExpression + ":\n"
        "\t\treturn True\n"
        "\telse:\n"
        "\t\traise ValidationError(\n"
        "\t\t\t'{value} does not abide by rule: '.format(value) + \'" + rule.get_processed_text() + "',\n"
        "\t\t\tparams={'value': value}, )\n"
    )


def get_standard_if_statement_spaces(conditionalExpression, rule):
    return (
        "if "
        + conditionalExpression
        + ":\n"
        + indent("return True\n", 4, 1)
        + "else:\n"
        + indent("raise ValidationError(\n", 4, 1)
        + indent("'{value} does not abide by rule: '.format(value) + \'" + rule.get_processed_text() + "',\n", 4, 2)
        + indent("params={'value': value}, )\n", 4, 2)
    )


def indent(text, indentation, depth):
    new_text = ""
    for line in text.splitlines():
        new_text += " " * indentation * depth + line + "\n"
    return new_text


def generate_model_definition(classifier):
    return "class " + classifier.name + "(models.Model):"


# adds a block of code to a method in a classifier in runtime/models.py
# adds the method if it doesn't exist yet
def add_to_method_in_classifier(classifier, method, block):
    # if ValidationError is not already imported in models.py, import it
    # needed to throw validationErrors
    # add_import_statement("from django.core.exceptions import ValidationError")
    model_text = read_from_shared_file("models").splitlines()
    model_text.replace("pass", "")
    # generate model definition so we can find it
    model_definition = generate_model_definition(classifier)
    # the number of spaces for indentation of the models file
    indentation = 4
    # the depth of the clean method
    depth = 1
    # the line in the models.py that 'def clean():' is on, or should be added to
    line = 0
    classifierFound = False
    methodFound = False
    unindentFound = False
    index = 0
    for iteration, line in enumerate(model_text):
        index = iteration
        if classifierFound and method in line:
            methodFound = True
            # find the end of the method instead, which is one indentation deeper
            depth += 1
        # if we've found the classifier, look for the next unindent to find the end of the classifier definitin
        elif classifierFound and not line[0: indentation * depth].isspace():
            unindentFound = True
            break
        elif model_definition in line:
            classifierFound = True

    # if we never find the unindent, we're stopped one index too early, so increment
    if not unindentFound:
        index += 1

    if not methodFound:
        model_text.insert(index, indent(method, indentation, depth))
        index += 1
        depth += 1
    model_text.insert(index, indent(block, indentation, depth))
    model_text = "\n".join(model_text)
    write_to_shared_file("models", model_text)
