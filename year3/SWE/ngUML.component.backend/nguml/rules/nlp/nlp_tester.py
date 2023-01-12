from pipeline import get_rules_list
from rule_str_generator import get_rule_strings

rules_list, doc = get_rules_list()
rule_strings = get_rule_strings(rules_list, doc)
