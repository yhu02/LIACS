from django.shortcuts import render
from model.models import Application

# Create your views here.


def index(request):
    application_name_lower = request.build_absolute_uri().split('/')[3]
    application = Application.objects.get(name=application_name_lower)
    classifiers = application.classifiers.all()
    for classifier in classifiers:
        classifier.name_lower = classifier.name.lower()
    context = {'application': application, 'classifiers': classifiers}
    return render(request, 'index3.html', context)
