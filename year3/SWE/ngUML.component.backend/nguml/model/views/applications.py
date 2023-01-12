from django.shortcuts import render
from django.shortcuts import redirect
from ..models import *
from ..generators import *
import pprint


def index(request):
    application_list = Application.objects.all()
    sections = Section.objects
    pages = Page.objects
    applications = []
    # for prop in properties if prop.name not in inherited
    for application in [
        application
        for application in application_list
        if not sections.filter(id=application.id).exists() and not pages.filter(id=application.id).exists()
    ]:
        applications.append(application)
    context = {'applications': applications}
    return render(request, 'applications.html', context)


def add(request):
    if request.method == 'GET':
        return render(request, 'add_application.html')

    name = request.POST['name']

    application = Application(name=name.lower())
    application.save()

    ApplicationGenerator(application).generate()

    return redirect('/model/applications')


def view(request, application_id):
    classifiers_to_add = Classifier.objects.all()
    application = Application.objects.get(id=application_id)
    categories = application.categories.all()
    classifiers = application.classifiers.all()
    for classifier in [x for x in classifiers]:
        classifier.properties = Property.objects.filter(classifier=classifier, applications=application).all()
        # classifier.operations = Operation.objects.filter(classifier_id=classifier.id)
    classifiers_to_add = [x for x in classifiers_to_add if x not in classifiers]
    for classifier in classifiers:
        classifier.name_lower = classifier.name.lower()
    context = {
        'application': application,
        'classifiers': classifiers,
        'categories': categories,
        'classifiers_to_add': classifiers_to_add,
    }

    return render(request, 'application_view.html', context)


def add_classifier(request, application_id):
    application = Application.objects.get(id=application_id)
    classifier = Classifier.objects.get(id=request.POST['classifier_id'])
    application.classifiers.add(classifier)
    application.save()

    ClassifierGenerator(classifier).link_application(application_id)

    return redirect('/model/applications/' + application_id)


def unlink(request, application_id, classifier_id):
    classifier = Classifier.objects.get(id=classifier_id)
    application = Application.objects.get(id=application_id)
    application.classifiers.remove(classifier)
    application.save()
    ClassifierGenerator(classifier).unlink_application(application)

    return redirect('/model/applications/' + application_id)


def delete(request, application_id):
    application = Application.objects.get(id=application_id)
    application.delete()

    ApplicationGenerator(application).delete()

    return redirect('/model/applications')


def properties(request, application_id, classifier_id):
    application = Application.objects.get(id=application_id)
    classifier = Classifier.objects.get(id=classifier_id)
    properties = Property.objects.filter(classifier=classifier).all()
    if request.method == 'GET':
        for property in properties:
            property.linked = False
            if application in property.applications.all():
                property.linked = True

        context = {'properties': properties, 'application': application, 'classifier': classifier}
        return render(request, 'application_link_properties.html', context)

    for property in properties:
        if property.name in request.POST:
            if application not in property.applications.all():
                property.applications.add(application)
                PropertyGenerator(property).link_to_application(application)
        else:
            if application in property.applications.all():
                property.applications.remove(application)
                PropertyGenerator(property).unlink_from_application(application)

    return redirect('/model/applications/' + application_id)
