from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.RulesManager.BaseRule import BaseRule

# TODO: If two different classes, then .save for the relationship
# TODO: Add validator for databaseObject.save() instead of just a single property
# class PropertiesEqualValue(BaseRule):
#     """
#     The syntax of the PropertiesEqualValue rule is: classifier.property classifier.property EQUAL #.
#     The purpose of this rule is to enforce two properties of the same classifier to be equivalent to a certain value.
#     """
#     @staticmethod
#     def is_type(dict):
#         equal = ["=="]
#         if(len(dict)["classifiers"]==1 and len(dict["properties"])==2 and dict["operators"][0] in equal):
#             return True
#         else:
#             return False

#     def get_processed_text(self):
#         return self.rule_db.classifiers.all()[0].name + "." + self.rule_db.properties.all()[0].name + self.rule_db.classifiers.all()[0].name + "." + self.rule_db.properties.all()[1].name + "EQUALS" + self.rule_db.value

#     def add_validator(self):
#         pass
