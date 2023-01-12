from ..models import *
from ..generators import *
import os
import json
import uuid
from ..views.activityInterface import ActivityFrontendInterface
from threading import Thread
from django.apps import apps as django_apps
from rest_framework.decorators import api_view, schema
from rest_framework.response import Response
from rest_framework import status


class FrontendInterface:
    def __init__(self):
        self.nodes = dict()
        self.connections = dict()
        self.changes = dict()

    def classifierOperations(self, classifier):
        operations = Operation.objects.filter(classifier=classifier)

        return [
            {'name': operation.name, 'type': operation.type, 'code': operation.implementation, 'id': operation.id}
            for operation in operations
        ]

    def classifierProperties(self, classifier):
        var = django_apps.get_model('shared', classifier.name)
        inherited = var.get_deferred_fields(var)
        properties = Property.objects.filter(classifier=classifier)

        return [
            {'name': prop.name, 'type': prop.type, 'id': prop.id} for prop in properties if prop.name not in inherited
        ]

    def node(self, classifier):
        return {
            'type': 'Class',
            'name': str(classifier),
            'data': {
                'properties': self.classifierProperties(classifier),
                'methods': self.classifierOperations(classifier),
            },
            'instances': {},
            'id': classifier.id,
        }

    def connection(self, relation, relation_type):
        if relation_type == 'generalization':
            m_from = ''
            m_to = ''
        else:
            m_from = relation.multiplicity_from
            m_to = relation.multiplicity_to
        return dict(
            {
                str(uuid.uuid4()): {
                    'name': str(relation),
                    'type': relation_type,
                    'labelFrom': str(m_from),
                    'label': str(relation),
                    'labelTo': str(m_to),
                    'from': self.nodeUUID(str(relation.classifier_from)),
                    'to': self.nodeUUID(str(relation.classifier_to)),
                    'id': relation.id,
                }
            }
        )

    def populateNodes(self):
        for classifier in Classifier.objects.all():
            self.nodes[str(uuid.uuid4())] = self.node(classifier)

    def nodeUUID(self, name):
        for key, value in self.nodes.items():
            if value.get('name') == name:
                return key
        return ''

    def populateConnections(self):
        compositions = Composition.objects.all()
        associations = Association.objects.all()
        generalizations = Generalization.objects.all()

        for relation in compositions:
            self.connections = {**self.connections, **self.connection(relation, 'composition')}

        for relation in associations:
            self.connections = {**self.connections, **self.connection(relation, 'association')}

        for relation in generalizations:
            self.connections = {**self.connections, **self.connection(relation, 'generalization')}

    def request(self):
        self.populateNodes()
        self.populateConnections()
        response = Response({'nodes': self.nodes, 'connections': self.connections})
        return response

    def delete(self, obj_type, pk):
        if obj_type == 'property' or obj_type == 'method':
            p = Property.objects.get(id=pk)
            print(p)
            PropertyGenerator(p).delete(False)
            p.delete()
        if obj_type == 'connection':
            r = Relationship.objects.get(id=pk)
            r.delete()
        if obj_type == 'classifier':
            c = Class.objects.get(id=pk)
            ClassifierGenerator(c).delete(False)
            c.delete()

    def newRelationship(self, obj):
        classifier_from = Class.objects.get(id=self.nodes.get(obj.get('from')).get('id'))

        classifier_to = Class.objects.get(id=self.nodes.get(obj.get('to')).get('id'))

        if obj.get('type') == 'generalization':
            return Generalization.objects.create(
                name=obj.get('label'), classifier_from=classifier_from, classifier_to=classifier_to
            )

        if obj.get('type') == 'association':
            return Association.objects.create(
                name=obj.get('label'),
                classifier_from=classifier_from,
                classifier_to=classifier_to,
                multiplicity_from=obj.get('labelFrom'),
                multiplicity_to=obj.get('labelTo'),
            )

        if obj.get('type') == 'composition':
            return Composition.objects.create(
                name=obj.get('label'),
                classifier_from=classifier_from,
                classifier_to=classifier_to,
                multiplicity_from=obj.get('labelFrom'),
                multiplicity_to=obj.get('labelTo'),
            )

    def new(self, obj_type, key, obj):
        if obj_type == 'classifier':
            c = Class.objects.create(name=obj.get('name'))
            self.nodes[key] = {'id': c.id}
            ClassifierGenerator(c).generate(False)
        if obj_type == 'connection':
            c = self.newRelationship(obj)
            RelationshipGenerator(c).generate(False)
        if obj_type == 'method':
            c = Class.objects.get(name=key.get('name'))
            oper = Operation.objects.create(
                name=obj.get('name'), implementation=obj.get('code'), classifier=c, type=obj.get('type')
            )
            OperationGenerator(oper).generate()
        if obj_type == 'property':
            c = Class.objects.get(name=key.get('name'))
            prop = Property.objects.create(name=obj.get('name'), classifier=c, type=obj.get('type'))
            PropertyGenerator(prop).generate(False)

    def retype(self, obj_type, obj):
        if obj_type == 'method':
            op = Operation.objects.get(id=obj.get('id'))
            op.type = obj.get('type')
            op.save()
        if obj_type == 'property':
            prop = Property.objects.get(id=obj.get('id'))
            prop.type = obj.get('type')
            prop.save()

    def modifyConnection(self, action, obj):
        if obj.get('type') == 'generalization':
            relation = Generalization.objects.get(relationship_ptr_id=obj.get('id'))
        if obj.get('type') == 'association':
            relation = Association.objects.get(relationship_ptr_id=obj.get('id'))
        if obj.get('type') == 'composition':
            relation = Composition.objects.get(relationship_ptr_id=obj.get('id'))
        if relation and action == 'mirror':
            old_from = relation.classifier_from
            old_to = relation.classifier_to
            old_mfrom = relation.multiplicity_from or ''
            old_mto = relation.multiplicity_to or ''
            relation.classifier_to = old_from
            relation.classifier_from = old_to
            relation.multiplicity_from = old_mto
            relation.multiplicity_to = old_mfrom
            relation.save()
        if relation and action == 'rename':
            relation.name = obj.label
            relation.save()
        if relation and action == 'cardinality-from':
            relation.multiplicity_from = obj.get('labelFrom')
            relation.save()
        if relation and action == 'cardinality-to':
            relation.multiplicity_to = obj.get('labelTo')
            relation.save()
        if relation:
            RelationshipGenerator(relation).generate(False)

    def push(self, request):
        body = request.body.decode('utf-8')
        body_data = json.loads(body)

        self.changes = body_data.get('changes')
        self.nodes = body_data.get('nodes')
        self.connections = body_data.get('connections')

        for item in self.changes:
            action = item.get('type')
            if action.startswith('delete'):
                self.delete(action.split('-')[1], item.get('to').get('id'))
            if action.startswith('new'):
                self.new(action.split('-')[1], item.get('key') or item.get('nodeKey'), item.get('to'))
            if action.startswith('retype'):
                self.retype(action.split('-')[1], item.get('to'))
            if action.startswith('connection'):
                cmd = action.split('-')
                if len(cmd) == 3:
                    self.modifyConnection(cmd[1] + '-' + cmd[2], item.get('from'))
                if len(cmd) == 2:
                    self.modifyConnection(cmd[1], item.get('from'))
        Thread(target=systemcall()).start()
        # use threading to call the migrations while we continue on
        return Response(status=status.HTTP_204_NO_CONTENT)


def systemcall():
    # use systemcall to get around process caching
    os.system('python manage.py makemigrations --noinput && python manage.py migrate --noinput ')
    return


@api_view(['GET', 'POST'])
@schema(None)
def data(request):
    frontend_interface = FrontendInterface()
    activity = ActivityFrontendInterface()
    if request.method == 'POST':
        if request.query_params.get('uml-type', '') == 'activity':
            return activity.push(request)
        else:
            return frontend_interface.push(request)
    else:
        if request.query_params.get('uml-type', '') == 'activity':
            request_type = request.query_params.get('request-type', '')
            activity_id = request.query_params.get('activity-id', '')
            return activity.request(request_type, activity_id)
        else:
            return frontend_interface.request()
