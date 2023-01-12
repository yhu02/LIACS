"""Rule API."""
from drf_spectacular.utils import extend_schema_view, extend_schema
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.forms.models import model_to_dict
from textblob import TextBlob

from rules.RulesManager.RulesManager import RulesManager, generate_rule
from rules.RulesManager.Terms import Operator, Value
from rules.models import Rule
from model.models import Project, System, Classifier, Property

"""
    APIS ARE UNTESTED
"""


@extend_schema_view(
    get=extend_schema(
        summary="Get a rule",
    ),
    delete=extend_schema(
        summary="Delete a rule",
    ),
)
@api_view(["GET", "DELETE"])
def index(request, project_id, system_id, rule_id):
    """
    get:
        Get a rule

        Args:
            system: System instance that the rule is requested of
            rule_id: Identifier of th rule requested

        Returns: object: rest framework Response
    delete:
        Delete a rule

        Args:
            system: System instance that the rule is requested of
            rule_id: Identifier of teh rule that will be deleted

        Returns: object: rest framework Response

    """
    if not project_id:
        return Response({"detail": "A system id is required"}, status=status.HTTP_400_BAD_REQUEST)
    elif not system_id:
        return Response({"detail": "A system id is required"}, status=status.HTTP_400_BAD_REQUEST)
    elif not rule_id:
        return Response(
            {"detail": "For this operation a rule id is required"},
            status=status.HTTP_400_BAD_REQUEST,
        )
    else:
        try:
            project = Project.objectd.get(pk=project_id)
            system = System.objects.get(pk=system_id)
        except System.DoesNotExist:
            return Response(
                {"detail": "The requested system cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
    if request.method == "GET":
        return get_rule(system, rule_id)
    elif request.method == "DELETE":
        return delete_rule(system, rule_id)
    else:
        return Response(status=status.HTTP_405_METHOD_NOT_ALLOWED)


@extend_schema_view(
    get=extend_schema(
        summary="Get all rule for this system",
    ),
    post=extend_schema(
        summary="Add a new rule",
    ),
)
@api_view(["GET", "POST"])
def index_without_rule_id(request, project_id, system_id):
    """
    get:
        Get all the rules

        Args:
             system: System instance that the rules is requested of

        Returns: object: rest framework Response list of rules
    post:
        Add a new rule

        Args:
            request: data containing "rule"
            system: System instance that the rule is requested of

        Returns: object: rest framework Response with the added rule
    """
    if not project_id:
        return Response({"detail": "A system id is required"}, status=status.HTTP_400_BAD_REQUEST)
    elif not system_id:
        return Response({"detail": "A system id is required"}, status=status.HTTP_400_BAD_REQUEST)
    else:
        try:
            project = Project.objectd.get(pk=project_id)
            system = System.objects.get(pk=system_id)
        except System.DoesNotExist:
            return Response(
                {"detail": "The requested system cannot be found"},
                status=status.HTTP_404_NOT_FOUND,
            )
    if request.method == "GET":
        return get_rules(system)
    elif request.method == "POST":
        return post_rule(request, system)
    else:
        return Response(status=status.HTTP_405_METHOD_NOT_ALLOWED)


def get_rule(system, rule_id):
    """Get a rule

    Args:
        system: System instance that the rule is requested of
        rule_id: Identifier of th rule requested

    Returns: object: rest framework Response
    """
    rule_item = Rule.objects.filter(system=system).get(pk=rule_id)
    rule = (
        rule_item.original_input,
        rule_item.pk,
        generate_rule(rule_item.original_input, rule_item.pk),
    )
    classifiers = []
    for classifier in rule[2].termlist.get_all(Classifier):
        classifiers.append({"name": classifier.name, "pk": classifier.pk})

    properties = []
    for selected_property in rule[2].termlist.get_all(Property):
        properties.append(
            {
                "pk": selected_property.pk,
                "name": selected_property.name,
                "classifier": selected_property.classifier.name,
            }
        )

    return Response(
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


def delete_rule(system, rule_id):
    """Delete a rule

    Args:
        system: System instance that the rule is requested of
        rule_id: Identifier of teh rule that will be deleted

    Returns: object: rest framework Response
    """
    try:
        rule = Rule.objects.filter(system=system).get(pk=rule_id)
        generate_rule(rule, rule_id).remove_validator()
        rule.delete()
    except Exception as err:
        # Return the error as JSON if exception
        return Response(
            {
                "FAIL": "Rule not removed from database",
                "type": str(type(err)),
                "message": str(err),
            },
            status=status.HTTP_500_INTERNAL_SERVER_ERROR,
        )
    return Response({"SUCCES": "Rule with pk: " + rule_id + " removed from rules."})


def get_rules(system):
    """Get all the rules

    Args:
         system: System instance that the rules is requested of

    Returns: object: rest framework Response list of rules
    """

    rules = []
    output = []

    for rule in Rule.objects.filter(system=system):
        rules.append((rule.original_input, rule.pk, generate_rule(rule.original_input, rule.pk)))

    for rule in rules:
        classifiers = []
        for classifier in rule[2].termlist.get_all(Classifier):
            classifiers.append({"name": classifier.name, "pk": classifier.pk})

        properties = []
        for selected_property in rule[2].termlist.get_all(Property):
            properties.append(
                {
                    "pk": selected_property.pk,
                    "name": selected_property.name,
                    "classifier": selected_property.classifier.name,
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

    return Response(output)


def post_rule(request, system):
    """Add a new rule

    Args:
        request: data containing "rule"
        system: System instance that the rule is requested of

    Returns: object: rest framework Response with the added rule
    """
    if "rule" not in request.data:
        return Response(
            {"FAIL": "Supply rule : ruletext in post argument"},
            status=status.HTTP_400_BAD_REQUEST,
        )
    else:
        textrule = request.data.get("rule", "")

    try:
        generate_rule(textrule, -1)
        new_rule_db = Rule(original_input=textrule, system=system)
        new_rule_db.save()

        rule = generate_rule(textrule, new_rule_db.pk)
        rule.add_validator()

    except Exception as err:
        # Return the error as JSON if exception
        return Response(
            {
                "FAIL": "Rule not saved to database",
                "type": str(type(err)),
                "message": str(err),
            },
            status=status.HTTP_500_INTERNAL_SERVER_ERROR,
        )
    return Response({"SUCCES": "Rule: " + str(TextBlob(textrule).correct()) + " saved to rules."})
