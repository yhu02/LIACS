from model.models import (
    Project,
    System,
    Class,
    Classifier,
    Operation,
    Property,
    Model,
    Composition,
    Generalization,
    Association,
    Relationship,
    ModelChanges,
    ModelLayout,
)

from model.generators import (
    ClassifierGenerator,
    RelationshipGenerator,
    PropertyGenerator,
    OperationGenerator,
)
from model.utils import apply_auto_layout

# This file contains internal APIs to get and update class models


def get_class_model(class_model_id):
    """
        Get a db element of Model based on the class model id, this function is useful if your class model is not
        part of a system. For example when you made a class model in the (legacy) class model extractor (http://localhost:8000/)
    Args:
        class_model_id: (integer) identifier of class model

    Returns: a db instance of model

    """
    try:
        return Model.objects.filter(type="class").get(id=class_model_id)
    except Model.DoesNotExist:
        print("The requested class model cannot be found")
        return


def populate_nodes(model):
    """Get all the class nodes (classifiers) from the given model

    Args:
        model: instance of db element Model

    Returns: a list of all the nodes

    """
    nodes = []
    for classifier in Classifier.objects.filter(class_model=model):
        nodes.append(generate_node(classifier))
    return nodes


def generate_node(classifier):
    """Generate an individual node

    Args:
        classifier: instance of db element Classifier

    Returns: a dict with the whole node

    """
    node = {
        "id": classifier.id,
        "name": str(classifier),
        "type": "Class",
        "data": {
            "properties": generate_classifier_properties(classifier),
            "methods": generate_classifier_operations(classifier),
        },
        "instances": {},
    }
    if classifier.xpos and classifier.ypos:
        node["position"] = {"x": classifier.xpos, "y": classifier.ypos}
    return node


def generate_classifier_operations(classifier):
    """Generates the operation (methods) that this classifier has

    Args:
        classifier: instance of db element Classifier

    Returns: a list with dicts containing the information on the operations within a node (id, name, type and code)

    """
    operations = []
    for selected_operation in Operation.objects.filter(classifier=classifier):
        operations.append(
            {
                "id": selected_operation.id,
                "name": selected_operation.name,
                "type": selected_operation.type,
                "code": selected_operation.implementation,
            }
        )
    return operations


def generate_classifier_properties(classifier):
    """Generates the properties (attributes) that this classifier has

    Args:
        classifier: instance of db element Classifier

    Returns: a list with dicts containing the attributes (its id, name and type)

    """
    properties = []
    for selected_property in Property.objects.filter(classifier=classifier).filter(inherited=False):
        properties.append(
            {
                "id": selected_property.id,
                "name": selected_property.name,
                "type": selected_property.type,
            }
        )
    return properties


def populate_connections(model):
    """Generates a list of all relationships in a given class model

    Args:
        model: instance of db element Model

    Returns: a list containing all the relations in the given model

    """
    compositions = Composition.objects.filter(class_model=model)
    associations = Association.objects.filter(class_model=model)
    generalizations = Generalization.objects.filter(class_model=model)
    connections = []
    for relation in compositions:
        connections.append(get_connection(relation, "composition"))

    for relation in associations:
        connections.append(get_connection(relation, "association"))

    for relation in generalizations:
        connections.append(get_connection(relation, "generalization"))
    return connections


def get_connection(relation, relation_type):
    """Get a relation

    Args:
        relation: instance fo db element either composition, association or generalization
        relation_type: defines the type of the relation

    Returns: a dict containing all the information on a single relation

    """
    if relation_type == "generalization":
        m_from = ""
        m_to = ""
    else:
        m_from = relation.multiplicity_from
        m_to = relation.multiplicity_to
    return {
        "id": relation.id,
        "name": str(relation),
        "type": relation_type,
        "labelFrom": str(m_from),
        "label": str(relation),
        "labelTo": str(m_to),
        "from": relation.classifier_from_id,
        "to": relation.classifier_to_id,
    }


def import_changes(target_model, changes):
    """
    This function is used to import changes to a class model from a different class model.
    Args:
        target_model: a DB instance of the class model the changes have to be written to
        changes: a list of changes that need to be applied to the model

    Returns: a success boolean, a list of errors and a list of changes that are applied

    """
    return make_changes(target_model, changes, skip_errors=True)


def make_changes(model, changes, skip_errors=False):
    """Update a class model
    This function takes a list of changes an applies that to the specified class model.
    Available commands:
        - retype-method
        - new-method
        - delete-method
        - retype-property
        - new-property
        - delete-property
        - new-classifier
        - moved-classifier
        - new-connection
        - connection-rename
        - connection-mirror
        - connection-cardinality-from
        - connection-cardinality-to
        - connection-retype
        - delete-connection
    Example of 3 changes that adds a "test(id:8)" class with a "test" property and a "test" generalised
    connection to "order(id:1)":

    [{
        "type": "new-classifier",
        "to": {
            "name": "test",
            "type": "Class",
            "position": {
                "x": 624,
                "y": 360
            },
            "data": {
                "properties": [{
                    "name": "Test",
                    "type": "string"
                }]
            },
            "instances": {}
        },
        "nodeKey": 8,
        "key": null
    }, {
        "type": "new-property",
        "to": {
            "name": "Test",
            "type": "string"
        },
        "key": {
            "name": "test",
            "type": "Class",
            "position": {
                "x": 624,
                "y": 360
            },
            "data": {
                "properties": [{
                    "name": "Test",
                    "type": "string"
                }]
            },
            "instances": {}
        }
    }, {
        "type": "new-connection",
        "to": {
            "name": "",
            "type": "generalization",
            "from": 8,
            "to": 1,
            "label": "Test",
            "labelFrom": "*",
            "labelTo": "*"
        },
        "key": ["test", "Order"]
    }, {
        "type": "model-layout",
        "to": {
            "name": "",
            "type": "generalization",
            "from": 8,
            "to": 1,
            "label": "Test",
            "labelFrom": "*",
            "labelTo": "*"
        },
        "key": ["test", "Order"]
        }]
    More examples can be made yourself in the editor. Just make some changes and click save locally.
    The changes array is now downloaded.
    Args:
        model: instance of db element Model
        changes: json list of dicts that contain details about what to change and to what
        skip_errors: boolean, if true making changes will continue after encountering an error,
                     if False the process will stop as soon as an error is encountered.
    """
    success = True
    error = ""
    error_list = []
    applied_changes = []
    for item in changes:
        action = item["type"]
        if action.startswith("delete"):
            success, error = delete(
                action.split("-")[1],
                item["to"]["id"],
                model,
                item["key"] or item["nodeKey"],
            )
        elif action.startswith("new"):
            success, error = new(action.split("-")[1], item["key"] or item["nodeKey"], item["to"], model)
        elif action.startswith("retype"):
            success, error = retype(action.split("-")[1], item["key"] or item["nodeKey"], item["to"], model)
        elif action.startswith("connection"):
            cmd = action.split("-")
            if len(cmd) == 3:
                success, error = modify_connection(cmd[1] + "-" + cmd[2], item["from"], model)
            if len(cmd) == 2:
                success, error = modify_connection(cmd[1], item["from"], model)
        elif action == "moved-classifier":
            success, error = reposition(item, model)
        else:
            success = False
            error = "Unknown command"

        if not success:
            if not skip_errors:
                return False, error
            else:
                error_list.append(error)
        else:
            applied_changes.append(item)

    ModelChanges(model=model, changes=applied_changes).save()  # save json of applied changes to db to use for import
    if not skip_errors:
        return True, ""
    else:
        if not error_list:
            return True, [], applied_changes
        else:
            return False, error_list, applied_changes


def find_node_by_key(model, key):
    """Helper function for find_node

    This function attempts to find a class node in a specified class model based on its key.
    The key can contain either a name or an id but this function is meant for finding it by name.
    This is useful for finding nodes that the id is not known of but the name is.
    Args:
        model: instance of db element Model
        key: either a dict or a string, if a dict it needs to contain a "name"

    Returns: instance of db element Class (class node)

    """
    name = ""
    if key:
        if isinstance(key, dict):
            if "name" in key and key["name"]:
                name = key["name"]
            if "id" in key and key["id"]:
                return find_node(model, node_id=key["id"])
        else:
            name = key
        try:
            return Class.objects.filter(class_model=model).get(name=name)

        except Class.DoesNotExist:
            print("Class node could not be found with the following key:" + str(key))
            return


def find_node(model, node_id=None, key=None):
    """
    Main function to find a node in a specified class model.
    This function attempts to find a node either by id or by name.
    Args:
        model: instance of db element Model
        node_id: identifier of the node
        key: either a dict or a string containing the name of the node

    Returns: instance of db element Class (class node) if found, it returns None if the node cannot be found

    """
    if node_id:
        try:
            return Class.objects.filter(class_model=model).get(
                id=node_id,
            )
        except Class.DoesNotExist:  # if not found with id, we try with finding it by name
            if key:
                return find_node_by_key(model, key)
            else:
                print("Class node could not be found with the following id:" + str(id))
                return
    else:
        return find_node_by_key(model, key)


def find_connection_with_data(model, data):
    """
    Main function to find a connection in a specified class model.
    This function attempts to find a node with the data about which nodes are connected (to and from)
    Args:
        model: instance of db element Model
        data: information about the connection (which nodes make up connection)

    Returns: instance of db element Relationship if found, it returns None if the node cannot be found

    """
    if "to" in data and "from" in data:
        try:
            return Relationship.objects.filter(class_model=model).get(
                classifier_from_id=data["from"], classifier_to_id=data["to"]
            )
        except Relationship.DoesNotExist:
            print("Relationship not found")
            return
    else:
        print("both a to and from id are needed")
        return


def set_layout(model):
    """
    This function calls teh autolayout function for class models and sets the new location.

     Args:
         model: instance of db element Model

     Returns: a boolean(success) and a string(error), the boolean is true if the layout setting was successful,
     if it was not successful the string will have the error message

    """
    model_with_layout = apply_auto_layout(populate_nodes(model), populate_connections(model))
    for node in model_with_layout["nodes"]:
        reposition_data = {"key": {"id": node["id"]}, "to": {"position": node["position"]}}
        success, error = reposition(reposition_data, model)
        if not success:
            return success, error
    return True, ""


def reposition(item, model):
    """Function to save a moved classifier

    Args:
        item: a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the repositioning was successful,
    if it was not successful the string will have the error message

    """
    selected_node = find_node(model, key=item["key"])
    if selected_node:
        if "position" in item["to"] and "x" in item["to"]["position"] and "y" in item["to"]["position"]:
            selected_node.xpos = item["to"]["position"]["x"]
            selected_node.ypos = item["to"]["position"]["y"]
            selected_node.save()
            return True, ""
        else:
            return False, "Position data missing to reposition:" + str(item)
    else:
        return False, "Class node could not be found"


def delete(obj_type, pk, model, key):
    """Delete an element, this can be everything in a class model, so classifier, relationship, properties and methods

    Args:
        key:
        obj_type: type of the element to delete
        pk: identifier of the element that needs to be deleted
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the deletion was successful,
    if it was not successful the string will have the error message
    """
    if obj_type == "property" or obj_type == "method":
        selected_property = Property.objects.filter(class_model=model).get(id=pk)
        if model.system.application_generated:
            PropertyGenerator(selected_property).delete(False)
        selected_property.delete()
        return True, ""
    elif obj_type == "connection":
        selected_relationship = Relationship.objects.filter(class_model=model).get(id=pk)
        selected_relationship.delete()
        return True, ""
    elif obj_type == "classifier":
        selected_class = find_node(model, node_id=pk, key=key)
        if selected_class:
            if model.system.application_generated:
                ClassifierGenerator(selected_class).delete(False)
            selected_class.delete()
            return True, ""
        else:
            return False, "Class node could not be found"
    return False, "Unknown type:" + obj_type


def new_relationship(obj, model, key):
    """Generate a new relationship between 2 nodes

    Args:
        key:
        obj: change element, a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the generation of the relationship was
    successful, if it was not successful the string will have the error message
    """
    classifier_from = find_node(model, obj["from"], key[0])
    classifier_to = find_node(model, obj["to"], key[1])
    if classifier_from and classifier_to:
        if obj["type"] == "generalization":
            Generalization.objects.create(
                class_model=model,
                name=obj["label"],
                classifier_from=classifier_from,
                classifier_to=classifier_to,
            )
            return True, ""

        if obj["type"] == "association":
            Association.objects.create(
                class_model=model,
                name=obj["label"],
                classifier_from=classifier_from,
                classifier_to=classifier_to,
                multiplicity_from=obj["labelFrom"],
                multiplicity_to=obj["labelTo"],
            )
            return True, ""

        if obj["type"] == "composition":
            Composition.objects.create(
                class_model=model,
                name=obj["label"],
                classifier_from=classifier_from,
                classifier_to=classifier_to,
                multiplicity_from=obj["labelFrom"],
                multiplicity_to=obj["labelTo"],
            )
            return True, ""

    else:
        return False, "One of the nodes to make a new relationship could not be found"


def new(obj_type, key, obj, model):
    """Create a new element in a given class model,this can be everything in a class model,
    so classifier, relationship, properties and methods

    Args:
        obj_type: type of element to create
        key: parent if needed, so if a new method is made then this will be a child of a classifier
        obj: element to add, a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if adding a new element was successful,
    if it was not successful the string will have the error message
    """
    if obj_type == "classifier":
        if Class.objects.filter(class_model=model).filter(name=obj["name"]).exists():
            return False, "A class with this name already exists: " + obj["name"]
        new_class = Class.objects.create(
            class_model=model,
            name=obj["name"],
            xpos=obj.get("position", "").get("x", None),
            ypos=obj.get("position", None).get("y", ""),
        )
        if model.system.application_generated:
            ClassifierGenerator(new_class).generate(False)
        return True, ""

    elif obj_type == "connection":
        new_connection, error = new_relationship(obj, model, key)
        if new_connection:
            if model.system.application_generated:
                RelationshipGenerator(new_connection).generate(False)
            return True, ""
        else:
            return False, error

    elif obj_type == "method":
        selected_class = find_node(model, key=key)
        if selected_class:
            new_operation = Operation.objects.create(
                name=obj["name"],
                implementation=obj["code"],
                classifier=selected_class,
                type=obj["type"],
            )
            if model.system.application_generated:
                OperationGenerator(new_operation).generate()
            return True, ""
        else:
            return False, "Class node with the following key not found: " + str(key)

    elif obj_type == "property":
        selected_class = find_node(model, key=key)
        if selected_class:
            new_property = Property.objects.create(name=obj["name"], classifier=selected_class, type=obj["type"])
            if model.system.application_generated:
                PropertyGenerator(new_property).generate(False)
            return True, ""
        else:
            return False, "Class node with the following key not found: " + str(key)

    else:
        return False, "Unknown element type to make: " + obj_type


def retype(obj_type, key, obj, model):
    """Changes type of methods and properties

    Args:
        obj_type: type of element to changes, either method or property
        obj: change element, a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the retyping was successful,
    if it was not successful the string will have the error message
    """
    selected_node = find_node(model, key=key)
    if obj_type == "method":
        selected_operation = Operation.objects.filter(classifier=selected_node).get(id=obj["id"])
        selected_operation.type = obj["type"]
        selected_operation.save()
        return True, ""
    if obj_type == "property":
        selected_property = Property.objects.filter(classifier=selected_node).get(id=obj["id"])
        selected_property.type = obj["type"]
        selected_property.save()
        return True, ""

    return False, "Unknown type: " + obj_type


def modify_connection(action, obj, model):
    """changes any aspect of a relationship
    Available aspects to change(action):
        - connection-rename
        - connection-mirror
        - connection-cardinality-from
        - connection-cardinality-to
        - connection-retype

    Args:
        action: aspect of relationship
        obj: change element, a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the modification was successful,
    if it was not successful the string will have the error message
    """
    relation = None
    relationship = find_connection_with_data(model, obj)
    if relationship:
        if obj["type"] == "generalization":
            relation = Generalization.objects.filter(class_model=model).get(relationship_ptr_id=relationship.id)
        elif obj["type"] == "association":
            relation = Association.objects.filter(class_model=model).get(relationship_ptr_id=relationship.id)
        elif obj["type"] == "composition":
            relation = Composition.objects.filter(class_model=model).get(relationship_ptr_id=relationship.id)
    else:
        return False, "Connection/relationship not found"
    if relation:
        if action == "mirror":
            old_from = relation.classifier_from
            old_to = relation.classifier_to
            old_mfrom = relation.multiplicity_from or ""
            old_mto = relation.multiplicity_to or ""
            relation.classifier_to = old_from
            relation.classifier_from = old_to
            relation.multiplicity_from = old_mto
            relation.multiplicity_to = old_mfrom
            relation.save()
        elif action == "rename":
            relation.name = obj["label"]
            relation.save()
        elif action == "cardinality-from":
            relation.multiplicity_from = obj["labelFrom"]
            relation.save()
        elif action == "cardinality-to":
            relation.multiplicity_to = obj["labelTo"]
            relation.save()
        else:
            return False, "Unknown action " + action
        if model.system.application_generated:
            RelationshipGenerator(relation).generate(False)
        return True, ""
    else:
        return False, "Connection/relationship not found"
