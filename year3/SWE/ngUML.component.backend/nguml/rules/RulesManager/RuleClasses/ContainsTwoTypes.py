from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.RulesManager.BaseRule import BaseRule
from rules.RulesManager.Terms import Operator
from model.models import Classifier, Property


class ContainsTwoTypes(BaseRule):
    """
    The syntax of the ContainsTwoTypes rule is: classifier.property CONTAINS # TYPE1 AND # TYPE2.
    The purpose of this rule is to enforce a property to consist of two types only.
    """

    operator_keys = ["NUMBERS", "LETTERS"]

    text_examples = ["A zipcode of an adress should only consist of numbers and letters."]

    def is_type(termlist):
        return termlist.count(1, 1, 2, 0) and termlist[Operator, 0].name in ContainsTwoTypes.operator_keys

    def get_processed_text(self):
        return (
            self.termlist[Classifier, 0].name
            + "."
            + self.termlist[Property, 0].name
            + " CONTAINS "
            + self.termlist[Operator, 0]
            + " AND "
            + self.termlist[Operator, 1]
        )

    def get_validator(self):
        # TODO: Do it with regular expressions
        return get_standard_if_statement("not value.isalpha() and not value.isnumeric() and not len(value) == 0", self)

    def add_validator(self):
        VP_add_validator(self.termlist[Property, 0], self.pk, self.get_validator())

    def remove_validator(self):
        VP_remove_validator(self)
