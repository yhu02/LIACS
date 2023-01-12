from os import terminal_size
from rules.RulesManager.TermList import TermList
from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.RulesManager.BaseRule import BaseRule
from rules.RulesManager.Terms import Operator, Value
from model.models import Classifier, Property


class NumericalRule(BaseRule):
    """
    The syntax of the NumericalRule rule is: classifier.property operator #.
    The purpose of this rule is to enforce a property of an object to meet a certain value requirement based on the operator.
    """

    operator_keys = [">", ">=", "==", "<", "<="]

    text_examples = [
        "Product price > 0",
        "Elevator load should be at most 5000 kg.",
        "Team size should be at least two members.",
        "The capacity of a small delivery truck is equal to 1000 kgs.",
    ]

    @staticmethod
    def is_type(termlist):
        return termlist.count(1, 1, 1, 1) and termlist[Operator, 0] in NumericalRule.operator_keys

    def get_processed_text(self):
        return (
            self.termlist[Classifier, 0].name
            + "."
            + self.termlist[Property, 0].name
            + " "
            + self.termlist[Operator, 0]
            + " "
            + self.termlist[Value, 0]
        )

    def get_validator(self):
        return get_standard_if_statement(
            "int(value) " + self.termlist[Operator, 0] + " " + self.termlist[Value, 0], self
        )

    def add_validator(self):
        VP_add_validator(self.termlist[Property, 0], self.pk, self.get_validator())

    def remove_validator(self):
        VP_remove_validator(self)
