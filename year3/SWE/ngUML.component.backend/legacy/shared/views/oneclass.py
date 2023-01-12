from django.shortcuts import render
from django.shortcuts import redirect
from runtime.models import *
from model.models import Application


def index(request):
    properties = []
    objects = OneClass.objects.all()
    for object in objects:
        object.properties = []
        object.properties.reverse()
    context = {
        'application': __get_application(request),
        'properties': properties,
        'objects': objects,
        'object_name': 'OneClass',
        'object_name_lower': 'oneclass',
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
            'object_name': 'OneClass',
            'object_name_lower': 'oneclass',
        }
        return render(request, 'add_object.html', add_context)

    oneclass = OneClass()
    oneclass.save()

    return redirect('/' + __get_application(request).name + '/oneclass')


def edit(request, id):
    properties = []
    oneclass = OneClass.objects.get(id=id)
    if request.method == 'GET':
        properties.reverse()
        edit_objects = []
        edit_context = {
            'application': __get_application(request),
            'object_id': id,
            'properties': properties,
            'object_name': 'OneClass',
            'object_name_lower': 'oneclass',
        }
        return render(request, 'edit_object.html', edit_context)

    oneclass.save()

    return redirect('/' + __get_application(request).name + '/oneclass')


def delete(request, id):
    oneclass = OneClass.objects.get(id=id)
    oneclass.delete()
    return redirect('/' + __get_application(request).name + '/oneclass')


def __get_application(request):
    application_name_lower = request.build_absolute_uri().split('/')[3]
    application = Application.objects.get(name=application_name_lower)
    return application
