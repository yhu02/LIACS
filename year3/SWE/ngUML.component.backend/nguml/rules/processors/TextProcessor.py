# import rules to check if processed text is a rule
from nltk.corpus.reader.conll import ConllSRLInstance
import nltk
import re

# import contractions
from textblob import TextBlob
from nltk.stem import WordNetLemmatizer
from model.models import Classifier, Property
from rules.RulesManager.Terms import Operator, Value, AwaitingDesignation
from rules.RulesManager.TermList import TermList
import json

# Preprocessing
# TODO: n't to not: shouldn't -> should not
# TODO: remove 's: Customer's name -> Customer name

# Name is called name because then you can do something like this:
# for item in rule:
#   print(item.name)
# Then you don't need to first ask whether it is a property or classifier or operator or value


def get_classifiers():
    '''Returns all classfiers'''
    return Classifier.objects.all()


# should return all properties of that class
def get_properties_from_classifier(inputClassifier):
    '''Returns all properties from the classfier in the parameter. Parameter is the name of the classfier.'''
    return Property.objects.filter(classifier=Classifier.objects.get(name=inputClassifier.name))


'''The goal of text processor is to map messy_text into processed_text
That is to say: the user inputs messy text, and the text processor should unambigiously
map all those possible inputs to the real, singular meaning
Example of user input: "I want the User class's name not to be longer than 20 characters"
Another example: "user's name should be less or equal to 20 letters"
Both map to: user name.length <= 20
'''


def wordToNumber(text):
    # if word does not need to be converged to number: put _NUMBER_
    units = {
        "zero": 0,
        "one": 1,
        "two": 2,
        "three": 3,
        "four": 4,
        "five": 5,
        "six": 6,
        "seven": 7,
        "eight": 8,
        "nine": 9,
        "ten": 10,
        "eleven": 11,
        "twelve": 12,
        "thirteen": 13,
        "fourteen": 14,
        "fifteen": 15,
        "sixteen": 16,
        "seventeen": 17,
        "eighteen": 18,
        "nineteen": 19,
    }
    tens = {
        "twenty": 20,
        "thirty": 30,
        "fourty": 40,
        "fifty": 50,
        "sixty": 60,
        "seventy": 70,
        "eighty": 80,
        "ninety": 90,
    }
    amount = {"hundred": 100, "thousand": 1000, "million": 1000000}

    newNumber, numbers, idNumbers = [], [], []
    copyText = text.copy()
    for i in range(len(copyText)):
        if copyText[i] in units:
            numbers.append(units[copyText[i]])
            idNumbers.append(i)
        elif copyText[i] in tens:
            numbers.append(tens[copyText[i]])
            idNumbers.append(i)
        elif copyText[i] in amount:
            numbers.append(amount[copyText[i]])
            id = numbers.index(amount[copyText[i]])
            if len(numbers) > 1:
                correctNumber = numbers[id - 1] * numbers[id]
                numbers[id - 1] = correctNumber
                del numbers[id]
                idNumbers.append(i)
        else:
            continue
    idNumbers.sort()
    newNumber.append(str(sum(numbers)))
    if len(copyText) > 0 and len(idNumbers) > 0:
        newText = copyText[0: idNumbers[0]] + newNumber + copyText[idNumbers[-1] + 1:]
        return newText
    else:
        return text


def split_rule(text):
    '''Splits a string into individual words and cleans the data'''
    text = text.lower()  # set all symbols to lower case
    text = re.sub(r'[^\w\s]-[<, =, >]', '', text)  # remove puntuation (fixed)
    textBlb = TextBlob(text).correct()  # spell correction
    tokens = textBlb.tokens  # split rule into seperate words
    text = wordToNumber(tokens)
    lemmatizer = WordNetLemmatizer()
    text = [lemmatizer.lemmatize(text[i]) for i in range(len(text))]
    return text


def get_first_index_of_classifier(list):
    for item in list:
        if isinstance(item, Classifier):
            return list.index(item)

    return -1


# if the user gives input that maps to 1 single representation, convert it to that single representation
# example of user input: I want the User class's name not to be longer than 20 characters
# another example: user's name should be less or equal to 20 letters
# both map to: user name.length <= 20
# input: result of split_rule
def process_text(original_text):
    '''Maps messy user input to a singular representation'''
    original_text = str(TextBlob(original_text).correct())
    # contractions.fix(original_text)
    text = split_rule(original_text)

    all_classifiers = get_classifiers()
    classifiers_in_text = []
    properties_in_text = []
    terms = TermList()

    # All combinations of base operators and equivalent aliases
    jsonfile = open("rules/operatorsets.json", "r")
    operator_keywords = json.load(jsonfile)
    jsonfile.close()

    # Remove all classifiers that are not in the text
    for classifier in all_classifiers:
        if classifier.name.lower() in text:
            classifiers_in_text.append(classifier)

    # Get all properties from the classifiers in the text
    for classifier in classifiers_in_text:
        properties_in_text += get_properties_from_classifier(classifier)

    # Cycle through every word in the text
    for word in text:
        continue_flag = False
        # If word is equal to a classifier
        for classifier in classifiers_in_text:
            if classifier.name.lower() == word:
                terms.append(classifier)
                continue_flag = True
                break

        if continue_flag:
            continue

        # If word is equal to a property from any of the present classifiers
        for property in properties_in_text:
            if property.name.lower() == word:
                terms.append(AwaitingDesignation(word))
                continue_flag = True
                break

        if continue_flag:
            continue

        # If word is numeric
        if word.isnumeric():
            terms.append(Value(word))
            continue

        # If word is equal to any operator aliases, save the key operator
        for key in operator_keywords:
            if word in operator_keywords[key]:
                terms.append(Operator(key))
                break

    # If a property has the same name as another property, get the property of the closest classifier
    for term in terms:
        if isinstance(term, AwaitingDesignation):
            index = terms.value.index(term)

            # Reverse list
            reversed = terms.value[:index]
            reversed.reverse()
            backwards = get_first_index_of_classifier(reversed)
            forwards = get_first_index_of_classifier(terms.value[index:])

            # If forwards is closer than the backwards property, add the property from the forwards classifier
            if backwards > forwards and forwards != -1:
                for property in get_properties_from_classifier(terms.value[index + forwards]):
                    if property.name.lower() == term.name:
                        terms.value[index] = property
            elif backwards != -1:
                for property in get_properties_from_classifier(terms.value[index - backwards - 1]):
                    if property.name.lower() == term.name:
                        terms.value[index] = property

    # Warning: This assumes that every rules at least contains a classifier, a property and an operator, but this might become outdated
    if terms.size(Classifier) == 0:
        raise Exception("No classifiers were recognized or present in this rules.")
    elif terms.size(Property) == 0:
        raise Exception("No properties were recognized or present in this rules.")
    elif terms.size(Operator) == 0:
        raise Exception("No operators were recognized or present in this rules.")

    return terms
