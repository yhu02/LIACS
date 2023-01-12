"""Project API."""
from drf_spectacular.types import OpenApiTypes
from drf_spectacular.utils import extend_schema_view, extend_schema, inline_serializer, OpenApiExample
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.forms.models import model_to_dict

from model.tools import project as project_tools


@extend_schema_view(
    get=extend_schema(
        summary="Get a specific project",
        description="Get all the data on a project (name, description and version) as well as all the systems it enbodies",
    ),
    put=extend_schema(
        summary="Update a project",
        description="Update the details on a project",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo update project",
                value={"name": "new name", "desctiption": "new description", "version": "1.2.3"},
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    ),
    delete=extend_schema(
        summary="Delete a project",
        description="Delete a project as well as all the systems and models it holds",
    ),
)
@api_view(["GET", "PUT", "DELETE"])
def index(request, project_id):
    if project_id is None:
        return Response(
            {"detail": "For this operation a project id is required"},
            status=status.HTTP_400_BAD_REQUEST,
        )
    if request.method == "GET":
        return get_project(project_id)
    elif request.method == "PUT":
        return put_project(request, project_id)
    elif request.method == "DELETE":
        return delete_project(project_id)
    else:
        return Response(status=status.HTTP_405_METHOD_NOT_ALLOWED)


@extend_schema_view(
    get=extend_schema(
        summary="Get all projects",
        description="Get all the data on all the projects (name, description and version)",
    ),
    post=extend_schema(
        summary="Add a new project",
        description="Add a new project (needs name, requirements text, version and description)",
        request={"application/json": {OpenApiTypes.OBJECT}},
        examples=[
            OpenApiExample(
                name="Demo new project",
                value={
                    "name": "Test project",
                    "text": "This is the project text. A text about classes and activities.",
                    "version": "v1",
                    "description": "This project is a test project",
                },
                request_only=True,  # signal that example only applies to requests
                response_only=False,  # signal that example only applies to responses
            ),
        ],
    ),
)
@api_view(["GET", "POST"])
def index_without_project_id(request):
    if request.method == "GET":
        return get_projects()
    elif request.method == "POST":
        return post_project(request)
    else:
        return Response(status=status.HTTP_405_METHOD_NOT_ALLOWED)


def get_project(project_id):
    """Get a specific project
    This function gets the requested project and adds all systems that are part of this project.
    Args:
        project_id: Identifier of the project

    Returns: A rest framework response either 404 if the project is not found, if it exists it returns a dict with all
    the projects data.

    """
    project = project_tools.populate_project(project_id)
    if project:
        return Response(project)
    else:
        return Response(
            {"detail": "The requested project cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )


def put_project(request, project_id):
    """Update a project
    This function enables the possibility to update a few elements within a project. Only the elements that need
    updating need to be sent.
    Possible elements to update: name, description and version.
    example:
    {
        "name": "new name",
        "version: "1.2.3"
    }
    Args:
        request: dict containing the data to update
        project_id: Identifier of the project

    Returns: A rest framework response with the updated project as a dict

    """
    success = project_tools.make_changes(project_id, request.data)
    if success:
        return Response(model_to_dict(project_tools.get_project(project_id)))
    else:
        return Response(
            {"detail": "The requested project cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )


def delete_project(project_id):
    """Delete a project

    Deletes a specified project
    THIS WILL ALSO DELETE ALL SYSTEMS AND MODELS WITHIN THIS PROJECT.

    Args:
        project_id: Identifier of the project

    Returns: either 404: project not found, or 204: project found and deleted but no content was given

    """
    success = project_tools.delete_project(project_id)
    if success:
        return Response(status.HTTP_204_NO_CONTENT)

    else:
        return Response(
            {"detail": "The requested project cannot be found"},
            status=status.HTTP_404_NOT_FOUND,
        )


def get_projects():
    """get all projects within the database

    Returns: a rest framework response containing all projects as all list of dicts

    """
    systems = project_tools.get_all_projects()
    return Response(systems)


def post_project(request):
    """
    Create a project

    Example:
    {
        "name": "Test project",
        "text": "This is the project text. A text about classes and activities.",
        "version": "v1",
        "description": "This project is a test project",
    }


    Args:
        request: rest framework request that contains the request data (title and
                 requirements text) for the class pipeline

    Returns: the project id that is generated."""
    inserted_id, entities = project_tools.create_project(request.data)
    if not inserted_id:
        return Response(
            {"detail": "To make a new project, a name, version, description and a requirements text is required"},
            status=status.HTTP_400_BAD_REQUEST,
        )
    else:
        return Response({"inserted_id": inserted_id, "available_topics": entities})
