from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.RulesManager.BaseRule import BaseRule
from rules.RulesManager.Terms import Operator
from model.models import Classifier, Property


class ContainsOneType(BaseRule):
    """
    The syntax of the ContainsOneType rule is: classifier.property CONTAINS ONLY TYPE.
    The purpose of this rule is to enforce a property of an object to consist of one type only.
    """

    operator_keys = ["NUMBERS", "LETTERS"]

    text_examples = [
        "The name of a city only contains letters.",
        "A serical code of a product only contains numbers.",
    ]

    def is_type(termlist):
        return termlist.count(1, 1, 1, 0) and termlist[Operator, 0].name in ContainsOneType.operator_keys

    def get_processed_text(self):
        return (
            self.termlist[Classifier, 0].name
            + "."
            + self.termlist[Property, 0].name
            + " CONTAINS ONLY "
            + self.termlist[Operator, 0]
        )

    def get_validator(self):
        # TODO: Do it with regular expressions
        if self.termlist[Operator, 0] == "LETTERS":
            return get_standard_if_statement("value.isalpha()", self.termlist)
        else:
            return get_standard_if_statement("value.isnumeric()", self.termlist)

    def add_validator(self):
        VP_add_validator(self.termlist[Property, 0], self.pk, self.get_validator())

    def remove_validator(self):
        VP_remove_validator(self)
