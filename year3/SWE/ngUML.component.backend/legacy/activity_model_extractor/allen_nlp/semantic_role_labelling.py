"""Module to connect to api with AllenNLP running and transform the outcomes."""
import nltk

# from ..configuration.indicators import (
#     conditional_indicators as cond_ind,
# )
from ..allen_nlp.allen_nlp_interface import AllenNLPinterface
from .nlp_config import *


nltk.download("punkt")


class SemanticRoleLabelling(AllenNLPinterface):
    """SemanticRoleLabelling interface to the AllenNLP SRL."""

    # Functions
    def __init__(self) -> None:
        """Initialise class and set variables."""
        AllenNLPinterface.__init__(self, nlp_urls["srl"])
        self.result = {}
        self.triples = []
        self.actors = []
        self.verbs = []
        self.objects = []

    def semrol_text(self, text):
        """Semantic Role Labelling for a text."""
        data = self.create_input_object(text)
        if not self.connect(data):
            return []
        return self.result["output"]

    def get_triple(self, sentence, verb_tags):
        """Get a triple based on the sentence and the verb_tags.

        Description:
           Gets the triple using the ARG0, B-V and ARG1

        Args:
           - sentence(str): the target sentence
           - verb_tags(list): the list of tags from the semantic role labelling info
        Returns:
           - list with arg0, verb, arg1 in separate items with indexes. e.g. ["he", [12,12]]
        """
        arg0 = ["", []]
        verb = ["", []]
        arg1 = ["", []]
        # word tokenization should be done one time. so in an overarching function
        sent = sentence
        # verbs = out['verbs']
        res = []
        for index, tag in enumerate(verb_tags):
            if "ARG0" in tag:
                arg0[0] += sent[index] + " "
                arg0[1].append(index)
            if "B-V" in tag:
                verb[0] += sent[index] + " "
                verb[1].append(index)
            if "ARG1" in tag:
                arg1[0] += sent[index] + " "
                arg1[1].append(index)
        arg0[0] = arg0[0].strip()
        verb[0] = verb[0].strip()
        arg1[0] = arg1[0].strip()
        res = [arg0, verb, arg1]
        return res

    def get_triples(self, output):
        """Get all triples for a certain sentence.

        Args:
           - output (list[dict]): verbs, tokenised sentence and other items within the sentence.

        Returns:
           - triples (list[triple]): list of triples.
        """
        triples = []
        verbs = output["verbs"]
        sent = output["words"]
        for i in range(len(verbs)):
            res = self.get_triple(sent, verbs[i]["tags"])
            triples.append(res)
        return triples

    def print_triples(self, triples):
        """Print triples in a clear overview."""
        for triple in triples:
            agent = triple[0][0] if triple[0][0] != "" else ".."
            verb = triple[1][0] if triple[1][0] != "" else ".."
            obj = triple[2][0] if triple[2][0] != "" else ".."
            print(agent + " " + verb + " " + obj)

    def print_all_triples(self, list_sents, output):
        """Print all the triples from SRL output.

        Args:
           - list_sents (list(dict)): a list of all the sentences in the input_object format.
           - output (list): list of the output that is for each sentence a key to verbs and words.

        Description:
           Prints all the triples for the given result.
        """
        for i in range(len(output)):
            triples = self.get_triples(output[i])
            print("sent: {}".format(i))
            self.print_triples(triples)

    def get_all_triples(self, output):
        """Retrieve the triples for all sentences.

        Args:
           - list_sents (list(dict)): a list of all the sentences in the input_object format.
           - output (list): list of the output that is for each sentence a key to verbs and words.

        Description:
           Retrieves the triples for each sentence.

        Returns:
           - list (): with all triples
        """
        self.triples = []
        for i in range(len(output)):
            triple = self.get_triples(output[i])
            self.triples.append(triple)
        return self.triples

    def get_actor_verb_object(self):
        """Retrieve the actors,verbs and objects from the triples.

        Args:
           None

        Description:
           Retrieves actors, verbs and objects and fills them into the list variables.

        Returns:
           list [agents, verbs, objects] e.g. [[['A customer', [0, 1]], 0], [['the CRS', [8, 9]], 0]]]
        """
        for index, sentence in enumerate(self.triples):
            for triple in sentence:
                if triple[0][1]:
                    if [triple[0], index] not in self.actors:
                        self.actors.append([triple[0], index])
                if triple[1][1]:
                    self.verbs.append([triple[1], index])
                if triple[2][1]:
                    self.objects.append([triple[2], index])
        return [self.actors, self.verbs, self.objects]

    ## From here we work on the condition extraction.

    def get_sentences_from_srl_tag(self, tag: str, output: list) -> list:
        """Get all sentences from SRL output that contain a specific SRL tag.

        Args:
           - tag (str): tag we are looking for. For example 'B-ARGM-ADV'
           - output (list(dict)): containing all the results from the SRL

        Returns:
           - result (list(str)): List of all sentences found in output that have the given tag.

        """
        result = []
        for sen in output:
            for res in sen["verbs"]:
                if tag in res["tags"]:
                    result.append(res["description"])
        return result

    def get_agent_verb_object_data(self, srl_sentence_result: dict) -> dict:
        """For each result in the SRL results check if there is agent_verb_object combination and return it.

        Description:
           For each result in the SRL results we check if there is a B-ARG in there. If that is
           the case we probably have an action. Then we return it as a double. We also return the
           begin and end index of the combination. Which can be used further down the line.

        Args:
           - srl_sentence_result (dict): a dictionary with a SRL result for a sentence.

        Returns:
           - result (dict): a list of a agent,verb,object combination with the following:
                 agent, verb, object, begin_index, end_index
        """
        result = {
            "agent": [],
            "verb": [],
            "object": [],
            "begin_index": -1,
            "end_index": -1,
            "ADV": [],
        }

        begin_index = -1
        end_index = -1
        agent = [-1, -1]
        verb = [-1, -1]
        object = [-1, -1]
        adv = [-1, -1]
        for index, tag in enumerate(srl_sentence_result["tags"]):
            if tag != "O":
                if begin_index == -1:
                    begin_index = index
                end_index = index
                if "ARG0" in tag:
                    if agent[0] == -1:
                        agent[0] = index
                    agent[1] = index
                if "B-V" in tag or "I-V" in tag:
                    if verb[0] == -1:
                        verb[0] = index
                    verb[1] = index
                if "ARG1" in tag:
                    if object[0] == -1:
                        object[0] = index
                    object[1] = index
                if "ARGM-ADV" in tag:
                    if adv[0] == -1:
                        adv[0] = index
                    adv[1] = index
        result["agent"] = agent
        result["verb"] = verb
        result["object"] = object
        result["begin_index"] = begin_index
        result["end_index"] = end_index
        result["ADV"] = adv
        return result

    def process_adverbial(self, avo_data: dict) -> dict:
        """Remove the adverbial data from the avo_data, by setting the begin or end index based on the agent,verb,object."""
        if avo_data["begin_index"] == avo_data["ADV"][0]:
            # adverb in start of sentence
            b_indexes = []
            for key in ["agent", "verb", "object"]:
                if avo_data[key][0] != -1:
                    b_indexes.append(avo_data[key][0])
            new_begin = min(b_indexes)
            avo_data["begin_index"] = new_begin
        else:
            # adverb in end of the sent
            e_indexes = []
            for key in ["agent", "verb", "object"]:
                if avo_data[key][1] != -1:
                    e_indexes.append(avo_data[key][1])
            new_end = max(e_indexes)
            avo_data["end_index"] = new_end
        return avo_data

    def combine_agent_verb_objects_in_sentence(
        self,
        agent_verb_object_results: list,
    ) -> list:
        """Combine duplicate agent verb objects into one larger object.

        Description:
           The different ranges are compared to find out if there is an intersection. If that is
           the case update the found_ranges and use it, otherwise add a new found range.

        Args:
           - agent_verb_object_results (list): Results for each sentence from get_agent_verb_object_data

        Returns:
           - found_ranges (list): of dictionary items with all the found ranges, longest sentence_index and
              which sentence_items belong to it.
        """
        found_ranges = []
        for index, res in enumerate(agent_verb_object_results):
            check_range = range(res["begin_index"], res["end_index"] + 1)
            intersects = []
            ranges = [y["range"] for y in found_ranges]
            for range_index, possible_range in enumerate(ranges):
                if set(possible_range).intersection(check_range):
                    if len(check_range) > len(possible_range):
                        found_ranges[range_index]["range"] = range(
                            res["begin_index"], res["end_index"] + 1
                        )
                        found_ranges[range_index]["longest"] = index
                    found_ranges[range_index]["items"].append(index)
                    intersects.append(possible_range)
            if not intersects:
                found_ranges.append(
                    {
                        "range": range(res["begin_index"], res["end_index"] + 1),
                        "longest": index,
                        "items": [index],
                    }
                )
                # comparing the results - keep the ones we want.
        return found_ranges

    def get_avo_sentence(self, srl_result: dict) -> list:
        """Get all agent, verb, object combinations for a sentence and defines logic to split them.

        Description:
           Not all data should be considered. If there is overlap the combination needs
           to be split accordingly.

        Args:
           - srl_result (dict): result from the semantic role labelling to extract data from.

        Returns:
           - agent_verb_object (list): a list for all the agent_verb_objects for the given input.
        """
        agent_verb_object_results = []
        verbs = srl_result["verbs"]
        for srl_verbs_result in verbs:
            res = self.get_agent_verb_object_data(srl_verbs_result)
            if res["ADV"][0] != -1:
                res = self.process_adverbial(res)
            agent_verb_object_results.append(res)
        foundRanges = self.combine_agent_verb_objects_in_sentence(
            agent_verb_object_results
        )
        return [agent_verb_object_results, foundRanges]

    def print_avo_sent(self, avo_sent_result, srl_result):
        """Print avo sentence based on the given information."""
        for res in avo_sent_result[1]:
            begin = res["range"][0]
            end = res["range"][len(res["range"]) - 1]
            print(" ".join(srl_result["words"][begin : end + 1]))

    def get_avo_for_sentences(self, srl_results: list) -> list:
        """Get an avo result for a a sentence.

        Args:
           - srl_results (list): semantic role labelling result for a text.

        Returns:
           - avo_result (list): with elements for each result that contains agent, verb and object combinations with extra data.
        """
        avo_results = []
        for sent_index, srl_result in enumerate(srl_results):
            avo_res = self.get_avo_sentence(srl_result)
            for avo_range in avo_res[1]:
                avo_data = {
                    "action_text": "",
                    "begin_index": -1,
                    "end_index": -1,
                    "sent_index": sent_index,
                    "avo_results": [],
                    "condition": False,
                    "action": False,
                }
                avo_data["begin_index"] = avo_range["range"][0]
                avo_data["end_index"] = avo_range["range"][len(avo_range["range"]) - 1]
                avo_data["action_text"] = srl_result["words"][
                    avo_data["begin_index"] : avo_data["end_index"] + 1
                ]
                avo_data["avo_results"] = [
                    result
                    for index, result in enumerate(avo_res[0])
                    if index in avo_range["items"]
                ]
                avo_results.append(avo_data)
        return avo_results

    def list_rindex(self, li: list, x: any):
        """Reverse index in a list."""
        for i in reversed(range(len(li))):
            if li[i] == x:
                return i
        raise ValueError("{} is not in list".format(x))

    def tag_conjunction_based_index(
        self,
        conj_indices: list,
        ranges_avo_sents: list,
        end_index_avo_sents: list,
        sent_avo_results: list,
        begin_key: str,
        end_key: str,
    ) -> None:
        """Tag 'and' conjunctions."""
        for conj_index in conj_indices:
            and_in_range = [
                True for avo_ranges in ranges_avo_sents if conj_index in avo_ranges
            ]
            if not and_in_range:
                # check if it is between two ranges.
                larger_index = [
                    True if last_index < conj_index else False
                    for last_index in end_index_avo_sents
                ]
                # get the furthest True -> this is possible the one we need to check.
                if True in larger_index:
                    last_index = self.list_rindex(larger_index, True)
                    if last_index < len(larger_index) - 1:
                        # not the last one
                        # add a tag on the last_index avo_sents and the next one.
                        sent_avo_results[last_index][begin_key] = conj_index
                        sent_avo_results[last_index + 1][end_key] = conj_index

    def tag_conjunctions(
        self, agent_verb_object_results: list, semantic_role_labelling_result: list
    ):
        """Tag conjunctions in avo_sentences."""
        for sent_index, sentence_result in enumerate(semantic_role_labelling_result):
            words = sentence_result["words"]
            and_indices = [index for index, word in enumerate(words) if word == "and"]
            or_indices = [index for index, word in enumerate(words) if word == "or"]
            sent_avo_sents = [
                avo_sent
                for avo_sent in agent_verb_object_results
                if avo_sent["sent_index"] == sent_index
            ]
            ranges_avo_sents = [
                range(avo["begin_index"], avo["end_index"] + 1)
                for avo in sent_avo_sents
            ]
            end_index_avo_sents = [avo["end_index"] for avo in sent_avo_sents]
            if and_indices:
                # check the 'and' indices of the avo_sents
                self.tag_conjunction_based_index(
                    conj_indices=and_indices,
                    ranges_avo_sents=ranges_avo_sents,
                    end_index_avo_sents=end_index_avo_sents,
                    sent_avo_results=sent_avo_sents,
                    begin_key="and-conj-begin",
                    end_key="and-conj-end",
                )
            if or_indices:
                # check 'or' indices of the avo_sents
                self.tag_conjunction_based_index(
                    conj_indices=or_indices,
                    ranges_avo_sents=ranges_avo_sents,
                    end_index_avo_sents=end_index_avo_sents,
                    sent_avo_results=sent_avo_sents,
                    begin_key="or-conj-begin",
                    end_key="or-conj-end",
                )
