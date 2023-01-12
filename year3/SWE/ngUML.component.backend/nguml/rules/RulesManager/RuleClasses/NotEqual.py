from rules.processors.ValidatorProcessor import (
    add_keyword,
    get_standard_if_statement,
    add_validator as VP_add_validator,
    remove_validator as VP_remove_validator,
)
from rules.RulesManager.BaseRule import BaseRule


# class NotEqual(BaseRule):
#     """
#     The syntax of the NotEqual rule is: classifier.property NOT(EQUAL) #
#     The purpose of this rule is to enforce a property of an object to not equivalent to a certain value.
#     """
#     @staticmethod
#     def is_type(dict):
#         notEqual = ["NOT"]
#         if(len(dict["classifiers"])==1 and len(dict["properties"])==1 and dict["operators"][0] in notEqual):
#             return True
#         else:
#             return False

#     def get_processed_text(self):
#         return self.rule_db.classifiers.all()[0].name + "." + self.rule_db.properties.all()[0].name + " " + self.rule_db.operator + " " + self.rule_db.value

#     def add_validator(self):
#         pass
