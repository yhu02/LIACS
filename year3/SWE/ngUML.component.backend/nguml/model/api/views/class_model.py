"""Activity model API."""
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from django.core import management
from rest_framework import status, viewsets, serializers
from rest_framework.decorators import api_view, action
from rest_framework.response import Response

# from extractor.tools import FileTools
from threading import Thread
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
)
from nguml.utils import IdSerializer
from model.tools import class_model as class_tools


class ClassViewSet(viewsets.GenericViewSet):
    serializer_class = IdSerializer
    project = None
    system = None

    def validate(self, request, system, project):
        if not system:
            return Response({"detail": "A project id is required"}, status=status.HTTP_400_BAD_REQUEST)
        if not project:
            return Response({"detail": "A system id is required"}, status=status.HTTP_400_BAD_REQUEST)
        try:
            self.project = Project.objects.get(pk=project)
            self.system = System.objects.filter(project=project).get(pk=system)
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

    @extend_schema(
        summary="Get a specific class model",
        description="Get all the data on a project (name, description and version) as well as all the systems it enbodies",
    )
    def get(self, request, project_id=None, system_id=None):
        """Get a specific class model
            This function returns the class model of the requested system

        Args:
            system: System instance that the class model is requested of

        Returns: object: rest framework Response containing a dict that has the nodes (list) and connections (list) of
        the requested class model.

        """
        self.validate(request, system_id, project_id)

        try:
            model = Model.objects.filter(type="class").get(system=self.system)
        except Model.DoesNotExist:
            return Response(
                {"detail": "The requested class model cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )

        return Response(
            {
                "nodes": class_tools.populate_nodes(model),
                "connections": class_tools.populate_connections(model),
            }
        )

    @extend_schema(
        summary="Add a new class model",
        description="Creates a new (empty) class model.",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo create class model",
                value={
                    "title": "Order Entry",
                },
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    )
    def post(self, request, project_id=None, system_id=None):
        """Add class model This function takes a dict containing title and requirements text and puts it through the
        class model pipeline to generate a class model.
        Example:

        {
            "title": "Order Entry",
        }

        Args:
            request: rest framework request that contains the request data (title and
                    requirements text) for the class pipeline
            system: System instance that the class model will be part of

        Returns: the class model id that is generated and the generated metadata

        """
        self.validate(request, system_id, project_id)
        data = request.data
        if "title" in data:
            if Model.objects.filter(type="class").filter(system=self.system).exists():
                return Response(
                    {"detail": "A class model already exist in this system"},
                    status=status.HTTP_400_BAD_REQUEST,
                )

            model = Model(
                type="class",
                title=data.get("title"),
                raw_text="",
                system=self.system,
            )
            model.save()
            return Response({"inserted_id": model.id, "metadata": dict()})
        else:
            return Response(
                {"detail": "either text or title is missing"},
                status=status.HTTP_400_BAD_REQUEST,
            )

    @extend_schema(
        summary="Update a class model",
        description="This function takes a list of changes an applies that to the specified class model."
        " Available commands: retype-method, new-method, delete-method, retype-property, new-property, "
        "delete-property, new-classifier, new-connection connection-rename, connection-mirror, "
        "connection-cardinality-from, connection-cardinality-to, connection-retype, delete-connection",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo update class model",
                value=[
                    {
                        "type": "new-classifier",
                        "to": {
                            "name": "test",
                            "type": "Class",
                            "position": {"x": 624, "y": 360},
                            "data": {"properties": [{"name": "Test", "type": "string"}]},
                            "instances": {},
                        },
                        "nodeKey": 8,
                        "key": "null",
                    },
                    {
                        "type": "new-property",
                        "to": {"name": "Test", "type": "string"},
                        "key": {
                            "name": "test",
                            "type": "Class",
                            "position": {"x": 624, "y": 360},
                            "data": {"properties": [{"name": "Test", "type": "string"}]},
                            "instances": {},
                        },
                    },
                    {
                        "type": "new-connection",
                        "to": {
                            "name": "",
                            "type": "generalization",
                            "from": 8,
                            "to": 1,
                            "label": "Test",
                            "labelFrom": "*",
                            "labelTo": "*",
                        },
                        "key": ["test", "Order"],
                    },
                ],
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    )
    def put(self, request, project_id=None, system_id=None):
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
        }]

        Args:
            request: rest framework request that contains the request data (list of changes)
            system: System instance that the class model is part of

        Returns: The new class model generated by get_class_model()

        """
        self.validate(request, system_id, project_id)
        try:
            model = Model.objects.filter(type="class").get(system=self.system)
        except Model.DoesNotExist:
            return Response(
                {"detail": "The requested class model cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
        success, message = class_tools.make_changes(model, request.data)
        if success:
            # Thread(target=management.call_command("make_and_run_migrations"))
            # use threading to call the migrations while we continue on
            return Response(
                {
                    "nodes": class_tools.populate_nodes(model),
                    "connections": class_tools.populate_connections(model),
                }
            )
        else:
            return Response({"detail": message}, status=status.HTTP_400_BAD_REQUEST)

    @extend_schema(
        summary="Delete a class model",
        description="This function deletes the entire class model from the database. The requirements text and if an "
        "application exists based on this model are not deleted.",
    )
    def delete(self, request, project_id=None, system_id=None):
        """Delete a class model

        This function deletes the entire class model from the database. The requirements text and if an application exists
        based on this model are not deleted.
        Args:
            system: System instance that the class model will be part of

        Returns: Response HTTP 204, meaning successful with no other response

        """
        self.validate(request, system_id, project_id)

        try:
            model = Model.objects.filter(type="class").get(system=self.system)
        except Model.DoesNotExist:
            return Response(
                {"detail": "The requested class model cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
        model.delete()
        return Response(status=status.HTTP_204_NO_CONTENT)

    @extend_schema(summary="Layout", description="Runs autolayout over the diagram")
    def layout(self, request, project_id=None, system_id=None):
        """Layout a class model

        This function auto_layouts the class model
        Args:
            project: Project instance that the class model is part of
            system: System instance that the class model will be part of

        Returns: Response HTTP 204, meaning successful with no other response

        """
        self.validate(request, system_id, project_id)

        try:
            model = Model.objects.filter(type="class").get(system=self.system)
        except Model.DoesNotExist:
            return Response(
                {"detail": "The requested class model cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
        class_tools.set_layout(model)
        return Response(status=status.HTTP_204_NO_CONTENT)
