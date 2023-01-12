from django.shortcuts import render
from django.shortcuts import redirect
import string
from django.http import HttpResponse
from django.db.models import Q
from ..models import *
import json
import copy
from django.forms.models import model_to_dict
import os
from ..generators import *
import re
from django.apps import apps as django_apps


def loop_through_relationships(relationships, paths, added, new_paths, currentpath, model):
    for relationship in [
        rela
        for rela in relationships
        if (rela.multiplicity_to == '1' or rela.multiplicity_from == '1')
        and (
            (
                rela.classifier_to.name not in paths
                or (rela.classifier_to.name in paths and len(paths[rela.classifier_to.name]) > len(currentpath) + 1)
            )
            or (
                rela.classifier_from.name not in paths
                or (rela.classifier_from.name in paths and len(paths[rela.classifier_from.name]) > len(currentpath) + 1)
            )
        )
    ]:
        if relationship.classifier_to.name == model.name:
            new_currentpath = (
                currentpath + [relationship.classifier_from.name]
                if len(currentpath) > 0
                else [relationship.classifier_to.name, relationship.classifier_from.name]
            )
            paths[relationship.classifier_from.name] = new_currentpath
            added.append(relationship.classifier_from)
        else:
            new_currentpath = (
                currentpath + [relationship.classifier_to.name]
                if len(currentpath) > 0
                else [relationship.classifier_from.name, relationship.classifier_to.name]
            )
            paths[relationship.classifier_to.name] = new_currentpath
            added.append(relationship.classifier_to)
        new_paths.append(new_currentpath)


def get_possible_classifiers(page, model, paths, currentpath):
    added = []
    new_paths = []
    relationships = Association.objects.filter(Q(classifier_from=model) | Q(classifier_to=model)).all()
    loop_through_relationships(relationships, paths, added, new_paths, currentpath, model)
    relationships = Composition.objects.filter(Q(classifier_from=model) | Q(classifier_to=model)).all()
    loop_through_relationships(relationships, paths, added, new_paths, currentpath, model)

    for i, classifier in enumerate(added):
        get_possible_classifiers(page, classifier, paths, new_paths[i])
    return paths


def page_link_properties(request, application_id, page_id):
    application = Application.objects.get(id=application_id)
    classifiers = application.classifiers.all()
    page = Page.objects.get(id=page_id)
    allProperties = application.property_set
    selectedProperties = page.property_set.all()
    if page.data_paths == "":
        context = {
            'application': application,
            'page': page,
            'classifiers': classifiers,
            'msg': "You have to select a main model first",
        }
        return render(request, 'page_link_properties.html', context)
    else:
        paths = json.loads(page.data_paths)
    if request.method == "GET":
        classifiersToBeShown = []
        for classifier in classifiers:
            if classifier.name not in paths:
                classifiers.exclude(name=classifier.name)
                continue
            classifier.properties = []
            thisProperties = allProperties.filter(classifier=classifier.id).all()
            for property in [x for x in thisProperties]:
                if property in selectedProperties:
                    property.linked = True
                classifier.properties.append(property)
            classifiersToBeShown.append(classifier)
        context = {
            'application': application,
            'page': page,
            'classifiers': classifiersToBeShown,
        }
        return render(request, 'page_link_properties.html', context)
    else:
        for property in allProperties.all():
            if property.classifier.name + "-" + property.name in request.POST:
                if page not in property.applications.all():
                    property.applications.add(page)
                    if property.classifier not in page.classifiers.all():
                        page.classifiers.add(property.classifier)
            else:
                if property.applications.filter(id=page_id).exists():
                    property.applications.remove(page)
                    if (
                        property.classifier in page.classifiers.all()
                        and page.property_set.filter(~Q(id=property.id), classifier=property.classifier).count() == 0
                    ):
                        page.classifiers.remove(property.classifier)

        return redirect('/model/applications/' + application_id + '/page/' + page_id + '/properties')


def make_children_list(data_paths):
    children = {}
    classifiers = [value for value in data_paths]
    for parent in data_paths:
        children[parent] = []
        classifiers.remove(parent)
        for classifier in classifiers:
            list = data_paths[classifier]
            if list[-2] == parent:
                children[parent].append(classifier)
    return children


def get_data(current_data, model, children, all_children, page=None):
    data = {'data': current_data, 'children': []}
    for i in range(len(current_data)):
        data_line = current_data[i]
        if len(children) > 0:
            data['children'].insert(i, {})
        for child in children:
            children_info = data['children'][i]
            lowercase = str(child).lower()
            if hasattr(data_line, lowercase):
                children_info[child] = get_data(
                    [getattr(data_line, lowercase)], child, all_children[child], all_children, page
                )
            else:
                query = {str(model).lower(): data_line.id}
                result = django_apps.get_model('shared', child).objects.filter(**query).all()
                children_info[child] = get_data(list(result), child, all_children[child], all_children, page)
    if page and page.create:
        data['children'].insert(0, {})
        for child in children:
            data['children'][0][child] = get_data([], child, all_children[child], all_children, page)
    return data


def fill_data(page, queryset):
    data_paths = json.loads(page.data_paths)
    children = make_children_list(data_paths)
    data = {page.type: get_data(queryset, page.type, children[page.type], children, page)}
    return data, children


def handle_sent_data(application, page, items, children):
    objects = {}
    for key in items:
        value = items[key]
        if "-" in key:
            key = key.split("-")
            object_name = key[0]
            classifier_name = key[1]
            if len(key) == 3:
                if key[2].isnumeric():
                    if object_name not in objects:
                        object = django_apps.get_model('shared', object_name).objects.filter(id=key[2]).first()
                        add_object_to_objects(objects, object, object_name, object.id)
                        object_key = object.id
                    else:
                        if int(key[2]) not in objects[object_name]:
                            object = django_apps.get_model('shared', object_name).objects.filter(id=key[2]).first()
                            add_object_to_objects(objects, object, object_name, object.id)
                        object_key = int(key[2])
                    setattr(objects[object_name][object_key], classifier_name, value)
                else:
                    if key[0].isnumeric():
                        object_name = key[1]
                        classifier_name = key[2]
                        if object_name not in objects:
                            new_object = django_apps.get_model('shared', object_name).objects.create()
                            add_object_to_objects(objects, new_object, object_name, int(key[0]))
                            object_key = int(key[0])
                        else:
                            if int(key[0]) not in objects[object_name]:
                                object = django_apps.get_model('shared', object_name).objects.create()
                                add_object_to_objects(objects, object, object_name, int(key[0]))
                            object_key = int(key[0])
                        setattr(objects[object_name][object_key], classifier_name, value)
            else:
                if key[0].isnumeric():
                    object_name = key[1]
                    key[0] = int(key[0])
                    if object_name in objects:
                        objects[object_name][key[0]] = (
                            django_apps.get_model('shared', object_name).objects.filter(id=value).first()
                        )
                    else:
                        objects[object_name] = {
                            key[0]: django_apps.get_model('shared', object_name).objects.filter(id=value).first()
                        }
                if object_name not in objects:
                    new_object = django_apps.get_model('shared', object_name).objects.create()
                    add_object_to_objects(objects, new_object, object_name, new_object.id)
                    object_key = new_object.id
                else:
                    keys = list(objects[object_name].keys())
                    object_key = keys[0]
                setattr(objects[object_name][object_key], classifier_name, value)
        else:
            if key in objects:
                objects[key][value] = django_apps.get_model('shared', key).objects.filter(id=value).first()
            else:
                objects[key] = {value: django_apps.get_model('shared', key).objects.filter(id=value).first()}

    assign_relationships(objects, False)
    assign_relationships(objects, True)
    return


def assign_relationships(objects, trydefault):
    for key in objects:
        object_list = objects[key]
        ids = list(object_list.keys())
        for id in object_list:
            object = object_list[id]
            object_key = ids.index(id)
            for relation in objects:
                if hasattr(object, str(relation).lower()) and getattr(object, str(relation).lower()) is None:
                    relation_keys = list(objects[relation].keys())
                    if object_key in relation_keys:
                        setattr(object, str(relation).lower(), objects[relation][relation_keys[object_key]])
                    else:
                        if trydefault:
                            setattr(object, str(relation).lower(), objects[relation][relation_keys[0]])
            if trydefault:
                object.save()


def add_object_to_objects(objects, object, object_name, key):
    if object_name in objects:
        objects[object_name][key] = object
    else:
        objects[object_name] = {key: object}


def show_page(request, application_id, page_id):
    application = Application.objects.get(id=application_id)
    page = Page.objects.get(id=page_id)
    all_sections = Section.objects.filter(linked_page=page).order_by("sorting").all()
    sections = change_sections(all_sections, False)

    query = {}
    page.query = page.query.replace('[id]', request.GET.get('id', ''))
    for value in page.query.split(","):
        if value.strip() == "":
            break
        split = value.split("=")
        if split[1].strip() == "":
            continue
        query[split[0].strip()] = split[1].strip()
    if not page.create:
        queryset = django_apps.get_model('shared', page.type).objects.filter(**query).all()
    else:
        queryset = []
    data, children = fill_data(page, queryset)
    if request.method == "POST":
        items = request.POST.dict()
        del items['csrfmiddlewaretoken']
        handle_sent_data(application, page, items, children)
        if not page.create:
            queryset = django_apps.get_model('shared', page.type).objects.filter(**query).all()
        data, children = fill_data(page, queryset)

    html = make_html(sections, page, data, children, json.loads(page.data_paths))
    categories = application.categories.all()
    for category in categories:
        category.pages = Page.objects.filter(category=category).all()

    context = {
        'application': application,
        'categories': categories,
        'page': page,
        'html': html,
    }
    return render(request, 'page_view.html', context)


def change_sections(all_sections, tojson):
    sections = {}
    for section in all_sections:
        section.children = {}
        sorting = section.sorting.split('.')
        target = sections
        if '.' in section.sorting:
            for sort in sorting[:-1]:
                target = target[int(sort)].children
        target[int(sorting[-1])] = section

    if tojson:
        sections = convert_section_to_json(sections)

    return sections


def convert_section_to_json(object):
    value = "["
    for item in object.values():
        value += (
            '{"classes":"'
            + item.classes
            + '","content":"'
            + item.content.replace('"', '\\\"')
            + '","children":'
            + convert_section_to_json(item.children)
            + '},'
        )
    value = value if value[-1] == '[' else value[:-1]
    value += "]"
    return value


def extract_data(result, paths, classifier):
    paths_copy = copy.deepcopy(paths)
    current_classifier = ""
    data = copy.deepcopy(result)
    while len(paths_copy[classifier]) > 1:
        current_classifier = paths_copy[classifier][0]
        paths_copy[classifier].remove(current_classifier)
        data = data[current_classifier]
        if len(data) > 1:
            returnable_data = []
            returnable_children = []
            next_classifier = paths_copy[classifier][0]
            for item in data['children']:
                info = extract_data(item[next_classifier], paths_copy, classifier)
                for data in info['data']:
                    returnable_data.append(data)
                for children in info['children']:
                    returnable_children.append(children)
            return {classifier: {'data': returnable_data, 'children': returnable_children}}
        else:
            data = data[0]['children']
    return data


def adjust_paths(paths, classifier):
    paths_copy = copy.deepcopy(paths)
    if list(paths_copy.keys())[0] == classifier:
        return paths_copy

    new_paths = {}
    for classifers in paths_copy.keys():
        if classifier in paths_copy[classifers]:
            new_paths[classifers] = copy.deepcopy(paths_copy[classifers])

    for classifiers in new_paths:
        worker = new_paths[classifiers]
        counter = 0
        for path in worker:
            if path == classifier:
                new_paths[classifiers] = copy.deepcopy(worker[counter:])
                break
            else:
                counter = counter + 1
    return new_paths


def get_highest_parent(page, result, children, paths, classifiers):
    if len(classifiers) == 0:
        return None
    if page.type in classifiers:
        return extract_data(result, paths, page.type)
    if len(classifiers) < 2:
        return extract_data(result, paths, classifiers[0])

    path_copy = {}
    for classifier in classifiers:
        path_copy[classifier] = copy.deepcopy(paths[classifier])

    stop = False
    parent = ""
    deepest = 0
    for classifier in path_copy.values():
        if len(classifier) > deepest:
            deepest = len(classifier)
    while not stop:
        for classifier in path_copy.values():
            if len(classifier) == deepest:
                classifier.pop(-1)

        parent = ""
        for classifier in path_copy.values():
            if parent == "":
                parent = classifier[-1]
                continue
            if classifier[-1] != parent:
                stop = False
                break
            stop = True
        deepest = deepest - 1
    return extract_data(result, paths, parent)


def convert_to_pure_html(section, result, paths):
    data = {}
    for classifier in section.classifiers.all():
        classifier = classifier.name
        extracted_data = extract_data(result, paths, classifier)
        if classifier in extracted_data and len(extracted_data[classifier]['data']) > 0:
            data[classifier] = extract_data(result, paths, classifier)[classifier]['data'][0]
            if data[classifier] is None:
                del data[classifier]

    content = section.content.replace("\\\"", "\"")
    inputfields = re.findall(r"<input id=\"(([a-zA-Z]+)-([a-zA-Z_]+))\" name=\"([a-zA-Z]+)-([a-zA-Z_]+)\"", content)
    for field in inputfields:
        if field[1] in data:
            content = content.replace(
                "<input id=\"" + field[0] + "\" name=\"" + field[0] + "\" type=\"text\" value=\"\">",
                "<input id=\"" + field[0] + "-" + str(data[field[1]].id) + "\" "
                "name=\"" + field[0] + "-" + str(data[field[1]].id) + "\" "
                "type=\"text\" value=\"{{" + field[0] + "}}\">",
            )
    touples = re.findall(r"{{([a-zA-Z]+)-([a-zA-Z_]+)}}", content)
    for touple in touples:
        if touple[0] in data:
            content = content.replace("{{" + touple[0] + "-" + touple[1] + "}}", getattr(data[touple[0]], touple[1]))
    selectboxes = re.findall(r"<select>(\s)*<option>(\s)*([a-zA-Z]+)(\s)*</option>(\s)*</select>", content)
    for touples in selectboxes:
        classifier = touples[2]
        data = django_apps.get_model('shared', classifier).objects.all()
        options = ""
        for item in data:
            options = options + '<option value="' + str(item.id) + '">' + str(item) + '</option>'
        content = content.replace(
            "<select>"
            + touples[0]
            + "<option>"
            + touples[1]
            + ""
            + classifier
            + ""
            + touples[3]
            + "</option>"
            + touples[4]
            + "</select>",
            '<select name="' + classifier + '" id="' + classifier + '">' + options + '</select>',
        )
    return content


def make_html(sections, page, result, children, paths):
    if sections == {}:
        return ''
    value = "<div class='row'>"
    # multiple line items per order
    for section in sections.values():
        maximum = 0
        continued_data = []
        highest_parent = get_highest_parent(
            page, result, children, paths, [classifier.name for classifier in section.classifiers.all()]
        )
        if highest_parent is None:
            continued_data = [result]
            maximum = 1
        else:
            parent = list(highest_parent.keys())[0]
            highest_parent = highest_parent[parent]
            paths = adjust_paths(paths, parent)

            for i in range(0, len(highest_parent['data'])):
                object = {'data': [highest_parent['data'][i]], 'children': []}
                if len(highest_parent['children']) > 0:
                    object['children'].append(highest_parent['children'][i])
                continued_data.append({parent: object})
                maximum = maximum + 1

            if "<input" in section.content and maximum == 0:
                maximum = 1
                continued_data = [result]
        for amount in range(0, maximum):
            data = continued_data[amount]
            children_html = make_html(section.children, page, data, children, paths)
            content = convert_to_pure_html(section, data, paths)
            value += (
                "<div class ='" + section.classes + "'>"
                "<div class='content'>" + content + "</div>"
                "<div class='children'>" + children_html + "</div></div>"
            )
    value += "</div>"
    return value


def edit_page(request, application_id, page_id):
    application = Application.objects.get(id=application_id)
    page = Page.objects.get(id=page_id)
    allClassifiers = application.classifiers.all()
    classifiers = []
    properties = page.property_set
    all_sections = Section.objects.filter(linked_page=page).order_by("sorting").all()
    sections = change_sections(all_sections, True).replace('\n', '')
    for classifier in allClassifiers:
        classifier.properties = []
        classifier.machine_name = classifier.name.translate({ord(c): None for c in string.whitespace})
        classifierProperties = properties.filter(classifier=classifier).all()
        for property in classifierProperties:
            property.machine_name = property.name.translate({ord(c): None for c in string.whitespace})
            classifier.properties.append(property)
        if len(classifier.properties) > 0:
            classifiers.append(classifier)
    context = {'application': application, 'page': page, 'classifiers': classifiers, 'sections': sections}
    return render(request, 'page_edit.html', context)


def save_page(request, application_id, page_id):
    page = Page.objects.get(id=page_id)
    page.name = request.POST['name']
    page.save()
    Section.objects.filter(linked_page=page).delete()
    sections = json.loads(request.POST['sections'])
    make_sections(sections, page)
    return HttpResponse('')


def page_query(request, application_id, page_id):
    application = Application.objects.get(id=application_id)
    page = Page.objects.get(id=page_id)
    classifiers = application.classifiers.all()
    if request.method == "POST":
        page.type = request.POST['model']
        page.query = request.POST['filter']
        page.create = 'create' in request.POST or False
        main = Classifier.objects.get(name=page.type)
        paths = {main.name: [main.name]}
        get_possible_classifiers(page, main, paths, [])
        page.data_paths = json.dumps(paths)
        page.save()
    context = {
        'application': application,
        'page': page,
        'classifiers': classifiers,
    }
    return render(request, 'page_query.html', context)


def make_sections(sections, page):
    for section in sections:
        new_section = Section.objects.create(
            name='page-' + str(page.id) + 'section' + str(section['sorting']),
            classes=section['classes'],
            content=section['content'].replace("\"", "\\\""),
            sorting=section['sorting'],
            linked_page=page,
        )
        touples = re.findall(r"([a-zA-Z]+)-([a-zA-Z_]+)", section['content'])
        for touple in touples:
            try:
                classifier = Classifier.objects.filter(name=touple[0]).get()
                new_section.classifiers.add(classifier)
            except Classifier.DoesNotExist:
                print(touple)
        make_sections(section['children'], page)


def delete_page(request, application_id, page_id):
    page = Page.objects.get(id=page_id)
    category = page.category.get()
    page.delete()
    category.page_set.remove(page)
    return redirect('/model/applications/' + application_id)
