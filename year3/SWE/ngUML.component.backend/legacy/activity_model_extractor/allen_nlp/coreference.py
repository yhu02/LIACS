"""Module to connect to AllenNLP library and use Coreference implementation"""
import json
import requests
import numpy as np
from nltk.tokenize import word_tokenize
from ..tools.common_methods import CommonFunctions
from .nlp_config import *


class Coreference:
    """Class to do coreference based on a document."""

    def __init__(self) -> None:
        """Initialize the different variables."""
        self.result = {}
        self.output = {}
        self.antecedents = []
        self.pronouns = []

    def connect(self, document):
        """Connects to the AllenNLP Container and performs a prediction on the document."""
        url = nlp_urls["coref"]
        input_obj = {"document": document}
        res = requests.post(url, json=input_obj)
        self.result = json.loads(res.text)

    def parse_data(self):
        """Parse the data into the different variables."""
        self.output = self.result["output"]
        self.antecedents = []
        self.pronouns = []
        predicted_ant = self.output["predicted_antecedents"]

        for index, item in enumerate(predicted_ant):
            if predicted_ant[index] != -1:
                ant_span = self.output["top_spans"][predicted_ant[index]]
                curr_span = self.output["top_spans"][index]
                ant_word = " ".join(
                    self.output["document"][ant_span[0] : (ant_span[1] + 1)]
                )
                curr_word = " ".join(
                    self.output["document"][curr_span[0] : (curr_span[1] + 1)]
                )
                self.antecedents.append([ant_word, ant_span])
                self.pronouns.append([curr_word, curr_span])

    def find_main_antecedents(self):
        """Finds the antecedents that do not exist in the pronouns.

        Which is important as these antecedents are the main agents/persons.

        """
        main_ant = []
        for item in self.antecedents:
            if item not in self.pronouns:
                main_ant.append(item)
        return main_ant

    def find_pron_in_ant(self):
        """Finds the pronouns within the antecedents."""
        found_pronouns = []
        for pron in self.pronouns:
            if pron in self.antecedents:
                found_pronouns.append(pron)
        return found_pronouns

    def get_pronoun_index(self, pronoun):
        """Return index of pronoun in pronouns.

        Args:
           pronouns: list ['pronounWord',[span]] e.g. ['his', [15,15]]
        Returns:
           index of the pronoun (int)
        """
        try:
            pos = self.pronouns.index(pronoun)
        except Exception as _:
            pos = -1
        return pos

    def return_personal_pronouns(self):
        """Return personal pronouns from the list of pronouns.

        Note:
           Might have problems with longer words. As currently only scans for personal pronouns.
        """
        pers_pronouns = ["i", "we", "he", "she", "you", "they", "it"]
        result = []
        for item in self.pronouns:
            if item[0].lower() in pers_pronouns:
                result.append(item)
        return result

    def find_main_pronoun_antecedent(self, pronoun):
        """Find main pronoun antecedent."""
        main = self.find_main_antecedents()
        while True:
            index = self.get_pronoun_index(pronoun)
            if index == -1:
                break
            ant = self.antecedents[index]
            if ant in main:
                return ant
            pronoun = ant
        return ""

    def find_all_personal_ant(self):
        """Finds all personal antecedents in the text."""
        personal_pronouns = self.return_personal_pronouns()
        result = []
        for personal_pronoun in personal_pronouns:
            out = self.find_main_pronoun_antecedent(personal_pronoun)
            if out != "":
                result.append([personal_pronoun, out])
        return result

    def replace_partial_text_with_coref(
        self,
        avo_sen: list,
        result_per_avo_sent: dict,
        avo_results_index: int,
        input_text: list,
    ) -> list:
        """Replace a piece of text with coref references.

        Assumption
        The given text where parts of the sentence are taken out have been
           substituted with three #'s
        """
        length_difference = 0
        action_copy = input_text[:]
        if avo_results_index in result_per_avo_sent.keys():
            coref_result = result_per_avo_sent[avo_results_index]
            sen_begin_index = avo_sen["begin_index"]
            replacements = [
                [
                    res[0][0],
                    res[1][0],
                    res[2][1] - sen_begin_index,
                    res[2][2] - sen_begin_index,
                ]
                for res in coref_result
            ]
            for replacement in replacements:
                begin = replacement[2] + length_difference
                end = replacement[3] + length_difference + 1
                old_word = word_tokenize(replacement[0])
                new_word = word_tokenize(replacement[1])
                if action_copy[begin:end] == old_word:
                    # replace the word
                    action_copy[begin:end] = new_word
                    length_difference += len(new_word) - len(old_word)
                else:
                    # in some cases we moved the word to the swimminglane.
                    pass
        else:
            pass
        return action_copy

    def order_coref_on_avo_index(
        self, avo_sents: list, coref_results: list, coref_text: list
    ) -> dict:
        """Order the coreference result per avo_sentence index and add begin and end index."""
        result_per_avo_index = {}
        com_method = CommonFunctions()
        sentence_lengths = com_method.get_list_sent_lengths(" ".join(coref_text))
        avo_sents_ordered = com_method.order_avo_on_sent_index(avo_sents)
        for reference_result in coref_results:
            sent_index = reference_result[2]
            sent_begin_index = com_method.find_sent_begin_text_index(
                sent_index, sentence_lengths
            )
            begin_reference_sent_index = reference_result[0][1][0] - sent_begin_index
            end_reference_sent_index = reference_result[0][1][1] - sent_begin_index
            if sent_index in avo_sents_ordered:
                possible_avos = avo_sents_ordered[sent_index]
                possible_ranges = [
                    range(avo["begin_index"], avo["end_index"] + 1)
                    for avo in possible_avos
                ]
                found_avo_indices = [
                    index
                    for index, test_range in enumerate(possible_ranges)
                    if begin_reference_sent_index in test_range
                ]
                avo_sent_indices = [
                    avo_sents.index(possible_avos[avo_id])
                    for avo_id in found_avo_indices
                ]
                for avo_sent_index in avo_sent_indices:
                    reference_result_item = reference_result[:]
                    reference_result_item[2] = [
                        reference_result_item[2],
                        begin_reference_sent_index,
                        end_reference_sent_index,
                    ]
                    if avo_sent_index in result_per_avo_index.keys():
                        result_per_avo_index[avo_sent_index].append(
                            reference_result_item
                        )
                    else:
                        result_per_avo_index[avo_sent_index] = [reference_result_item]
        return result_per_avo_index

    def fill_swimming_lanes_and_coref_sents(
        self, avo_sents: list, coref_results: list, coref_text: list
    ) -> list:
        """Fill swimming lanes and add coreference to sentences"""
        result_per_avo_sent = self.order_coref_on_avo_index(
            avo_sents, coref_results, coref_text
        )
        for avo_index, avo_sent in enumerate(avo_sents):
            action_text = avo_sent["action_text"][:]
            if avo_sent["sw_lane"]:
                begin_sw = avo_sent["sw_lane"][0]
                end_sw = avo_sent["sw_lane"][1] + 1
                # change all words to be a ### if they are not the swimminglane.
                # Such that only the words we need are used for coreferencing.
                rest_of_sent = [
                    item if index not in range(begin_sw, end_sw) else "###"
                    for index, item in enumerate(action_text)
                ]
                coref_sent_result = self.replace_partial_text_with_coref(
                    avo_sent, result_per_avo_sent, avo_index, rest_of_sent
                )
                # Keep the words that are not a ###
                sentence = [word for word in coref_sent_result if word != "###"]
                # replace all words other than the swimminglane with "###"
                swimming_lane_in_sent = [
                    item if index in range(begin_sw, end_sw) else "###"
                    for index, item in enumerate(action_text)
                ]
                coref_swimming_lane_result = self.replace_partial_text_with_coref(
                    avo_sent, result_per_avo_sent, avo_index, swimming_lane_in_sent
                )
                swimming_lane = [
                    word for word in coref_swimming_lane_result if word != "###"
                ]
                # complete sent
                coref_complete_sent = self.replace_partial_text_with_coref(
                    avo_sent, result_per_avo_sent, avo_index, action_text
                )
                # assign the data back to the avo_sent result
                avo_sent["node_text"] = sentence
                avo_sent["sw_lane_text"] = swimming_lane
                avo_sent["complete_sent"] = coref_complete_sent
            else:
                coref_sent_result = self.replace_partial_text_with_coref(
                    avo_sent, result_per_avo_sent, avo_index, action_text
                )
                avo_sent["node_text"] = coref_sent_result
                avo_sent["complete_sent"] = coref_sent_result

    def organise_cluster_per_sentence_and_coref_id(
        self, clusters: list, sentence_lengths: list
    ) -> dict:
        """Organise all the clusters per sentence id and coref id."""
        np_sen_lens = np.array(sentence_lengths)
        cluster_per_sent = {}
        for cluster_id, cluster in enumerate(clusters):
            for span in cluster:
                sentence_index = np.argmax(np_sen_lens > span[0])
                if sentence_index in cluster_per_sent:
                    sent_cluster_dict = cluster_per_sent[sentence_index]
                    if cluster_id in sent_cluster_dict:
                        sent_cluster_dict[cluster_id].append(span)
                    else:
                        sent_cluster_dict[cluster_id] = [span]
                else:
                    cluster_per_sent[sentence_index] = {}
                    cluster_per_sent[sentence_index][cluster_id] = [span]
        return cluster_per_sent

    def organise_cluster_per_sentence_span(
        self, clusters: list, sentence_lengths: list
    ) -> dict:
        """Organise all the clusters per sentence id and coref id."""
        np_sen_lens = np.array(sentence_lengths)
        cluster_per_sent = {}
        for cluster_id, cluster in enumerate(clusters):
            for span in cluster:
                sentence_index = np.argmax(np_sen_lens > span[0])
                if sentence_index in cluster_per_sent:
                    sent_cluster_dict = cluster_per_sent[sentence_index]
                    if sent_cluster_dict:
                        sent_cluster_dict.append([span, cluster_id])
                    else:
                        sent_cluster_dict = [[span, cluster_id]]
                else:
                    cluster_per_sent[sentence_index] = [[span, cluster_id]]
        return cluster_per_sent

    def tag_clusters_avo_sents(
        self, avo_sents: list, coref_document: list, clusters: list
    ) -> None:
        """Tag all the coref clusters in avo_sents.

        Args:
           - avo_sents(list): list of all sentences we are considering.
           - coref_document(list): list of all words in the document as a result from the Coreference.
           - clusters (list): List of clusters with spans representing the clusters.

        Returns:
           - None, but we change the values in avo_sents.
        """
        com_method = CommonFunctions()
        sentence_lengths = com_method.get_list_sent_lengths(" ".join(coref_document))
        # Step one organise all clusters into a dict per sent
        cluster_per_sent = self.organise_cluster_per_sentence_span(
            clusters, sentence_lengths
        )
        # Check for each sent if the clusters belong to it.
        for avo_sent in avo_sents:
            sent_index = avo_sent["sent_index"]
            if sent_index in cluster_per_sent:
                sent_cluster = cluster_per_sent[sent_index]
                start = 0 if sent_index == 0 else sentence_lengths[sent_index - 1]
                begin_index = avo_sent["begin_index"] + start
                end_index = avo_sent["end_index"] + start
                for cluster in sent_cluster:
                    cluster_start = cluster[0][0]
                    if cluster_start in range(begin_index, end_index + 1):
                        if "coref_ids" in avo_sent:
                            avo_sent["coref_ids"].append(cluster[1])
                        else:
                            avo_sent["coref_ids"] = [cluster[1]]
                        coref_span = [cluster[0][0] - start, cluster[0][1] - start]
                        if "coref_spans" in avo_sent:
                            if cluster[1] in avo_sent["coref_spans"]:
                                avo_sent["coref_spans"][cluster[1]].append(coref_span)
                            else:
                                avo_sent["coref_spans"][cluster[1]] = [coref_span]
                        else:
                            avo_sent["coref_spans"] = {cluster[1]: [coref_span]}
            else:
                pass


# demo
# coref = Coreference()
# INPUT_DOCUMENT = ('The legal pressures facing Michael Cohen are growing in a wide-ranging '
#                   'investigation of his personal business affairs and his work on behalf '
#                   'of his former client, President Trump. In addition to his work for Mr. '
#                   'Trump, he pursued his own business interests, including ventures in '
#                   'real estate, personal loans and investments in taxi medallions.')
# coref.connect(document=INPUT_DOCUMENT)
# coref.parse_data()
# output = coref.find_all_personal_ant()
