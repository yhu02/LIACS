from django.shortcuts import render
from django.shortcuts import redirect
import string
from django.http import HttpResponse
from django.db.models import Q
from ..models import *
import json
import copy
from django.forms.models import model_to_dict
import os
from ..generators import *
import re
from django.apps import apps as django_apps


def add_category(request, application_id):
    application = Application.objects.get(id=application_id)
    if request.method == 'GET':
        context = {
            'application': application,
        }
        return render(request, 'add_category.html', context)
    else:
        name = request.POST['name']
        category = Category(name=name.lower())
        category.save()
        application.categories.add(category)
        return redirect('/model/applications/' + application_id)


def show_category(request, application_id, category_id):
    application = Application.objects.get(id=application_id)
    category = application.categories.filter(id=category_id).get()
    pages = category.page_set.all()
    context = {
        'application': application,
        'category': category,
        'pages': pages,
    }
    return render(request, 'show_category.html', context)


def edit_category(request, application_id, category_id):
    application = Application.objects.get(id=application_id)
    category = Category.objects.get(id=category_id)
    if request.method == 'POST':
        name = request.POST['name']
        category.name = name
        category.save()
    context = {'application': application, 'category': category}
    return render(request, 'edit_category.html', context)


def delete_category(request, application_id, category_id):
    application = Application.objects.get(id=application_id)
    category = application.categories.filter(id=category_id).get()

    category.delete()
    application.categories.remove(category)
    return redirect('/model/applications/' + application_id)


def add_page(request, application_id, category_id):
    application = Application.objects.get(id=application_id)
    category = application.categories.filter(id=category_id).get()
    if request.method == 'GET':
        context = {'application': application, 'category': category}
        return render(request, 'add_page.html', context)
    else:
        name = request.POST['name']
        page = Page(name=name.lower())
        page.save()
        page.category.add(category)
        return redirect('/model/applications/' + application_id + '/category/' + category_id)
