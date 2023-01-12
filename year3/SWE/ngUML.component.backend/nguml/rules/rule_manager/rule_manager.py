from rule_classes.DomainRule import DomainRule

"""Return indentation that's to be used when writing code.
"""


def get_indent():
    indent = " "
    indentation = 4
    return indent * indentation


"""Return the code for overarching validation function.

This function combines all related rule-block components.
E.g., "... |=[2,4]| or (|=10| and |=5|)" results in three separate
blocks, combined using the supplied logical operators and brackets:
b1 or (b2 and b3).

Args:
    val_name:
        String containing name of validator.
    scope:
        Scope-level (top-level) representation of rule blocks as list
        e.g. ['b1', 'and', '(', 'b2', 'and', 'b3', ')'].
    val_params:
        List of parameters that the validation function takes.
    indent:
        Indent-size to be used for writing code.
"""


def get_main_val_code(val_name, scope, val_params, indent):
    val_params = ", ".join(val_params)
    output = "def " + val_name + "(" + val_params + "):\n"
    validator_blocks = ""
    for item in scope:
        if "b" in item:
            validator_blocks += val_name + "_" + item + "(" + val_params + "), "
    # Remove last comma and space
    validator_blocks = validator_blocks[:-2]
    output += indent + "results = [" + validator_blocks + "]\n"
    output += indent + "if not ("
    for item in scope:
        if "b" in item:
            output += "results[" + str(int(item[-1]) - 1) + "][0]"
        else:
            output += item.lower()
    output += "):\n"
    output += indent * 2 + "errors = []\n"
    output += indent * 2 + "for result in results:\n"
    output += indent * 3 + "if result[0] == 0:\n"
    output += indent * 4 + "errors.append(result[1])\n"
    output += indent * 2 + "raise ValidationError(errors)\n\n"
    return output


"""Check if directories and files for DomainRules exist for prototype,
create if not.
"""


def prepare_files(rule_obj):
    pass


"""Create rule object and return this.

Args:
    rule_str:
        Structured syntax definition of domain rule as string. e.g.
        'DomainRule---field.Person.address---|int=4| and |char or int=[3,4] or 6|)'
"""


def create_rule(rule_str):
    rule_type = rule_str.split("---")[0]
    if rule_type == "DomainRule":
        return DomainRule(rule_str)
    # TODO
    # Add to database?


"""Get and return the python code for overarching validation function and its
rule-block components.
"""


def get_validator_code(rule_obj):
    indent = get_indent()
    if isinstance(rule_obj, DomainRule):
        val_name = rule_obj.get_val_name()
        val_params = rule_obj.get_val_params()
        main_val_code = get_main_val_code(val_name, rule_obj.scope, val_params, indent)
        blocks_val_code = rule_obj.get_blocks_code(indent)
        return main_val_code, blocks_val_code


"""Write the python code for overarching validation function and its
rule-block components to prototype file.

Args:
    main_val_code:
        Code for overarching validation function.
    blocks_val_code:
        Code for rule-block components.
    rule_obj:
        Rule object.
"""


def write_validator_code(main_val_code, blocks_val_code, rule_obj):
    pass


def link_validator():
    pass
