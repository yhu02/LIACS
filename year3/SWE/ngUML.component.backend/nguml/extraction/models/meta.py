from django.db import models
from django.utils.translation import gettext_lazy as _


class NLPModel(models.TextChoices):
    CLASS = 'CLASS', _('UML Class Model')
    USECASE = 'USECASE', _('UML Use Case Model')
    COMPONENT = 'COMPONENT', _('UML Component Model')
    ACTIVITY = 'ACTIVITY', _('UML Activity Model')
    BUCKET = 'BUCKET', _('Bucketing')
