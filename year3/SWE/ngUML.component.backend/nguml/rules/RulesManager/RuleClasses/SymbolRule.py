from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.RulesManager.BaseRule import BaseRule
from rules.RulesManager.Terms import Operator, Value
from model.models import Classifier, Property


class SymbolRule(BaseRule):
    """
    The syntax of the SymbolRule rule is: classifier.property CONTAINS # SYMBOLS.
    The purpose of this rule is to enforce the property of an object to have a limited amount of specified symbols.
    """

    operator_keys = ["SYMBOLS"]

    text_examples = ["The name of a product should consist of at most 10 symbols.", "Book title maximum 20 symbols."]

    @staticmethod
    def is_type(termlist):
        return termlist.count(1, 1, 1, 0) and termlist[Operator, 0] in SymbolRule.operator_keys

    def get_processed_text(self):
        return (
            self.termlist[Classifier, 0].name
            + "."
            + self.termlist[Property, 0].name
            + " "
            + "CONTAINS"
            + self.termlist[Value, 0]
            + " "
            + self.termlist[Operator, 0]
        )

    def get_validator(self):
        return get_standard_if_statement("len(value) == " + self.termlist[Value, 0], self)

    def add_validator(self):
        VP_add_validator(self.termlist[Property, 0], self.pk, self.get_validator())

    def remove_validator(self):
        VP_remove_validator(self)
