"""ActivityInterface to define an interface for the Activity Model.

This is a description todo based on https://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_google.html

Todo:
    * Finish documentation.
    * Object Flow & Object Node (future dev.)
"""
from ..models import *
import json
import uuid
from rest_framework.response import Response


class ActivityFrontendInterface:
    """The ActivityFrontendInterface class defines the interface for Activities.

    The class does not take any inputs.

    """

    def __init__(self):
        """Init method for the class.

        Initializes all the nodes, connections, changes and activities. The predefined nodeTypes are also defined here.
        """
        self.nodes = dict()
        self.connections = dict()
        self.changes = dict()
        self.activities = dict()
        self.activityName = str
        self.nodeTypes = ['Initial', 'FlowFinal', 'ActivityFinal', 'Fork', 'Merge', 'Join', 'Decision', 'Action']

    def test1(self):
        """Create a test set of activities."""
        #    All in one activity
        #
        #    Initial -> Write name -> Read name -> Check name -> FinalNode
        #
        a = Activity.objects.create(name='Test10')
        i = InitialNode.objects.create(activity=a)
        aWrite = Action.objects.create(name='Write_name', description='Write name', activity=a)
        aRead = Action.objects.create(name='Read_name', description='Read name', activity=a)
        aCheck = Action.objects.create(name='Check_name', description='Check name', activity=a)
        f = ActivityFinalNode.objects.create(activity=a)
        e1 = ActivityEdge.objects.create(incoming_node=i, outgoing_node=aWrite, activity=a)
        e2 = ActivityEdge.objects.create(incoming_node=aWrite, outgoing_node=aRead, activity=a)
        e3 = ActivityEdge.objects.create(incoming_node=aRead, outgoing_node=aCheck, activity=a)
        e4 = ActivityEdge.objects.create(incoming_node=aCheck, outgoing_node=f, activity=a)

    def ActionOperations(self, node):
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
            {'name': operation.name, 'type': operation.type, 'code': operation.implementation, 'id': operation.id}
            for operation in operations
        ]

    def nodeCallBehaviorAction(self, node):
        """Return callbehavioraction id if it exists.

        Args:
            node (): The node for which the operations are generated.

        Returns:
            id of the activity, if it had a FK to the given node.
            0, if there is no activity for the given node.
        """
        try:
            action = Action.objects.get(id=node.id)
            behAction = Activity.objects.filter(callBehaviorAction_id=action.id).first()
        except Exception as _:
            return 0
        if behAction:
            return behAction.id
        else:
            return 0

    def node(self, activityNode, node_type, node_id, node_activity_id):
        """Create a node that can be returned."""
        nodeInfo = ActivityNode.objects.get(id=node_id)
        return {
            'type': node_type,
            'name': str(nodeInfo.name),
            'data': {
                'description': nodeInfo.description,
                'activity_id': node_activity_id,
                'operations': self.ActionOperations(nodeInfo),
                'callBehaviorAction': self.nodeCallBehaviorAction(nodeInfo),
            },
            'instances': {},
            'id': node_id,
            'position': {'x': nodeInfo.xpos, 'y': nodeInfo.ypos},
        }

    def connection(self, edge):
        """Create a connection that can be returned."""
        return dict(
            {
                str(uuid.uuid4()): {
                    'label': edge.guard,
                    'weight': edge.weight,
                    'from_id': str(edge.incoming_node_id),
                    'to_id': str(edge.outgoing_node_id),
                    'from': self.nodeUUID(edge.incoming_node_id),
                    'to': self.nodeUUID(edge.outgoing_node_id),
                    'type': 'edge',
                    'id': edge.id,
                }
            }
        )

    def nodeUUID(self, node_id):
        """Return a nodeUUID based on the node_id."""
        for key, value in self.nodes.items():
            if value.get('id') == node_id:
                return key
        return ''

    def activity(self, activity_obj):
        """Fill a dictionary with info to return a activity."""
        return {
            'id': activity_obj.id,
            'name': str(activity_obj.name),
            'condition': {'pre-condition': activity_obj.precondition, 'post-condition': activity_obj.postcondition},
            'isReadOnly': activity_obj.isReadOnly,
            'isSingleExecution': activity_obj.isSingleExecution,
            'callBehaviorAction_id': activity_obj.callBehaviorAction_id,
        }

    def generateControlNodeObjects(self):
        """Return Dict of control node objects."""
        nodeObjects = {}
        nodeObjects['Initial'] = InitialNode.objects
        nodeObjects['FlowFinal'] = FlowFinalNode.objects
        nodeObjects['ActivityFinal'] = ActivityFinalNode.objects
        nodeObjects['Fork'] = ForkNode.objects
        nodeObjects['Merge'] = MergeNode.objects
        nodeObjects['Join'] = JoinNode.objects
        nodeObjects['Decision'] = DecisionNode.objects
        return nodeObjects

    def getAllNodes(self):
        """Return a Dict of all nodes."""
        listOfNodes = {}
        listOfNodes = self.generateControlNodeObjects()
        for key, value in listOfNodes.items():
            listOfNodes[key] = value.all()

        listOfNodes['Action'] = Action.objects.all()

        return listOfNodes

    def getNodesById(self, activity_id):
        """Return a dict of nodes within a activity id."""
        listOfNodes = {}
        try:
            activity = Activity.objects.get(id=activity_id)
        except Exception as _:
            activity = None
        if not activity:
            return listOfNodes

        listOfNodes = self.generateControlNodeObjects()
        listOfNodes['Action'] = Action.objects

        for key, value in listOfNodes.items():
            listOfNodes[key] = value.filter(activity=activity)

        return listOfNodes

    def populateNodes(self, activity_id):
        """Populate the different nodes.

        Creates a dictionary with all the types as key and the object set as value. Than for
        each of the object sets the nodes are added to the self.nodes
        """
        listOfNodes = {}

        if activity_id == -1:
            listOfNodes = self.getAllNodes()
        elif activity_id == 0:
            # find first activity with no callbehaviorAction id
            firstAct = Activity.objects.filter(callBehaviorAction_id__exact=None).first()
            if not firstAct:
                firstAct = Activity.objects.first()
            listOfNodes = self.getNodesById(firstAct.id)
        else:
            listOfNodes = self.getNodesById(activity_id)

        for key, value in listOfNodes.items():
            for node in value:
                nodeId = node.parentId()
                activityId = ActivityNode.objects.get(id=nodeId).activity_id
                self.nodes[str(uuid.uuid4())] = self.node(node, key, nodeId, activityId)

    def populateConnections(self, activity_id):
        """Populate the connections in memory."""
        # implement control and objectflow
        if activity_id == -1:
            edges = ActivityEdge.objects.all()
        elif activity_id == 0:
            activity = Activity.objects.filter(callBehaviorAction_id__exact=None).first()
            if not activity:
                activity = Activity.objects.first()
            edges = ActivityEdge.objects.filter(activity=activity)
        else:
            try:
                activity = Activity.objects.get(id=activity_id)
            except Exception as _:
                activity = None
            edges = ActivityEdge.objects.filter(activity=activity)
        for edge in edges:
            self.connections = {**self.connections, **self.connection(edge)}

    def populateActivities(self):
        """Populate the activities in memory for usage."""
        for activity_obj in Activity.objects.all():
            self.activities[str(uuid.uuid4())] = self.activity(activity_obj)

    def request(self, request_type, activity_id):
        """Process the request from the frontend.

        Returns:
            HttpResponse with json, that can contain
            activities or nodes and connections.
        """
        if request_type == 'activities':
            # Return a list of activities
            self.populateActivities()
            print(self.activities)
            return Response({"activities": self.activities})
        else:
            if activity_id != '':
                ac_id = int(activity_id)
                self.populateNodes(ac_id)
                self.populateConnections(ac_id)
            else:
                self.populateNodes(-1)
                self.populateConnections(-1)
            print(self.nodes)
            print(self.connections)
            return Response({"nodes": self.nodes, "connections": self.connections})

    def setActivityBehaviorAction(self, obj):
        """Create a new link from Action to Activity.

        As a precondition the Action and Activity must exist.

        Args:
            obj (json): json object representing the information of the new object.
        """
        action_id = self.nodes.get(obj.get('action')).get('id')
        try:
            action = Action.objects.get(id=action_id)
            behaviorAction = Activity.objects.get(id=obj.get('activity_id'))
        except Exception as _:
            print("newBehaviorAction: Action or Activity not found")
            return
        behaviorAction.callBehaviorAction_id = action.id
        behaviorAction.save()

    def deleteActivityBehaviorAction(self, pk):
        """Delete the reference to a activity from an action.

        Args:
            pk (str): primary key in the table
        """
        activity = Activity.objects.get(id=pk)
        activity.callBehaviorAction_id = None
        activity.save()

    def delete(self, obj_type, pk):
        """Orchestration of the deletion of nodes and connections.

        Args:
            obj_type (str): the string representing a node or connection
            pk (str): primary key in the table
        """
        if obj_type == 'node':
            n = ActivityNode.objects.get(id=pk)
            n.delete()
        if obj_type == 'connection':
            edge = ActivityEdge.objects.get(id=pk)
            edge.delete()
        if obj_type == 'operation':
            operation = OperationAction.objects.get(id=pk)
            operation.delete()
        if obj_type == 'behaviorAction':
            self.deleteActivityBehaviorAction(pk)
        if obj_type == 'activity':
            a = Activity.objects.get(id=pk)
            a.delete()

    def newNode(self, obj_type, key, obj, activity_id):
        """Create a node for the defined node type.

        Args:
            obj_type (str): the type of node
            key (str): the key to the node
            obj (dict): information representing the object
        """
        # TODO
        activity = None
        if activity_id is None:
            print("Activity id is None")
            return
        else:
            activity = Activity.objects.get(id=activity_id)

        if obj_type in self.nodeTypes and obj_type != 'Action':
            listObjects = self.generateControlNodeObjects()
            node = listObjects[obj_type].create(activity=activity, name=obj.get('name'))
        elif obj_type == 'Action':
            node = Action.objects.create(name=obj.get('name'), description=obj.get('description'), activity=activity)
        ac_node = ActivityNode.objects.get(pk=node.id)
        ac_node.xpos = obj['position'].get('x')
        ac_node.ypos = obj['position'].get('y')
        ac_node.save()

        print("nodeID: {}".format(node.id))
        self.nodes[key] = {'id': node.id}

    def newRelationship(self, obj, activity_id):
        """Create a relationship for the object.

        Args:
            obj (?): the object defining the relationship
        """
        node_from_id = self.nodes.get(obj.get('from')).get('id')
        node_from = ActivityNode.objects.get(id=node_from_id)

        node_to_id = self.nodes.get(obj.get('to')).get('id')
        node_to = ActivityNode.objects.get(id=node_to_id)

        activity = Activity.objects.get(id=activity_id)
        return ActivityEdge.objects.create(
            incoming_node=node_from,
            outgoing_node=node_to,
            activity=activity,
            guard=obj.get('guard'),
            weight=obj.get('weight'),
        )

    def newActivity(self, obj):
        """Create new Activity for the given object.

        Args:
            obj (json): json object representing the information for the activity.
        """
        if obj.get('action') != '':
            action_id = self.nodes.get(obj.get('action')).get('id')
            try:
                action = Action.objects.get(id=action_id)
            except Exception as _:
                action = None
        else:
            action = None
        activity = Activity.objects.create(
            name=obj.get('name'),
            precondition=obj.get('precondition') or '',
            postcondition=obj.get('postcondition') or '',
            isReadOnly=obj.get('isReadOnly') or False,
            isSingleExecution=obj.get('isSingleExecution') or False,
            callBehaviorAction_id=action,
        )
        return activity

    def newOperation(self, obj):
        """Create new operation for the object.

        Args:
            obj (json): json object representing the information of the new object.
        """
        action_id = self.nodes.get(obj.get('action')).get('id')
        try:
            action = Action.objects.get(id=action_id)
        except Exception as _:
            print("newOperation: Action not found in Actions")
            return
        oper = OperationAction.objects.create(
            name=obj.get('name'), implementation=obj.get('code'), callOperationAction=action, type=obj.get('type')
        )

    def new(self, obj_type, key, obj):
        """Orchestrate the creation of new objects.

        Args:
            obj_type (str): the type of the new created object.
            key (str): key to the node from the frontend. (uuid)
            obj (json): json object representing the information of the new object.
        """
        activity_id = obj.get('activity_id')
        if obj.get('type') in self.nodeTypes:
            self.newNode(obj.get('type'), key, obj, activity_id)
        if obj_type == 'connection':
            self.newRelationship(obj, activity_id)
        if obj_type == 'operation':
            self.newOperation(obj)
        if obj_type == 'behaviorAction':
            self.setActivityBehaviorAction(obj)
        if obj_type == 'activity':
            self.newActivity(obj)

    def retypeNode(self, obj_type, obj):
        """Retype certain properties of a node.

        Args:
            obj_type (str): type of object to be retyped
            obj (str): contains the data about the object
        """
        node = ActivityNode.objects.get(id=obj.get('id'))
        if obj.get('retype') == 'name':
            node.name = obj.get('name')
            node.save()
        if obj.get('retype') == 'description':
            node.description = obj.get('description')
            node.save()
        if obj.get('retype') == 'position':
            node.xpos = obj.get('x')
            node.ypos = obj.get('y')
            node.save()

    def retypeConnection(self, obj_type, obj):
        """Retype certain properties of a connection.

        Args:
            obj_type (str): type of object to be retyped
            obj (str): contains the data about the object
        """
        connection = ActivityEdge.objects.get(id=obj.get('id'))
        if obj.get('retype') == 'guard':
            connection.guard = obj.get('guard')
            connection.save()
        if obj.get('retype') == 'weight':
            connection.weight = obj.get('weight')
            connection.save()
        if obj.get('retype') == 'mirror':
            old_from = connection.incoming_node_id
            old_to = connection.outgoing_node_id
            connection.incoming_node_id = old_to
            connection.outgoing_node_id = old_from
            connection.save()

    def retypeOperation(self, obj_type, obj):
        """Retype a operation.

        Args:
            obj_type (str): type of object to be retyped
            obj (str): contains the data about the object
        """
        try:
            operation = OperationAction.objects.get(id=obj.get('id'))
        except Exception as _:
            print("retypeOperation: Operation not found.")
            return
        if obj.get('retype') == 'name':
            operation.name = obj.get('name')
            operation.save()
        if obj.get('retype') == 'code':
            operation.implementation = obj.get('code')
            operation.save()
        if obj.get('retype') == 'action':
            try:
                # get new action
                action_id = self.nodes.get(obj.get('action')).get('id')
                action = Action.objects.get(pk=action_id)
            except Exception as _:
                print("retypeOperation: New action not found.")
                return
            operation.callOperationAction = action
            operation.save()
        if obj.get('retype') == 'type':
            operation.type = obj.get('type')
            operation.save()

    def retypeBehaviorAction(self, obj):
        """Retype the behavior action in the Activity Table.

        Args:
            obj (str): contains the data about the object
        """
        # get current action id
        try:
            action_id = self.nodes.get(obj.get('action')).get('id')
            action = Action.objects.get(pk=action_id)
            activity = Activity.objects.get(callBehaviorAction_id=action.id)
        except Exception as _:
            print("retypeBehaviorAction: Action or Activity not found.")
            return
        self.deleteActivityBehaviorAction(activity.id)
        self.setActivityBehaviorAction(obj)

    def retypeActivity(self, obj):
        """Retype an Activity.

        Args:
            obj (json): representing the data of the activity.
        """
        try:
            activity = Activity.objects.get(pk=obj.get('id'))
        except Exception as _:
            print("retypeActivity: Activity not found.")
            return
        retype = obj.get('retype')
        if retype == 'name':
            activity.name = obj.get('name')
            activity.save()
        if retype == 'precondition':
            activity.precondition = obj.get('precondition')
            activity.save()
        if retype == 'postcondition':
            activity.postcondition = obj.get('postcondition')
            activity.save()
        if retype == 'isReadOnly':
            activity.isReadOnly = obj.get('isReadOnly')
            activity.save()
        if retype == 'isSingleExecution':
            activity.isSingleExecution = obj.get('isSingleExecution')
            activity.save()
        return activity

    def retype(self, obj_type, obj):
        """Retype certain properties of nodes and connections.

        Args:
            obj_type (str): type of object to be retyped
            obj (str): contains the data about the object
        """
        if obj_type == 'node':
            self.retypeNode(obj_type, obj)
        if obj_type == 'connection':
            self.retypeConnection(obj_type, obj)
        if obj_type == 'operation':
            self.retypeOperation(obj_type, obj)
        if obj_type == 'behaviorAction':
            self.retypeBehaviorAction(obj)
        if obj_type == 'activity':
            self.retypeActivity(obj)

    def push(self, request):
        """Push changes to the backend of the system.

        Description:
            Push the changes of the activities to the database of the system.

            Preliminary: the changes must first start with the activity. If it exists use retype to get it otherwise create an activity.

            TODO: This is not the correct way. An activity should not be the first to

        Args:
            request (?): request from the webpage
        """
        body = request.body.decode('utf-8')
        body_data = json.loads(body)

        self.changes = body_data.get('changes')
        self.nodes = body_data.get('nodes')
        self.connections = body_data.get('connections')
        item = self.changes[0]
        first_activity = item.get('type')
        if first_activity.startswith('new'):
            activity = self.newActivity(item.get('to'))
        if first_activity.startswith('retype'):
            activity = self.retypeActivity(item.get('to'))
        print(self.changes[0])
        print(activity)
        del self.changes[0]

        for item in self.changes:
            action = item.get('type')
            to = item.get('to')
            if to.get('activity_id') is None:
                to['activity_id'] = activity.id
            if action.startswith('delete'):
                self.delete(action.split('-')[1], item.get('key').get('id'))
            if action.startswith('new'):
                self.new(action.split('-')[1], item.get('key') or item.get('nodeKey'), to)
            if action.startswith("retype"):
                self.retype(action.split("-")[1], to)
        return Response("Ok")
