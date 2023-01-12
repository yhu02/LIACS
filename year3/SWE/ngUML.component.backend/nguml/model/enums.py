import enum


class ModelTypes(enum.Enum):
    Class = "Class"
    Activity = "Activity"
    UseCase = "Usecase"
    Component = "Component"

    @classmethod
    def choices(types):
        return tuple((i.name, i.value) for i in types)

    def get_attribute(self, string_name):
        return getattr(self, string_name)


class LinkType(enum.Enum):
    Association = "Association"
    DirectedAssociation = "DirectedAssociation"
    Aggregation = "Aggregation"
    Composition = "Composition"
    Inheritance = "Inheritance"
    Realization = "Realization"
    Dependency = "Dependency"

    @classmethod
    def choices(types):
        return tuple((i.name, i.value) for i in types)

class PortType(enum.Enum):
    SimplePort = 'SimplePort'
    ComplexPort = 'ComplexPort'
    
    @classmethod
    def choices(types):
        return tuple((i.name, i.value) for i in types)

    
class Type(enum.Enum):
    String = "string"
    Int = "int"
    Bool = "bool"

    @classmethod
    def choices(types):
        return tuple((i.name, i.value) for i in types)
