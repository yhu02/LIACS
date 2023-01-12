from django.forms.models import model_to_dict
import pickle
import base64

from model.models import Project, System, Document, AvailableEntities

# from bucketing.nlp.pipeline import Pipeline
# from bucketing.nlp.summarizer import Summarizer


def get_all_projects():
    """get all projects within the database

    Returns: returns all projects as all list of dicts

    """
    return list(Project.objects.all().values())


def get_project(project_id):
    """get a db instance of a specific project

    Args:
        project_id: identifier of the project

    Returns: a db instance of Project, or None if not found

    """
    try:
        return Project.objects.get(pk=project_id)
    except Project.DoesNotExist:
        print("Project is not found, project id:" + project_id)
        return


def populate_project(project_id):
    """get a project as a dict with all the systems it holds

    Args:
        project_id: identifier of the project

    Returns: a dict with the project and dict["systems"] is a list of descriptions of the systems it has

    """
    try:
        selected_project = Project.objects.get(pk=project_id)
        selected_project_dict = model_to_dict(selected_project)
        selected_project_dict["systems"] = System.objects.filter(project=selected_project).all().values()

    except Project.DoesNotExist:
        print("Project is not found, project id:" + project_id)
        return
    else:
        return selected_project_dict


def make_changes(project_id, changes):
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
        project_id: Identifier of the project
        chnages: dict containing the data to update

    Returns: boolean true if project found and changes are made, false if project does not exist

    """
    project = get_project(project_id)
    if project:
        if "name" in changes:
            project.name = changes["name"]
        if "description" in changes:
            project.description = changes["description"]
        if "version" in changes:
            project.version = changes["version"]
        project.save()
        return True
    else:
        return False


def delete_project(project_id):
    """Delete a project

    Deletes a specified project
    THIS WILL ALSO DELETE ALL SYSTEMS AND MODELS WITHIN THIS PROJECT.

    Args:
        project_id: Identifier of the project

    Returns: boolean true if project found and is deleted, false if project does not exist

    """
    project = get_project(project_id)
    if project:
        project.delete()
        return True
    else:
        return False


def create_project(data, preprocess_text=True):
    """Create a project.

    Example:
    {
        "name": "Test project",
        "text": "This is the project text. A text about classes and activities.",
        "version": "v1",
        "description": "This project is a test project",
    }


    Args:
        - data: the request data (title, version, description and requirements text) for the preprocessing pipeline
        - preprocess_text: If the text in data needs to be preprocessed, if False the pipeline will be skipped and the
            project will be saved to the db as is, this also means that there NO entities available for this project and that
            when creating a system you CANNOT run the bucketizer.

    Returns: the project id that is generated, and list of topics that are generated. Both are None if there are
             attributes missing from data

    """
    if "name" not in data or "text" not in data or "version" not in data or "description" not in data:
        return None, None
    else:
        new_project = Project(
            name=data["name"],
            version=data["version"],
            text=data["text"],
            description=data["description"],
        ).save()
        return new_project.id, []
        # if preprocess_text:
        #     # Get metadata of requirements texts
        #     pipeline = Pipeline()
        #     pipeline_data = pipeline.generate_text_data(data["text"])

        #     # Save metadata as csv
        #     pickbytes = pickle.dumps(pipeline_data["tokens"])
        #     b64_pickbytes = base64.b64encode(pickbytes)

        #     # Save requirements in Document
        #     requirements_document = Document(
        #         content=data["text"],
        #         metadata=b64_pickbytes,
        #         project=new_project,
        #     )
        #     requirements_document.save()

        #     # Get entities
        #     summarizer = Summarizer(pipeline_data["tokens"], pipeline_data["entities"])
        #     filtered_entities = summarizer.get_filtered_entities()

        #     # Save entities
        #     AvailableEntities.objects.bulk_create(
        #         [
        #             AvailableEntities(
        #                 document=requirements_document,
        #                 entity=entity,
        #                 entity_numbers=",".join([str(nmbr) for nmbr in entity_numbers]),
        #             )
        #             for entity, entity_numbers in filtered_entities.items()
        #         ]
        #     )
        #     return inserted_id, filtered_entities.keys()
        # else:
        #     return inserted_id, []
