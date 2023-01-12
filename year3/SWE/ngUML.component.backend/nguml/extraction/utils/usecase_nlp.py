import django_rq


def execute_nlp(text=None):
    import usecase.nlp.spacy as nlp

    if text:
        objects = nlp.generate_uml(text)
        queue = django_rq.get_queue('model')
        queue.enqueue(store_nlp_result, objects)
        return objects
    return None


def store_nlp_result(object_dict, sum, subtypes):
    return True


def usecase_nlp(text):
    queue = django_rq.get_queue('usecase')
    job = queue.enqueue(execute_nlp, text=text)
    return 'COOL!'
