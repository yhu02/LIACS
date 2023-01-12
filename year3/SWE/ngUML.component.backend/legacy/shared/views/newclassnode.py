from django.shortcuts import render
from django.shortcuts import redirect
from runtime.models import *
from model.models import Application


def index(request):
    properties = []
    objects = newClassNode.objects.all()
    for object in objects:
        object.properties = []
        object.properties.reverse()
    context = {
        'application': __get_application(request),
        'properties': properties,
        'objects': objects,
        'object_name': 'newClassNode',
        'object_name_lower': 'newclassnode',
    }
    return render(request, 'view_list.html', context)


def add(request):
    properties = []
    if request.method == 'GET':
        properties.reverse()
        add_objects = []
        add_context = {
            'application': __get_application(request),
            'properties': properties,
            'object_name': 'newClassNode',
            'object_name_lower': 'newclassnode',
        }
        return render(request, 'add_object.html', add_context)

    newclassnode = newClassNode()
    newclassnode.save()

    return redirect('/' + __get_application(request).name + '/newclassnode')


def edit(request, id):
    properties = []
    newclassnode = newClassNode.objects.get(id=id)
    if request.method == 'GET':
        properties.reverse()
        edit_objects = []
        edit_context = {
            'application': __get_application(request),
            'object_id': id,
            'properties': properties,
            'object_name': 'newClassNode',
            'object_name_lower': 'newclassnode',
        }
        return render(request, 'edit_object.html', edit_context)

    newclassnode.save()

    return redirect('/' + __get_application(request).name + '/newclassnode')


def delete(request, id):
    newclassnode = newClassNode.objects.get(id=id)
    newclassnode.delete()
    return redirect('/' + __get_application(request).name + '/newclassnode')


def __get_application(request):
    application_name_lower = request.build_absolute_uri().split('/')[3]
    application = Application.objects.get(name=application_name_lower)
    return application
