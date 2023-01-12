"""Module to connect to AllenNLP library and use entailment implementation"""
import json
import requests
from ..allen_nlp.allen_nlp_interface import AllenNLPinterface
from .nlp_config import *


class Entailment(AllenNLPinterface):
    """Class calculate entailment between sentences."""

    def __init__(self) -> None:
        """Init class."""
        # AllenNLPinterface.__init__(self, "http://allen_abgs:5000/predict/snliadv/loac")
        AllenNLPinterface.__init__(self, nlp_urls["entail"])

    # def create_input_dict(self, premise: str, hypothesis: str):
    #     """Create input object with premise and hypothesis"""
    #     input_object = {"premise": premise, "hypothesis": hypothesis}
    #     return input_object

    def entailment(self, document: list) -> list:
        """Process entailment for a premise and hypothesis.

        Description:
           The entailment between a premise and hypothesis is calculated.
           The calculation takes place within a docker container where the
           AllenNLPinterface is the wrapper to acces the container. This function
           processes the input data of a entailment.
        Args:
            - document (list): contains a list of dictionary elements with each a
                premise and hypothesis.

        Returns:
            - result (list): a list with all the results from the entailment
        """
        if not self.connect(document):
            return []
        return self.result["output"]
