from model.models import Classifier, Property
from rules.RulesManager.Terms import Operator, Value, AwaitingDesignation


class TermList:
    AllowedTypes = [Operator, Value, Classifier, Property, AwaitingDesignation]

    def __init__(self):
        self.value = []

    def append(self, item):
        if type(item) in TermList.AllowedTypes:
            self.value.append(item)
        else:
            raise Exception("This item is not an instance of Operator, Value, Classifier or Property.")

    # TODO: Test

    def __getitem__(self, input):
        """Returns the specified type with the specified index from list. So for example get_from(list, classifier, 2) returns the
        third classifier from the list"""

        if isinstance(input, tuple):
            type = input[0]
            index = input[1]

            searchindex = 0
            for item in self.value:
                if isinstance(item, type):
                    if index == searchindex:
                        if isinstance(item, Operator) or isinstance(item, Value):
                            return item.name
                        else:
                            return item
                    else:
                        searchindex += 1

            raise Exception("No type found. with that index")

        else:
            return self.value[input]

    def __contains__(self, item):
        return item in self.name

    def __iter__(self):
        return iter(self.value)

    def get_all(self, type):
        """Returns a list with all items that are of the specified type. The list is in original order."""
        result = []
        for item in self.value:
            if isinstance(item, type):
                if isinstance(item, Operator) or isinstance(item, Value):
                    result.append(item.name)
                else:
                    result.append(item)
        return result

    def size(self, type):
        """Returns the count of items with that type."""
        return len(self.get_all(type))

    def count(self, classifiercount, propertycount, operatorcount, valuecount):
        """Checks the count of each object type"""
        if (
            classifiercount == self.size(Classifier)
            and propertycount == self.size(Property)
            and operatorcount == self.size(Operator)
            and valuecount == self.size(Value)
        ):
            return True
        else:
            return False
