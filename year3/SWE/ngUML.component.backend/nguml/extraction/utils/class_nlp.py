import django_rq


def execute_nlp(text=None):
    import classes.nlp.corenlp as nlp

    if text:
        object_dict, sum, subtypes, output = nlp.generate_uml(text)
        queue = django_rq.get_queue('model')
        queue.enqueue(store_nlp_result, object_dict, sum, subtypes)
        return nlp.generate_uml(text)
    return None


def class_nlp(text):
    queue = django_rq.get_queue('class')
    job = queue.enqueue(execute_nlp, text=text)
    return 'COOL!'


def store_nlp_result(object_dict, sum, subtypes):
    from model.models import (
        System,
        Class,
        Model,
        Classifier,
        Generalization,
        Relationship,
        Property,
        Composition,
        Association,
    )
    from django.db.models import Q

    class_model = Model.objects.get(pk=3)

    for item in object_dict.items():
        classifier = Class(name=item[0], class_model=class_model)
        classifier.save()
        # ClassifierGenerator(classifier, subtypes[item[0]] if item[0] in subtypes else '').generate(False)
        # add attributes of parent class
        if item[0] in subtypes:
            parent = Class.objects.filter(Q(class_model=class_model), Q(name=subtypes[item[0]])).get()
            properties = Property.objects.filter(classifier=parent).all()
            for property in properties:
                attribute = Property(name=property.name, classifier=classifier, type='string', inherited=True)
                attribute.save()
        # save attributes
        if item[1]['Attribute'] != '':
            get_cls = Classifier.objects.get(Q(class_model=class_model), Q(name=item[0]))
            for i in item[1]['Attribute']:
                i = '_'.join(i.split(' '))
                attr = Property(name=i, classifier=get_cls, type='string', inherited=False)
                attr.save()
                # PropertyGenerator(attr).generate(False)

    # save relation and direction into Ralph's demo
    for item in sum:
        # direction
        cls_from_name = item[1][1]
        cls_to_name = item[2][1]
        # get class name
        cls_from = Classifier.objects.get(Q(class_model=class_model), Q(name=cls_from_name))
        cls_to = Classifier.objects.get(Q(class_model=class_model), Q(name=cls_to_name))

        # passing an instance rather than a variable
        # relationship = Relationship(name=item[0][1], classifier_to=cls_to, classifier_from=cls_from)
        # relationship.save()

        if item[0][0] == 'Composition':
            composition = Composition(
                class_model=class_model,
                name=item[0][1],
                classifier_to=cls_to,
                classifier_from=cls_from,
                multiplicity_to=item[3][1][1],
                multiplicity_from=item[3][1][0],
                end_from=item[1][1],
                end_to=item[2][1],
            )
            composition.save()
            # RelationshipGenerator(composition).generate(False)
            # if Ralph's demo cannot execute many to many, replace the above code with this:
            # composition = Composition(name=item[0][1], classifier_to=cls_to, classifier_from=cls_from,
            #                           multiplicity_to='*', multiplicity_from='1',
            #                           end_from=item[1][1], end_to=item[2][1])
            # composition.save()
            # RelationshipGenerator(composition).generate(False)

        elif item[0][0] == 'Association':
            association = Association(
                class_model=class_model,
                name=item[0][1],
                classifier_to=cls_to,
                classifier_from=cls_from,
                multiplicity_to=item[3][1][1],
                multiplicity_from=item[3][1][0],
                end_from=item[1][1],
                end_to=item[2][1],
            )
            association.save()
            # RelationshipGenerator(association).generate(False)
            # if Ralph's demo cannot execute many to many, replace the above code with this:
            # association = Association(name=item[0][1], classifier_to=cls_to, classifier_from=cls_from,
            #                           multiplicity_to='*', multiplicity_from='1',
            #                           end_from=item[1][1], end_to=item[2][1])
            # association.save()
            # RelationshipGenerator(association).generate(False)

        else:
            generalization = Generalization(
                class_model=class_model, name=item[0][1], classifier_to=cls_to, classifier_from=cls_from
            )
            generalization.save()
    return True
