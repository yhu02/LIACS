"""
Here, rule lists are validated. This separate validation script is created to signify
the difference of the functions below in compared to the functions in input_validators.py.

The functions within this script utilize behavior from rule_str_generator.py to enable
the enforcement of constraints on rules. For example, validate_m2o_direction() applies
the functionalities that are used for determining the directionality of a rule.
"""

from rule_str_generator import get_target_context, get_m2o_direction


def get_nr_classes(rule):
    """Return the number of classes for a rule."""
    classes = []
    for i in range(len(rule)):
        if rule[i][1] == "class" and rule[i][0] not in classes:
            classes.append(rule[0])
    return len(classes)


def validate_m2o_direction(rules_list, doc, err_report, connections):
    """Check if the direction of a many-to-one rule matches the dependency
    relationship in the model, append error if not.

    In a Mentor(One)-(Many)Mentee relationship, inclusive demarcation rules can
    only be enforced when they restrict the Many side of the relationship. This
    is the case because the One side of the relationship should always have
    a multiplicity of 1.

    Assumption:
        No many-to-many relationships are passed to this function.

    Args:
        rules_list:
            List with annotated terms that were extracted from rule input.
        doc:
            Stanza's annotated Document object.
        err_report:
            A dictionary with a key for every rule from the input. Each key has
            a corresponding list containing the aggregated errors.
        connections:
            List of dicts. Every dict contains information on the direct relationships
            within the metadata.
    """
    for i in range(len(rules_list)):
        if get_nr_classes(rules_list[i]) == 2:
            target_context = get_target_context(rules_list[i], doc, i)
            possessor, possession = get_m2o_direction(target_context)
            for con in connections:
                if (
                    con["labelFrom"] == "1"
                    and con["labelTo"] == "*"
                    and possessor == con["to"]
                    and possession == con["from"]
                ) or (
                    con["labelFrom"] == "*"
                    and con["labelTo"] == "1"
                    and possessor == con["from"]
                    and possession == con["to"]
                ):
                    err_report[i].append(
                        "m2o: wrong direction. Tried to enforce rule from '"
                        + possessor
                        + "' to '"
                        + possession
                        + "', but '"
                        + possessor
                        + "' is the dependent class. "
                    )
    return err_report
