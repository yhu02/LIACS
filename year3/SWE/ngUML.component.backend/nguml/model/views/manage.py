from django.shortcuts import HttpResponse, redirect
from shutil import copyfile
import os
import threading
import subprocess
import sys
import signal
from ..models import *
from ..generators import *
from django.conf import settings
from django.utils import autoreload
from django.core import management
import time
import json
from threading import Thread


def restart(request):
    management.call_command('makemigrations')
    management.call_command('migrate')
    # for thread in threading.enumerate():
    #     os.kill(
    #         thread.native_id,
    #         signal.SIGHUP
    #     )
    return HttpResponse(str(json.dumps('Restarting Application Server...')))


def clear(request):
    for c in Classifier.objects.all():
        ClassifierGenerator(c).delete()
        c.delete()
    for p in Property.objects.all():
        PropertyGenerator(p).delete()
        p.delete()
    for o in Operation.objects.all():
        o.delete()
    for r in Relationship.objects.all():
        RelationshipGenerator(r).delete()
        r.delete()
    for a in Application.objects.all():
        ApplicationGenerator(a).delete()
        a.delete()
    models = settings.BASE_DIR + '/runtime/models.py'
    default = settings.BASE_DIR + '/runtime/models.default.py'
    copyfile(default, models)
    notDelete = ['__init__.py', 'views.py', 'newclassifier.py', 'otherclassifier.py']
    for f in os.listdir(settings.BASE_DIR + '/runtime/views'):
        if f not in notDelete and f:
            if not os.path.isdir(f):
                os.remove(settings.BASE_DIR + '/runtime/views/' + f)
    Thread(target=restart, args=(True,)).start()
    return HttpResponse(str(json.dumps('Restarting Server...')))


def generate(request):
    for c in Classifier.objects.all():
        ClassifierGenerator(c).generate(True)
    for r in Relationship.objects.all():
        RelationshipGenerator(r).generate(True)
    for p in Property.objects.all():
        PropertyGenerator(p).generate(True)
    for a in Application.objects.all():
        ApplicationGenerator(a).generate()
    management.call_command('make_and_run_migrations')
    restart(True)
    return HttpResponse(str(json.dumps('Running Generations & Migrations...')))


def migrate(request):
    management.call_command('make_and_run_migrations')
    return HttpResponse(str(json.dumps('Making and Running Migrations...')))


def reset(request):
    management.call_command("flush", "--noinput")
    management.call_command("make_and_run_migrations")
    return HttpResponse(str(json.dumps('emptying database')))
