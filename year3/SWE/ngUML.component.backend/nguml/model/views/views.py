from django.shortcuts import render
from django.shortcuts import redirect
from ..models import Classifier, Property
import os


def index(request):
    return render(request, '../../model/templates/index2.html')


def generate(request):
    classifiers = Classifier.objects.all()
    for classifier in classifiers:
        classifier.properties = Property.objects.filter(classifier=classifier.id)
    context = {'classifiers': classifiers}

    management.call_command('generate')
    return redirect('/model/classifiers')


def diagram(request):
    os.system('python manage.py graph_models shared --arrow-shape normal -o static/graphs/classdiagram.png')
    return render(request, 'diagram.html')
