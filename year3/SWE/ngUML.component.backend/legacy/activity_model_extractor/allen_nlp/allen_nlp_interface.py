"""Module to connect to the allen nlp api."""
import json
import requests
import nltk
from ..tools.error_handler import handle_request_error


class AllenNLPinterface:
    """Main class to enable reuse between classes."""

    def __init__(self, url) -> None:
        self.result = []
        self.url = url

    def service_online(self):
        """Check if the service on the self.url is online."""
        try:
            get = requests.get(self.url)
            if get.status_code == 200:
                # print(f"{self.url}: is reachable")
                return True
            else:
                handle_request_error(
                    404,
                    (
                        f"{self.url}: is Not reachable, status_code: {get.status_code}. "
                        + "Is the AllenNLP service running?"
                    ),
                )
                return False
        except requests.exceptions.RequestException as exception:
            handle_request_error(
                404,
                f"{self.url}: is Not reachable \nErr:{exception}. Is the AllenNLP service running?",
            )
            return False

    def connect(self, sentences):
        """Connects to the AllenNLP Container and performs a prediction on the document.

        Args:
           - sentences (list(dict)): list of sentences. [{"sentence": "Pete went to the shop."},
                {"sentence": "..."}]

        Returns:
           - False: if the service is not online
           - True: if the service is online and the data is loaded.
        """
        if not self.service_online():
            return False
        res = requests.post(self.url, json=sentences)
        print(
            "Doing another call, to free up memory. service is {} online".format(
                self.service_online()
            )
        )
        self.result = json.loads(res.text)
        return True

    def create_input_object(self, input_text):
        """Create input object from text.

        Description:
           Split the text into sentences and add them to a list in the format necessary to
           process them simultanuously.

        Args:
           - input_text (str): string of text needed to parse with semantic role labelling.

        Returns:
           - input_object (list(dict_items)): List of dict items. Each dict item specifies
                a sentence.
        """
        sen_list = nltk.tokenize.sent_tokenize(input_text)
        input_object = []
        for sentence in sen_list:
            input_object.append({"sentence": sentence})
        return input_object
