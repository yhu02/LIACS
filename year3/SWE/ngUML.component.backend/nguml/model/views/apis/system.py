"""System API."""
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, OpenApiExample
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response

from model.models import Project
from model.tools import system as system_tools

"""
System is an overarching element that holds different models. In this way a systems has a collection of models, rules
and chatbots. In other words a system is a collection of metadata models and descriptions.
"""


@extend_schema_view(
    get=extend_schema(
        summary="Get all the models and information about a specific system",
        description="Get all the data on a project (name, description and version) as well as all the systems it "
        "embodies",
    ),
    put=extend_schema(
        summary="Make a small change to the system (change the name)",
        description="This function updates the system with the given changes. Systems only have a name, version and a "
        "collection of models. The models are changes through their own APIs. Whereas the version and "
        "name can be updated through this endpoint. The version and name can be changed individually or "
        "together. It is also possible to important changes from different systems within the current "
        "project. These changes are model changes that are made after the requirements text is processed, "
        "so in other words changes that are made through editor or chatbot. To use this you need to set "
        "import_changes with the system_id of the system you want to import changes from.",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo update system",
                value={"name": "new name", "version": "1.2.4", "description": "new description", "import_changes": 1},
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    ),
    delete=extend_schema(
        summary="Delete an existing system",
        description="Delete a system, THIS WILL ALSO DELETE ALL THE MODELS THE SYSTEM HOLDS",
    ),
)
@api_view(["GET", "PUT", "DELETE"])
def index(request, project_id, system_id):
    if not project_id:
        return Response({"detail": "A project id is required"}, status=status.HTTP_400_BAD_REQUEST)
    else:
        try:
            project = Project.objects.get(pk=project_id)
        except Project.DoesNotExist:
            return Response(
                {"detail": "The requested system cannot be found, because the overarching project cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
        if system_id is None:
            return Response(
                {"detail": "For this operation a system id is required"},
                status=status.HTTP_400_BAD_REQUEST,
            )
        if request.method == "GET":
            return get_system(project, system_id)
        elif request.method == "PUT":
            return put_system(request, project, system_id)
        elif request.method == "DELETE":
            return delete_system(project, system_id)
    return Response(status=status.HTTP_405_METHOD_NOT_ALLOWED)


@extend_schema_view(
    get=extend_schema(
        summary="Get all systems within a project",
        description="gets all the systems from the requested project and returns them as a list",
    ),
    post=extend_schema(
        summary="Add a new system to the project",
        description="Create a new system, 'name' and 'version' attributes are required. When a list of entities (topics) and a list of UML model types are given then the bucketizer will be run. This will split the requirements texts to the appropriate uml types and will return them. Entities are generated in project. Types is a list and can have 'class', 'activity' and 'usecase'.",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo create system",
                value={
                    "name": "Bicycle shop system",
                    "description": "This is a preprocessed example",
                    "entities": ["bicycle", "member", "customized bike"],
                    "types": ["activity"],
                    "version": "1.1.0",
                },
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    ),
)
@api_view(["GET", "POST"])
def index_without_id(request, project_id):
    if not project_id:
        return Response({"detail": "A project id is required"}, status=status.HTTP_400_BAD_REQUEST)
    else:
        try:
            project = Project.objects.get(pk=project_id)
        except Project.DoesNotExist:
            return Response(
                {"detail": "The requested system cannot be found, because the overarching project cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
    if request.method == "GET":
        return get_systems(project)
    elif request.method == "POST":
        return post_system(request, project)
    else:
        return Response(status=status.HTTP_405_METHOD_NOT_ALLOWED)


def get_system(project, system_id):
    """Get one specified system

        Returns a specific system.
    Args:
        system_id: identifier of the requested system

    Returns: object: rest framework Response containing a dict with name, version and timestamp of creation of the
    system

    """
    system = system_tools.populate_system(project, system_id)
    if system:
        return Response(system)
    else:
        return Response(
            {"detail": "The requested system cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )


def put_system(request, project, system_id):
    """Update a system

    This function updates the system with the given changes. Systems only have a name, version and a collection of
    models. The models are changes through their own APIs. Whereas the version and name can be updated through this
    endpoint. The version and name can be changed individually or together.

    It is also possible to important changes from different systems within the current project. These changes are
    model changes that are made after the requirements text is processed, so in other words changes that are made
    through editor or chatbot. To use this you need to set "import_changes" with the system_id of the system you want
    to import changes from.
    Individual example:
     {
        "version": "1.2.3"
    }
    Combined example:
    {
        "name": "Test System 2",
        "version": "1.2.4",
        "import_changes": 1
    }
    Args:
        request: rest framework request containing body and method
        system_id: identifier of the requested system .

    Returns: object: rest framework Response contain the updated system (name, version and timestamp)

    """
    errors, applied_changes = system_tools.make_changes(project, system_id, request.data)
    if not errors and not applied_changes:
        return Response(
            {"detail": "The requested system cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )
    else:
        if "import_changes" in request.data:
            if errors:
                return Response(
                    {
                        "detail": "The import of changes was successful but a number of errors was encountered",
                        "errors": errors,
                        "changes": applied_changes,
                    }
                )
            else:
                return Response(
                    {
                        "detail": "All changes were successful imported",
                        "errors": [],
                        "changes": applied_changes,
                    }
                )
        else:
            return Response(system_tools.populate_system(project, system_id))


def delete_system(project, system_id):
    """Delete a system

    Delete a system, THIS WILL ALSO DELETE ALL THE MODELS THE SYSTEM HOLDS

    Args:
        system_id: identifier of the requested system

    Returns :object: rest framework Response HTTP 204, meaning request successful but no other response (body empty)

    """
    success = system_tools.delete_system(project, system_id)
    if success:
        return Response(status.HTTP_204_NO_CONTENT)

    else:
        return Response(
            {"detail": "The requested system cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )


def get_systems(project):
    """Get all systems

    This function gets all the systems from the database and returns them as a list
    It checks if systems have a model that is changed after it is processed through a model extractor
    pipeline (text -> model). If so models_changed becomes True
    Returns: object: rest framework Response that has a list of all the systems

    """
    selected_systems = []
    for system in system_tools.get_all_systems(project):
        system_details = system_tools.populate_system(project, system["id"])
        changes = [True for element in system_details["models"] if element["model_changed"]]
        system["models_changed"] = any(changes)
        selected_systems.append(system)
    return Response(selected_systems)


def post_system(request, project):
    """Add a system

    Create a new system, "name" and "version" attributes are required.
    When a list of entities (topics) and a list of UML model types are given then the bucketizer will be run. This will
    split the requirements texts to the appropriate uml types and will return them. Entities are generated in project.
    Types is a list and can have "class", "activity" and "usecase".
    Example:
    {
            "name": "Bicycle shop system",
            "description": "This is a preprocessed example",
            "entities": ["bicycle", "member", "customized bike"],
            "types": ["activity"],
            "version": "1.1.0"
    }
    Args:
         request: rest framework request containing body and method (name and version)

    Returns: object: rest framework Response with the id of the new system

    """
    if "name" not in request.data or "version" not in request.data:
        return Response(
            {"detail": "To make a new system name and version is required"},
            status=status.HTTP_400_BAD_REQUEST,
        )
    else:
        inserted_id, selected_buckets = system_tools.create_system(project, request.data)
        if inserted_id:
            return Response({"inserted_id": inserted_id, "buckets": selected_buckets})
        else:
            Response(
                {"detail": "To make a new system name and version is required"},
                status=status.HTTP_400_BAD_REQUEST,
            )
