from django.core.management.base import BaseCommand
from django.core import management
import threading
import os


class Command(BaseCommand):
    def runCommands(self):
        os.system('python manage.py makemigrations --noinput && python manage.py migrate --noinput ')

    def handle(self, *args, **options):
        th = threading.Thread(target=self.runCommands)
        th.start()
