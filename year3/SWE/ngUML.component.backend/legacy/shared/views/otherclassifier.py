from django.shortcuts import render
from django.shortcuts import redirect
from runtime.models import *
from model.models import Application


def index(request):
    properties = []
    objects = OtherClassifier.objects.all()
    for object in objects:
        object.properties = []
        object.properties.reverse()
    context = {
        'application': __get_application(request),
        'properties': properties,
        'objects': objects,
        'object_name': 'OtherClassifier',
        'object_name_lower': 'otherclassifier',
    }
    return render(request, 'view_list.html', context)


def add(request):
    properties = []
    if request.method == 'GET':
        properties.reverse()
        context = {
            'application': __get_application(request),
            'properties': properties,
            'object_name': 'OtherClassifier',
            'object_name_lower': 'otherclassifier',
        }
        return render(request, 'add_object.html', context)

    otherclassifier = OtherClassifier()
    otherclassifier.save()

    return redirect('/' + __get_application(request).name + '/otherclassifier')


def edit(request, id):
    properties = []
    otherclassifier = OtherClassifier.objects.get(id=id)
    if request.method == 'GET':
        properties.reverse()
        context = {
            'application': __get_application(request),
            'object_id': id,
            'properties': properties,
            'object_name': 'OtherClassifier',
            'object_name_lower': 'otherclassifier',
        }
        return render(request, 'edit_object.html', context)

    otherclassifier.save()

    return redirect('/' + __get_application(request).name + '/otherclassifier')


def delete(request, id):
    otherclassifier = OtherClassifier.objects.get(id=id)
    otherclassifier.delete()
    return redirect('/' + __get_application(request).name + '/otherclassifier')


def __get_application(request):
    application_name_lower = request.build_absolute_uri().split('/')[3]
    application = Application.objects.get(name=application_name_lower)
    return application
