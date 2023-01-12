"""This script contains functions that request and return metadata for downstream
application.
"""


def get_metadata():
    """Get metadata from metamodel and return as dictionary.

    Returns:
        metadata:
            Dictionary containing metadata, formatted as definad in the API explorer
            under '/api/model/class/{project_id}/{system_id}'.
    """
    metadata = {
        "nodes": [
            {
                "id": 1,
                "name": "Administrator",
                "type": "Class",
                "data": {
                    "properties": [
                        {"id": 1, "name": "Name", "type": "string"},
                        {"id": 2, "name": "Name", "type": "string"},
                        {"id": 3, "name": "Address", "type": "string"},
                        {"id": 4, "name": "Age", "type": "int"},
                        {"id": 5, "name": "Username", "type": "string"},
                    ],
                    "methods": [],
                },
                "instances": {},
            },
            {
                "id": 2,
                "name": "Announcement",
                "type": "Class",
                "data": {
                    "properties": [{"id": 6, "name": "Header", "type": "string"}],
                    "methods": [],
                },
                "instances": {},
            },
            {
                "id": 3,
                "name": "Lecturer",
                "type": "Class",
                "data": {"properties": [], "methods": []},
                "instances": {},
            },
            {
                "id": 4,
                "name": "Lecture",
                "type": "Class",
                "data": {"properties": [], "methods": []},
                "instances": {},
            },
        ],
        "connections": [
            {
                "id": 1,
                "name": "make",
                "type": "association",
                "labelFrom": "*",
                "label": "make",
                "labelTo": "*",
                "from": 1,
                "to": 2,
            },
            {
                "id": 2,
                "name": "give",
                "type": "association",
                "labelFrom": "*",
                "label": "give",
                "labelTo": "1",
                "from": 3,
                "to": 4,
            },
        ],
    }
    return metadata


def get_connections(metadata):
    """Extract connections from metadata, formatted for usage in NLP validation.

    Args:
        metadata:
            As defined under /api/model/class/{project_id}/{system_id} in the API explorer.

    Returns:
        connections:
            A list of dicts. Every dict represents a connection in the metadata. The dicts
            contains data in the following format:
                "name": "give",
                "type": "association",
                "labelFrom": "*",
                "label": "give",
                "labelTo": "*",
                "from": "Lecturer",
                "to": "Lecture",
    """
    connections = []
    for con in metadata["connections"]:
        connection = con.copy()
        connection.pop("id")
        from_id = con["from"]
        to_id = con["to"]
        for node in metadata["nodes"]:
            if node["type"] == "Class":
                if node["id"] == from_id:
                    connection["from"] = node["name"]
                elif node["id"] == to_id:
                    connection["to"] = node["name"]
        connections.append(connection)
    return connections
