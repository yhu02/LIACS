"""Module to define classes for the activity interface."""
import copy
from dataclasses import dataclass

from enum import Enum


class NodeType(Enum):
    """Class to manage all node types."""

    ACTION = "Action"
    DECISION = "Decision"
    MERGE = "Merge"
    INITIAL = "Initial"
    ACTIVITY_FINAL = "ActivityFinal"

    def __str__(self):
        return str(self.value)


@dataclass
class Node:
    """Class to define a node."""

    type: NodeType
    name: str
    activity_id: int
    description: str
    instances: dict
    id: int

    def create_backend_dict(self) -> dict:
        """Generate Dict to push data to backend."""
        node_copy = copy.deepcopy(self.__dict__)
        # node_dict = {}
        node_copy["data"] = {}
        node_copy["type"] = str(self.type)
        node_copy["data"]["description"] = self.description
        node_copy["data"]["activity_id"] = self.activity_id
        node_copy.pop("description")
        node_copy.pop("activity_id")
        return node_copy


@dataclass
class NodeChange:
    """Class to define a nodechange."""

    type: str
    node_type: NodeType
    activity_id: int
    key: str
    node_key: str
    x_axis: int
    y_axis: int
    name: str
    description: str

    def create_backend_dict(self) -> dict:
        """Generate Dict to push data to backend."""
        node_copy = copy.deepcopy(self.__dict__)
        node_change_dict = {}
        node_change_dict["to"] = node_copy
        node_change_dict["type"] = self.type
        node_change_dict["to"]["position"] = {"x": self.x_axis, "y": self.y_axis}
        node_change_dict["to"]["type"] = str(self.node_type)
        node_change_dict["nodeKey"] = self.node_key
        node_change_dict["key"] = None
        node_change_dict["to"].pop("x_axis")
        node_change_dict["to"].pop("y_axis")
        node_change_dict["to"].pop("node_type")
        node_change_dict["to"].pop("node_key")
        node_change_dict["to"].pop("key")
        return node_change_dict
