from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.processors.ValidatorProcessor import (
    get_standard_if_statement_spaces,
    add_to_method_in_classifier as VP_add_to_method_in_classifier,
)
from rules.RulesManager.BaseRule import BaseRule
from rules.RulesManager.Terms import Operator
from model.models import Classifier, Property


class NumericalTwoProperties(BaseRule):
    """
    The syntax of the NumericalTwoProperties rule is: classifier.property operator classifier.property.
    The purpose of this rule is to enforce two properties of an object to meet a certain numerical relation based on the operator.
    """

    operator_keys = [">", ">=", "==", "<", "<="]

    text_examples = [
        "Elevator load <= Elevator capacity",
        "The number of doors in a house must be at least equal to the number of rooms",
        "Team size should be at least minimumTeamSize",
    ]

    @staticmethod
    def is_type(termlist):
        return termlist.count(1, 2, 1, 0) and termlist[Operator, 0] in NumericalTwoProperties.operator_keys

    def get_processed_text(self):
        return (
            self.termlist[Classifier, 0].name
            + "."
            + self.termlist[Property, 0].name
            + " "
            + self.termlist[Operator, 0]
            + " "
            + self.termlist[Classifier, 0].name
            + "."
            + self.termlist[Property, 1].name
        )

    def get_validator(self):
        return get_standard_if_statement_spaces(
            "int(self."
            + self.termlist[Property, 0].name
            + ") "
            + self.termlist[Operator, 0]
            + " int(self."
            + self.termlist[Property, 1].name
            + ")",
            self,
        )

    def add_validator(self):
        VP_add_to_method_in_classifier(self.termlist[Classifier, 0], "def clean(self):", self.get_validator())

    def remove_validator(self):
        pass
