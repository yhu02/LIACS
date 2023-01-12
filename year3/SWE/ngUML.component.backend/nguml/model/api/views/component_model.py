"""Component Model API"""

from rest_framework import viewsets, serializers
from model.api.serializers import (ComponentSerializer, ModelSerializer, PortSerializer, InterfaceSerializer, NodeSerializer)
from rest_framework.viewsets import GenericViewSet
from rest_framework.mixins import RetrieveModelMixin
from rest_framework.decorators import action
from rest_framework.response import Response
from rest_framework import status
from model.models import (
    Project,
    System,
    Model,
)
from nguml.utils import IdSerializer
from model.models import *
import json
from django.http import JsonResponse
from django.db.models import Q
from ...enums import ModelTypes
from django.forms.models import model_to_dict


class ComponentModelViewSet(viewsets.ReadOnlyModelViewSet):
    """
    A viewset for viewing and editing component model instances and their relations.
    This viewset contains the following endpoints:
    - edit_component (PUT)
    - add_component (POST)
    - add_interface (POST)
    - delete_component (DELETE)
    - list_components (GET)
    - add_model (POST)
    - all_components (GET)
    - switch_port (PUT) 
    - all_ports (GET)
    - all_interfaces (GET)
    - add_model (POST)
    - get_model (GET)
    - add_relation (POST)
    - edit_relation (PUT)
    - list_relations (GET)
     
    """
    lookup_field = 'type'
    serializer_id = IdSerializer

    def get_queryset(self):
        return Model.objects.filter(system=self.kwargs['system_id'], type=self.kwargs['type'])

    def validate(self, request, system, project):
        if not project:
            return Response({"detail": "A project id is required"}, status=status.HTTP_400_BAD_REQUEST)
        if not system:
            return Response({"detail": "A system id is required"}, status=status.HTTP_400_BAD_REQUEST)
        try:
            self.project = Project.objects.get(pk=project)
            self.system = System.objects.filter(project=project).get(pk=system)
            return True
        except System.DoesNotExist:
            return Response(
                {"detail": "The requested system cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
        except Project.DoesNotExist:
            return Response(
                {"detail": "The requested project cannot be found in the given system"},
                status=status.HTTP_404_NOT_FOUND,
            )
            
    # Edit an existing component   
    @action(detail=True, methods=['PUT'])
    def edit_component(self, request, *args, **kwargs):
        data = request.data
        # Convert data dict to database compatible dict
        if 'style' in data.keys():
            if 'backgroundColor' in data['style'].keys():
                data['backgroundColor'] = data['style']['backgroundColor']
            data['width'] = data['style']['width']
            data['height'] = data['style']['height']
            data['border'] = data['style']['border']
            del data['style']
        if 'position' in data.keys():
            data['x'] = data['position']['x']
            data['y'] = data['position']['y']
            del data['position']
        if 'data' in data.keys():
            data['name'] = data['data']['label']
            del data['data']
        if 'type' in data.keys():
            data['nodeType'] = data.pop('type')
        if 'parentNode' in data.keys():
            data['parent'] = data.pop('parentNode')
            if not bool(data['parent']):
                data['parent'] = None
        id = str(data['id'])
        del data['id']
    
        Component.objects.filter(id=id).update(**data)
        
        return Response(model_to_dict(Component.objects.get(id=id)))
        
    # Add a new component to a model
    @action(detail=True, methods=['POST'])    
    def add_component(self, request, *args, **kwargs):
        data = request.data
        style = request.data['style']
        # If no parent set parentNode to None instead of ""
        if not bool(data['parentNode']):
            data['parentNode'] = None
        comp = Component(
            name = data['data']['label'],
            parent_id = data['parentNode'],
            x = data['position']['x'], 
            y = data['position']['y'],
            model_id = data['model'],
            background_color = style['backgroundColor'],
            width = style['width'],
            height = style['height'],
            class_name = data['className'],
            node_type = data['type'],
            border = data['style']['border'],
        )
        comp.save()
        
        # For debugging purposes
        d = {"node": model_to_dict(comp)}
        
        # If the component is an interface, make a relation to the given target
        if comp.node_type in ['socketUp', 'socketDown', 'socketRight', 'socketLeft', 'circle']:
            rel = make_relation(comp, data['target'])
            Relation.objects.filter(id=rel.id).update(label = str(rel.id))
            d['edge'] = model_to_dict(rel)        
        
        return Response(d)
    
       # Add a new interface to a model
    @action(detail=True, methods=['POST'])    
    def add_interface(self, request, *args, **kwargs):
        data = request.data
        if not bool(data['parentNode']):
            data['parentNode'] = None
        comp = Interface(
            name = data['data']['label'],
            parent_id = data['parentNode'],
            x = data['position']['x'], 
            y = data['position']['y'],
            model_id = data['model'],
        )
        comp.save()
        return Response(model_to_dict(comp))
        
    # Delete a component from a model
    @action(detail=True, methods=['DELETE'])
    def delete_component(self, request, *args, **kwargs):
        id = str(request.data['id'])
        comp = Component.objects.get(id=id)
        comp.delete()
        
        return Response(True)
    
   # Return a list of all component names
    @action(detail=True, methods=['GET'])
    def list_components(self, request, *args, **kwargs):
        all_comps = Component.objects.values_list('name', 'id')
        all_comps = list(all_comps)
        
        return Response(all_comps)
    
    # Add a new model to the database
    @action(detail=True, methods=['POST'])
    def add_model(self, request, *args, **kwargs):
        data = request.data 
        model = Model(
            title = data['title'],
            system_id = data['system'],
            raw_text = data['raw_text'],
            type = data['type'],
            time = data['time']
        )
        model.save()
        
        return Response(model_to_dict(model))
        
   # Return a list of all component names
    @action(detail=True, methods=['GET'])
    def all_components(self, request, *args, **kwargs):
        self.serializer_class = ComponentSerializer
        items = Component.objects.values()
        items = list(items)
        return Response(items)

    # Switch to the other port of the component
    @action(detail=True, methods=['PUT'])
    def switch_port(self, request, *args, **kwargs):
        id = request.data['id']
        edge = Relation.objects.get(id = id)
        original_source = edge.source
        edge.source = edge.target
        edge.target = original_source
        edge.save()
        return Response(model_to_dict(edge))

    # Return a list of all component names
    @action(detail=True, methods=['GET'])
    def all_ports(self, request, *args, **kwargs):
        self.serializer_class = PortSerializer
        items = Port.objects.all()
        ports = list(Port.objects.values())
        items = list(items)

        for i in range(len(items)):
            item = items[i]
            provided = list(item.provided.values('id'))
            required = list(item.required.values('id'))

            ports[i]['provided'] = provided
            ports[i]['required'] = required
      
        return Response(ports)

    # Return a list of all component names
    @action(detail=True, methods=['GET'])
    def all_interfaces(self, request, *args, **kwargs):
        self.serializer_class =InterfaceSerializer
        items = Interface.objects.values()
        items = list(items)
        return Response(items)
    
    # Add a new model to the database
    @action(detail=True, methods=['POST'])
    def add_model(self, request, *args, **kwargs):
        data = request.data 
        model = Model(
            title = data['title'],
            system_id = data['system'],
            raw_text = data['raw_text'],
            type = data['type'],
            time = data['time']
        )
        model.save()
        return Response(model_to_dict(model))
    
    # Get model data
    @action(detail=True, methods=['GET'])
    def get_model(self, request, *args, **kwargs):
        validation = self.validate(
            request, system=kwargs['system_id'], project=kwargs['project_id'])
        if (validation == True):
            model = get_model(self.kwargs['system_id'], self.kwargs['type'])
            nodes = get_topnode(model['id'])
            edges = list(Relation.objects.all().values())
            
            # Convert the node data to react flow compatible json
            nodes = nodes_db_to_json(nodes)
            
            # Convert the edge data to react flow compatible json
            edges = edge_db_to_json(edges)
            
            result = {"nodes": nodes, "edges": edges}
            return Response(result)

    # Return a list of relations ID, source and target
    @action(detail=True, methods=['GET'])
    def list_relations(self, request, *args, **kwargs):
        all_relations = Relation.objects.values_list('id','label', 'dependency')
        all_relations = list(all_relations)
        new_relations = []
        for rel in all_relations:
            new_rel = list(rel)
            if new_rel[2]:
                new_rel.append('Dependency')
                new_rel.remove(True)
            else:
                new_rel.append('Relation')
                new_rel.remove(False)
            new_relations.append(new_rel)
        
        return Response(new_relations)
    
    # Add a new relation
    @action(detail=True, methods=['POST'])
    def add_relation(self, request, *args, **kwargs):
        data = request.data
        # Convert data to database compatible form
        if 'markerStart' in data.keys():
            data['marker_start_type'] = data['markerStart']['type']
            data['start_width'] = data['markerStart']['width']
            data['start_height'] = data['markerStart']['height']
            del data['markerStart']
        if 'markerEnd' in data.keys():
            data['marker_end_type'] = data['markerEnd']['type']
            data['end_width'] = data['markerEnd']['width']
            data['end_height'] = data['markerEnd']['height']
            del data['markerEnd']
        data['source_id'] = data.pop('source')
        data['target_id'] = data.pop('target')
        new_relation = Relation(**data)
        new_relation.save()
        
        return Response(model_to_dict(new_relation))

    # Delete a relation
    @action(detail=True, methods=['DELETE'])
    def delete_relation(self, request, *args, **kwargs):
        id = request.data['id']
        relation = Relation.objects.get(id=id)
        relation.delete()
        
        return Response(True)
                
    # Edit a relation
    @action(detail=True, methods=['PUT'])
    def edit_relation(self, request, *args, **kwargs):
        data = request.data
        id = data['id']
        del data['id']
        
        # Remove markerEnd if you have a markerStart and the other way around
        if 'markerStart' in data.keys():
            data['marker_start_type'] = data['markerStart']['type']
            data['start_width'] = data['markerStart']['width']
            data['start_height'] = data['markerStart']['height']
            del data['markerStart']
            Relation.objects.filter(id=id).update(
                marker_end_type = None,
                end_width = None,
                end_height = None                
            )
        if 'markerEnd' in data.keys():
            data['marker_end_type'] = data['markerEnd']['type']
            data['end_width'] = data['markerEnd']['width']
            data['end_height'] = data['markerEnd']['height']
            del data['markerEnd']
            Relation.objects.filter(id=id).update(
                marker_start_type = None,
                start_width = None,
                start_height = None                
            )
            
        if 'source' in data.keys():
            data['source_id'] = data.pop('source')
        if 'target' in data.keys():
            data['target_id'] = data.pop('target')
        
        Relation.objects.filter(id=id).update(**data)
        
        return Response(model_to_dict(Relation.objects.get(id=id)))

    # testing purposes, remove later and merge with component
    @action(detail=True, methods=['POST', 'GET', 'PUT'])
    def port(self, request, *args, **kwargs):
        self.serializer_class = PortSerializer
        if (request.method == 'POST'):
            pass
        elif (request.method == 'GET'):
            pass
        elif (request.method == 'PUT'):
            pass
        return Response(request.data)


# Creates a new relation to a component whenever an interface is made
def make_relation(component, target):
    rel = Relation(
        type = "step",
        animated = False,
        dependency = False,
        source_id = str(component.id),
        target_id = str(target)
    )
    rel.save()
        
    return rel

# Convert database relation data to react flow jsons
def edge_db_to_json(edges):
    for edge in edges:
        edge['id'] = str(edge['id'])
        edge['source'] = edge.pop('source_id')
        edge['source'] = str(edge['source'])
        edge['target'] = edge.pop('target_id')
        edge['target'] = str(edge['target'])
        
        if bool(edge['marker_start_type']):
            edge['markerStart'] = {
                'type': edge['marker_start_type'],
                'width': edge['start_width'],
                'height': edge['start_height']
            }
        del edge['marker_start_type'], edge['start_width'], edge['start_height']
        
        if bool(edge['marker_end_type']):
            edge['markerEnd'] = {
                'type': edge['marker_end_type'],
                'width': edge['end_width'],
                'height': edge['end_height']
            }
        del edge['marker_end_type'], edge['end_width'], edge['end_height']
        
    return edges
        
# Convert database node data to react flow json
def nodes_db_to_json(nodes):
    # Place all nodes at top level
    subnodes = []
    for node in nodes:
        # Check for subcomponent
        if bool(node['nodes']):
            # Add them to the top level
            subnodes.extend(relocate_subcomponents(node['nodes']))
        del node['nodes']
    nodes.extend(subnodes)
    
    # convert the dict to react flow format
    for node in nodes:
        node["data"] = {"label": node["name"]}
        node["position"] = {"x": node["x"], "y": node["y"]}
        node["style"] = {
            "backgroundColor": node["background_color"],
            "width": node["width"],
            "height": node["height"],
            "border": node["border"]
        }
        node['type'] = node.pop('node_type')
        node['parentNode'] = node.pop('parent')
        node['parentNode'] = str(node['parentNode'])
        node['id'] = str(node['id'])
        del (
            node["x"], node["y"], node["name"], node["background_color"], 
            node['width'], node['height'], node['ports']
        )
    
        if node['parentNode'] == 'None':
            node['parentNode'] = ""
    # Sort nodes on ID
    nodes = sorted(nodes, key=lambda node: node['id'])
    return nodes

# Takes a list of nested components and makes it unnested
def relocate_subcomponents(nodes):
    new_list = []
    for node in nodes:
        if bool(node['nodes']):
            new_list.extend(relocate_subcomponents(node['nodes']))
        del node['nodes']
        new_list.append(node)

    return new_list

# Return all components with their ports
def get_component(pk, relation):
    item_component = list()

    if(relation == 'parent'):
        item_component = list(Component.objects.filter(parent=pk))
 
    serializer_component = ComponentSerializer(item_component, many=True)
    components = serializer_component.data


    for component in components:
        subcomponents = get_component(component['id'], 'parent')
        component['nodes'] = subcomponents

        ports = get_port(component['id'], 'null')
        component['ports'] = ports

    interfaces = get_interface(pk, 'null')

    return  components + interfaces

# Gets a specific port
def get_port(pk, relation):
    item_port = list(Port.objects.filter(component=pk))
    serializer_port = PortSerializer(item_port, many=True)
    ports = serializer_port.data
    
    return ports

# Gets a specific interface
def get_interface(pk, relation):
    item_interface = list(Interface.objects.filter(parent=pk))
    serializer_interface = InterfaceSerializer(item_interface, many=True)
    interfaces = serializer_interface.data

    return interfaces

# Gets all components and their interfaces at the top level (no parent) 
def get_topnode(model_pk):
    #get all nodes that do not map 1 to 1 with a classifier(so only top level nodes)
    item_component = list(Component.objects.filter(model=model_pk).filter(parent=None))
    serializer_component = ComponentSerializer(item_component, many=True)
    components = serializer_component.data

    for component in components:
        subcomponent = get_component(component['id'], 'parent')
        component['nodes'] = subcomponent

        ports = get_port(component['id'], 'null')
        component['ports'] = ports

    item_interface = list(Interface.objects.filter(model=model_pk).filter(parent=None))
    serializer_interface = InterfaceSerializer(item_interface, many=True)
    interfaces = serializer_interface.data
    
    return  components + interfaces

# Get a specific model
def get_model(system_id, type_id):
    item = Model.objects.get(system=system_id, type=getattr(ModelTypes, type_id.capitalize()).value)
    serializer = ModelSerializer(item)
    
    model = serializer.data
    
    return model