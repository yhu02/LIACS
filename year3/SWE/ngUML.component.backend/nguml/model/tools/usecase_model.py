"""UseCase model methods to build a usecase model."""
from django.db import connection
from model.models import (
    Model,
    Postcondition,
    Precondition,
    Project,
    Relationship,
    Scenario,
    System,
    Trigger,
    UseCase,
    UseCaseClassifier,
    UseCaseSystem,
    Actor,
    Interaction,
    Inclusion,
    Extension,
    UseCaseRelationship,
    ModelChanges,
    ModelLayout,
)

# This file contains internal APIs to get and update usecase models

NODE_TYPES = [
    "UseCase",
    "Actor",
]


def populate_nodes(model) -> list:
    """Get all the usecases and actor nodes from the given model

    Args:
        usecase_mod: instance of db element Model

    Returns: a list of all the nodes

    """
    nodes = []
    nodes_per_type = get_nodes_per_type(model)

    for node_type, value in nodes_per_type.items():
        for node in value:
            node_id = node.parentId()
            nodes.append(generate_node_data(node_type, node_id))
    return nodes


def retrieve_node_objects() -> dict:
    """Retrieve all node objects."""
    node_objects = {}
    node_objects["UseCase"] = UseCase.objects
    node_objects["Actor"] = Actor.objects
    return node_objects


def get_nodes_per_type(model):
    """Get nodes per type.

    Args:
        - model: Model the nodes are part of.

    Returns:
        - nodes_per_type (dict): dictionary with all nodes per type
            key = node type.
            e.g. nodes_per_type["UseCase"] = object usecase nodes
    """
    node_objects = retrieve_node_objects()
    nodes_per_type = {}
    for key, value in node_objects.items():
        nodes_per_type[key] = value.filter(usecase_model=model)
    return nodes_per_type


def generate_node_data(node_type: str, node_id: int) -> dict:
    """Create a dictionary of data for a node.

    Args:
        - node_type (str): node type of the node e.g. UseCase, Actor
        - node_id (int): id of the node

    Returns:
        - dict: with all the data of the node.
    """
    node = UseCaseClassifier.objects.get(id=node_id)
    return {
        "type": node_type,
        "name": str(node.name),
        "data": {},
        "instances": {},
        "id": node_id,
        "position": {"x": node.xpos, "y": node.ypos},
    }


def populate_relationships(model):
    """Generates a list of all relationships in a given usecase model

    Args:
        model: instance of db element Model

    Returns: a list containing all the relationships in the given model

    """
    interactions = Interaction.objects.filter(usecase=model)
    extensions = Extension.objects.filter(usecase=model)
    inclusions = Inclusion.objects.filter(usecase=model)
    relationships = []
    for relation in interactions:
        relationships.append(get_relationship(relation, "interaction"))

    for relation in extensions:
        relationships.append(get_relationship(relation, "extension"))

    for relation in inclusions:
        relationships.append(get_relationship(relation, "inclusion"))

    return relationships


def get_relationship(relation, relation_type):
    """Get a relationship

    Args:
        relationship: instance fo db element Relationship


    Returns: a dict containing all the information on a single relationship

    """
    return {
        "id": relation.id,
        "name": str(relation),
        "type": relation_type,
        "from": relation.from_classifier,
        "to": relation.to_classifier,
    }


def generate_usecase_system(usecase_system):
    """Generate the usecase system

    Args:
        usecase_system: instance of db element UseCaseSystem

    Returns: a dict with a usecase System

    """
    usecase_system = {
        "id": usecase_system.id,
        "name": str(usecase_system),
        "type": "UseCaseSystem",
        "data": {},
        "instances": {},
    }
    return usecase_system


def generate_usecase_preconditions(usecase):
    """Generates the preconditions that this usecase has

    Args:
        usecase: instance of db element UseCase

    Returns: a list with dicts containing the information on the preconditions within a node (id, name, type and code)

    """
    preconditions = []
    for selected_precondition in Precondition.objects.filter(usecase=usecase):
        preconditions.append(
            {
                "id": selected_precondition.id,
                "name": selected_precondition.name,
                "type": selected_precondition.type,
                "code": selected_precondition.implementation,
            }
        )
    return preconditions


def generate_usecase_postconditions(usecase):
    """Generates the postconditions that this usecase has

    Args:
        usecase: instance of db element UseCase

    Returns: a list with dicts containing the information on the postconditions within a node (id, name, type and code)

    """
    postconditions = []
    for selected_postcondition in Postcondition.objects.filter(usecase=usecase):
        postconditions.append(
            {
                "id": selected_postcondition.id,
                "name": selected_postcondition.name,
                "type": selected_postcondition.type,
                "code": selected_postcondition.implementation,
            }
        )
    return postconditions


def generate_usecase_triggers(usecase):
    """Generates the triggers that this usecase has

    Args:
        usecase: instance of db element UseCase

    Returns: a list with dicts containing the information on the triggers within a node (id, name, type and code)

    """
    triggers = []
    for selected_trigger in Trigger.objects.filter(usecase=usecase):
        triggers.append(
            {
                "id": selected_trigger.id,
                "name": selected_trigger.name,
                "type": selected_trigger.type,
                "code": selected_trigger.implementation,
            }
        )
    return triggers


def generate_usecase_scenario(usecase):
    """Generates the scenarios that this usecase has

    Args:
        usecase: instance of db element UseCase

    Returns: a list with dicts containing the information on the scenarios within a node (id, name, type and code)

    """
    scenarios = []
    for selected_scenario in Scenario.objects.filter(usecase=usecase):
        scenarios.append(
            {
                "id": selected_scenario.id,
                "name": selected_scenario.name,
                "type": selected_scenario.type,
                "code": selected_scenario.implementation,
            }
        )
    return scenarios


def create_usecase_model(project_id: int, system_id: int, usecase_model_name: str, model_text: str) -> int:
    """Create usecase model.

    Args:
        - project_id (int): project_id where the model is part of.
        - system_id (int): system_id of which the model is part of.
        - usecase_model_name(str): name of the activity model.
        - model_text(str): text of the model.

    Returns:
        - usecase_model.pk (int): primary key of the generated activity model.
    """
    if not project_id or not system_id:
        print("Missing argument")
        return -1
    if not usecase_model_name:
        print("usecase model name missing.")
        return -1
    try:
        project = Project.objects.get(pk=project_id)
        system = System.objects.filter(project=project).get(pk=system_id)
    except System.DoesNotExist:
        print("System does not exist.")
        return -1
    usecase_mod = Model(type="usecase", title=usecase_model_name, raw_text=model_text, system=system)
    usecase_mod.save()
    return usecase_mod.pk


def create_usecase_model(project_id: int, system_id: int, usecase_model_name: str, model_text: str) -> int:
    """Create usecase model.

    Args:
        - project_id (int): project_id where the model is part of.
        - system_id (int): system_id of which the model is part of.
        - usecase_model_name(str): name of the activity model.
        - model_text(str): text of the model.

    Returns:
        - usecase_model.pk (int): primary key of the generated activity model.
    """
    if not project_id or not system_id:
        print("Missing argument")
        return -1
    if not usecase_model_name:
        print("usecase model name missing.")
        return -1
    try:
        project = Project.objects.get(pk=project_id)
        system = System.objects.filter(project=project).get(pk=system_id)
    except System.DoesNotExist:
        print("System does not exist.")
        return -1
    usecase_mod = Model(type="usecase", title=usecase_model_name, raw_text=model_text, system=system)
    usecase_mod.save()
    return usecase_mod.pk


def import_changes(target_model, changes):
    """
    This function is used to import changes to a usecase model from a usecase class model.
    Args:
        target_model: a DB instance of the usecase model the changes have to be written to
        changes: a list of changes that need to be applied to the model

    Returns: a success boolean, a list of errors and a list of changes that are applied

    """
    return make_changes(target_model, changes, skip_errors=True)


def make_changes(model: Model, changes, skip_errors=False):
    """Update a usecase model
    This function takes a list of changes and applies that to the specified usecase model.
    Available commands:
        - new-precondition
        - delete-precondition
        - retype-precondition
        - new-postcondition
        - delete-postcondition
        - retype-postcondition
        - new-trigger
        - delete-trigger
        - retype-trigger
        - new-scenario
        - delete-scenario
        - retype-scenario
        - new-classifier
        - delete-classifier
        - moved-classifier
        - new-system
        - delete-system
        - moved-system
        - new-relationship
        - delete-relationship
        - relationship-rename
        - relationship-mirror
        - relationship-retype
        Example of 3 changes that adds a "login(id:8)" usecase with a "test" precondition and a "test" interaction
    connection to "type_password(id:1)":

    [{
        "type": "new-classifier",
        "to": {
            "name": "login",
            "type": "UseCase",
            "position": {
                "x": 624,
                "y": 360
            },
            "data": {
                "precondition": [{
                    "name": "Test",
                    "type": "string"
                }]
            },
            "instances": {}
        },
        "nodeKey": 8,
        "key": null
    }, {
        "type": "new-precondition",
        "to": {
            "name": "Test",
            "type": "string"
        },
        "key": {
            "name": "type_password",
            "type": "UseCase",
            "position": {
                "x": 624,
                "y": 360
            },
            "data": {
                "precondition": [{
                    "name": "Test1"
                }]
            },
            "instances": {}
        }
    }, {
        "type": "new-Relationship",
        "to": {
            "name": "",
            "type": "Inclusion",
            "from": 8,
            "to": 1,
            "label": "Test",
        },
        "key": ["login", "type_password"]
        },

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
            success, error = delete_item(
                action.split("-")[1],
                item["to"]["id"],
                model,
                item["key"] or item["nodeKey"],
            )
        elif action.startswith("new"):
            success, error = new_item(action.split("-")[1], item["key"] or item["nodeKey"], item["to"], model)
        elif action.startswith("retype"):
            success, error = retype_item(action.split("-")[1], item["key"] or item["nodeKey"], item["to"], model)
        elif action.startswith("relationship"):
            cmd = action.split("-")
            if len(cmd) == 3:
                success, error = modify_relationship(cmd[1] + "-" + cmd[2], item["from"], model)
            if len(cmd) == 2:
                success, error = modify_relationship(cmd[1], item["from"], model)
        elif action == "moved-classifier":
            success, error = node_reposition(item, model)
        elif action == "moved-system":
            success, error = system_reposition(item, model)
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

    This function attempts to find a usecase or actor node in a specified usecase model based on its key.
    The key can contain either a name or an id but this function is meant for finding it by name.
    This is useful for finding usecase that the id is not known of but the name is.
    Args:
        model: instance of db element Model
        key: either a dict or a string, if a dict it needs to contain a "name"

    Returns: instance of db element UseCaseClassifier (usecase or actor node)

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
            return UseCase.objects.filter(usecase_model=model).get(name=name)

        except UseCase.DoesNotExist:
            print("Use Case or Actor could not be found with the following key:" + str(key))
            return


def find_node(model, node_id=None, key=None):
    """
    Main function to find a usecase in a specified usecase model.
    This function attempts to find a usecase or an actor either by id or by name.
    Args:
        model: instance of db element Model
        node_id: identifier of the node
        key: either a dict or a string containing the name of the usecase

    Returns: instance of db element UseCase if found, it returns None if the usecase cannot be found

    """
    if node_id:
        try:
            return UseCase.objects.filter(usecase_model=model).get(
                id=node_id,
            )
        except UseCase.DoesNotExist:  # if not found with id, we try with finding it by name
            if key:
                return find_node_by_key(model, key)
            else:
                print("Use Case or Actor could not be found with the following id:" + str(id))
                return
    else:
        return find_node_by_key(model, key)


def find_usecase_by_key(model, key):
    """Helper function for find_node

    This function attempts to find a usecase or actor node in a specified usecase model based on its key.
    The key can contain either a name or an id but this function is meant for finding it by name.
    This is useful for finding usecase that the id is not known of but the name is.
    Args:
        model: instance of db element Model
        key: either a dict or a string, if a dict it needs to contain a "name"

    Returns: instance of db element UseCaseClassifier (usecase or actor node)

    """
    name = ""
    if key:
        if isinstance(key, dict):
            if "name" in key and key["name"]:
                name = key["name"]
            if "id" in key and key["id"]:
                return find_usecase(model, usecase_id=key["id"])
        else:
            name = key
        try:
            return UseCase.objects.filter(usecase_model=model).get(name=name)

        except UseCase.DoesNotExist:
            print("Use Case could not be found with the following key:" + str(key))
            return


def find_usecase(model, usecase_id=None, key=None):
    """
    Main function to find a usecase in a specified usecase model.
    This function attempts to find a usecase or an actor either by id or by name.
    Args:
        model: instance of db element Model
        node_id: identifier of the node
        key: either a dict or a string containing the name of the usecase

    Returns: instance of db element UseCase if found, it returns None if the usecase cannot be found

    """
    if usecase_id:
        try:
            return UseCase.objects.filter(usecase_model=model).get(
                id=usecase_id,
            )
        except UseCase.DoesNotExist:  # if not found with id, we try with finding it by name
            if key:
                return find_usecase_by_key(model, key)
            else:
                print("Use Case could not be found with the following id:" + str(id))
                return
    else:
        return find_usecase_by_key(model, key)


def find_system_by_key(model, key):
    """Helper function for find_system

    This function attempts to find a system in a specified usecase model based on its key.
    The key can contain either a name or an id but this function is meant for finding it by name.
    This is useful for finding system that the id is not known of but the name is.
    Args:
        model: instance of db element Model
        key: either a dict or a string, if a dict it needs to contain a "name"

    Returns: instance of db element UseCaseSystem

    """
    name = ""
    if key:
        if isinstance(key, dict):
            if "name" in key and key["name"]:
                name = key["name"]
            if "id" in key and key["id"]:
                return find_system(model, system_id=key["id"])
        else:
            name = key
        try:
            return UseCaseSystem.objects.filter(usecase_model=model).get(name=name)

        except UseCaseSystem.DoesNotExist:
            print("System could not be found with the following key:" + str(key))
            return


def find_system(model, system_id=None, key=None):
    """
    Main function to find a system in a specified usecase model.
    This function attempts to find a system either by id or by name.
    Args:
        model: instance of db element Model
        actor_id: identifier of the system
        key: either a dict or a string containing the name of the system

    Returns: instance of db element UseCaseSystem if found, it returns None if the system cannot be found

    """
    if system_id:
        try:
            return UseCaseSystem.objects.filter(usecase_model=model).get(
                id=system_id,
            )
        except UseCaseSystem.DoesNotExist:  # if not found with id, we try with finding it by name
            if key:
                return find_system_by_key(model, key)
            else:
                print("UseCase System could not be found with the following id:" + str(id))
                return
    else:
        return find_system_by_key(model, key)


def find_relationship_with_data(model, data):
    """
    Main function to find a relationship in a specified usecase model.
    This function attempts to find a node with the data about which usecases are connected (to and from)
    Args:
        model: instance of db element Model
        data: information about the relationship (which usecases make up connection)

    Returns: instance of db element Relationship if found, it returns None if the node cannot be found

    """
    if "to" in data and "from" in data:
        try:
            return Relationship.objects.filter(usecase_model=model).get(
                from_classifier_id=data["from"], to_classifier_id=data["to"]
            )
        except Relationship.DoesNotExist:
            print("Relationship not found")
            return
    else:
        print("both a to and from id are needed")
        return


def node_reposition(item, model):
    """Function to save a moved usecase or actor

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
        else:
            return False, "Position data missing to reposition:" + item
    else:
        return False, "UseCase or Actor node could not be found"


def system_reposition(item, model):
    """Function to save a moved system

    Args:
        item: a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the repositioning was successful,
    if it was not successful the string will have the error message

    """
    selected_system = find_system(model, key=item["key"])
    if selected_system:
        if "position" in item["to"] and "x" in item["to"]["position"] and "y" in item["to"]["position"]:
            selected_system.xpos = item["to"]["position"]["x"]
            selected_system.ypos = item["to"]["position"]["y"]
            selected_system.save()
        else:
            return False, "Position data missing to reposition:" + item
    else:
        return False, "System could not be found"


def find_relationship_with_data(model, data):
    """
    Main function to find a relationship in a specified usecase model.
    This function attempts to find a node with the data about which nodes are connected (to and from)
    Args:
        model: instance of db element Model
        data: information about the relationship (which nodes make up relationship)

    Returns: instance of db element Relationship if found, it returns None if the node cannot be found

    """
    if "to" in data and "from" in data:
        try:
            return Relationship.objects.filter(usecase_model=model).get(
                from_classifier_id=data["from"], to_classifier_id=data["to"]
            )
        except Relationship.DoesNotExist:
            print("Relationship not found")
            return
    else:
        print("both a to and from id are needed")
        return


def delete_item(obj_type, pk, model, key):
    """Delete an element, this can be everything in a usecase model, so usecases, actors, relationships, preconditions, etc.

    Args:
        key:
        obj_type: type of the element to delete
        pk: identifier of the element that needs to be deleted
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the deletion was successful,
    if it was not successful the string will have the error message
    """
    if obj_type == "precondition":
        selected_precondition = Precondition.objects.filter(usecase_model=model).get(id=pk)
        selected_precondition.delete()
        return True, ""
    elif obj_type == "postcondition":
        selected_postcondition = Postcondition.objects.filter(usecase_model=model).get(id=pk)
        selected_postcondition.delete()
        return True, ""
    elif obj_type == "trigger":
        selected_trigger = Trigger.objects.filter(usecase_model=model).get(id=pk)
        selected_trigger.delete()
        return True, ""
    elif obj_type == "scenario":
        selected_scenario = Scenario.objects.filter(usecase_model=model).get(id=pk)
        selected_scenario.delete()
        return True, ""
    elif obj_type == "relationship":
        selected_relationship = Relationship.objects.filter(usecase_model=model).get(id=pk)
        selected_relationship.delete()
        return True, ""
    elif obj_type == "classifier":
        selected_class = find_node(model, node_id=pk, key=key)
        selected_class.delete()
        return True, ""
    elif obj_type == "system":
        selected_system = UseCaseSystem.objects.filter(usecase_model=model).get(id=pk)
        selected_system.delete()
        return True, ""
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
    from_classifier = find_node(model, obj["from"], key[0])
    to_classifier = find_node(model, obj["to"], key[1])
    if from_classifier and to_classifier:
        if obj["type"] == "inclusion":
            Inclusion.objects.create(
                usecase=model,
                from_classifier=from_classifier,
                to_classifier=to_classifier,
            )
            return True, ""

        if obj["type"] == "extension":
            Extension.objects.create(
                usecase_model=model,
                name=obj["label"],
                from_classifier=from_classifier,
                to_classifier_to=to_classifier,
            )
            return True, ""

        if from_classifier["type"] == "Actor" and to_classifier["type"] == "UseCase":
            if obj["type"] == "interaction":
                Interaction.objects.create(
                    usecase_model=model,
                    name=obj["label"],
                    from_classifier=from_classifier,
                    to_classifier=to_classifier,
                )
                return True, ""
    else:
        return False, "One of the nodes to make a new relationship could not be found"


def new_item(obj_type, key, obj, model):
    """Create a new element in a given usecase model,this can be everything in a usecase model,
    so usecase, actor, system, relationship, etc

    Args:
        obj_type: type of element to create
        key: parent if needed, so if a new method is made then this will be a child of a classifier
        obj: element to add, a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if adding a new element was successful,
    if it was not successful the string will have the error message
    """
    if obj_type == "classifier":
        if UseCaseClassifier.objects.filter(usecase_model=model).filter(name=obj["name"]).exists():
            return False, "A class with this name already exists: " + obj["name"]
        if obj.get("type") in NODE_TYPES:
            node_type = obj.get("type")
            node_objects = retrieve_node_objects()
            node = node_objects[node_type].create(
                usecase_model=model,
                name=obj["name"],
                xpos=obj.get("position", "").get("x", None),
                ypos=obj.get("position", None).get("y", ""),
            )
            return True, ""

    elif obj_type == "relationship":
        new_connection, error = new_relationship(obj, model, key)
        return True, ""

    elif obj_type == "precondition":
        selected_class = find_usecase(model, key=key)
        if selected_class:
            new_precondition = Precondition.objects.create(name=obj["name"], usecase=selected_class, type=obj["type"])
            return True, ""
        else:
            return False, "Class node with the following key not found: " + str(key)

    elif obj_type == "postcondition":
        selected_class = find_usecase(model, key=key)
        if selected_class:
            new_postcondition = Postcondition.objects.create(name=obj["name"], usecase=selected_class, type=obj["type"])
            return new_postcondition(obj, model)
        else:
            return False, "Class node with the following key not found: " + str(key)

    elif obj_type == "trigger":
        selected_class = find_usecase(model, key=key)
        if selected_class:
            new_trigger = Trigger.objects.create(name=obj["name"], usecase=selected_class, type=obj["type"])
            return True, ""
        else:
            return False, "Class node with the following key not found: " + str(key)

    elif obj_type == "scenario":
        selected_class = find_usecase(model, key=key)
        if selected_class:
            new_scenario = Scenario.objects.create(name=obj["name"], usecase=selected_class, type=obj["type"])
            return True, ""
        else:
            return False, "Class node with the following key not found: " + str(key)

    elif obj_type == "system":
        return new_system(obj)

    else:
        return False, "Unknown element type to make: " + obj_type


def retype_item(obj_type, key, obj, model):
    """Changes type of preconditions, postconditions, triggers and scenarios

    Args:
        obj_type: type of element to change, preconditions, postconditions, triggers or scenarios
        obj: change element, a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the retyping was successful,
    if it was not successful the string will have the error message
    """
    selected_node = find_node(model, key=key)
    if obj_type == "precondition":
        selected_precondition = Precondition.objects.filter(classifier=selected_node).get(id=obj["id"])
        selected_precondition.type = obj["type"]
        selected_precondition.save()
        return True, ""

    if obj_type == "postcondition":
        selected_postcondition = Postcondition.objects.filter(classifier=selected_node).get(id=obj["id"])
        selected_postcondition.type = obj["type"]
        selected_postcondition.save()
        return True, ""

    if obj_type == "trigger":
        selected_trigger = Trigger.objects.filter(classifier=selected_node).get(id=obj["id"])
        selected_trigger.type = obj["type"]
        selected_trigger.save()
        return True, ""

    if obj_type == "scenario":
        selected_scenario = Scenario.objects.filter(classifier=selected_node).get(id=obj["id"])
        selected_scenario.type = obj["type"]
        selected_scenario.save()
        return True, ""


def modify_relationship(action, obj, model):
    """changes any aspect of a relationship
    Available aspects to change(action):
        - relationship-rename
        - relationship-mirror
        - relationship-retype

    Args:
        action: aspect of relationship
        obj: change element, a dict about the change of an element, the shape of the dict is given in make_changes()
        model: instance of db element Model

    Returns: a boolean(success) and a string(error), the boolean is true if the modification was successful,
    if it was not successful the string will have the error message
    """
    relation = None
    relationship = find_relationship_with_data(model, obj)
    if relationship:
        if obj["type"] == "interaction":
            relation = Interaction.objects.filter(usecase_model=model).get(relationship_ptr_id=relationship.id)
        elif obj["type"] == "inclusion":
            relation = Inclusion.objects.filter(usecase_model=model).get(relationship_ptr_id=relationship.id)
        elif obj["type"] == "extension":
            relation = Extension.objects.filter(usecase_model=model).get(relationship_ptr_id=relationship.id)
    else:
        return False, "Relationship not found"
    if relation:
        if action == "mirror":
            old_from = relation.from_classifier
            old_to = relation.to_classifier
            relation.from_classifier = old_from
            relation.to_classifier = old_to
            relation.save()
        elif action == "rename":
            relation.name = obj["label"]
            relation.save()
        else:
            return False, "Unknown action " + action
        return True, ""
    else:
        return False, "Relationship not found"
    return False, "Unknown type: " + obj_type


def new_system(obj, model):
    if UseCaseSystem.objects.filter(usecase_model=model).filter(name=obj["name"]).exists():
        return False, "A system with this name already exists: " + obj["name"]
    new_system = UseCaseSystem.objects.create(
        usecase_model=model,
        name=obj["name"],
        xpos=obj.get("position", "").get("x", None),
        ypos=obj.get("position", None).get("y", ""),
    )
    new_system.save()
    return True, ""


def create_usecase_model(project_id: int, system_id: int, usecase_model_name: str, model_text: str) -> int:
    """Create usecase model.

    Args:
        - project_id (int): project_id where the model is part of.
        - system_id (int): system_id of which the model is part of.
        - usecase_model_name(str): name of the activity model.
        - model_text(str): text of the model.

    Returns:
        - usecase_model.pk (int): primary key of the generated activity model.
    """
    if not project_id or not system_id:
        print("Missing argument")
        return -1
    if not usecase_model_name:
        print("usecase model name missing.")
        return -1
    try:
        project = Project.objects.get(pk=project_id)
        system = System.objects.filter(project=project).get(pk=system_id)
    except System.DoesNotExist:
        print("System does not exist.")
        return -1
    usecase_mod = Model(type="usecase", title=usecase_model_name, raw_text=model_text, system=system)
    usecase_mod.save()
    return usecase_mod.pk
