# File for the rule engine of the chatbot
# Here all the logic is stated that will be used to generate the answer the chatbot will give

from curses import raw
from logging import warning
import logging
import os
import re
import json
from extractor.models import Chat_history
from nltk.stem import WordNetLemmatizer
from nltk.corpus import wordnet
from nltk.tokenize import word_tokenize
from nltk.tag import pos_tag
from model.models import *

# from extractor.models import Chat_history

# Loading the JSON file with all the intents, patterns, responses and tags.
# This file can be found in the static folder.
with open("./static/knowledge_base.json", "r") as f:
    data = json.load(f)

# Storing the lemmatizer function from the NLTK library
lemmatizer = WordNetLemmatizer()


def chatbot_main(input):
    # Receiving input and making it lowercase for easier pattern matching
    bericht = input
    message_lower = bericht.lower()

    # non-processed input message
    raw_input = input

    # Calling preprocessing function for lemmatizing
    processed = process_sentence(message_lower)

    logging.warning("Preprocessed msg: " + processed)
    test = ActivityNode.objects.all()
    logging.warning(test)

    # Default (error) message and tag
    antwoord = "I'm sorry I did not quite get that, could you rephrase that?"
    current_tag = "default"

    # Searching and matching with the patterns from the JSON file
    for intent in data["intents"]:
        for pattern in intent["patterns"]:
            if re.search(pattern, processed):
                antwoord = intent["responses"]
                current_tag = intent["tag"]

    # Checking for previous answer tag to create conversation
    if Chat_history.objects.exists():
        # Accessing previous message of user
        latest_message = Chat_history.objects.values("pattern_tag", "user_message").order_by("-id")[0]
        latest_tag = latest_message["pattern_tag"]
        prev_input = latest_message["user_message"]

        # Conditional statements for changing datatype via the chatbot
        if latest_tag == "asking_class":
            antwoord = "Which attribute of " + input + " do you want to change?"
            current_tag = "changing_datatype"
        elif latest_tag == "changing_datatype":
            antwoord = "To which datatype? Boolean or Integer? By default all attributes are a string"
            current_tag = "asking_datatype"
        elif latest_tag == "asking_datatype":
            if message_lower == "integer" or message_lower == "int":
                dtype = "int"
            if message_lower == "boolean" or message_lower == "bool":
                dtype = "bool"
            class_message = Chat_history.objects.values("pattern_tag", "user_message").order_by("-id")[1]
            input_class = class_message["user_message"]
            # Calling function for changing datatype, returns Boolean
            if change_datatype(dtype, prev_input, input_class):
                antwoord = (
                    "The datatype of " + prev_input + " was changed succesfully! Reload the data to see the changes."
                )
            else:
                antwoord = "Something went wrong, are you sure the attribute exists?"

        # Conditional statements for changing the name of a relationship via the chatbot
        if latest_tag == "change_relationship_name" and message_lower == "yes":
            antwoord = "What is the name of the relationship?"
            current_tag = "asking_relationship_name"
            old_name = input
        # Relationship has no name so we cannot filter on the name
        elif latest_tag == "change_relationship_name" and message_lower == "no":
            antwoord = "From which classifier does the relationship start?"
            current_tag = "asking_relationship_from"
        elif latest_tag == "asking_relationship_from":
            antwoord = "To which classifier does the relationship go?"
            current_tag = "asking_relationship_to"
        elif latest_tag == "asking_relationship_name":
            antwoord = "What would you like the name of " + input + " to be?"
            current_tag = "asking_new_name"
        elif latest_tag == "asking_new_name":
            old_name_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
            old_name = old_name_msg["user_message"]
            new_name = raw_input
            if change_relationship_name(old_name, new_name):
                antwoord = "De name of " + old_name + " was succesfully changed! Reload the data to see the changes."
            else:
                antwoord = "Something went wrong, are you sure the relationship exists?"
        # if relationship does not have a name yet, we look at the classifiers of the relationship
        elif latest_tag == "asking_relationship_to":
            antwoord = "What would you like the name of the relationship to be?"
            current_tag = "setting_new_name"
        elif latest_tag == "setting_new_name":
            new_name = raw_input
            if change_relationship_name_unknown(new_name):
                antwoord = "The name of the relationship was succesfully changed! Reload the data to see the changes."
            else:
                antwoord = "Something went wrong, are you sure the relationship exists?"

        # Conditional statements for changing the name of classifier
        elif latest_tag == "change_classifer_name":
            antwoord = "What would you like the name of " + raw_input + " to be?"
            current_tag = "asking_class_name"
        elif latest_tag == "asking_class_name":
            new_name = raw_input
            if change_classifier_name(new_name):
                antwoord = "The name of the class was succesfully changed! Reload the data to see the changes."
            else:
                antwoord = "Something went wrong, are you sure the class exists?"

        # Conditional statements for adding a new relationship
        elif latest_tag == "add_new_relation":
            antwoord = "To which classifier should the relationship go?"
            current_tag = "relation_asking_to"
        elif latest_tag == "relation_asking_to":
            antwoord = "What should the name the name of the new relationship be?"
            current_tag = "new_relation_name"
        elif latest_tag == "new_relation_name":
            antwoord = "What kind of relationship should it be? Assocation, composition or generalization?"
            current_tag = "new_relation_asking_type"
        elif latest_tag == "new_relation_asking_type":
            new_type = message_lower
            name_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
            name = name_msg["user_message"]
            if add_new_relationship(new_type):
                antwoord = "The relation " + name + " was succesfully created! Reload the data to see the changes."
            else:
                antwoord = "Something went wrong, are you sure the classifiers exist?"

        # Conditional statements for changing the type of relationship
        elif latest_tag == "change_relationship_type":
            antwoord = "From which classifier does the relationship start?"
            current_tag = "type_asking_from"
        elif latest_tag == "type_asking_from":
            antwoord = "To which classifier does the relationship go?"
            current_tag = "type_asking_to"
        elif latest_tag == "type_asking_to":
            antwoord = "Which type should the relation be? Assocation, composition or generalization?"
            current_tag = "setting_new_type"
        elif latest_tag == "setting_new_type":
            new_type = raw_input.lower()
            if change_relationship_type(new_type):
                antwoord = "The type of the relationship was succesfully changed! Reload the data to see the changes."
            else:
                antwoord = "Something went wrong, are you sure the relationship exists?"

        # Conditional statements for changing the name of an attribute of a classifier
        elif latest_tag == "change_name_attribute":
            antwoord = "What is the current name of the attribute?"
            current_tag = "change_name_attribute_cur_name"
        elif latest_tag == "change_name_attribute_cur_name":
            antwoord = "What would you like the new name of " + raw_input + " to be?"
            current_tag = "change_name_attribute_new_name"
        elif latest_tag == "change_name_attribute_new_name":
            new_name = raw_input
            if change_name_attribute(new_name):
                antwoord = "The name of the attribute was succesfully changed! Reload the data to see the changes."
            else:
                antwoord = "Something went wrong, are you sure the attribute and the classifier exist?"

        # Conditional statements for adding a new attribute of a classifier
        elif latest_tag == "add_new_attr":
            antwoord = "What should the name of the new attribute be?"
            current_tag = "new_attr_name"
        elif latest_tag == "new_attr_name":
            antwoord = "What should the datatype be? Integer, string or boolean?"
            current_tag = "new_attr_type"
        elif latest_tag == "new_attr_type":
            if message_lower == "integer" or message_lower == "int":
                dtype = "int"
            elif message_lower == "boolean" or message_lower == "bool":
                dtype = "bool"
            elif message_lower == "string" or message_lower == "str":
                dtype = "str"
            if add_attribute(dtype):
                antwoord = "The attribute was added succesfully! Reload the data to see the changes."
            else:
                antwoord = "Something went wrong, are you sure the classifier exists? And the datatype is correct?"

        #                #
        #                #
        # ACTIVITY MODEL #
        #                #
        #                #

    # Breaking the cycle at any moment
    if message_lower == "exit" or message_lower == "break" or message_lower == "cancel":
        antwoord = "You canceled the process, can I help you with anything else?"
        current_tag = "break"

    # Saving current data in the model for the chat history
    chat_save = Chat_history(user_message=input, answer=antwoord, pattern_tag=current_tag)
    chat_save.save()

    # Constructing JSON response and returning it to the view
    response = {"antwoord": antwoord}
    return response


# Function for changing type of a relationship with the chatbot
def change_relationship_type(new_type):
    to_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
    to_str = to_msg["user_message"]
    from_msg = Chat_history.objects.values("user_message").order_by("-id")[1]
    from_str = from_msg["user_message"]
    old_type_msg = Chat_history.objects.values("user_message").order_by("-id")[2]
    old_type_str = old_type_msg["user_message"].lower()

    if Relationship.objects.filter(
        classifier_from__name__contains=from_str, classifier_to__name__contains=to_str
    ).exists():
        # Checking if a name for the relationship exists and retrieving that name if it does
        current_obj = Relationship.objects.get(
            classifier_from__name__contains=from_str,
            classifier_to__name__contains=to_str,
        )
        if current_obj.name is not None or current_obj.name != "":
            current_name = current_obj.name
        else:
            current_name = ""

        # Deleting old relation based on given type
        if old_type_str == "generalization":
            Generalization.objects.filter(
                classifier_from__name__contains=from_str,
                classifier_to__name__contains=to_str,
            ).delete()
        elif old_type_str == "association":
            Association.objects.filter(
                classifier_from__name__contains=from_str,
                classifier_to__name__contains=to_str,
            ).delete()
        elif old_type_str == "composition":
            Composition.objects.filter(
                classifier_from__name__contains=from_str,
                classifier_to__name__contains=to_str,
            ).delete()

        # Retrieving the model instances for the Foreign Key fields
        class_to = Classifier.objects.get(name=to_str)
        class_from = Classifier.objects.get(name=from_str)
        # Creating and saving the new relation, based on the given type
        if new_type == "generalization":
            save = Generalization(name=current_name, classifier_to=class_to, classifier_from=class_from)
            save.save()
            return True
        elif new_type == "association":
            save = Association(name=current_name, classifier_to=class_to, classifier_from=class_from)
            save.save()
            return True
        elif new_type == "composition":
            save = Composition(name=current_name, classifier_to=class_to, classifier_from=class_from)
            save.save()
            return True
        else:
            return False
    else:
        return False


# Function for changing datatype with the chatbot
def change_datatype(dtype, attribute, input_class):
    # Checking if requested attribute exists
    if Classifier.objects.filter(name=input_class).exists():
        # Filtering on the attribute name and the foreign key, which is the classifier
        if Property.objects.filter(name=attribute, classifier__name__contains=input_class).exists():
            # Changing requested attribute in the database
            Property.objects.filter(name=attribute, classifier__name__contains=input_class).update(type=dtype)
            return True
        else:
            return False
    else:
        return False


# Function for changing the name of relationship, when a name already exists
def change_relationship_name(old_name, new_name):
    # Checking if requested relationship exists
    if Relationship.objects.filter(name=old_name).exists():
        # Changing the name of the relationhsip in the database
        Relationship.objects.filter(name=old_name).update(name=new_name)
        return True
    else:
        return False


# Function for changing the name of relationship, when the relationship does not have a name yet
def change_relationship_name_unknown(new_name):
    to_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
    to_str = to_msg["user_message"]
    from_msg = Chat_history.objects.values("user_message").order_by("-id")[1]
    from_str = from_msg["user_message"]
    if Classifier.objects.filter(name=from_str).exists() and Classifier.objects.filter(name=to_str).exists():
        # Checking if requested relationship exists
        if Relationship.objects.filter(
            classifier_from__name__contains=from_str,
            classifier_to__name__contains=to_str,
        ).exists():
            # Changing the name of the relationhsip in the database
            Relationship.objects.filter(
                classifier_from__name__contains=from_str,
                classifier_to__name__contains=to_str,
            ).update(name=new_name)
            return True
        else:
            return False
    else:
        return False


# Function for changing the name of a classifier via the chatbot
def change_classifier_name(new_name):
    # Retrieving previous input from the user via the Django model
    old_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
    old_name = old_msg["user_message"]
    # Checking if class exists in the model
    if Classifier.objects.filter(name=old_name).exists():
        Classifier.objects.filter(name=old_name).update(name=new_name)
        return True
    else:
        return False


# Function for creating a new relation via the chatbot
def add_new_relationship(new_type):
    # Retrieving previous inputs from the user via the Django model
    name_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
    name = name_msg["user_message"]
    to_msg = Chat_history.objects.values("user_message").order_by("-id")[1]
    to = to_msg["user_message"]
    from_msg = Chat_history.objects.values("user_message").order_by("-id")[2]
    from_str = from_msg["user_message"]
    if Classifier.objects.filter(name=to).exists() and Classifier.objects.filter(name=from_str).exists():
        class_to = Classifier.objects.get(name=to)
        class_from = Classifier.objects.get(name=from_str)
        if new_type == "generalization":
            new_relationship = Generalization(name=name, classifier_to=class_to, classifier_from=class_from)
            new_relationship.save()
            return True
        elif new_type == "association":
            new_relationship = Association(name=name, classifier_to=class_to, classifier_from=class_from)
            new_relationship.save()
            return True
        elif new_type == "composition":
            new_relationship = Composition(name=name, classifier_to=class_to, classifier_from=class_from)
            new_relationship.save()
            return True
        else:
            return False
    else:
        return False


# Function for changing the name of attribute via the chatbot
def change_name_attribute(new_name):
    # Retrieving previous inputs from the user via the Django model
    old_name_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
    old_name = old_name_msg["user_message"]
    class_msg = Chat_history.objects.values("user_message").order_by("-id")[1]
    class_str = class_msg["user_message"]
    if Property.objects.filter(name=old_name, classifier__name__exact=class_str).exists():
        Property.objects.filter(name=old_name, classifier__name__exact=class_str).update(name=new_name)
        return True
    else:
        return False


# Function for adding a new attribute of a classifier via the chatbot
def add_attribute(dtype):
    # Retrieving previous inputs from the user via the Django model
    new_name_msg = Chat_history.objects.values("user_message").order_by("-id")[0]
    new_name = new_name_msg["user_message"]
    class_msg = Chat_history.objects.values("user_message").order_by("-id")[1]
    class_str = class_msg["user_message"]
    if Classifier.objects.filter(name=class_str).exists():
        classifier = Classifier.objects.get(name=class_str)
        new_property = Property(name=new_name, classifier=classifier, type=dtype)
        new_property.save()
        return True
    else:
        return False


# function to convert nltk tag to wordnet tag
def nltk_tag_to_wordnet_tag(nltk_tag):
    if nltk_tag.startswith("J"):
        return wordnet.ADJ
    elif nltk_tag.startswith("V"):
        return wordnet.VERB
    elif nltk_tag.startswith("N"):
        return wordnet.NOUN
    elif nltk_tag.startswith("R"):
        return wordnet.ADV
    else:
        return None


# function to lemmatize a full sentence
def process_sentence(sentence):
    # tokenize the sentence and find the POS tag for each token
    nltk_tagged = pos_tag(word_tokenize(sentence))
    # tuple of (token, wordnet_tag)
    wordnet_tagged = map(lambda x: (x[0], nltk_tag_to_wordnet_tag(x[1])), nltk_tagged)
    lemmatized_sentence = []
    for word, tag in wordnet_tagged:
        if tag is None:
            # if there is no available tag, append the token as is
            lemmatized_sentence.append(word)
        else:
            # else use the tag to lemmatize the token
            lemmatized_sentence.append(lemmatizer.lemmatize(word, tag))
    return " ".join(lemmatized_sentence)
