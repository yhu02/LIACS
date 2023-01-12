"""Parse structured rule string and return information to be used
for DomainRule class.

Args:
    rule str:
        As defined at DomainRule class.
"""


def parse_rule(rule_str):
    rule_split = rule_str.split("---")
    target_info = rule_split[1].split(".")
    blocks = rule_split[2].split("|")
    block_count = 1
    scope = []
    for block in blocks:
        if "=" in block:
            scope.append("b" + str(block_count))
            block_count += 1
        else:
            scope.append(block)
    declarations = []
    for block in blocks:
        if "=" in block:
            declarations.append(block.split("="))
    return target_info, scope, declarations


"""Return error message with the handled parameters taken into account.

Args:
    val_params:
        List of parameters that a validation function takes.
"""


def get_error_code(val_params):
    output = 'errors.append(ValidationError(_("%('
    for param in val_params:
        output += param + ") err_msg, _(%("
    output = output[:-6]
    output += '"), params={"'
    for param in val_params:
        output += param + '": ' + param + ', "'
    output = output[:-3]
    output += "},))\n"
    return output


def get_sub_type_code(sub_type_rule):
    # Check individual symbols
    if "*" in sub_type_rule:
        sub_type_rule = sub_type_rule.replace("*", "")
        return 'check_input_type(value,"' + sub_type_rule + '")'
    return 'check_symbol_type(symbol, "' + sub_type_rule + '")'


"""Return modified list with type-level or range-level condition for
easier downstream processing.

Args:
    declaration:
        String with type or range-level condition.
        e.g. 'let or int'  returns ['let', 'OP_or', 'int']
        '([3,4] and 6)'     returns ['(', '[3,4]' 'OP_and' '6', ')']
"""


def preprocess_declaration(declaration):
    ops = ["or", "and", "xor"]
    # Surround brackets in declaration with whitespace for easy parsing
    decla = [" " + item + " " if item == "(" or item == ")" else item for item in declaration]
    # Insert OP_ prefix for operators in declaration for easy parsing
    return ["OP_" + item if item in ops else item for item in "".join(decla).split()]


"""Return python code for type-condition in rule block.

Args:
    type-rule:
        String with type-condition.
        e.g. 'let or int'
"""


def get_type_code(type_rule, val_params, indent):
    append_err = get_error_code(val_params)
    type_c = preprocess_declaration(type_rule)
    if "*" in type_rule:
        type_code = indent + "if not("
        end_statement = indent * 2 + append_err
    else:
        type_code = indent + "for symbol in value:\n"
        type_code += indent * 2 + "if not("
        end_statement = indent * 3 + append_err
        end_statement += indent * 3 + "break\n"
    for item in type_c:
        if "OP_" in item:
            type_code += " " + item[3:] + " "
        elif "(" in item or ")" in item:
            type_code += item
        else:
            type_code += get_sub_type_code(item)
    type_code += "):\n" + end_statement
    f = open("test.txt", "w")
    f.write(type_code)
    f.close()
    return type_code


"""Return value that's to be compared in order to enforce domain rule.
"""


def get_compare_value(condition, target_info, field):
    if field is True:
        # Dereference symbol > compare the actual value.
        if "*" in condition[0]:
            return "value"
        # Compare how many of a certain type.
        else:
            return "len(value)"
    else:
        if target_info[2] == "m2m":
            # target_info[4] = look-up operator
            return "len(instance." + target_info[4] + ".all())"
        elif target_info[2] == "m2o":
            # target_info[4] = property with foreign key entry
            # target_info[5] = look-up operator
            return "len(instance." + target_info[4] + "." + target_info[5] + ".all())"
        else:
            raise Exception("Invalid relation type passed")


"""Return python code for components of range-condition.

Where x is compare_value as received from get_compare_value()
    [3,4]   returns 3 <= compare_value <= 4
    ,3}     returns compare_value < 3
    3       returns compare_value == 3

Args:
    range_rule:
        List with items of range-condition component. e.g. '[3,4]'.
    range_code:
        String that will hold the final python code. When received
        it consists of the value that is compared, as chosen in
        get_compare_value().
"""


def get_sub_range_code(range_rule, range_code):
    if "," in range_rule:
        range_rule = range_rule.split(",")
        # type=,5] or (type=[3,.. and type=..,6])
        if range_rule[1] != "" or (range_rule[0] != "" and range_rule[1] != ""):
            # range_code -> x <= 6
            if range_rule[1] != "0":
                if range_rule[1][-1] == "]":
                    range_code += " <= " + range_rule[1][:-1]
                elif range_rule[1][-1] == "}":
                    range_code += " < " + range_rule[1][:-1]
            # range_code -> 3 <= x <= 6
            if range_rule[0] != "":
                if range_rule[0][0] == "[":
                    range_code = range_rule[0][1:] + " <= " + range_code
                elif range_rule[0][0] == "{":
                    range_code = range_rule[0][1:] + " < " + range_code
        elif range_rule[1] == "":
            if range_rule[0][0] == "[":
                range_code += " >= " + range_rule[0][1:]
            if range_rule[0][0] == "{":
                range_code += " > " + range_rule[0][1:]
    else:
        range_code += " == " + range_rule
    return range_code


"""Return python code for range-condition in rule block.

Args:
    condition:
        List with 2 items. The first is type-condition and second
        is range-condition. e.g.
        ['let or int', '[3,4] or 6']
"""


def get_range_code(condition, target_info, val_params, indent, field):
    append_err = get_error_code(val_params)
    range_c = preprocess_declaration(condition[1])
    compare_value = get_compare_value(condition, target_info, field)
    range_code = indent + "if not("
    for item in range_c:
        if "OP_" in item:
            range_code += " " + item[3:] + " "
        elif "(" in item or ")" in item:
            range_code += item
        else:
            range_code += get_sub_range_code(item, compare_value)
    range_code += "):\n" + indent * 2 + append_err
    return range_code


"""Compile function-code for rule blocks and return.

Returns:
    A list where every item is a string with function-code that represents
    a rule block.
"""


def get_blocks_code(target_info, declarations, val_name, val_params, indent):
    blocks_code = []
    block_count = 1
    for item in declarations:
        block_code = "def " + val_name + "_b" + str(block_count) + "(" + ", ".join(val_params) + "):\n"
        block_code += indent + "errors = []\n"
        if target_info[0] == "field":
            if item[0] != "":
                block_code += get_type_code(item[0], val_params, indent)
            if item[1] != "":
                block_code += get_range_code(item, target_info, val_params, indent, True)
        elif target_info[0] == "rel":
            block_code += get_range_code(item, target_info, val_params, indent, False)
        block_code += indent + "if len(errors) > 0:\n"
        block_code += indent * 2 + "return[0, errors]\n"
        block_code += indent + "return [1]\n\n"
        blocks_code.append(block_code)
        block_count += 1
    return blocks_code


"""Datatype containing domain-rule information.

Args:
    rule_str:
        Structured syntax definition of domain rule as string. e.g.
        'DomainRule---field.Person.address---|int=4| and |let or int=[3,4] or 6|)'

Attributes:
    rule_str:
        As described.
    target_info:
        List of target-components. e.g ['field', 'Person', 'address']
    scope:
        Scope-level (top-level) representation of rule blocks as list
        e.g. ['b1', 'and', '(', 'b2', 'and', 'b3', ')']
    declarations:
        List of rule blocks, where every rule block is split into
        type-requirement and range-level requirements.
        e.g. [['int', '4'], ['let or int', '[3,4] or 6']]
"""


class DomainRule:
    # rule_str = "DomainRule---field.Person.address---|int or let=5|"
    def __init__(self, rule_str):
        target_info, scope, declarations = parse_rule(rule_str)
        self.rule_str = rule_str
        self.target_info = target_info
        self.scope = scope
        self.declarations = declarations

    def __str__(self):
        return (
            "rule type: "
            + "\tDomainRule\n"
            + "target: "
            + "\t"
            + self.rule_str.split("---")[1]
            + "\n"
            + "rule: "
            + "\t" * 2
            + self.rule_str.split("---")[2]
        )

    def get_val_name(self):
        if self.target_info[0] == "field":
            return "domain_" + self.target_info[1] + "_" + self.target_info[2]
        elif self.target_info[0] == "rel":
            return "domain_" + self.target_info[1] + "_" + self.target_info[2] + "_" + self.target_info[3]

    """Return the parameters that domain validator function needs.
    """

    def get_val_params(self):
        if self.target_info[0] == "field":
            return ["value"]
        elif self.target_info[0] == "rel":
            return ["instance"]

    def get_blocks_code(self, indent):
        val_name = self.get_val_name()
        val_params = self.get_val_params()
        return get_blocks_code(self.target_info, self.declarations, val_name, val_params, indent)
