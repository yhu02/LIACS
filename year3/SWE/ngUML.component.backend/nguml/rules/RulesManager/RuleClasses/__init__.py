import os
import importlib

for module in os.listdir("rules/RulesManager/RuleClasses"):
    if module == '__init__.py' or module[-3:] != '.py':
        continue
    importlib.import_module("rules.RulesManager.RuleClasses." + module[:-3])
del module

# Load all classes dynamically (Is needed for BaseRule.__subclasses__() and
# making all classes accessible by either RuleClasses.classname or from RuleClasses import *)

# Filename should be identical to the class name
