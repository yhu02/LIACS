"""Module to define classes for the activity interface."""
import copy
from dataclasses import dataclass

@dataclass
class Activity:
    """Class to define an activity."""
    id: int
    type: str
    name: str
    precondition: str
    postcondition: str
    is_read_only: bool
    is_single_execution: bool
    action_id: str
    node_key: str
    retype_definition: str

    def create_backend_dict(self) -> dict:
        """Generate Dict to push data to backend."""
        activity_copy = copy.deepcopy(self.__dict__)
        activity_dict = {}
        activity_dict["type"] = self.type
        activity_dict["nodekey"] = self.node_key
        activity_dict["to"] = activity_copy
        activity_dict["to"]["isReadOnly"] = self.is_read_only
        activity_dict["to"]["isSingleExecution"] = self.is_single_execution
        activity_dict["to"]["action"] = self.action_id
        activity_dict["to"]["retype"] = self.retype_definition
        activity_dict["to"].pop("type")
        activity_dict["to"].pop("node_key")
        activity_dict["to"].pop("is_read_only")
        activity_dict["to"].pop("is_single_execution")
        activity_dict["to"].pop("action_id")
        activity_dict["to"].pop("retype_definition")
        return activity_dict
