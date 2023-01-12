"""Activity model API."""
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from django.core import management
from rest_framework import status, viewsets
from rest_framework.decorators import api_view, action
from rest_framework.response import Response

# from extractor.tools import FileTools
from threading import Thread
from model.models import (
    Model,
    Project,
    System,
)
from nguml.utils import IdSerializer
from model.tools import activity_model as activity_tools


class ActivityViewSet(viewsets.GenericViewSet):
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
        summary="Get a specific activity model",
        description="Get all the data on a project (name, description and version) as well as all the systems it enbodies",
    )
    def get(self, request, project_id=None, system_id=None):
        """Get a specific activity model.

        Description:
            This function returns the activity model of the requested system

        Args:
            system: System instance that the activity model is requested of

        Returns: object: rest framework Response containing a dict that has the nodes (list) and connections (list) of
        the requested activity model.

        """
        self.validate(request, system_id, project_id)
        try:
            model = Model.objects.filter(type="activity").get(system=self.system)
        except Model.DoesNotExist:
            return Response(
                {"detail": "The requested activity model cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )

        return Response(
            {
                "nodes": activity_tools.populate_nodes(model),
                "connections": activity_tools.populate_connections(model),
            }
        )

    @extend_schema(
        summary="Add a new activity model",
        description="Add activity model.",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo create activity model",
                value={
                    "title": "Order Entry",
                },
            )
        ],
    )
    def post(self, request, project_id=None, system_id=None):
        """Add activity model.

        Description:
            This function takes a dict containing title and creates a new empty activity model.
        Example:
        {
            "title": "Order Entry",
            }

        Args:
            request: rest framework request that contains the request data (title and
                    requirements text) for the activity pipeline
            system: System instance that the activity model will be part of

        Returns: the activity model id that is generated and the generated metadata

        """
        self.validate(request, system_id, project_id)
        data = request.data
        if "title" in data:
            if Model.objects.filter(type="activity").filter(system=self.system).exists():
                return Response(
                    {"detail": "An activity model already exist in this system"},
                    status=status.HTTP_400_BAD_REQUEST,
                )
            model = Model(
                type="activity",
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
        summary="Update an activity model",
        description="This API takes a list of changes an applies that to the specified activity model. "
        "Available commands: new-classifier, retype-node, delete-node, new-connection, retype-connection, "
        "delete-connection, new-operation, retype-operation, delete-operation, new-behaviorAction, "
        "retype-behaviorAction, delete-behaviorAction, new-activity, retype-activity, delete-activity",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo update activity model",
                value=[
                    {
                        "type": "new-classifier",
                        "to": {
                            "name": "Decision",
                            "type": "Decision",
                            "position": {"x": 248, "y": 248},
                            "data": {},
                            "instances": {},
                        },
                        "key": "null",
                    },
                    {
                        "type": "new-classifier",
                        "to": {
                            "name": "Approve_SP_order",
                            "type": "Action",
                            "position": {"x": 680, "y": 280},
                            "data": {},
                            "instances": {},
                        },
                        "key": "null",
                    },
                    {
                        "type": "new-connection",
                        "to": {
                            "name": "",
                            "type": "edge",
                            "from": 1,
                            "to": 8,
                            "label": "[order.Amount > 10.000]",
                            "labelFrom": "*",
                            "labelTo": "*",
                        },
                        "key": ["Decision", "Approve_SP_order"],
                    },
                    {
                        "type": "retype-node",
                        "to": {
                            "id": 4,
                            "retype": "name",
                            "name": "[by the warehouse manager]Finally the order is processed!",
                        },
                        "key": "null",
                    },
                ],
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    )
    def put(self, request, project_id=None, system_id=None):
        """Update a activity model.

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
        Example of 3 changes that adds a "Decision(id:8)" node with a "[order.Amount > 10.000]" connection to "Approve order" node:
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
            "key": null
        },
        {
            "type": "new-connection",
            "to": {
                "name": "",
                "type": "edge",
                "from": 1,
                "to": 8,
                "label": "[order.Amount > 10.000]",
                "labelFrom": "*",
                "labelTo": "*"
            },
            "key": [
                "Decision",
                "Approve_SP_order"
            ]
        },
        {
            "type":"retype-node",
            "to": {
                    "id": 4,
                    "retype":"name",
                    "name":"[by the warehouse manager]Finally the order is processed!"
                }
            },
            "key": null
        }
        ]

        Args:
            request: rest framework request that contains the request data (list of changes)
            system: System instance that the activity model is part of

        Returns: The new activity model generated by get_activity_model()

        """
        self.validate(request, system_id, project_id)
        try:
            model = Model.objects.filter(type="activity").get(system=self.system)
        except Model.DoesNotExist:
            return Response(
                {"detail": "The requested activity model cannot be found."},
                status=status.HTTP_404_NOT_FOUND,
            )

        changes = request.data
        success, errors, applied_changes = activity_tools.make_changes(model, changes)

        # Thread(target=management.call_command("make_and_run_migrations"))
        # use threading to call the migrations while we continue on
        if success:
            Model.objects.filter(type="activity").get(system=self.system)
            return Response(
                {
                    "nodes": activity_tools.populate_nodes(model),
                    "connections": activity_tools.populate_connections(model),
                }
            )
        else:
            return Response({"detail": errors}, status=status.HTTP_400_BAD_REQUEST)

    @extend_schema(
        summary="Delete an activity model",
        description="deletes the entire activity model from the database.",
    )
    def delete(self, request, project_id=None, system_id=None):
        """Delete a activity model.

        Description:
            This function deletes the entire activity model from the database. The requirements text and if an application exists
            based on this model are not deleted.

        Args:
            system: System instance that the class model will be part of

        Returns: Response HTTP 204, meaning successful with no other response

        """
        self.validate(request, system_id, project_id)
        try:
            model = Model.objects.filter(type="activity").get(system=self.system)
        except Model.DoesNotExist:
            return Response(
                {"detail": "The requested activity model cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
        model.delete()
        return Response(status=status.HTTP_204_NO_CONTENT)
