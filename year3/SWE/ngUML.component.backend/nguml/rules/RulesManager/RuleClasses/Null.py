from rules.RulesManager.Terms import Operator
from model.models import Classifier, Property
from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.RulesManager.BaseRule import BaseRule

# TODO: Add a generic add field thingy for a property instead of OP validator
class NullRule(BaseRule):
    """
    The syntax of the NotNullRule rule is: classifier.property "NOT NULL".
    The purpose of this rule is to enforce a property of an object to not be null or empty.
    """

    operator_keys = ["NULL"]

    text_examples = [
        "The description of an item can be empty.",
    ]

    def is_type(termlist):
        return (termlist.count(1, 1, 2, 0) or termlist.count(1, 1, 1, 0)) and NullRule.operator_keys[
            0
        ] in termlist.get_all(Operator)

    def get_processed_text(self):
        return self.termlist[Classifier, 0].name + "." + self.termlist[Property, 0].name + " " + "NOT NULL"

    def add_validator(self):
        if "NOT" in self.termlist[Operator, 0]:
            add_keyword(self.termlist[Property, 0], "blank", False)
        else:
            add_keyword(self.termlist[Property, 0], "blank", True)

    def remove_validator(self):
        pass
