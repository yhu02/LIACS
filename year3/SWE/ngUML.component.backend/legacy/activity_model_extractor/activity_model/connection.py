"""Module to define connection and connection change for the Activity_interface."""
import copy
from dataclasses import dataclass


@dataclass
class Connection:
    """Define connection."""

    guard: str
    weight: str
    from_id: str
    to_id: str
    from_node_key: str
    to_node_key: str
    id: int

    def create_backend_dict(self) -> dict:
        """Generate Dict to push data to backend."""
        connection_dict = copy.deepcopy(self.__dict__)
        connection_dict["from"] = self.from_node_key
        connection_dict.pop("from_node_key")
        connection_dict["to"] = self.to_node_key
        connection_dict.pop("to_node_key")
        return connection_dict


@dataclass
class ConnectionChange:
    """Define ConnectionChange."""

    type: str
    guard: str
    weight: str
    from_node_key: str
    to_node_key: str
    activity_id: int
    key: str

    def create_backend_dict(self) -> dict:
        """Generate Dict to push data to backend."""
        connection_dict = {}
        connection_dict["to"] = copy.deepcopy(self.__dict__)
        connection_dict["type"] = self.type
        connection_dict["key"] = self.key
        connection_dict["to"]["from"] = self.from_node_key
        connection_dict["to"]["to"] = self.to_node_key
        connection_dict["to"].pop("from_node_key")
        connection_dict["to"].pop("to_node_key")
        connection_dict["to"].pop("type")
        connection_dict["to"].pop("key")
        return connection_dict
