from abc import abstractmethod
from rules.RulesManager.TermList import TermList
from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
    add_to_method_in_classifier as VP_add_to_method_in_classifier,
    get_standard_if_statement_spaces,
)

# from rules.processors.ModelPaths import get_pathing_from_classifier
from rules.RulesManager.BaseRule import BaseRule
from rules.RulesManager.Terms import Operator, Value
from model.models import Class, Classifier, Property
from django.apps import apps


class RelationshipRule(BaseRule):
    '''Relationship rule allows the user to make a rule about a property and the property of a linked object'''

    operator_keys = [["SUM"], [">", ">=", "==", "<", "<="]]

    # A couple of examples of text input for this rule
    # Should be a list of strings
    text_examples = [
        "SUM product price > order description",
        "The sum of the prices of products should not exceed the order's total",
    ]

    # Should test if an incoming rule dictionary fits the syntax of the rule
    # Returns rule_type or False
    @abstractmethod
    def is_type(termlist):
        return (
            termlist.count(2, 2, 2, 0)
            and termlist[Operator, 0] in RelationshipRule.operator_keys[0]
            and termlist[Operator, 1] in RelationshipRule.operator_keys[1]
        )

    # Returns the rule as clear, processed (simplified) text
    # Returns a string
    @abstractmethod
    def get_processed_text(self):
        return (
            self.termlist[Classifier, 0].name
            + self.termlist[Property, 0].name
            + self.termlist[Operator, 0]
            + self.termlist[Classifier, 1].name
            + self.termlist[Property, 1].name
        )

    def create_function(self, path):
        # Route up
        parent = path[0]
        statement = "sum = 0\n"
        parent_string = ""
        for classifier in path[0:]:
            parent_string += classifier.name.lower() + "."

        parent_string = parent_string[:-1]

        statement += "for " + path[1].name.upper() + " in " + parent_string + ":\n"
        path = path[::-1]
        parent = path[-2]
        count = 1
        for classifier in path[2:]:
            statement += (
                count * "\t"
                + "for "
                + classifier.name.upper()
                + " in "
                + parent.name.lower()
                + "."
                + classifier.name.lower()
                + ":\n"
            )
            parent = classifier
            count += 1

        statement += (
            count * "\t" + "sum += " + path[-1].name.upper() + "." + self.termlist[Property, 0].name.lower() + "\n\n"
        )

        statement += (
            "if not (sum "
            + self.termlist[Operator, 1]
            + " "
            + self.termlist[Classifier, 1].name.lower()
            + "."
            + self.termlist[Property, 1].name.lower()
            + "):\n"
        )
        statement += "\traise Exception('Did not conform to relationship rule')"

        return statement

    # Returns a string that contains a python function as code that validates the rule
    # Returns a string
    @abstractmethod
    def get_validator(self):
        # Get path between classifiers (A-B) and (B-A)
        route = get_pathing_from_classifier(self.termlist[Classifier, 0])[self.termlist[Classifier, 1]]

        return self.create_function(route)

    # Adds the validator to the application
    # Returns nothing
    @abstractmethod
    def add_validator(self):
        VP_add_to_method_in_classifier(self.termlist[Classifier, 0], "def clean(self):", self.get_validator())

    # Removes the validator from the application
    # Returns nothing
    @abstractmethod
    def remove_validator(self, dict):
        pass
