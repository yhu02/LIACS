# Generated by Django 3.2.13 on 2022-09-30 22:27

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('extraction', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='ExtractionResult',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('model', models.CharField(blank=True, max_length=200)),
                ('result', models.JSONField()),
                (
                    'requirement',
                    models.ForeignKey(
                        on_delete=django.db.models.deletion.CASCADE,
                        related_name='extractions',
                        to='extraction.requirement',
                    ),
                ),
            ],
        ),
    ]