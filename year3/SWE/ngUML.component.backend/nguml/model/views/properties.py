# from django.shortcuts import render
from django.shortcuts import redirect
from django.views.decorators.csrf import csrf_exempt
from ..models import Classifier, Operation, Property
from ..enums import Type
from ..generators import *


@csrf_exempt
def delete(request, property_id):
    property = Property.objects.get(id=property_id)
    classifier_id = str(property.classifier.id)

    PropertyGenerator(property).delete()
    property.delete()

    return redirect('/model/classifiers/' + classifier_id)


@csrf_exempt
def add(request):
    print(request.POST)
    classifier_id = request.POST['classifier_id']
    classifier = Classifier.objects.get(id=classifier_id)
    prop_type = request.POST['type']

    # if prop_type == 'string':
    #     prop_type = Type.String
    # elif prop_type == 'int':
    #     prop_type = Type.Int
    # elif prop_type == 'bool':
    #     prop_type = Type.Bool

    property = Property(name=request.POST['name'], type=prop_type, classifier=classifier)
    property.save()

    PropertyGenerator(property).generate()

    return redirect('/model/classifiers/' + classifier_id)


@csrf_exempt
def edit(request, property_id):
    property = Property.objects.get(id=property_id)
    classifier_id = property.classifier.id
    PropertyGenerator(property).delete()
    property.delete()
    prop_type = request.POST['type']

    # if prop_type == 'string':
    #     prop_type = Type.String
    # elif prop_type == 'int':
    #     prop_type = Type.Int
    # elif prop_type == 'bool':
    #     prop_type = Type.Bool

    property = Property(id=property_id, classifier_id=classifier_id, name=request.POST['name'], type=prop_type)
    property.save()

    PropertyGenerator(property).generate()

    return redirect('/model/classifiers/' + str(classifier_id))
