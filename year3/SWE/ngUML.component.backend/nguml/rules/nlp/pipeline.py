import stanza
import json
import os
from word2number import w2n
from input_validators import validate_metadata
from rule_list_validators import validate_m2o_direction
from get_metadata import get_metadata, get_connections

"""
    Input assertions are stated in input_validators.py

    The following should be taken into account to create well-interpretable input strings.

    - Explicitly defined type constraints. E.g., 'A username should be between
    10 and 20 characters' >> 'A username should be between 10 characters and 20 characters'.

    - If a numerical value is given without mentioning a type constraint, the specified
    value is not interpreted as a constraint on the length of the attribute. Rather,
    it is interpreted as the value that the input should be.
        E.g., 'A customer's age should be twenty one' enforces that the age of a customer
        should be twenty one. 'A customer's username should be twenty one characters'
        enforces that a customter's username should be twenty one characters long.

    - Enforcing a string value is done explicitly using double quotes.
        E.g., 'The department of an employee should be "IT" or "HR"' enforces that only
        the two specified values are valid for the department property.
"""


def run_annotation(corpus):
    """Return annotated corpus using Stanford's Stanza model.

    Annotate corpus by applying tokenization, POS tagging, multi-word token expansion,
    and dependency parse.

    Args:
        corpus:
            List of strings, every string is a sentence.

    Returns:
        doc:
            Stanza's Document object containing all annotation information.
    """
    # stanza.download('en') # download English model
    nlp = stanza.Pipeline(
        "en", processors="tokenize,pos,mwt,lemma,depparse", tokenize_no_ssplit=True
    )  # initialize English neural pipeline
    doc = nlp(corpus)
    return doc


def preprocess(corpus):
    """Convert all tokens in the corpus to lower case and return corpus."""
    for i in range(len(corpus)):
        corpus[i] = corpus[i].lower()
    return corpus


def get_input():
    """Get the rules from user input and return a corpus.

    Returns:
        corpus:
            List of strings, separated by newlines in input stream.
    """
    f = open(os.path.join("src", "rules", "nlp", "test_rules.txt"), "r")
    corpus = f.read().splitlines()
    return corpus


def get_operator_keywords():
    """Get all keyword-term mappings from json file and return as dictionary."""
    jsonfile = open(os.path.join("src", "rules", "nlp", "operatorsets.json"), "r")
    operator_keywords = json.load(jsonfile)
    jsonfile.close()
    return operator_keywords


def convert_single_nums(numbers):
    """Convert single numbers in string-form to integers and return.

    Args:
        Numbers is a list of numbers in the form of strings.
        E.g., ['five', '7'].

    Returns:
        List of tupels with number and "num" tag.
    """
    for i in range(len(numbers)):
        if numbers[i] == "and":
            numbers[i] = ("and", "log_op")
            continue
        numbers[i] = (w2n.word_to_num(numbers[i]), "num")
    return numbers


def convert_all_nums(numbers):
    """Convert list of numbers in string-form to single integer and return.

    Use word2number library to convert list of strings to single number, if
    this does not work (cannot be converted to single number), convert_single_nums()
    is called by parent function. E.g., ['twenty', 'one'] would be converted to 21.

    Args:
        numbers:
        Numbers is a list of numbers in the form of strings. E.g., ['five', '7'].

    Returns:
        Tupel with number and "num" tag.
    """
    # Sequences in the form number and number get summed by default,
    # circument this.
    if len(numbers) == 3 and numbers[1] == "and":
        return convert_single_nums(numbers)
    return [(w2n.word_to_num(" ".join(numbers)), "num")]


def convert_numbers(sentences):
    """Iterate through list with extracted items and convert numbers.

    Use word2number library to convert numbers in string-form to combined integers
    or to individual integers. E.g., 'twenty', 'one' would be converted to 21, and
    'five', 'seven' would be converted to 5 and 7.

    Args:
        sentences:
            List of lists. Every sub-list consists of extracted and tagged terms from
            the corpus. E.g., 'The age of a customer should be at least twenty one.'
            is transformed to [('age', 'prop'), ('Customer', 'class'), ('[', 'val_op'),
            ('twenty', 'num'), ('one', 'num')].
    """
    for sentence in sentences:
        i = 0
        while i < len(sentence):
            if "num" in sentence[i]:
                start_idx = j = i
                numbers = [sentence[j][0]]
                while j + 1 < len(sentence) and ("num" == sentence[j + 1][1] or "and" == sentence[j + 1][0]):
                    j += 1
                    numbers.append(sentence[j][0])
                if numbers[-1] == "and":
                    numbers.pop()
                end_idx = start_idx + len(numbers)
                # Try to convert sequence of numbers with possibly
                # 'and' in between to single num.
                try:
                    numbers = convert_all_nums(numbers)
                except ValueError:
                    numbers = convert_single_nums(numbers)
                del sentence[start_idx:end_idx]
                if start_idx == len(sentence):
                    for k in range(len(numbers)):
                        sentence.append(numbers[k])
                else:
                    for k in range(len(numbers)):
                        sentence.insert(k + start_idx, numbers[k])
                i = end_idx
            else:
                i += 1
    return sentences


def annotate_literal(rule_li, word, get_literal):
    """If get_literal is true: tag token as literal and incorporate in rule_li.

    Convert word to extracted token with a literal tag and incorporate in
    rule_li. Every token enclosed with double-quotes is extracted.

    Args:
        rule_li:
            List of of extracted and tagged terms from the corpus. See
            get_rule_lists() for more info.
        word:
            Token from corpus (type is string).
        get_literal:
            Bool. If true, a double-quote was previously found.

    Returns:
        Bool:
            Return True if the current word was extracted and tagged, False if not.
        get_literal:
            The value of get_literal. If a double-quote is found, the inverse
            is returned. So, if it was True, it is returned as False.
    """
    if word.text == '"':
        return False, not get_literal
    if get_literal is True and word.text != '"':
        rule_li.append((word.text, "lit"))
        return True, get_literal
    return False, get_literal


def mark_target(rule_li, word, class_props):
    """If token is mapped to metadata: tag token as PLACEHOLDER for further handling
    downstream to preserver the word-order.

    Try to map token to every available target (classes, properties) from the metadata.
    Incorporate in rule_li if so.

    Args:
        rule_li:
            As defined at annotate_literal().
        word:
            As defined at annotate_literal().
        class_props:
            Dictionary of all metadata as defined at get_class_props().

    Returns:
        Bool:
            As defined at annotate_literal().
    """
    for cl in class_props.keys():
        if cl.lower() == word.lemma:
            rule_li.append((word.lemma, "PLACEHOLDER"))
            return True
        for prop in class_props[cl][1:]:
            if prop.lower() == word.lemma:
                rule_li.append((word.lemma, "PLACEHOLDER"))
                return True
    return False


def annotate_log_op(rule_li, word, operator_keywords):
    """If token is mapped to a logical operator: tag as an operator and incorporate in rule_li.

    Try to map token to every keyword that is related to a logical operator as defined in
    'operatorsets.json'. If mapped, the token is converted and a tag is added. E.g.,
    "and" is converted to ("and", "log_op").

    Args:
        rule_li:
            As defined at annotate_literal().
        word:
            As defined at annotate_literal().
        operator_keywords:
            Dictionary where every key is mapped to a list of words.

    Returns:
        Bool:
            As defined at annotate_literal().
    """
    for key in operator_keywords["log_op"]:
        for op in operator_keywords["log_op"][key]:
            if word.lemma == op or word.text == op:
                rule_li.append((key, "log_op"))
                return True
    return False


def annotate_val_op(rule_li, word, operator_keywords):
    """If token is mapped to a value operator: tag as an operator and incorporate in rule_li.

    Try to map token to every keyword that is related to a value operator as defined in
    'operatorsets.json'. If mapped, the token is converted and a tag is added. E.g.,
    "less" is converted to ("}", "val_op").

    Args:
        operator_keywords:
            Dictionary where every key is mapped to a list of words.
    """
    for key in operator_keywords["val_op"]:
        for op in operator_keywords["val_op"][key]:
            if word.lemma == op or word.text == op:
                rule_li.append((key, "val_op"))
                return True
    return False


def annotate_terminal(rule_li, word, operator_keywords):
    """If token is mapped to a terminal: tag as a literal and incorporate in rule_li.

    Try to map token to every keyword that is related to a terminal as defined in
    'operatorsets.json'. If mapped, the token is converted and a tag is added. E.g.,
    "symbol" is converted to ("char", "terminal").

    Args:
        operator_keywords:
            Dictionary where every key is mapped to a list of words.
    """
    for key in operator_keywords["terminal"]:
        for op in operator_keywords["terminal"][key]:
            if word.lemma == op or word.text == op:
                rule_li.append((key, "terminal"))
                return True
    return False


def annotate_targets(rule_li, class_props):
    """Annotate the targets (classes, properties) in the rule list.

    In mark_target(), a word was marked if it matched any metadata to
    preserve ordering in the final rule list. Here, firstly all mentioned
    classes are determined and annotated. Secondly, terms are compared
    to only the properties as determined by the classes that are mentioned
    (as opposed to all properties in the metadata).
    """
    classes_found = []
    for i in range(len(rule_li)):
        if "PLACEHOLDER" in rule_li[i]:
            for cl in class_props.keys():
                if rule_li[i][0] == cl.lower():
                    rule_li[i] = (cl, "class")
                    classes_found.append(cl)
    i = 0
    while i < len(rule_li):
        if "PLACEHOLDER" in rule_li[i]:
            found = False
            for cl in classes_found:
                for prop in class_props[cl][1:]:
                    if rule_li[i][0] == prop.lower():
                        rule_li[i] = (prop, "prop")
                        found = True
            # Marked property did not correspond to the mentioned
            # classes in the rule.
            if not found:
                del rule_li[i]
        i += 1


def get_rule_lists(doc, operator_keywords, class_props, skip_doc):
    """Extract relevant tokens from sentences in annotated Stanza Document object
    and return as list of lists.

    Iterate over every sentence and every word, extract and tag all relevant tokens
    and add to a list. All lists are wrapped in rules_list and returned.

    Args:
        doc:
            Stanza's Document object. Contains annotated corpus.
        operator_keywords:
            Dictionary where every key is mapped to a list of words.
        class_props:
            Dictionary with metadata from metamodel as compiled in get_class_props().
        skip_doc:
            List where every index corresponds to the index of a sentence in
            'Document.sentences'. A "Y" is inserted at indexes where the corresponding
            sentences in the Document did not pass all input requirements. Used for
            skipping these sentences.

    Returns:
        rules_list:
            List of lists. Every sub-list consists of extracted and tagged terms from
            the corpus. E.g., 'The age of a customer should be at least twenty one.'
            is transformed to [('age', 'prop'), ('Customer', 'class'), ('[', 'val_op'),
            ('twenty', 'num'), ('one', 'num')].
    """
    rules_list = []
    for i in range(len(doc.sentences)):
        rule_li = []
        if skip_doc[i] == "N":
            get_literal = False
            for word in doc.sentences[i].words:
                assigned, get_literal = annotate_literal(rule_li, word, get_literal)
                if not assigned:
                    assigned = mark_target(rule_li, word, class_props)
                if not assigned:
                    assigned = annotate_log_op(rule_li, word, operator_keywords)
                if not assigned:
                    assigned = annotate_val_op(rule_li, word, operator_keywords)
                if not assigned:
                    assigned = annotate_terminal(rule_li, word, operator_keywords)
                if not assigned and word.upos == "NUM":
                    rule_li.append((word.text, "num"))
            annotate_targets(rule_li, class_props)
        rules_list.append(rule_li)
    return rules_list


def get_class_props(metadata):
    """Get all classes and their corresponding properties from the metadata and return.

    Args:
        metadata:
            As defined under /api/model/class/{project_id}/{system_id} in the API explorer.

    Returns:
        class_props:
            A dictionary containing all classes and their corresponding properties in the
            metadata. Every class-key also has a nested list, used to contain the property names
            of properties that share the same name for the same class. This nested list is filled
            in check_duplicate_props().
            E.g., {'class_name_1': [[prop_name_1], prop_name_1, prop_name_2, prop_name_1, ..],
            ..}.
    """

    class_props = {}
    for node in metadata["nodes"]:
        if node["type"] == "Class":
            class_props[node["name"]] = [[]]
            for prop in node["data"]["properties"]:
                class_props[node["name"]].append(prop["name"])
    return class_props


def check_duplicate_props(class_props):
    """Checks for identically named properties corresponding to a single class.

    Gathers all duplicate-named property names and saves them in the nested list at the
    corresponding class-key.

    Args:
        class_props:
            As defined at get_class_props().
    """
    for cl in class_props.keys():
        duplicates = [prop for prop in class_props[cl] if class_props[cl].count(prop) > 1]
        duplicates = list(set(duplicates))
        if len(duplicates) > 0:
            class_props[cl][0].extend(duplicates)
    return class_props


def interval_handler(rules_list):
    """Substitute an interval token to a lesser than and more than operator.


    rules_list:
        As defined at get_rule_lists().
    """
    for i in range(len(rules_list)):
        for j in range(len(rules_list[i])):
            if rules_list[i][j][0] == "INTERVAL":
                rules_list[i][j] = ("[", "val_op")
                k = j
                while k < len(rules_list[i]):
                    if rules_list[i][k][0] == "and":
                        rules_list[i].insert(k + 1, ("]", "val_op"))
                        break
                    k += 1
                continue
    return rules_list


def merge_val_op(rule):
    """Merge value operators when used in the pattern BOUND,EQUALS,NUM or EQUALS,BOUND,NUM.

    E.g., "A x should have y equal to at least 5" is initially processed to [..., ('=', 'val_op'),
    ('[', 'val_op'), (5, 'num')]. This is converted to [..., ('[', 'val_op'), (5, 'num')].

    rules_list:
        As defined at get_rule_lists().
    """
    i = 0
    compare_ops = ["{", "[", "]", "}"]
    while i < len(rule):
        if rule[i][0] == "=":
            if i > 0 and rule[i - 1][0] in compare_ops:
                if rule[i + 1][1] == "num":
                    del rule[i]
            if rule[i + 1][0] in compare_ops:
                if rule[i + 2][1] == "num":
                    del rule[i]
        i += 1
    return rule


def wrap_rules(rules_list):
    """Wrap every rule from the rules_list in another list and return.

    The input manager uses this wrapper list to store information regarding the rule.

    rules_list:
        As defined at get_rule_lists().
    """
    for i in range(len(rules_list)):
        rules_list[i] = [rules_list[i]]
    return rules_list


def get_skip_doc(err_report):
    """Return a list that contains information on which input sentences did
    not pass the input validation checks.

    skip_doc a list where every index corresponds to the index of a sentence in
    'Document.sentences'. A "Y" is inserted at indexes where the corresponding
    sentences in the Document did not pass all input requirements. Used downstream
    for skipping these sentences.
    """
    skip_doc = ["N" for key in err_report.keys()]
    i = 0
    for key in err_report.keys():
        if len(err_report[key]) > 0:
            skip_doc[i] = "Y"
        i += 1
    return skip_doc


def get_correct_rules(err_report, doc, rules_list):
    """Return the corpus and rules_list with rules that passed the input
    validation tests.
    """
    del_li = []
    i = 0
    for key in err_report.keys():
        if len(err_report[key]) > 0:
            del_li.append(i)
        i += 1
    doc.sentences = [doc.sentences[i] for i in range(len(doc.sentences)) if i not in del_li]
    rules_list = [rules_list[i] for i in range(len(rules_list)) if i not in del_li]
    return doc, rules_list


def print_stanza_annotation(doc):
    """Output the annotated Document object."""
    print("\n")
    for i, sentence in enumerate(doc.sentences):
        print(f"====== Sentence {i+1} words =======")
        print(*[f"{word.text} ({word.upos})" for word in sentence.words], sep="|")
        # print(*[f"{word.lemma} ({word.upos})" for word in sentence.words], sep="|")
        """for word in sentence.words:
            # if word.deprel == "nsubj":
            #     print(
            #         f'word: {word.text}, head: {sentence.words[word.head-1].text if word.head > 0 else "root"}, deprel: {word.deprel}'
            #     )
            print(
                f'word: {word.text}, head: {sentence.words[word.head-1].text if word.head > 0 else "root"}, deprel: {word.deprel}'
            )"""


def get_rules_list():
    """Coordinate the NLP pipeline and return rules_lists.

    rules_list is as defined at get_rule_lists().
    """
    corpus = get_input()
    operator_keywords = get_operator_keywords()
    metadata = get_metadata()
    connections = get_connections(metadata)
    class_props = get_class_props(metadata)
    class_props = check_duplicate_props(class_props)
    corpus = preprocess(corpus)
    # run annotation
    doc = run_annotation(corpus)
    err_report = validate_metadata(metadata, doc, class_props, connections)
    print_stanza_annotation(doc)
    skip_doc = get_skip_doc(err_report)
    rules_list = get_rule_lists(doc, operator_keywords, class_props, skip_doc)
    rules_list = convert_numbers(rules_list)
    rules_list = interval_handler(rules_list)
    err_report = validate_m2o_direction(rules_list, doc, err_report, connections)
    doc, rules_list = get_correct_rules(err_report, doc, rules_list)
    rules_list = wrap_rules(rules_list)
    return rules_list, doc


# rules_list, doc = get_rules_list()

# for i in range(len(rules_list)):
#     print(i + 1)
#     print("\t", rules_list[i])
