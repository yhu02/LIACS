"""Use Case Model API."""
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.core import management
from extractor.tools import FileTools
from threading import Thread
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
)

from model.tools import usecase_model as usecase_tools

@extend_schema_view(
    get=extend_schema(
        summary="Get the full use case model of a given system",
        description="This function returns the use case model of the requested system"
    ),
    post=extend_schema(
        summary="Add an entire use case model",
        description="Add use case model. This function takes a dict containing title and requirements text and puts it through the"
                    " use case model pipeline to generate a use case model.",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo create use case model",
                value={
                    "title": "Order Entry",
                    "text": "The user browses restaurant options. Once the preferred restaurant is selected, they place an order through the application. The user pays online or verifies they will pay in person. The order is sent from the app to the restaurant’s internal system. The restaurant worker receives and processes the electronic order.",
                },
            )
        ],
    ),
    put=extend_schema(
        summary="Update a use case model",
        description="This API takes a list of changes an applies that to the specified usecase model. "
                    "Available commands: new-classifier, retype-classifier, moved-classifier, delete-classifier,"
                    "new-system, retype-system, delete-system, new-precondition, retype-precondition,delete-precondition, "
                    "new-postcondition, retype-postcondition, delete-postcondition, new-trigger, retype-trigger, delete-trigger, "
                    "new-scenario, retype-scenario delete-scenario,new-relationship,relationship-rename,relationship-mirror,relationship-from,relationship-to,"
                    "relationship-retype,delete-relationship",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo update usecase model",
                value=[{
                   "type": "new-classifier",
                    "to": {
                        "name": "test",
                        "type": "UseCase",
                        "position": {
                            "x": 624,
                            "y": 360
                        },
                        "data": {
                            "precondition": [{
                                "name": "Test"
                            }]
                        },
                        "instances": {}
                    },
                    "nodeKey": 8,
                    "key": "null"
                }, {
                    "type": "new-precondition",
                    "to": {
                        "name": "Test",
                        "type": "string"
                    },
                    "key": {
                        "name": "test",
                        "type": "UseCase",
                        "position": {
                            "x": 624,
                            "y": 360
                        },
                        "data": {
                            "precondition": [{
                                "name": "Test"
                            }]
                        },
                        "instances": {}
                    }
                }, {
                    "type": "new-relationship",
                    "to": {
                        "name": "",
                        "type": "inclusion",
                        "from": 8,
                        "to": 1,
                    },
                    "key": ["test", "Order"] 
            }],
        
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ]
    ),
    delete=extend_schema(
        summary="Delete a use case model",
        description="This function deletes the entire usecase model from the database. The requirements text and if an "
                    "application exists based on this model are not deleted.",
    ),
)
@api_view(["GET", "POST", "PUT", "DELETE"])
def index(request, project_id, system_id):
    if not project_id:
        return Response(
            {"detail": "A project id is required"}, status=status.HTTP_400_BAD_REQUEST
        )
    elif not system_id:
        return Response(
            {"detail": "A system id is required"}, status=status.HTTP_400_BAD_REQUEST
        )
    else:
        try:
            project = Project.objects.get(pk=project_id)
            system = System.objects.filter(project=project).get(pk=system_id)
        except System.DoesNotExist:
            return Response(
                {"detail": "The requested system cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
        except Project.DoesNotExist:
            return Response(
                {"detail": "The requested project cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
    if request.method == "GET":
        return get_usecase_model(system)
    elif request.method == "POST":
        return post_usecase_model(request, system)
    elif request.method == "PUT":
        return put_usecase_model(request, system)
    elif request.method == "DELETE":
        return delete_usecase_model(system)
    else:
        return Response(status=status.HTTP_405_METHOD_NOT_ALLOWED)


def get_usecase_model(system):
    """Get a specific use case model
        Description: This function returns the usecase model of the requested system

    Args:
        system: System instance that the usecase model is requested of

    Returns: object: rest framework Response containing a dict that has the nodes (list) and connections (list) of
    the requested usecase model.
    """
    try:
        model = Model.objects.filter(type="usecase").get(system=system)
    except Model.DoesNotExist:
        return Response(
            {"detail": "The requested usecase model cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )

    return Response(
        {
            "nodes": usecase_tools.populate_nodes(model),
            "relationships": usecase_tools.populate_relationships(model),
            "systems": usecase_tools.generate_usecase_system(model),
        }
    )

def post_usecase_model(request, system):
    """Add usecase model.

    Description:
        This function takes a dict containing title and requirements text and puts it through the
        usecase model pipeline to generate a usecase model.
    Example:
    {
        "title": "Order Entry",
        "text": "The user browses restaurant options. Once the preferred restaurant is selected, they place an order through the application. The user pays online or verifies they will pay in person. The order is sent from the app to the restaurant’s internal system. The restaurant worker receives and processes the electronic order."
    }

    Args:
        request: rest framework request that contains the request data (title and
                 requirements text) for the usecase pipeline
        system: System instance that the usecase model will be part of

    Returns: the usecase model id that is generated and the generated metadata

    """
    data = request.data
    if "title" in data and "text" in data:
        if Model.objects.filter(type='usecase').filter(system=system).exists():
            return Response(
                {"detail":"A usecase model already exists in this system."},
                status=status.HTTP_400_BAD_REQUEST,
            )
        if not data["text"]:
            return Response(
                {"detail":"The provided text is empty."},
                status=status.HTTP_400_BAD_REQUEST,
            )
        file_path = "data/" + data.get("title") + ".txt"

        model = Model(
            type="usecase",
            title=data.get("title"),
            raw_text=data.get("text"),
            system=system,
        )
        model.save()
        FileTools.save_text(file_path, data.get("text"))
        # result = pipeline.run_latest_demo(
        #     data.get("title"), data.get("text"), True, model.pk
        # )
        usecase_id = result["usecase_id"]
        usecase_print_format = usecase_tools.get_usecase_result_print_format(usecase_id)
        inserted_id = Model.objects.last().id
        return Response({"inserted_id": inserted_id, "metadata": usecase_print_format})
    else:
        return Response({"detail": "either text or title is missing"},
            status=status.HTTP_400_BAD_REQUEST,
        )

def put_usecase_model(request, system):
    """Update a usecase model.
    This function takes a list of changes and applies that to the specified usecase model.
    Available commands:
  - retype-precondition
        - new-precondition
        - delete-precondition
        - retype-postcondition
        - new-postcondition
        - delete-postcondition
        - retype-trigger
        - new-trigger
        - delete-trigger
        - retype-scenario
        - new-scenario
        - delete-scenario
        - new-classifier
        - moved-classifier
        - delete-classifier
        - retype-classifier
        - new-system
        - retype-system
        - delete-system
        - new-relationship
        - relationship-rename
        - relationship-mirror
        - relationship-from
        - relationship-to
        - relationship-retype
        - delete-relationship
       
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
            "name": "Test"
        },
        "key": {
            "name": "login",
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
    Args:
        request: rest framework request that contains the request data (list of changes)
        system: System instance that the usecase model is part of

    Returns: The new usecase model generated by get_usecase_model()
    """
    try:
        model = Model.objects.filter(type="usecase").get(system=system)
    except Model.DoesNotExist:
        return Response(
            {"detail": "The requested usecase model cannot be found."},
            status=status.HTTP_404_NOT_FOUND,
        )
    success, message = usecase_tools.make_changes(model, request.data)
    if success:
        # Thread(target=management.call_command("make_and_run_migrations"))
        # use threading to call the migrations while we continue on
        return Response(get_usecase_model(system).data)
    else:
        return Response({"detail": message}, status=status.HTTP_400_BAD_REQUEST)
                     


def delete_usecase_model(system):
    """Delete a usecase model.

    Description:
        This function deletes the entire usecase model from the database. The requirements text and if an application exists
        based on this model are not deleted.

    Args:
        system: System instance that the usecase model will be part of

    Returns: Response HTTP 204, meaning successful with no other response

    """
    try:
        model = Model.objects.filter(type="usecase").get(system=system)
    except Model.DoesNotExist:
        return Response(
            {"detail": "The requested usecase model cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )
    model.delete()
    return Response(status=status.HTTP_204_NO_CONTENT)

