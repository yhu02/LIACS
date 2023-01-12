from extraction.models import NLPModel
from rest_framework import exceptions


def enqueue_nlp(model, text):
    if model == NLPModel.CLASS:
        from .class_nlp import class_nlp

        return class_nlp(text)
    if model == NLPModel.USECASE:
        from .usecase_nlp import usecase_nlp

        return usecase_nlp(text)
    raise ValueError('Requested NLP model is not yet implemented.')
