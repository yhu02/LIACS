''' Views manages api calls and returning JSON
    Knows little of how rules work, are stored etc.
    Decodes requests, calls RulesManager, correctly handles exceptions and JSON returns'''

from textblob import TextBlob
import traceback

from rules.RulesManager.RulesManager import RulesManager
from rules.RulesManager.Terms import Operator, Value
from model.models import Classifier, Property
from rest_framework.decorators import api_view, schema
from rest_framework.response import Response
from rest_framework import status


@api_view(['GET'])
@schema(None)
def index(request):
    '''Accepts GET requests to return all currently saved rules from the database as text.'''

    # Get rules from database
    rules_query = RulesManager.get_all_rules()

    output = []

    for rule in rules_query:
        classifiers = []
        for classifier in rule[2].termlist.get_all(Classifier):
            classifiers.append({"name": classifier.name, "pk": classifier.pk})

        properties = []
        for property in rule[2].termlist.get_all(Property):
            properties.append(
                {
                    "pk": property.pk,
                    "name": property.name,
                    "classifier": property.classifier.name,
                }
            )

        output.append(
            {
                "pk": rule[1],
                "fields": {
                    "original_input": rule[0],
                    "processed_text": rule[2].get_processed_text(),
                    "operator": rule[2].termlist.get_all(Operator),
                    "value": rule[2].termlist.get_all(Value),
                    "classifiers": classifiers,
                    "properties": properties,
                },
            }
        )

    # output = json.dumps(output)

    return Response(output)


@api_view(['POST'])
@schema(None)
def add(request):
    '''Accepts POST requests to add rules to the database.'''
    # Extract the rule from the request

    textrule = request.data.get('rule', "")  # give default value in case there is no rule argument
    if textrule == "":
        return Response({'FAIL': 'Supply rule : ruletext in post argument'}, status=status.HTTP_400_BAD_REQUEST)

    # Create a rule database object from the string
    try:
        RulesManager.add_rule(textrule)
    except Exception as err:
        # Return the error as JSON if exception
        print(traceback.format_exc())
        return Response(
            {'FAIL': 'Rule not saved to database', 'type': str(type(err)), 'message': str(err)},
            status=status.HTTP_500_INTERNAL_SERVER_ERROR,
        )
    return Response({'SUCCES': 'Rule: ' + str(TextBlob(textrule).correct()) + " saved to rules."})


@api_view(['DELETE'])
@schema(None)
def remove(request):
    '''Accepts POST requests to remove rules to the database.'''
    # Extract the rule pk from the request
    pk = request.data.get('pk', -1)  # give default value in case there is no rule argument

    if pk == -1:
        return Response(
            {'FAIL': 'Supply pk : pk of rule to be removed in post argument'}, status=status.HTTP_400_BAD_REQUEST
        )

    # Remove the rule instance from database
    try:
        RulesManager.remove_rule_by_pk(pk)
    except Exception as err:
        # Return the error as JSON if exception
        return Response(
            {'FAIL': 'Rule not removed from database', 'type': str(type(err)), 'message': str(err)},
            status=status.HTTP_500_INTERNAL_SERVER_ERROR,
        )
    return Response({'SUCCES': 'Rule with pk: ' + pk + " removed from rules."})


@api_view(['GET'])
@schema(None)
def description(request):
    '''Accepts GET requests to get the description of every rule.'''
    # Create a rule database object from the string
    try:
        description = RulesManager.get_description()
    except Exception as err:
        # Return the error as JSON if exception
        print(traceback.format_exc())
        return Response(
            {
                'FAIL': 'Could not retrieve the description information of the rules',
                'type': str(type(err)),
                'message': str(err),
            },
            status=status.HTTP_500_INTERNAL_SERVER_ERROR,
        )
    return Response({'SUCCES': description})
