from get_metadata import get_metadata, get_connections

"""
Convert rule lists, as compiled in the nlp pipeline, to structured rule strings.

Input assumptions:
    --------- Poperty rules ---------
    - Explicitly defined type constraints. E.g., A username should be between
    10 and 20 letters >> A username should be between 10 letters and 20 letters.
    - If a numerical value is given without mentioning a type constraint, the specified
    value is not interpreted as a constraint on the length of the attribute. Rather,
    it is interpreted as the value that the input should be.
        E.g., 'A customer's age should be twenty one' enforces that the age of a customer
        should be twenty one. 'A customer's username should be twenty one letters'
        enforces that a customter's username should consist of twenty one letters.
    - Enforcing a string value is done explicitly using double quotes.
        E.g., 'The department of an employee should be "IT" or "HR"' enforces that only
        the two specified values are valid for the department property.
"""


def get_target_context(rule, doc, rule_idx):
    """Get the context regarding the targets in a rule, used down-stream
    for generating the target-string.

    Firstly, check if the target-string can be constructed using noun-subject
    dependency in the annotated rule string. If not, gather information for a
    simple pattern heuristic.

    Args:
        rule:
            Rule list containing only the extracted terms from the original rule input.
        doc:
            Stanza's annotated Document object.
        rule_idx:
            The index of the current rule.

    Returns:
        target_context:
            List containing information regarding the targets in a string.
    """
    target_context, classifiers = [], []
    nsubj_class = False
    for term in rule:
        if term[1] == "class" and term[0].lower() not in classifiers:
            classifiers.append(term[0].lower())
    annotated_str = doc.sentences[rule_idx]
    for word in annotated_str.words:
        if word.deprel == "nsubj" and word.text in classifiers:
            target_context.append(word.text)
            nsubj_class = True
            break
    if nsubj_class is True:
        for classifier in classifiers:
            if classifier not in target_context:
                target_context.append(classifier)
                target_context = [cl[:1].upper() + cl[1:] for cl in target_context]
                return target_context
    # If no classifier was associated with nsubj dependency, then
    # provide info for simple heuristic.
    for item in rule:
        if item[1] == "class":
            target_context.append(item[0][:1].upper() + item[0][1:])
        else:
            if "RANGE" not in target_context:
                target_context.append("RANGE")
    return target_context


def get_constraint_string(rule, rule_count, proto_constr):
    """Convert proto-constraints to structured string and return.

    Args:
        rule:
            List containing only the extracted terms from the original rule input.
        rule_count:
            A list containing the number of classes, properties, numbers and
            terminals in the rule string.
        block:
            List of lists. Every sub-list contains a proto-constraint as defined
            at get_proto_constraints().

    Returns:
        Structured rule string containing the constraints for a rule.
    """
    constraints = ""
    for item in proto_constr:
        if type(item) is list:
            constraints += "|" + get_blocks_string(rule, rule_count, item) + "|"
        elif "log_op_" in item:
            constraints += " " + item[7:] + " "
        elif "lit_" in item:
            constraints += '|*let="' + item[4:] + '"|'
    return constraints


def get_blocks_string(rule, rule_count, block):
    """Convert a proto-constraint to structured rule-string for a block.

    Args:
        rule:
            List containing only the extracted terms from the original rule input.
        rule_count:
            A list containing the number of classes, properties, numbers and
            terminals in the rule string.
        block:
            Single proto-constraint.

    Returns:
        Structured string for a proto-constraint.
    """
    block_string = [-1, "=", -1]
    if block[0] != -1 and block[1] != -1:
        val_op = rule[block[0]][0]
        num = str(rule[block[1]][0])
        if val_op == "{" or val_op == "[":
            block_string[2] = val_op + str(num) + ","
        elif val_op == "}" or val_op == "]":
            block_string[2] = "," + str(num) + val_op
        elif val_op == "=":
            block_string[2] = str(num)
    elif block[0] == -1 and block[1] != -1:
        num = str(rule[block[1]][0])
        # Add unsign only when considering property constraints.
        if rule_count[0] == 1:
            block_string[0] = "*int"
        block_string[2] = str(num)
    if block[2] != -1:
        terminal = rule[block[2]][0]
        block_string[0] = terminal
    return "".join([item for item in block_string if item != -1])


def merge_proto_blocks(proto_constr):
    """Merges constraint blocks when there is no operator in between.

    'A customer's age should be 21 letters or 21 letters.' does not explicitly
    use any keywword that is mapped to a value operator. Proto_constr therefore
    consists of [[-1, 2, -1], [-1, -1, 3], 'log_op_or', [-1, 5, -1], [-1, -1, 6]],
    which would be converted to four separate blocks |=21||let=| or |=21||let=|.
    This method merges the consecutive blocks: [[-1, 2, 3], 'log_op_or', [-1, 5, 6]].
    Now, the rule will be converted downstream to |let=21| or |let=21|.

    Args:
        proto_constr:
            See get_proto_constraints().

    Returns:
        proto_constr
    """
    i = 0
    while i < len(proto_constr) - 1:
        if type(proto_constr[i]) is list and type(proto_constr[i + 1]) is list:
            merge = True
            new_block = [-1, -1, -1]
            for j in range(len(proto_constr[i])):
                if proto_constr[i][j] != -1:
                    new_block[j] = proto_constr[i][j]
            for j in range(len(proto_constr[i + 1])):
                if proto_constr[i + 1][j] != -1:
                    # Only combine blocks if no overlapping values
                    # (::= val_op_idx, num_idx, term_idx) assigned.
                    if new_block[j] == -1:
                        new_block[j] = proto_constr[i + 1][j]
                    else:
                        merge = False
            if merge is True:
                proto_constr[i] = new_block
                del proto_constr[i + 1]
        i += 1
    return proto_constr


def get_proto_constraints(rule):
    """Compile and return the proto constraints of a rule.

    Proto constraints is a list of lists. Every sub-list represents a block in the
    final structured rule string, or a logical operator. Every sub-list holds 3
    values: [val_op_idx, num_idx, term_idx], each value corresponds to an index
    from the rule list, or -1 if none is found. A logical operator is represented
    with a string (e.g., log_op_and). Proto constraints is used downstream to generate
    the blocks in string-format.

    Args:
        rule:
            List containing only the extracted terms from the original rule input.

    Returns:
        proto_constr:
            Proto constraints for a single rule, as defined above.
    """
    proto_constr = []
    i = 0
    while i < len(rule):
        if rule[i][1] == "val_op":
            get_range_block(rule, proto_constr, i)
        if rule[i][1] == "num":
            get_type_block(rule, proto_constr, i)
        if rule[i][1] == "terminal":
            found = any(i in block for block in proto_constr if type(block) is list)
            if not found:
                proto_constr.append([-1, -1, i])
        if rule[i][1] == "num":
            found = any(i in block for block in proto_constr if type(block) is list)
            if not found:
                proto_constr.append([-1, i, -1])
        if rule[i][1] == "log_op":
            proto_constr.append("log_op_" + rule[i][0])
        if rule[i][1] == "lit":
            proto_constr.append("lit_" + rule[i][0])
        i += 1
    proto_constr = merge_proto_blocks(proto_constr)
    return proto_constr


def get_range_block(rule, proto_constr, val_idx):
    """Fill proto_constraints list with range constraints.

    Iterate over terms in rule list, starting from the index of a value operator.
    The closest number is linked to the value operator. Save the result in the
    proto_constraints list.

    Args:
        rule:
            List containing only the extracted terms from the original rule input.
        proto_constr:
            List of lists. Every sub-list represents a block of the final rule string.
            It contains the indexes from the terms in the rule list that are used. See
            get_proto_constraints().
        val_idx:
            Index of the value operator in the rule list.
    """
    proto_constr.append([val_idx, -1, -1])
    candidates = [-1, -1]
    j = val_idx
    while j < len(rule):
        if rule[j][1] == "num":
            found = any(j in block for block in proto_constr if type(block) is list)
            if not found:
                candidates[1] = j
            break
        j += 1
    j = val_idx
    while j > 0:
        if rule[j][1] == "num":
            found = any(j in block for block in proto_constr if type(block) is list)
            if not found:
                candidates[0] = j
            break
        j -= 1
    if candidates[0] == -1 and candidates[1] == -1:
        raise Exception(
            "Found value operator without corresponding number for rule:",
            rule,
        )
    elif candidates[1] == -1:
        num_idx = candidates[0]
    elif candidates[0] == -1:
        num_idx = candidates[1]
    elif abs(val_idx - candidates[1]) > abs(val_idx - candidates[0]):
        num_idx = candidates[0]
    else:
        num_idx = candidates[1]
    for k in range(len(proto_constr)):
        if proto_constr[k][0] == val_idx:
            proto_constr[k][1] = num_idx


def get_type_block(rule, proto_constr, num_idx):
    """Fill proto_constraints list with type constraints.

    Iterate over terms in rule list, starting from the index of a number.
    The closest terminal is linked to the value operator. Save the result in the
    proto_constraints list.

    Args:
        rule:
            List containing only the extracted terms from the original rule input.
        proto_constr:
            List of lists. Every sub-list represents a block of the final rule string.
            It contains the indexes from the terms in the rule list that are used. See
            get_proto_constraints().
        num_idx:
            Index of the number in the rule list.
    """
    candidates = [-1, -1]
    j = num_idx
    while j < len(rule):
        if rule[j][1] == "terminal":
            found = any(j in block for block in proto_constr if type(block) is list)
            if not found:
                candidates[1] = j
            break
        j += 1
    j = num_idx
    while j > 0:
        if rule[j][1] == "terminal":
            found = any(j in block for block in proto_constr if type(block) is list)
            if not found:
                candidates[0] = j
            break
        j -= 1
    if candidates[0] == -1 and candidates[1] == -1:
        return proto_constr
    elif candidates[1] == -1:
        term_idx = candidates[0]
    elif candidates[0] == -1:
        term_idx = candidates[1]
    elif abs(num_idx - candidates[1]) > abs(num_idx - candidates[0]):
        term_idx = candidates[0]
    else:
        term_idx = candidates[1]
    for k in range(len(proto_constr)):
        if proto_constr[k][1] == num_idx:
            proto_constr[k][2] = term_idx


def get_alias_query_term(possessor, possession):
    """Return the foreign key alias and query term for structured rule
    string.

    Assumption:
        - Foreign key alias (property-name that holds the foreign key reference),
        is the lowercase class name of the possessing class, as defined in
        RelationshipGenerator.py.

    Args:
        possessor:
            String containing the class name of the possessing class.
        possession:
            String containing the class name of the possessed class.

    Returns:
        Foreign key alias:
            Property-name that holds the foreign key reference.
        Query term:
            The query term that should be used to retrieve the related instances.
    """
    return possessor.lower(), (possession.lower() + "_set")


def get_multiplicity(rule):
    """Return the multiplicity of a direct relationship as a string.

    Returns:
        A string containing either "m2m" or "m2o", used for the structured
        rule string.
    """
    metadata = get_metadata()
    connections = get_connections(metadata)
    classes = []
    for term in rule:
        if term[1] == "class" and term[0] not in classes:
            classes.append(term[0])
    for rel in connections:
        if (
            rel["from"] == classes[0]
            and rel["to"] == classes[1]
            or rel["from"] == classes[1]
            and rel["to"] == classes[0]
        ):
            if rel["labelFrom"] == "*" and rel["labelTo"] == "*":
                return "m2m"
            elif rel["labelFrom"] == "*" and rel["labelTo"] == "1" or rel["labelFrom"] == "1" and rel["labelTo"] == "*":
                return "m2o"
    raise Exception("Could not resolve multiplicity between ", classes[0], " and ", classes[1], ".")


def get_m2o_direction(target_context):
    # Using noun-subject
    if "RANGE" not in target_context:
        possessor = target_context[0]
        possession = target_context[1]
    # Pattern heuristic
    else:
        if target_context[0] == "RANGE":
            possessor = target_context[2]
            possession = target_context[1]
        elif target_context[2] == "RANGE":
            possessor = target_context[1]
            possession = target_context[0]
        else:
            possessor = target_context[2]
            possession = target_context[0]
    return possessor, possession


def get_target_string(rule_li, doc, rule_idx):
    """Construct the target-string and return.

    In the case of a direct relation rule:
    Firstly, check if the target-string can be constructed using noun-subject
    dependency in the annotated rule string. If not, use simple pattern heuristic.

    Args:
        rule_li:
            Rule list containing the extracted terms from the original rule input
            and other related data.
        doc:
            Stanza's annotated document object.
        rule_idx:
            The index of the rule list.

    Returns:
        target_string:
            Part of the final rule-string concerning the target.
    """
    if rule_li[1][0] == 2:
        target_context = get_target_context(rule_li[0], doc, rule_idx)
        possessor, possession = get_m2o_direction(target_context)
        multiplicity = get_multiplicity(rule_li[0])
        alias, query_term = get_alias_query_term(possessor, possession)
        target_string = "rel." + possessor + "." + multiplicity + "." + possession + "." + alias + "." + query_term
    elif rule_li[1][0] == 1 and rule_li[1][1] == 1:
        for item in rule_li[0]:
            if item[1] == "class":
                possessor = item[0]
            elif item[1] == "prop":
                possession = item[0]
        target_string = "field." + possessor + "." + possession
    return target_string


def get_rule_type(input):
    """Get and return the rule type."""
    return "A_DEM_INCL"


def count_terms(rule_li):
    """Count the number or classes, properties, terms and numbers in a rule list.

    The result is added to the rule list.

    Args:
        rule_li:
            Rule list containing the extracted terms from the original rule input
            and other related data.

    Returns:
        rule_li
    """
    classes, properties = [], []
    n_class = n_prop = n_term = n_num = 0
    for i in range(len(rule_li[0])):
        if rule_li[0][i][1] == "class" and rule_li[0][i][0] not in classes:
            n_class += 1
            classes.append(rule_li[0][i][0])
        elif rule_li[0][i][1] == "prop" and rule_li[0][i][0] not in properties:
            n_prop += 1
            properties.append(rule_li[0][i][0])
        elif rule_li[0][i][1] == "terminal":
            n_term += 1
        elif rule_li[0][i][1] == "num":
            n_num += 1
    rule_li.append([n_class, n_prop, n_term, n_num])
    return rule_li


def print_rule_strings(rule_strings, doc):
    """Print the structured rule strings."""
    print("\n" + "=" * 8 + " rules_list " + "=" * 8)
    for i in range(len(rule_strings)):
        print(i + 1)
        print("\t", *[f"{word.text}" for word in doc.sentences[i].words])
        print("\t", rule_strings[i])


def get_rule_strings(rules_list, doc):
    """Generate structured rule strings from rules_list and return them.

    Args:
        rules_list:
            List of lists. Every sub-list contains terms that are extracted from
            the original rule input. Contains ONLY valid rule lists.
            E.g. [[('Publication', 'class'), ('popularity', 'prop'), ('>=', 'val_op'),
            (3, 'num'), ..]
        doc:
            Stanza's annotated Document object.

    Returns:
        rule_strings:
            List strings, every string is a structured rule that is converted to source
            code downstream.
    """
    rule_strings = []
    for i in range(len(rules_list)):
        rules_list[i] = count_terms(rules_list[i])
        proto_constr = get_proto_constraints(rules_list[i][0])
        rule_str = get_rule_type(rules_list[i][0])
        rule_str += "---"
        rule_str += get_target_string(rules_list[i], doc, i)
        rule_str += "---"
        rule_str += get_constraint_string(rules_list[i][0], rules_list[i][1], proto_constr)
        rule_strings.append(rule_str)
    print_rule_strings(rule_strings, doc)
    return rule_strings
