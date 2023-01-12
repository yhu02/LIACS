"""Activity model methods to build an activity model."""
import uuid

from django.db import connection
from model.models import (
    Action,
    ActivityFinalNode,
    ExecutableNode,
    Model,
    DecisionNode,
    FlowFinalNode,
    ForkNode,
    InitialNode,
    JoinNode,
    MergeNode,
    ModelChanges,
    OperationAction,
    Project,
    System,
    Activity,
    ActivityEdge,
    ActivityNode,
)

# This file contains internal APIs to get and update activity models

NODE_TYPES = [
    "Initial",
    "FlowFinal",
    "ActivityFinal",
    "Fork",
    "Merge",
    "Join",
    "Decision",
    "Action",
]


def populate_nodes(activity_mod: Model) -> list:
    """Get all the activity nodes (activity) from the given model.

    Args:
        activity_mod: instance of db element ActivityModel

    Returns: a list of all the nodes

    """
    nodes = []
    try:
        activity = Activity.objects.filter(activity_model=activity_mod).first()
    except Activity.DoesNotExist:
        print("activity does not exist")
        return nodes
    nodes_per_type = get_nodes_per_type(activity)

    for node_type, value in nodes_per_type.items():
        for node in value:
            node_id = node.parentId()
            activity_id = ActivityNode.objects.get(id=node_id).activity_id
            nodes.append(generate_node_data(node_type, node_id, activity_id))
    return nodes


def retrieve_node_objects() -> dict:
    """Retrieve all node objects."""
    node_objects = {}
    node_objects["Initial"] = InitialNode.objects
    node_objects["FlowFinal"] = FlowFinalNode.objects
    node_objects["ActivityFinal"] = ActivityFinalNode.objects
    node_objects["Fork"] = ForkNode.objects
    node_objects["Merge"] = MergeNode.objects
    node_objects["Join"] = JoinNode.objects
    node_objects["Decision"] = DecisionNode.objects
    node_objects["Action"] = Action.objects
    return node_objects


def get_nodes_per_type(activity: Activity):
    """Get nodes per type.

    Args:
        - activity(Activity): activity the nodes are part of.

    Returns:
        - nodes_per_type (dict): dictionary with all nodes per type
            key = node type.
            e.g. nodes_per_type["Action"] = object action nodes
    """
    node_objects = retrieve_node_objects()
    nodes_per_type = {}
    for key, value in node_objects.items():
        nodes_per_type[key] = value.filter(activity=activity)
    return nodes_per_type


def generate_node_data(node_type: str, node_id: int, node_activity_id: int) -> dict:
    """Create a dictionary of data for a node.

    Args:
        - node_type (str): node type of the node e.g. Action, Initial
        - node_id (int): id of the node
        - node_activity_id (int): id of the activity the node is part of.

    Returns:
        - dict: with all the data of the node.
    """
    node = ActivityNode.objects.get(id=node_id)
    return {
        "type": node_type,
        "name": str(node.name),
        "data": {
            "description": node.description,
            "activity_id": node_activity_id,
            "operations": get_action_operations(node),
            "callBehaviorAction": get_node_call_behavior_action(node),
        },
        "instances": {},
        "id": node_id,
        "position": {"x": node.xpos, "y": node.ypos},
    }


def get_action_operations(node):
    """Create list of operations for a node from database.

    Args:
        node (): The node for which the operations are generated.

    Returns:
        List Operations
    """
    # Find a way to check if it is a action
    try:
        action = Action.objects.get(id=node.id)
    except Exception as _:
        return []

    operations = OperationAction.objects.filter(callOperationAction=action)

    return [
        {
            "name": operation.name,
            "type": operation.type,
            "code": operation.implementation,
            "id": operation.id,
        }
        for operation in operations
    ]


def get_node_call_behavior_action(node):
    """Return call_behavior_action id if it exists.

    Args:
        node (): The node for which the operations are generated.

    Returns:
        id of the activity, if it had a FK to the given node.
        0, if there is no activity for the given node.
    """
    try:
        action = Action.objects.get(id=node.id)
        behavior_Action = Activity.objects.filter(callBehaviorAction_id=action.id).first()
    except Exception as _:
        return 0
    if behavior_Action:
        return behavior_Action.id
    else:
        return 0


def populate_connections(activity_mod: Model) -> list:
    """Generate a list of all relationships in a given activity model.

    Args:
        activity_mod: instance of db element ActivityModel

    Returns: a list containing all the relations in the given model

    """
    connections = []
    try:
        activity = Activity.objects.get(activity_model=activity_mod)
    except Activity.DoesNotExist:
        print("activity does not exist")
        return connections
    edges = ActivityEdge.objects.filter(activity=activity)
    for edge in edges:
        connections.append(create_connection(edge))
    return connections


def create_connection(activity_edge: ActivityEdge) -> dict:
    """Create a connection that can be returned."""
    return {
        "label": activity_edge.guard,
        "guard": activity_edge.guard,
        "weight": activity_edge.weight,
        "from_id": str(activity_edge.incoming_node_id),
        "to_id": str(activity_edge.outgoing_node_id),
        "from": activity_edge.incoming_node_id,
        "to": activity_edge.outgoing_node_id,
        "type": "edge",
        "id": activity_edge.id,
    }


def create_activity_model(project_id: int, system_id: int, activity_model_name: str, model_text: str) -> int:
    """Create activity model.

    Args:
        - project_id (int): project_id where the model is part of.
        - system_id (int): system_id of which the model is part of.
        - activity_model_name(str): name of the activity model.
        - model_text(str): text of the model.

    Returns:
        - activity_model.pk (int): primary key of the generated activity model.
    """
    if not project_id or not system_id:
        print("Missing argument")
        return -1
    if not activity_model_name:
        print("activity model name missing.")
        return -1
    try:
        project = Project.objects.get(pk=project_id)
        system = System.objects.filter(project=project).get(pk=system_id)
    except System.DoesNotExist:
        print("System does not exist.")
        return -1
    activity_mod = Model(type="activity", title=activity_model_name, raw_text=model_text, system=system)
    activity_mod.save()
    return activity_mod.pk


def create_activity(activity_model_pk: int, activity_name: str) -> int:
    """Create an activity within an activity model.

    Args:
        - activity_model_pk (int): the primary key of the activity model.
        - activity_name(str): the name of the activity to be created.

    Returns:
        - activity.pk (int): primary key of the activity.
    """
    if not activity_model_pk:
        print("Create_activity: activity_model_pk was not defined. Activity not created.")
        return -1
    try:
        activity_mod = Model.objects.filter(type="activity").get(pk=activity_model_pk)
    except Model.DoesNotExist:
        print("Activity model does not exist!")
        return -1
    activity = Activity(name=activity_name, activity_model=activity_mod)
    activity.save()
    return activity.pk


def import_changes(target_model, changes):
    return make_changes(target_model, changes, skip_errors=True)


def make_changes(model: Model, changes: list, skip_errors=False):
    """Update an activity model.

    This function takes a list of changes and applies that to the specified activity model.
    Available commands:
        - new-classifier
        - retype-node
        - delete-node
        - new-connection
        - retype-connection
        - delete-connection
        - new-operation
        - retype-operation
        - delete-operation
        - new-behaviorAction
        - retype-behaviorAction
        - delete-behaviorAction
        - new-activity
        - retype-activity
        - delete-activity
    Example of 3 changes that adds a "Decision(id:8)" node,
    with a "[order.Amount > 10.000]" connection to "Approve order" node:
    [{
        "type": "new-classifier",
        "to": {
            "name": "Decision",
            "type": "Decision",
            "position": {
                "x": 248,
                "y": 248
            },
            "data": {},
            "instances": {}
        },
        "nodeKey": 10,,
        "key": null
    },
    {
        "type": "new-classifier",
        "to": {
            "name": "Approve_SP_order",
            "type": "Action",
            "position": {
                "x": 680,
                "y": 280
            },
            "data": {},
            "instances": {}
        },
        "nodeKey": 11,
        "key": null
    },
    {
        "type": "new-connection",
        "to": {
            "name": "",
            "type": "edge",
            "from": 10,
            "to": 11,
            "label": "[order.Amount > 10.000]",
            "labelFrom": "*",
            "labelTo": "*"
        },
        "key": [
            "Decision",
            "Approve_SP_order"
        ]
    }]

    Returns:
        success: boolean if all changes were applied successfully
        errors: list of errors that were encountered, if skip_errors is False this list has maximum of one error
        applied_changes:  list of all changes that were applied
    """
    try:
        activity = Activity.objects.filter(activity_model=model).first()
    except Activity.DoesNotExist:
        print("Activity in model not found. Maybe first create a model before updating.")
        return
    success, errors, applied_changes = process_activity_changes(changes, activity.id, skip_errors)
    ModelChanges(model=model, changes=applied_changes).save()  # save json of applied changes to db to use for import
    return success, errors, applied_changes


def populate_new_activity(nodes, connections, activity_id):
    """This function is only to be used by the Activity Model Extractor pipeline"""
    try:
        Activity.objects.get(pk=activity_id)
    except Activity.DoesNotExist:
        print("Activity not found")
        return
    for key in nodes.keys():
        node_data = {
            "name": nodes[key]["name"],
            "description": nodes[key]["data"]["description"],
            "position": {"x": 0, "y": 0},
        }
        success, node_id = new_node(nodes[key]["type"], node_data, activity_id)
        if success:
            nodes[key]["id"] = node_id
        else:
            print("error generating activity node, node:" + str(node_data))
    for key in connections.keys():
        connection_data = connections[key]
        keys = [
            nodes[connections[key]["from"]]["name"],
            nodes[connections[key]["to"]]["name"],
        ]
        connection_data["from"] = nodes[connections[key]["from"]]["id"]
        connection_data["to"] = nodes[connections[key]["to"]]["id"]
        success, _ = new_relationship(connection_data, activity_id, keys)
        if not success:
            print("error generating an activity relationship, relationship: " + str(connection_data))


def process_activity_changes(changes: list, activity_id: int, skip_errors=False):
    """Process activity changes."""
    print("processing changes.....")
    success = True
    error = ""
    error_list = []
    applied_changes = []
    for change in changes:
        change_action = change.get("type")
        change_data = change.get("to")
        change_key = change.get("key")
        if change_action.startswith("delete"):
            success, error = delete_item(activity_id, change_action.split("-")[1], change_data.get("id"))
        elif change_action.startswith("new"):
            success, error = new_item(activity_id, change_action.split("-")[1], change_data, change_key)
        elif change_action.startswith("retype"):
            success, error = retype_item(activity_id, change_action.split("-")[1], change_data)
        else:
            success = False
            error = "Unknown command"
        if not success:
            if not skip_errors:
                return False, error
            else:
                error_list.append(error)
        else:
            applied_changes.append(change)
    if not error_list:
        return True, [], applied_changes
    else:
        return False, error_list, applied_changes


def get_activity(activity_id, name=None):
    if activity_id:
        try:
            return Activity.objects.get(pk=activity_id)
        except Activity.DoesNotExist:
            if name:
                try:
                    return Activity.objects.get(name=name)
                except Activity.DoesNotExist:
                    print("Activity not found")
                    return
            print("Activity not found.")
            return


def find_activity_node(activity_id, node_id, name=None):
    try:
        return ActivityNode.objects.filter(activity_id=activity_id).get(id=node_id)
    except ActivityNode.DoesNotExist:
        if name:
            try:
                node = ActivityNode.objects.filter(activity_id=activity_id).filter(name=name)
                return node[0]
            except ActivityNode.DoesNotExist:
                print("Activity node with the following name could not be found: " + str(name))
                return
        else:
            print("Activity node with the following id could not be found: " + str(node_id))
            return


def find_activity_edge(activity_id, edge_id, guard=None):
    try:
        return ActivityEdge.objects.filter(activity_id=activity_id).filter(id=edge_id).first()
    except ActivityEdge.DoesNotExist:
        if guard:
            try:
                return ActivityEdge.objects.filter(activity_id=activity_id).filter(guard=guard).first()
            except ActivityEdge.DoesNotExist:
                print("Activity node with the following name could not be found: " + str(guard))
                return
        else:
            print("Activity node with the following id could not be found: " + str(edge_id))
            return


def find_action(action_id):
    try:
        return Action.objects.get(id=action_id)
    except Action.DoesNotExist:
        print("Action with the following id could not be found: " + str(action_id))
        return


def find_operation_action(operation_action_id):
    try:
        return OperationAction.objects.get(id=operation_action_id)
    except OperationAction.DoesNotExist:
        print("Operation action with the following id could not be found: " + str(operation_action_id))
        return


def delete_item(activity_id, obj_type, pk):
    """Orchestration of the deletion of nodes and connections.

    Args:
        obj_type (str): the string representing a node or connection
        pk (str): primary key in the table
    """
    if obj_type == "node":
        node = find_activity_node(activity_id, pk)
        if node:
            node.delete()
            return True, ""
        else:
            return False, "Activity node not found, id: " + str(pk)

    if obj_type == "connection":
        edge = find_activity_edge(activity_id, pk)
        if edge:
            edge.delete()
            return True, ""
        else:
            return False, "Activity edge not found, id: " + str(pk)

    if obj_type == "operation":
        operation = find_operation_action(pk)
        if operation:
            operation.delete()
            return True, ""
        else:
            return (
                False,
                "Operation action with the following id could not be found: " + str(pk),
            )
    if obj_type == "behaviorAction":
        return delete_activity_behavior_action(pk)
    if obj_type == "activity":
        activity = get_activity(pk)
        if activity:
            activity.delete()
            return True, ""
        else:
            return False, "Activity not found, id: " + str(pk)


def set_activity_behavior_action(obj):
    """Create a new link from Action to Activity.

    As a precondition the Action and Activity must exist.

    Args:
        obj (dict): json object representing the information of the new object.
    """
    action = find_action(obj.get("action"))
    if action:
        behavior_action = get_activity(obj.get("activity_id"))
        if behavior_action:
            behavior_action.callBehaviorAction_id = action.id
            behavior_action.save()
            return True, ""
        else:
            return False, "Activity not found, id: " + str(obj.get("activity_id"))
    else:
        return False, "Action not found" + str(obj.get("action"))


def delete_activity_behavior_action(pk):
    """Delete the reference to a activity from an action.

    Args:
        pk (str): primary key in the table
    """
    activity = get_activity(pk)
    if activity:
        activity.callBehaviorAction_id = None
        activity.save()
        return True, ""
    else:
        return False, "Activity not found, id: " + str(pk)


def new_item(activity_id, obj_type, obj, key):
    """Orchestrate the creation of new objects.

    Args:
        obj_type (str): the type of the new created object.
        key (str): key to the node from the frontend. (uuid)
        obj (dict): json object representing the information of the new object.
    """
    if obj.get("type") in NODE_TYPES:
        return new_node(obj.get("type"), obj, activity_id)
    if obj_type == "connection":
        return new_relationship(obj, activity_id, key)
    if obj_type == "operation":
        return new_operation(obj)
    if obj_type == "behaviorAction":
        return set_activity_behavior_action(obj)
    if obj_type == "activity":
        return new_activity(obj)


def new_node(node_type, obj, activity_id):
    """Create a node for the defined node type.

    Args:
        - node_type (str): the type of node
        - key (str): the key to the node
        - obj (dict): information representing the object
    """
    if activity_id is None:
        return False, "Activity id is None"
    else:
        activity = get_activity(activity_id)

    if activity:
        if node_type in NODE_TYPES and node_type != "Action":
            node_objects = retrieve_node_objects()
            node = node_objects[node_type].create(activity=activity, name=obj.get("name"))
        elif node_type == "Action":
            node = Action.objects.create(
                name=obj.get("name"),
                description=obj.get("description"),
                activity=activity,
            )
        else:
            return False, "Unknown activity node type:" + node_type
        activity_node = find_activity_node(activity_id, node.id)
        if activity_node:
            activity_node.xpos = obj["position"].get("x")
            activity_node.ypos = obj["position"].get("y")
            activity_node.save()
            return True, node.id
        else:
            return False, "Activity node could not be found, id: " + str(node.id)
    else:
        return False, "Activity not found, id: " + str(activity_id)


def new_relationship(obj, activity_id, key):
    """Create a relationship for the object.

    Args:
        obj (dict): the object defining the relationship
    """
    print("keys: " + str(key))
    node_from = find_activity_node(activity_id, obj.get("from"), name=key[0] if isinstance(key, list) else None)
    if not node_from:
        return False, "Activity node not found, id: " + str(obj.get("from"))

    node_to = find_activity_node(activity_id, obj.get("to"), name=key[1] if isinstance(key, list) else None)
    if not node_to:
        return False, "Activity node not found, id: " + str(obj.get("to"))

    activity = get_activity(activity_id)
    if not activity:
        return False, "Activity not found, id: " + str(activity)

    ActivityEdge.objects.create(
        incoming_node=node_from,
        outgoing_node=node_to,
        activity=activity,
        guard=obj.get("guard") or obj.get("label"),
        weight=obj.get("weight"),
    )
    return True, ""


def new_operation(activity_id, obj):
    """Create new operation for the object.

    Args:
        obj (dict): json object representing the information of the new object.
    """
    action = find_action(obj.get("action"))
    if action:
        OperationAction.objects.create(
            name=obj.get("name"),
            implementation=obj.get("code"),
            callOperationAction=action,
            type=obj.get("type"),
        )
    else:
        return False, "Action not found" + str(obj.get("action"))


def new_activity(obj):
    """Create new Activity for the given object.

    Args:
        obj (dict): json object representing the information for the activity.
    """
    action = None
    if "action" in obj and obj["action"]:
        action = find_action(obj.get("action"))

    if not obj.get("activity_model_pk"):
        return False, "Activity model pk not given."

    try:
        activity_mod = Model.objects.filter(type="activity").get(pk=obj.get("activity_model_pk"))
    except Model.DoesNotExist:
        return False, "Activity model does not exist."

    Activity.objects.create(
        name=obj.get("name"),
        precondition=obj.get("precondition") or "",
        postcondition=obj.get("postcondition") or "",
        isReadOnly=obj.get("isReadOnly") or False,
        isSingleExecution=obj.get("isSingleExecution") or False,
        callBehaviorAction_id=action,
        activity_model=activity_mod,
    )
    return True, ""


def retype_item(activity_id, obj_type, obj):
    """Retype certain properties of nodes and connections.

    Args:
        obj_type (str): type of object to be retyped
        obj (dict): contains the data about the object
    """
    if obj_type == "node":
        return retype_node(activity_id, obj)
    if obj_type == "connection":
        return retype_connection(activity_id, obj)
    if obj_type == "operation":
        return retype_operation(obj)
    if obj_type == "behaviorAction":
        return retype_behavior_action(obj)
    if obj_type == "activity":
        return retype_activity(obj)


def retype_node(activity_id, obj):
    """Retype certain properties of a node.

    obj examples
    {
        "id": 4,
        "retype":"name",
        "name":"new_name"
    }

    {
        "id": 4,
        "retype":"description",
        "description":"This is a new description"
    }

    {
        "id": 4,
        "retype":"position",
        "x":123,
        "y":334
    }

    Args:
        obj (dict): contains the data about the object
    """
    node = find_activity_node(activity_id, obj.get("id"))
    if node:
        if obj.get("retype") == "name":
            node.name = obj.get("name")
            node.save()
            return True, ""
        elif obj.get("retype") == "description":
            node.description = obj.get("description")
            node.save()
            return True, ""
        elif obj.get("retype") == "position":
            node.xpos = obj.get("x")
            node.ypos = obj.get("y")
            node.save()
            return True, ""
        else:
            return False, "node retype faild unknown retype action: " + obj.get("retype")
    else:
        return False, "Activity node not found, id: " + str(obj.get("id"))


def retype_connection(activity_id, obj):
    """Retype certain properties of a connection.

    {
        "retype":"guard" | "weight" | "mirror"
    }

    Args:
        obj (dict): contains the data about the object
    """
    selected_connection = find_activity_edge(activity_id, obj.get("id"))
    if selected_connection:
        if obj.get("retype") == "guard":
            selected_connection.guard = obj.get("guard")
            selected_connection.save()
        elif obj.get("retype") == "weight":
            selected_connection.weight = obj.get("weight")
            selected_connection.save()
        elif obj.get("retype") == "mirror":
            old_from = selected_connection.incoming_node_id
            old_to = selected_connection.outgoing_node_id
            selected_connection.incoming_node_id = old_to
            selected_connection.outgoing_node_id = old_from
            selected_connection.save()
        else:
            return False, "Activity edge retype action unknown, id:" + str(obj.get("id"))
    else:
        return False, "Activity edge not found, id: " + str(obj.get("id"))


def retype_operation(obj):
    """Retype a operation.

    obj example:
    {
        "retype":"name"|"code"|"action"|"type",
        "name"|"code"|"action"|"type": "str"
    }

    Args:
        obj (dict): contains the data about the object
    """
    operation = find_operation_action(obj.get("id"))
    if operation:
        if obj.get("retype") == "name":
            operation.name = obj.get("name")
            operation.save()
            return True, ""
        elif obj.get("retype") == "code":
            operation.implementation = obj.get("code")
            operation.save()
            return True, ""
        elif obj.get("retype") == "action":
            action = find_action(obj.get("action"))
            if action:
                operation.callOperationAction = action
                operation.save()
                return True, ""
            else:
                return False, "Action not found" + str(obj.get("action"))
        elif obj.get("retype") == "type":
            operation.type = obj.get("type")
            operation.save()
            return True, ""
        else:
            return (
                False,
                "Unknown retype action for operations, retype action: " + obj.get("retype"),
            )
    else:
        return (
            False,
            "Operation action with the following id could not be found: " + str(obj.get("id")),
        )


def retype_behavior_action(obj):
    """Retype the behavior action in the Activity Table.

    obj example
    {
        "action":"uuid - to action_id"
        TODO More?
    }
    Args:
        obj (dict): contains the data about the object
    """
    # get current action id
    action = find_action(obj.get("action"))
    if not action:
        return False, "Action not found, id: " + str(obj.get("action"))
    activity = Activity.objects.get(callBehaviorAction_id=action.id)
    if not activity:
        return False, "Activity not found, id: " + str(action.id)
    success, message = delete_activity_behavior_action(activity.id)
    if not success:
        return False, message
    success, message = set_activity_behavior_action(obj)
    if not success:
        return False, message
    return True, ""


def retype_activity(obj):
    """Retype an Activity.

    obj example:
    {
        "retype": "name"|"precondition"|"postcondition"|"isReadOnly"|"IsSingleExecution",
        "name"|"precondition"|"postcondition"|"isReadOnly"|"IsSingleExecution": "str"
    }

    Args:
        obj (dict): representing the data of the activity.
    """
    activity = get_activity(obj.get("id"))
    if activity:
        retype = obj.get("retype")
        if retype == "name":
            activity.name = obj.get("name")
            activity.save()
            return True, ""
        elif retype == "precondition":
            activity.precondition = obj.get("precondition")
            activity.save()
            return True, ""
        elif retype == "postcondition":
            activity.postcondition = obj.get("postcondition")
            activity.save()
            return True, ""
        elif retype == "isReadOnly":
            activity.isReadOnly = obj.get("isReadOnly")
            activity.save()
            return True, ""
        elif retype == "isSingleExecution":
            activity.isSingleExecution = obj.get("isSingleExecution")
            activity.save()
            return True, ""
        else:
            return False, "Retype action unknown: " + retype
    else:
        return False, "Activity not found, id: " + str(obj.get("id"))


def get_all_weighted_edges(activity_id: int) -> dict:
    """Get all weighted edges in an activity model."""
    all_filled_edges = ActivityEdge.objects.filter(activity_id=activity_id).exclude(guard="", weight="")
    edges_per_incoming_node = {}
    for edge in all_filled_edges:
        incoming_id = edge.incoming_node.id
        if incoming_id in edges_per_incoming_node:
            edges_per_incoming_node[incoming_id].append(edge)
        else:
            edges_per_incoming_node[incoming_id] = [edge]
    return edges_per_incoming_node


def get_activity_result_print_format(activity_id: int) -> str:
    """Get the results of a created activity model and format them to be printed."""
    all_nodes = ActivityNode.objects.filter(activity_id=activity_id)
    action_node_ids = [action.id for action in ExecutableNode.objects.filter(activity_id=activity_id)]
    incoming_weighted_nodes = get_all_weighted_edges(activity_id)
    text_output = "node_id \t node_text \t next_node_id \t condition?\n"
    first_id = all_nodes.first().id
    diff_to_start = first_id - 1 if first_id > 0 else 0
    for node in all_nodes:
        node_id = node.id
        print_node_id = node_id - diff_to_start
        node_name = node.name
        if node.id in action_node_ids:
            node_name = f"Action: {node.name}"
        print_line = f"{print_node_id} \t {node_name}"
        if node_id in incoming_weighted_nodes:
            # Node has weighted nodes, so we check for mulitple
            edges = incoming_weighted_nodes[node_id]
            condition_output = None
            for edge in edges:
                if not condition_output:
                    condition_output = ""
                else:
                    condition_output += "\n"
                condition_line = edge.guard
                outgoing_node_id = edge.outgoing_node.id - diff_to_start
                condition_output += print_line + f"\t {outgoing_node_id} \t {condition_line}"
            text_output += condition_output + "\n"
        else:
            possible_edges = ActivityEdge.objects.filter(activity_id=activity_id, incoming_node=node_id)
            edge = possible_edges.first()
            if not edge:
                text_output += print_line + "\n"
            else:
                outgoing_node_id = edge.outgoing_node.id - diff_to_start
                text_output += print_line + f"\t {outgoing_node_id} \n"
    return text_output
