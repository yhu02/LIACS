from django.contrib import admin
from extraction.models import Requirement, Bucket, Job

admin.site.register(Requirement)
admin.site.register(Job)
