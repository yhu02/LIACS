from django.forms.models import model_to_dict

# import pandas as pd
import pickle
import base64

from model.models import (
    Model,
    ModelChanges,
    System,
    AvailableEntities,
    SelectedEntities,
)
from model.tools import activity_model as activity_tools
from model.tools import class_model as class_tools

# from bucketing.nlp.summarizer import Summarizer
# from bucketing.nlp.bucketizer import Bucketizer


def get_all_systems(project):
    """get all the systems as a list of dicts
    Args:
        project: db instance of project the new system belongs to

    Returns: list of dicts of all the systems

    """
    return System.objects.filter(project=project).values()


def get_system(project, system_id):
    """get a specific db instance of a system

    Args:
        project: db instance of project the new system belongs to
        system_id: identifier of the system that is requested

    Returns: db instance of a single system, or None if not found

    """
    try:
        return System.objects.filter(project=project).get(pk=system_id)
    except System.DoesNotExist:
        print("System is not found, system id:" + system_id)
        return


def populate_system(project, system_id):
    """get a system as a dict together with a description of all the models it holds

    Args:
        project: db instance of project the new system belongs to
        system_id: identifier of the system that is requested

    Returns: a dict containing the system with all of its models

    """
    selected_system = get_system(project, system_id)
    if selected_system:
        selected_system_dict = model_to_dict(selected_system)
        selected_system_dict["models"] = []
        for model in Model.objects.filter(system=selected_system).all().values():
            model["model_changed"] = ModelChanges.objects.filter(model_id=model["id"]).exists()
            selected_system_dict["models"].append(model)
        return selected_system_dict
    else:
        return {}


def make_changes(project, system_id, changes):
    """Update the system
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
        project: db instance of project the new system belongs to
        system_id: identifier of the system that is requested
        changes:

    Returns:

    """
    errors = []
    applied_changes = []
    system = get_system(project, system_id)
    if system:
        if "name" in changes:
            system.name = changes["name"]
            applied_changes.append("name changed: " + changes["name"])
        if "version" in changes:
            system.version = changes["version"]
            applied_changes.append("version changed: " + changes["version"])
        if "description" in changes:
            system.description = changes["description"]
            applied_changes.append("description changed: " + changes["description"])
        if "import_changes" in changes:
            errors, applied_changes = import_changes(
                project,
                target_system_id=system_id,
                source_system_id=changes["import_changes"],
            )
        system.save()
        return errors, applied_changes
    else:
        print("System is not found, system id: " + str(system_id))
        return None, None


def import_changes(project, target_system_id, source_system_id):
    """This function is a helper function on make_changes()

    This function actually directs the import of the changes per model, it checks the models in source,
    when a model (class, activity or both) are changed after the initial model, then these changes are applied
    to the same model kind in the target system

    Args:
        project: db instance of project the new system belongs to
        target_system_id: Identifier of the systems that changes will be sent TO
        source_system_id: Identifier of the systems that changes will be sent FROM

    Returns: a list of errors that were encountered and a list of changes that are applied successfully

    """
    source_system = populate_system(project, source_system_id)
    target_system = populate_system(project, target_system_id)
    applied_changes = []
    errors_encountered = []
    for source_model in source_system["models"]:
        if "model_changed" in source_model and source_model["model_changed"]:
            target_model = next(
                (
                    selected_model
                    for selected_model in target_system["models"]
                    if selected_model["type"] == source_model["type"]
                ),
                None,
            )

            if target_model:  # check if a model of the same type (class, activity) exist in the target system
                changes_rows = ModelChanges.objects.filter(model_id=source_model["id"]).values()
                changes_list_with_list = [changes_column["changes"] for changes_column in changes_rows]
                changes_json = [item for sublist in changes_list_with_list for item in sublist]
                target_model = Model.objects.filter(type=target_model["type"]).get(system_id=target_system["id"])
                result, errors, successful_changes = import_changes_per_model(target_model, source_model, changes_json)
                applied_changes.extend(successful_changes)
                if not result:
                    errors_encountered.extend(errors)

    return errors_encountered, applied_changes


def import_changes_per_model(target_model, source_model, changes):
    """helper function of import_changes()

    This function redirects the changer that have to be made according to the type of model

    Args:
        target_model: db instance of the target model
        source_model: db instance of the source model
        changes: a list of changes that need to be made in the target model

    Returns: a boolean if the import was successful, a list of errors encountered and a list of changes that are
             applied to the target model

    """
    if target_model.type == "activity" and source_model["type"] == "activity":
        return activity_tools.import_changes(target_model, changes)
    elif target_model.type == "class" and source_model["type"] == "class":
        return class_tools.import_changes(target_model, changes)
    # elif target_model["type"] == "usecase" and source_model["type"] == "usecase":
    # use_case_tools.import_changes(target_model, source_model, changes)
    else:
        return False, ["The type of models mismatched or is not recognised"], []


def delete_system(project, system_id):
    """
        Delete a system, THIS WILL ALSO DELETE ALL THE MODELS THE SYSTEM HOLDS
    Args:
        project: db instance of project the new system belongs to
        system_id: identifier of the system that is requested
    Returns: a boolean if deletion of the system was successful
    """
    system = get_system(project, system_id)
    if system:
        system.delete()
        return True
    else:
        return False


def create_system(project, data):
    """
    Create a new system, "name" and "version" attributes are required. When a list of entities (topics) and a list of
    UML model types are given then the bucketizer will be run. This will split the requirements texts to the
    appropriate uml types and will return them Args: project: db instance of project the new system belongs to data:
    system attributes (name, version etc)

    Returns: the identifier of the created system

    """
    if "name" not in data or "version" not in data:
        print("name and version are mandatory when creating a system")
        return
    else:
        # TODO: support multiple documents, and select document in interface
        requirements = project.document_set.first()

        # Save new system
        new_system = System(
            project=project,
            name=data["name"],
            version=data["version"],
            description=data.get("description", ""),
        ).save()

        return new_system.id, {}

        # if "entities" not in data or "types" not in data:
        #     print(
        #         "Requirements text bucketizer is skipped, entities and UML types not given in the request"
        #     )
        #     return inserted_id, {}

        # # Save selected entities
        # available_entities = AvailableEntities.objects.filter(document=requirements)
        # selected_entities = list(
        #     filter(
        #         lambda entity: getattr(entity, "entity") in data["entities"],
        #         available_entities,
        #     )
        # )
        # SelectedEntities.objects.bulk_create(
        #     [
        #         SelectedEntities(
        #             system=new_system,
        #             entity=entity,
        #         )
        #         for entity in selected_entities
        #     ]
        # )

        # # Get the metadata
        # pickbytes = base64.b64decode(requirements.metadata)
        # df = pickle.loads(pickbytes)

        # # Get the selected entity object
        # entity_obj = {
        #     getattr(entity, "entity"): [
        #         float(nmbr) for nmbr in getattr(entity, "entity_numbers").split(",")
        #     ]
        #     for entity in selected_entities
        # }

        # # Get the filtered text
        # summarizer = Summarizer(df, entity_obj)
        # summarized_text = summarizer.get_filtered_text(entity_obj)

        # # Split the text into different buckets
        # bucketizer = Bucketizer(summarized_text)
        # buckets = bucketizer.apply_bucketing()

        # # Return the request with the inserted system id
        # return inserted_id, {uml_type: buckets[uml_type] for uml_type in data["types"]}
