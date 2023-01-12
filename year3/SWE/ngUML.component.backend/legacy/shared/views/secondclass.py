from django.shortcuts import render
from django.shortcuts import redirect
from runtime.models import *
from model.models import Application


def index(request):
    properties = []
    objects = SecondClass.objects.all()
    for object in objects:
        object.properties = []
        object.properties.reverse()
    context = {
        'application': __get_application(request),
        'properties': properties,
        'objects': objects,
        'object_name': 'SecondClass',
        'object_name_lower': 'secondclass',
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
            'object_name': 'SecondClass',
            'object_name_lower': 'secondclass',
        }
        return render(request, 'add_object.html', add_context)

    secondclass = SecondClass()
    secondclass.save()

    return redirect('/' + __get_application(request).name + '/secondclass')


def edit(request, id):
    properties = []
    secondclass = SecondClass.objects.get(id=id)
    if request.method == 'GET':
        properties.reverse()
        edit_objects = []
        edit_context = {
            'application': __get_application(request),
            'object_id': id,
            'properties': properties,
            'object_name': 'SecondClass',
            'object_name_lower': 'secondclass',
        }
        return render(request, 'edit_object.html', edit_context)

    secondclass.save()

    return redirect('/' + __get_application(request).name + '/secondclass')


def delete(request, id):
    secondclass = SecondClass.objects.get(id=id)
    secondclass.delete()
    return redirect('/' + __get_application(request).name + '/secondclass')


def __get_application(request):
    application_name_lower = request.build_absolute_uri().split('/')[3]
    application = Application.objects.get(name=application_name_lower)
    return application
