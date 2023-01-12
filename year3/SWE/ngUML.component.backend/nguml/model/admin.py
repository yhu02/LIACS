from django.contrib import admin
from model.models import System, Project, Model, Node, Component, Port, Interface, Relation


class Admin(admin.ModelAdmin):
    readonly_fields = ('id',)

admin.site.register(System, Admin)
admin.site.register(Project, Admin)
admin.site.register(Model, Admin)
admin.site.register(Node, Admin)
admin.site.register(Component, Admin)
admin.site.register(Port, Admin)
admin.site.register(Interface, Admin)
admin.site.register(Relation, Admin)




