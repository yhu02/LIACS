# Generated by Django 3.2.13 on 2022-09-30 22:42

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('extraction', '0002_extractionresult'),
    ]

    operations = [
        migrations.RenameModel(
            old_name='ExtractionResult',
            new_name='Extraction',
        ),
    ]
