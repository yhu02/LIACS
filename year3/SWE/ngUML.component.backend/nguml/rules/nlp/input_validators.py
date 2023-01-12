"""Validate rules with regards to the mentioned metadata.

The coordinating function is validate_metadata(). It returns a dictonary of aggregated errors
per rule. This can be used to skip the further handling of problematic rules, while enabling
feedback to the user.

Assertions enforced:
    - A rule may only contain:
        1. One class and one property.
        2. Two classes and zero properties.
    - Under assertion 1:
        A. The mentioned class should possess the mentioned property.
    - Under assertion 2:
        A. The mentioned classes should be directly linked.

Assumptions, based on models.py:
    - Different property names can be identical within the same class or among
    different classes.
    - The name of a class is unique when considered within a single model, where
    model is a class model, activity model, ..
    - Metadata validation is performed over classes within a single model, so
    class names are unique.

Hard errors:
    Errors that raise an exception and stop script execution.
    - No classes in metadata
    - Classes in metadata containing one or more properties that are identically named to
    the owning class. These rules cannot be evaluated correctly.

Soft errors:
    Errors do not stop script execution. They are gathered and returned in the dict of
    aggregated errors.
    validate_number_classes()
        - Checks: a rule should contain one or two classes.
    validate_number_properties()
        - Checks: a rule with one class contains one property.
        - Checks: a rule with two classes contains no properties.
    validate_direct_relation()
        - Checks: assetion 2A.
    validdate_duplicate_properties()
        - Checks: a class-property pair is mentioned in a rule that cannot be resolved
                because the class has two or more properties with the same mentioned name.
    validate_stray_properties()
        - Checks: a rule should not contain properties from classes that are not mentioned
                in the rule.
"""


def get_connections(metadata):
    """Extract connections from metadata, formatted for usage in NLP validation.

    Args:
        metadata:
            As defined under /api/model/class/{project_id}/{system_id} in the API explorer.

    Returns:
        connections:
            A list of dicts. Every dict represents a connection in the metadata. The dicts
            contains data in the following format:
                "name": "give",
                "type": "association",
                "labelFrom": "*",
                "label": "give",
                "labelTo": "*",
                "from": "Lecturer",
                "to": "Lecture",
    """
    connections = []
    for con in metadata["connections"]:
        connection = con.copy()
        connection.pop("id")
        from_id = con["from"]
        to_id = con["to"]
        for node in metadata["nodes"]:
            if node["type"] == "Class":
                if node["id"] == from_id:
                    connection["from"] = node["name"]
                elif node["id"] == to_id:
                    connection["to"] = node["name"]
        connections.append(connection)
    return connections


def validate_number_classes(err_report, metadata_found):
    """Enforces the assertion that a rule may only contain one or two classes.

    See all input assertions at validate_metadata().

    Args:
        metadata_found:
            A list with a dict for every rule. The dict contains class-names as keys
            with a list of corresponding property-names.

    Returns:
        err_report:
            A dictionary with a key for every rule from the input. Each key has
            a corresponding list containing the aggregated errors.
    """
    for i in range(len(metadata_found)):
        if len(metadata_found[i].keys()) == 0:
            err_report[i].append("No classes found in rule.")
        elif len(metadata_found[i].keys()) > 2:
            err_report[i].append("More than 2 classes found in rule.")
    return err_report


def validate_number_properties(err_report, metadata_found):
    """Enforces assertions 1 and 2 as defined at validate_metadata().

    Args:
        metadata_found:
            A list with a dict for every rule. The dict contains class-names as keys
            with a list of corresponding property-names.
        err_report:
            A dictionary with a key for every rule from the input. Each key has
            a corresponding list containing the aggregated errors.

    Returns:
        err_report
    """
    for i in range(len(metadata_found)):
        if len(metadata_found[i].keys()) == 1:
            for cl in metadata_found[i].keys():
                if len(metadata_found[i][cl]) < 1:
                    err_report[i].append(
                        "Missing property. If a rule contains one class, " + "it should also contain one property."
                    )
                elif len(metadata_found[i][cl]) > 1:
                    err_report[i].append(
                        "Too many properties. If a rule contains one class, " + "it should also contain one property."
                    )
        elif len(metadata_found[i].keys()) == 2:
            for cl in metadata_found[i].keys():
                if len(metadata_found[i][cl]) > 0:
                    err_report[i].append(
                        "Too many properties. If a rule contains two classes, " + "it should contain no properties."
                    )
    return err_report


def validate_direct_relation(err_report, metadata_found, connections):
    """For rules with two classes, check if they are directly connected.

    Args:
        err_report:
            A dictionary with a key for every rule from the input. Each key has
            a corresponding list containing the aggregated errors.
        metadata_found:
            A list with a dict for every rule. The dict contains class-names as keys
            with a list of corresponding property-names.
        connections:
            As defined at get_connections().

    Returns:
        err_report:
    """
    i = 0
    while i < len(metadata_found):
        if len(metadata_found[i]) == 2:
            found = False
            for con in connections:
                if con["from"] in metadata_found[i].keys() and con["to"] in metadata_found[i].keys():
                    found = True
                    break
            if not found:
                err_report[i].append(
                    "No direct connection. The classes '"
                    + list(metadata_found[i].keys())[0]
                    + "' and '"
                    + list(metadata_found[i].keys())[1]
                    + "' were found. They do not share a direct connection, "
                    + "therefore no rules can be applied."
                )
        i += 1
    return err_report


def validate_m2m_relation(err_report, metadata_found, connections):
    """Check if the relationship between the two classes found in the metadata
    is of the type Many-to-Many, insert error message if so.

    Args:
        err_report:
            A dictionary with a key for every rule from the input. Each key has
            a corresponding list containing the aggregated errors.
        metadata_found:
            A list with a dict for every rule. The dict contains class-names as keys
            with a list of corresponding property-names.
        connections:
            As defined at get_connections().

    Returns:
        err_report:
    """
    for i in range(len(metadata_found)):
        if len(metadata_found[i]) == 2:
            for con in connections:
                if con["from"] in metadata_found[i].keys() and con["to"] in metadata_found[i].keys():
                    if con["labelFrom"] == "*" and con["labelTo"] == "*":
                        err_report[i].append(
                            "Found many-to-many relationship for the classes '"
                            + list(metadata_found[i].keys())[0]
                            + "' and '"
                            + list(metadata_found[i].keys())[1]
                            + "'. Many-to-many class relationships "
                            + "are not implemented yet,"
                            + " therefore no rules can be applied."
                        )
    return err_report


def validate_duplicate_properties(err_report, metadata_found, class_props):
    """Check if a class-property pair is mentioned in a rule that cannot be resolved
    because the class has two or more properties with the same mentioned name.

    Args:
        err_report:
            A dictionary with a key for every rule from the input. Each key has
            a corresponding list containing the aggregated errors.
        metadata_found:
            A list with a dict for every rule. The dict contains class-names as keys
            with a list of corresponding property-names.
        class_props:
            A dictionary containing all classes and their corresponding properties in the
            metadata. Every class-key also has a nested list, used to contain the property names
            of properties that share the same name for the same class.

    Returns:
        err_report
    """
    for i in range(len(metadata_found)):
        for cl in metadata_found[i].keys():
            for prop in metadata_found[i][cl]:
                if prop in class_props[cl][0]:
                    err_report[i].append(
                        "Could not resolve which property was meant. Found class '"
                        + cl
                        + "' and property '"
                        + prop
                        + "' in rule string. '"
                        + cl
                        + "' has 2 or more properties with this name."
                    )
    return err_report


def validate_stray_properties(doc, class_props, metadata_found, err_report):
    """Check for every input rule if it contains stray properties.

    Stray properties are terms in the rule that do not correspond to any of the linked
    (mentioned) classes in the rule, but DO correspond to properties from other classes in
    the metadata.

    Args:
        doc
        class_props
        metadata_found
        err_report

    Returns:
        stray_props:
            A dictionary with a key for every rule from the input, each key has a list
            containing the stray properties. The keys in this dict correspond to the
            keys in err_report.
    """
    stray_props = {}
    for i in range(len(doc.sentences)):
        marked = [9 for word in doc.sentences[i].words]
        stray_rule = []
        word_idx = 0
        for word in doc.sentences[i].words:
            for cl in metadata_found[i].keys():
                for prop in metadata_found[i][cl]:
                    if word.lemma == cl.lower() or word.lemma == prop.lower():
                        marked[word_idx] = -1
                        break
            word_idx += 1
        word_idx = 0
        for word in doc.sentences[i].words:
            for cl in class_props.keys():
                for prop in class_props[cl][1:]:
                    if marked[word_idx] != -1 and (word.lemma == prop.lower()):
                        stray_rule.append((word.text, prop))
                        break
            word_idx += 1
        if len(stray_rule) > 0:
            err_str = ""
            for item in stray_rule:
                err_str += "Word '" + item[0] + "' was linked to property '" + item[1] + "'. "
            err_str = err_str[:-1]
            err_report[i].append("Stray property found. " + err_str)
        stray_props[i] = stray_rule
    return err_report, stray_props


def err_get_metadata(err_report, metadata_found, stray_props):
    """Get the associated metadata when one or more errors are found for ar rule.

    Args:
        err_report:
            A dictionary with a key for every rule from the input. Each key has
            a corresponding list containing the aggregated errors.
        metadata_found:
            A list with a dict for every rule. The dict contains class-names as keys
            with a list of corresponding property-names.
        stray_props:
            A dictionary with a key for every rule from the input, each key has a list
            containing the stray properties. The keys in this dict correspond to the
            keys in err_report.

    Returns:
        err_report
    """
    for err in err_report.keys():
        if len(err_report[err]) > 0:
            context = "Metadata found: "
            for cl in metadata_found[err].keys():
                context += " Class: '" + cl + "'"
                if len(metadata_found[err][cl]) > 0:
                    context += " with properties: "
                    for prop in metadata_found[err][cl]:
                        context += "'" + prop + "', "
                    context = context[:-2]
                context += "."
            if len(stray_props[err]) > 0:
                context += " Stray properties found: "
                for prop in stray_props[err]:
                    context += "'" + prop[1] + "', "
                context = context[:-2]
                context += "."
            err_report[err].append(context)
    return err_report


def get_metadata_per_rule(doc, class_props):
    """Get the mentioned classes and properties per rule and return.

    Iterate over every word per rule in the doc and try to match with existing metadata.
    1. Gather all classes.
    2. Gather all properties, considering only those that are linked to the extracted
    classes.

    Args:
        doc:
            Stanza's Document object. Contains annotated corpus.
        class_props:
            As defined at get_class_props()

    Returns:
        metadata_found:
            A list with a dict for every rule. The dict contains class-names as keys
            with a list of corresponding property-names.
    """
    metadata_found = []
    for i in range(len(doc.sentences)):
        metadata_rule = {}
        for word in doc.sentences[i].words:
            for cl in class_props.keys():
                if word.lemma == cl.lower() and cl not in metadata_rule.keys():
                    metadata_rule[cl] = []
                    break
        metadata_found.append(metadata_rule)
    for i in range(len(doc.sentences)):
        for word in doc.sentences[i].words:
            for cl in metadata_found[i].keys():
                for prop in class_props[cl][1:]:
                    if word.lemma == prop.lower() and prop not in metadata_found[i][cl]:
                        metadata_found[i][cl].append(prop)
                        break
    return metadata_found


def validate_class_prop_unique(class_props):
    """Raise exception if metadata contains classes containing properties with the same
    name.

    Raise exception because this results in the inability to unambiguously annotate input.

    Args:
        class_props:
            A dictionary containing all classes and their corresponding properties in the
            metadata. Every class-key also has a nested list, used to contain the property names
            of properties that share the same name for the same class.
    """
    for cl in class_props.keys():
        for prop in class_props[cl]:
            if cl == prop:
                raise Exception("Error: Metadata contains properties with the same name as their class.")


def validate_metadata(metadata, doc, class_props, connections):
    """Validate the rules with regards to the mentioned metadata.

    Args:
        metadata
        doc
        class_props

    Returns:
        err_report
    """
    if len(metadata["nodes"]) == 0:
        raise Exception("No classes found in metadata for this system.")
    validate_class_prop_unique(class_props)
    metadata_found = get_metadata_per_rule(doc, class_props)
    err_report = {}
    for i in range(len(metadata_found)):
        err_report[i] = []
    err_report, stray_props = validate_stray_properties(doc, class_props, metadata_found, err_report)
    err_report = validate_number_classes(err_report, metadata_found)
    err_report = validate_duplicate_properties(err_report, metadata_found, class_props)
    err_report = validate_number_properties(err_report, metadata_found)
    err_report = validate_direct_relation(err_report, metadata_found, connections)
    err_report = validate_m2m_relation(err_report, metadata_found, connections)
    err_report = err_get_metadata(err_report, metadata_found, stray_props)
    return err_report
