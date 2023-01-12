import os
from shutil import copyfile
from shutil import rmtree
import threading
from django.core import management


def generate_urls(application):
    os.mkdir(application.name + '/views')
    copyfile('model/templates/files/template_urls.py', application.name + '/urls.py')
    copyfile('model/templates/files/template_views_init.py', application.name + '/views/__init__.py')

    f = open("runtime/urls.py", "r")
    contents = f.readlines()
    index = contents.index(']\n')
    f.close()
    value = '    path(\'' + application.name + '/\', include(\'' + application.name + '.urls\')),\n'
    contents.insert(index, value)

    f = open("runtime/urls.py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()

    f = open(application.name + "/urls.py", "r")
    contents = f.readlines()
    index = contents.index(']\n')
    f.close()
    value = '    path(\'\', views.index),\n'
    contents.insert(index, value)

    f = open(application.name + "/urls.py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


def remove_urls(application):
    f = open("runtime/urls.py", "r")
    contents = f.readlines()
    value = '    path(\'' + application.name + '/\', include(\'' + application.name + '.urls\')),\n'
    f.close()
    contents.remove(value)

    f = open("runtime/urls.py", "w")
    contents = "".join(contents)
    f.write(contents)
    f.close()


class ApplicationGenerator:
    def __init__(self, application):
        self.application = application

    def generate(self):
        management.call_command('startapp', self.application.name)
        generate_urls(self.application)

    def delete(self):
        rmtree(self.application.name, ignore_errors=True)
        remove_urls(self.application)
