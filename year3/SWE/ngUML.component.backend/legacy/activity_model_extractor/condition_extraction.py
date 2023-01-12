"""Module to extract conditions and actions from text."""
import nltk
from tqdm import tqdm
from .configuration.indicators import (
    conditional_indicators as cond_ind,
    empty_conditional_indicators,
)
from .tools.text_support import TextSupport
from .allen_nlp.semantic_role_labelling import (
    SemanticRoleLabelling,
)


class ConditionExtraction:
    """ConditionExtraction Class to organise the extraction of conditions and actions."""

    def __init__(self) -> None:
        pass

    def get_sentences_from_srl_tag(self, tag, output):
        """Get all sentences form SRL output that contain a specific tag in them.

        Args:
            - tag (str): tag we are looking for. For example 'B-ARGM-ADV'
            - output (list(dict)): containing all the results from the SRL

        Returns:
            - result (list(string)): List of all sentences found in output that have the given tag.

        """
        result = []
        for sen in output:
            for res in sen["verbs"]:
                if tag in res["tags"]:
                    result.append(res["description"])
        return result

    def get_sentences_from_srl_tags(self, tags, output):
        """Get all sentences from SRL output that contain a specific tag in them.

        Args:
            - tags (list(str)): list of tags we are looking for. For example ['B-ARGM-ADV','B-V']
            - output (list(dict)): containing all the results from the SRL for a text

        Returns:
            - result (list(string)): List of all sentences found in output that have the given tag.
        """
        result = []
        for s_index, sen in enumerate(output):
            sent_res = {}
            sent_res["words"] = sen["words"]
            sent_res["verbs"] = []
            for res in sen["verbs"]:
                tags_in_sentence = [tag for tag in tags if tag in res["tags"]]
                if tags_in_sentence:
                    item = {}
                    item["description"] = res["description"]
                    item["tags"] = res["tags"]
                    item["foundTags"] = tags_in_sentence
                    item["s_index_text"] = s_index
                    sent_res["verbs"].append(item)
            # check if sen_res['verbs'] not an empty list.
            if sent_res["verbs"]:
                result.append(sent_res)
        return result

    def get_sents_with_tag_for_texts(self, tag, outputs):
        """Get text for each type of tag

        Args:
            - tag (str): tag wer are looking for. For example 'B-ARGM-ADV'
            - outputs (list(list(dict))): list of all outputs for multiple texts

        Returns:
            - results (list(list(str))): List of a list per text of found sentences.

        """
        sents = []
        for output in outputs:
            sents.append(self.get_sentences_from_srl_tag(tag, output))

        return sents

    def get_sents_with_tags_for_texts(self, tags, outputs):
        """Get text for each type of tag

        Args:
            - tags list((str)): tags we are looking for. For example 'B-ARGM-ADV'
            - outputs (list(list(dict))): list of all outputs for multiple texts

        Returns:
            - results (list(list(str))): A list per text with a list of found sentences.

        """
        sents = []
        for output in outputs:
            sents.append(self.get_sentences_from_srl_tags(tags, output))

        return sents

    def print_text_sents_descriptions(self, texts):
        """Print to sentence within a list of texts with each sentences.

        Args:
            - texts (list(list(str))): list of texts with a list of sentences.
        """
        for text in texts:
            print("New text")
            for sent in text:
                print(sent)

    def print_text_sents_data(self, texts):
        """Print to sentence within a list of texts with each sentences.

        Args:
            - texts (list(list(str))): list of texts with a list of sentences.
        """
        for text in texts:
            print("New text - Description - found tag")
            for sent in text:
                for item in sent["verbs"]:
                    print(item["description"])
                    print(item["foundTags"])

    def extract_data_based_on_tag(
        self, extr_tag, s_tags, s_words, s_index, s_index_text
    ):
        """extract data from a sentence item based on a tag.

        Args:
            - extr_tag (str): the tag searching for in the sentence. e.g. 'B-ADV-MOD'
            - s_tags: tags within a sentence
            - s_words: words of a sentence
            - s_index: index of the sentence in the set of found sentences with the tag
            - s_index_text: index of the sentence in the text

        Return:
            - Data about a sentence.
        """
        search_tag = extr_tag[2:]
        part_sent = {
            "senFoundTag": [],
            "senFoundTagIndex": [],
            "senOtherTags": [],
            "senOtherTagsIndex": [],
            "foundTag": "",
        }
        for index, tag in enumerate(s_tags):
            if search_tag in tag:
                part_sent["senFoundTag"].append(s_words[index])
                part_sent["senFoundTagIndex"].append(index)
            if tag != "O":
                part_sent["senOtherTags"].append(s_words[index])
                part_sent["senOtherTagsIndex"].append(index)
        part_sent["foundTag"] = search_tag
        part_sent["senIndex"] = s_index
        part_sent["s_index_text"] = s_index_text

        return part_sent

    def transform_data_for_sentences(self, sentences):
        """Transform the found data for sentences.

        Args:
            - sentences (list(data)): List with data from the SRL module with found tags,
                based on the specified tag for a specific sentence.

        Returns:
            - sentence_data (list(data)): A list with data for each sentence for conditions.
        """
        sentence_data = []
        for sen_ind, sentence in enumerate(sentences):
            sen_data = []
            for sen in sentence["verbs"]:
                if len(sen["foundTags"]) > 1:
                    # multiple tags in the same annotated sentence.
                    for tag in sen["foundTags"]:
                        found_data = self.extract_data_based_on_tag(
                            tag,
                            sen["tags"],
                            sentence["words"],
                            sen_ind,
                            sen["s_index_text"],
                        )
                        sen_data.append(found_data)
                else:
                    # normal case
                    found_data = self.extract_data_based_on_tag(
                        sen["foundTags"][0],
                        sen["tags"],
                        sentence["words"],
                        sen_ind,
                        sen["s_index_text"],
                    )
                    sen_data.append(found_data)
            sentence_data.append(sen_data)
        return sentence_data

    def select_sentences_with_condition_keywords(self, text: str) -> dict:
        """Select the sentence wich have conditional keywords in them.

        Args:
            - text (str): the text which is processed.

        Returns:
            - sentences_data (dict): indexes and found key per sentence, e.g. {sentence_ids: [0,1],
                sen_id_data: {'0': [tag0,tag1]}}
        """
        output = {"sen_ids": [], "sen_id_data": {}}
        sentence_list = nltk.tokenize.sent_tokenize(text)
        sentence_list = [sen.lower() for sen in sentence_list]
        for index, sen in enumerate(sentence_list):
            discovered_indicators = [word for word in cond_ind if word in sen]
            if discovered_indicators:
                output["sen_ids"].append(index)
                output["sen_id_data"][index] = discovered_indicators
        return output

    def mark_sentences_with_condition_keywords(self, sentences_data, words_data):
        """Mark all the sentences if the found tag is also a condition keyword.

        Args:
            - sentences_data (list(data)): a list of data that is generated in
                transform_data_for_sentences

        Returns:
            nothing - the data is modified in the current list.
        """
        for sent_data in sentences_data:
            for item in sent_data:
                found_tag = " ".join(item["senFoundTag"])
                found_tag = found_tag.strip().lower()
                discovered_tags = [tag for tag in cond_ind if tag in found_tag]
                if discovered_tags:
                    item["cond_key"] = True
                    item["condTag"] = discovered_tags
                    # get discovered tag indices
                    discovered_tags_index = []
                    words = [x.lower() for x in words_data[item["senIndex"]]["words"]]
                    for discovered_tag in discovered_tags:
                        index = []
                        discovered_tag = discovered_tag.split(" ")
                        for word in discovered_tag:
                            index.append(words.index(word.lower()))
                        discovered_tags_index.append(index)
                        item["condIndexes"] = discovered_tags_index
                else:
                    item["cond_key"] = False

    def return_condition_action_from_data(
        self, possible_condition, possible_action, sentence
    ):
        """Return condition and action from data.

        Description:
            Extract the condition and action from the data. The condition
            and action can be the same, which means the data is from the
            same semantic role label. If they are different the data is
            spread over 1 or more semantic roles.

        Args:
            - possible_condition (dict): data containing the possible condition
            - possible_action (dict): data containing the possible action
            - sentence (list(str)): the list containing the sentence

        Returns:
            A list item with a condition and action.
        """
        sentence_index = possible_condition["senIndex"]
        s_index_text = possible_condition["s_index_text"]
        if len(possible_condition["condIndexes"]) > 1:
            # Currently only one condition in a sentence is implemented
            #   -> see the [0] key on the ['condIndexes'][0]
            print(
                "There are multiple condition indexes in this sentence, which is not implemented"
            )
        condition_index = [
            x
            for x in possible_condition["senOtherTagsIndex"]
            if x not in possible_condition["senFoundTagIndex"]
        ]
        if possible_condition == possible_action:
            # if the condition is the same as the action we check in the
            #   same sentence instead of another.
            condition_index = [
                x
                for x in possible_condition["senFoundTagIndex"]
                if x not in possible_condition["condIndexes"][0]
            ]
        condition = [x for ix, x in enumerate(sentence) if ix in condition_index]
        cond = {
            "condition": condition,
            "conditionIndex": condition_index,
            "senIndex": sentence_index,
            "s_index_text": s_index_text,
        }
        # extract action
        action_index = [
            x
            for x in possible_action["senOtherTagsIndex"]
            if x not in possible_action["senFoundTagIndex"]
        ]
        action = [x for ix, x in enumerate(sentence) if ix in action_index]
        act = {
            "action": action,
            "actionIndex": action_index,
            "senIndex": sentence_index,
            "s_index_text": s_index_text,
        }
        return [cond, act]

    def extract_condition_action_from_data(self, condition_data, sentence_data):
        """Extract the condition and action from data.

        Args:
            - condition_data (dict): data representing the found information
                about the sentences - focussed on conditions
            - sentence_data (dict): data respresenting the data from SRL results for a text.

        Returns:
            A list of conditions and actions with their indexes.

        Todo if in one action there are multiple discovered tags.
        """
        conditions_actions = []
        for sent in condition_data:
            if len(sent) > 1:
                # do something with the sentence - got condition and action.
                # TODO fix this one to work for multiple
                if sent[0]["cond_key"]:
                    # found a conditional key in the text so we continue
                    possible_condition = []
                    possible_action = []
                    if len(sent) < 3:
                        # Longest found tags is the action.
                        if sent[1]["senOtherTags"] > sent[0]["senOtherTags"]:
                            possible_condition = sent[0]
                            possible_action = sent[1]
                        else:
                            possible_condition = sent[1]
                            possible_action = sent[0]
                        sentence_index = sent[0]["senIndex"]
                        condition_action = self.return_condition_action_from_data(
                            possible_condition,
                            possible_action,
                            sentence_data[sentence_index]["words"],
                        )
                        conditions_actions.append(condition_action)
                    else:
                        print("sent is not considered as it is too long")
                        print(sent[0]["senFoundTag"])
            else:
                if sent[0]["cond_key"]:
                    # found a condition
                    item = sent[0]
                    sentence_index = item["senIndex"]
                    condition_action = self.return_condition_action_from_data(
                        item, item, sentence_data[sentence_index]["words"]
                    )
                    conditions_actions.append(condition_action)
        return conditions_actions


# Interface / Demonstration part - and helper functions.
class ConditionExtractionInterface:
    """Interface for the condition extraction class."""

    def __init__(self) -> None:
        pass

    def semrol_text(self, text):
        """Semantic Role Labelling for a text."""
        srl = SemanticRoleLabelling()
        data = srl.create_input_object(text)
        if not srl.connect(data):
            return []
        return srl.result["output"]

    def get_text_from_folder(self, folder):
        """Get the text files into the memory based on a folder.

        Args:
            - folder (str): a folder with the text files that need to be loaded.

        Returns:
            - texts list(str): a list of all the texts (str) presented in the folder.
        """
        txt_s = TextSupport()
        print("Get all texts from the examples folder.")
        texts = txt_s.get_all_texts_activity(folder)
        # texts = txt_s.get_all_texts_activity('test-data')
        return texts

    def create_single_text_input(self, text_string):
        """Creates a list of texts based on a single string of text."""
        return [text_string]

    def srl_for_all_texts(self, texts):
        """Do semantic role labelling for each text in texts

        Args:
            - texts list(str): text for each given text file.

        Returns:
            - results list(list(str)): a list for each text containing a list of results
                per sentence from the text with SRL data.
        """
        results = []
        print("SRL for each text.")
        for text in tqdm(texts):
            sem_rol_output = self.semrol_text(text)
            results.append(sem_rol_output)
        return results

    def condition_extraction_for_texts(self, results):
        """Do condition extraction for the SRL results from several texts.

        Args:
            - results (list(list(dict))): SRL result for each text given
                as text and a dict of verbs and tags.

        Returns
            - condActions (list(list(dict))): a list with all the found
                condition, action combinations for each text.
        """
        cond_extract = ConditionExtraction()
        print("Extract tags for each text.")
        sents = cond_extract.get_sents_with_tags_for_texts(
            ["B-ARGM-ADV", "B-ARGM-TMP"], results
        )
        cond_extract.print_text_sents_descriptions(sents)
        print("Transform data for sentences based on tags")
        s_data = []
        for text in tqdm(sents):
            res = cond_extract.transform_data_for_sentences(text)
            cond_extract.mark_sentences_with_condition_keywords(res, text)
            s_data.append(res)

        condition_actions = []
        for index, text in enumerate(s_data):
            print(text)
            res = cond_extract.extract_condition_action_from_data(text, sents[index])
            condition_actions.append(res)
        return condition_actions

    def select_sentences_with_condition_keywords_for_texts(self, texts: list) -> list:
        """Select sentences with condition keywords for several texts."""
        cond_extraction = ConditionExtraction()
        results = []
        # sents = cond_extraction.get_sents_with_conditional_indicators(results)
        for text_item in texts:
            results.append(
                cond_extraction.select_sentences_with_condition_keywords(text_item)
            )
        return results

    def print_condition_actions(self, condition_actions):
        """Print the conditions and actions given in the condition_actions.

        Args:
            - list(list(dict)) : a list of texts with sentences that have a
                dict about each condition and action combination.
        """
        # ix = 0
        for text in condition_actions:
            for sent in text:
                print(
                    "c: {} \t a: {}".format(
                        " ".join(sent[0]["condition"]), " ".join(sent[1]["action"])
                    )
                )


def print_sent_and_tags_analysis(condition_results, results):
    """Print sentences with conditions and all SRL tags for easy analysis and copying to excel.

    Args:
        - condition_results (list(dict)): data on which sentences have conditional
            indicators. e.g. {'sen_ids': [6, 7, 10], 'sen_id_data': {6: ['if'],
            7: ['if'], 10: ['if']}}
        - results (list(dict)): Result data from the SRL result.
    Returns:
        - None
    """
    for index, text_item in enumerate(condition_results):
        for sent_id in text_item["sen_ids"]:
            print("\n")
            print(" ".join(results[index][sent_id]["words"]))
            for result in results[index][sent_id]["verbs"]:
                print(" ".join(result["tags"]))


def check_for_adverbial(cond_index_list, sent_index, result):
    """Check if there is an adverbial in the sentence on the condition index."""
    cond_index = cond_index_list[1]
    cond_tags = [x["tags"][cond_index] for x in result[sent_index]["verbs"]]
    cond_adv_sents_index = [i for i, x in enumerate(cond_tags) if x == "B-ARGM-ADV"]
    if not cond_adv_sents_index:
        return []
    return cond_adv_sents_index


def get_adverbial(cond_index_list, sent_index, result):
    """Retrieve an adverbial SRL if it is in the same index as the condition.

    Args:
        - cond_index_list [(int),(int)]: index of the condition in the sentence, where
            the first is the begin and second is the end.
        - sent_index (int): index of the sentence in the text.
        - result (list(dict(list(dict)))): a list of the SRL result, that will be searched.

    Returns:
        - condition [(int),(int),(int),(int),(int),(int)]: list of integers: sentence
            index, adverbial sentence index, conditional index, index of the end of condition,
            conditional indicator index begin and conditional indicator end.
    """
    cond_adv_sents_index = check_for_adverbial(cond_index_list, sent_index, result)
    cond_index = cond_index_list[1]
    if cond_adv_sents_index:
        # search further
        tags = [
            [t for t in result[sent_index]["verbs"][i]["tags"] if t == "I-ARGM-ADV"]
            for i in cond_adv_sents_index
        ]
        best_tags_list = max(tags)
        adv_sent_index = cond_adv_sents_index[tags.index(best_tags_list)]
        condition_sent = " ".join(
            result[sent_index]["words"][
                cond_index : cond_index + len(best_tags_list) + 1
            ]
        )
        description = result[sent_index]["verbs"][adv_sent_index]["description"]
        condition = [
            sent_index,
            adv_sent_index,
            cond_index + 1,
            cond_index + len(best_tags_list),
            cond_index_list[0],
            cond_index_list[1],
        ]
        return condition
    else:
        return []


def get_condition_SRL_after_indicator(cond_index_list, sent_index, result):
    """Retrieve an conditional SRL that follows after the condition index.

    Args:
        - cond_index_list [(int),(int)]: index of the condition in the sentence,
            where the first is the begin and second is the end.
        - sent_index (int): index of the sentence in the text.
        - result (list(dict(list(dict)))): a list of the SRL result, that will be searched.

    Returns:
        - condition [(int),(int),(int),(int),(int),(int)]: list of integers: sentence index,
            condition sentence index, conditional index and the index of the end of condition,
            conditional indicator index begin and conditional indicator end.
    """
    cond_index = cond_index_list[1]
    if len(result[sent_index]["verbs"]) < 1:
        print(
            f"In get_condition_SRL_after_indicator there was no verb set found in result."
            + f" sent_index: {sent_index}"
        )
        return []
    if cond_index + 1 <= len(result[sent_index]["verbs"][0]["tags"]):
        # not going out of bounds
        tags_next_cond = [
            x["tags"][cond_index + 1] for x in result[sent_index]["verbs"]
        ]
        # Get the ones with a B-ARG
        cond_next_sents_index = [
            i for i, x in enumerate(tags_next_cond) if "B-ARG" in x
        ]
        if not cond_next_sents_index:
            print(
                (
                    "Couldn't find a argument next to a conditional indicator in "
                    + "sen: {}, cond_ix: {}"
                ).format(sent_index, cond_index)
            )
            print("Sent: {}".format(result[sent_index]["verbs"][0]["description"]))
            # Try the items next to it.
            if cond_index + 2 <= len(result[sent_index]["verbs"][0]["tags"]):
                tags_next_cond = [
                    x["tags"][cond_index + 2] for x in result[sent_index]["verbs"]
                ]
                # Get the ones with a B-ARG
                cond_next_sents_index = [
                    i for i, x in enumerate(tags_next_cond) if "B-ARG" in x
                ]
            # return []
        # Find the longest and the last index of the SRL
        end_index = []
        for next_sen_index in cond_next_sents_index:
            counter = 0
            for index, item in enumerate(
                result[sent_index]["verbs"][next_sen_index]["tags"]
            ):
                if item != "O":
                    counter = index
            end_index.append(counter)
        if not end_index:
            # We could not find a better solution, so return nothing.
            return []
        longest_index = max(end_index)
        srl_res_index = cond_next_sents_index[end_index.index(longest_index)]
        return [
            sent_index,
            srl_res_index,
            cond_index + 1,
            longest_index,
            cond_index_list[0],
            cond_index_list[1],
        ]
    else:
        # The condition was at the end of the result sentences
        return []


# test = get_condition_SRL_after_indicator(0,6,result)


def get_srl_within_range(srl_index, search_index, after, srl_result_verbs):
    """Get SRL tags from a specified range and return the range from where it starts and ends.

    Args:
        - srl_index (int): index of the result within the SRL results.
        - search_index (int): index of where the search should begin or end.
        - after (bool): states of the search should start after the search_index if True,
            else it will start before.
        - srl_result_verbs (list(dict)): srl result starting with the verbs: e.g.
            result[10]['verbs']

    Returns:
        - [first_index,end_index] (list(int,int)): first_index indicates the start of a SRL
            set and end_index the end.
    """
    # advSent = adverbial_data[1]
    # endCond = adverbial_data[3]
    # Check for action from point
    first_index = -1
    end_index = -1
    search_item = []
    if after:
        search_item = srl_result_verbs[srl_index]["tags"][search_index + 1 :]
    else:
        search_item = srl_result_verbs[srl_index]["tags"][:search_index]
    # result[sent_index]['verbs'][srl_index]['tags'][endCond+1:]
    for index, item in enumerate(search_item):
        if item != "O":
            if first_index == -1:
                first_index = index + search_index + 1 if after else index
            end_index = index
    if end_index != -1:
        end_index = end_index + search_index + 1 if after else end_index
    return [first_index, end_index]


def get_action_srl_results(begin_arg_sents, end_condition, result_verbs):
    """Get the actions, using SRL and the last index of a condition.

    Args:
        - begin_arg_sents (list(list(int,int)): sentences with B-ARG in them, it contains
            the sentence index and SRL tag: [index, SRLTag]
        - end_condition (int): index of the end of the condition.
        - result_verbs (list(dict(list)): result for this specific sentence with the verbs.
            result[sent_index]['verbs']

    Returns:
        - action (list(int,int)): a list with two integers, begin index and end index of
            the action. e.g. [beginIndex,endIndex]
    """
    action_results = []
    for item in begin_arg_sents:
        action_results.append(
            get_srl_within_range(item[0], end_condition, True, result_verbs)
        )
    compare_lengths = [sum(x) for x in action_results]
    item_index = compare_lengths.index(max(compare_lengths))
    action = action_results[item_index]
    return action


def orchestration_condition_sentence(
    sent_index: int, sen_cond_data: list, result: list
) -> list:
    """Combine the different functionts for condition extraction.

    Args:
        - sent_index (int): The index of the sentence we are considering as part of the text.
        - sen_cond_data (list): element containing the found condition indicators for the sentence.
        - result (list(dict(list(dict)))): a list of the SRL result, that will be searched.

    Returns:
        - [condition,action] (list):
            - condition [(int),(int),(int),(int),(int),(int)]: list of integers: sentence
                    index, result sentence index, conditional index, index of the end of condition,
                    conditional indicator index begin and conditional indicator end.
            - action [(int),(int),(int),(int)]: list of integers: sentence index, result
                    sentence index, action begin index, index of the end of action.

    Notes (TODO):
        - what if we have multiple conditions? - for example two if statements?
            -> currently making this part -> we return everything.
        - We assume that the condition with an action following is the most important
    """
    condition_action_results = []
    # loop through the cond index
    for conditional_indicator in sen_cond_data:
        # If we have a conditional indicator with multiple words.
        cond_list = conditional_indicator.split(" ")
        cond_end_index = len(cond_list) - 1

        sent_words = [x.lower() for x in result[sent_index]["words"]]
        try:
            sen_begin_index = sent_words.index(cond_list[0])
            sen_end_index = sent_words.index(cond_list[cond_end_index])
        except Exception as _:
            print(
                (
                    "Could not find condition in sentence, while it should be there. "
                    + " sen_index: {}"
                ).format(sent_index)
            )
            sen_begin_index = -1
            sen_end_index = -1
        if sen_begin_index == -1 or sen_end_index == -1:
            return condition_action_results
        cond_index_list = [sen_begin_index, sen_end_index]
        # Check for adverbial
        adverbial_data = get_adverbial(cond_index_list, sent_index, result)
        action_in_front = False
        if adverbial_data:
            adv_sent = adverbial_data[1]
            begin_cond = adverbial_data[2]
            end_cond = adverbial_data[3]
            # Check for action from point
            adv_action_data = get_srl_within_range(
                adv_sent, end_cond, True, result[sent_index]["verbs"]
            )
            if adv_action_data[0] == -1:
                # Check for action in front of index
                cond_length = cond_index_list[1] - cond_index_list[0] + 1
                adv_action_data = get_srl_within_range(
                    adv_sent,
                    begin_cond - cond_length,
                    False,
                    result[sent_index]["verbs"],
                )
                if adv_action_data:
                    action_in_front = True
            if conditional_indicator.lower() in empty_conditional_indicators:
                # we tag the condition with the action, to make sure it
                # intersects and we skip the condition as it is empty.
                action_begin_index = adv_action_data[0]
                if end_cond < action_begin_index:
                    # if the next part is the start of the condition
                    if end_cond + 1 == action_begin_index:
                        # we add one to the end of the conditional indicator.
                        adverbial_data[3] += 1
                        adverbial_data.append(
                            {"conditional_keyword": conditional_indicator.lower()}
                        )
            action = [sent_index, adv_sent, adv_action_data[0], adv_action_data[1]]
            if action_in_front:
                action.append({"action_found_in_front": True})
            condition_action_results.append([adverbial_data, action])
        # If not adverbial check for SRL condition
        else:
            srl_data = get_condition_SRL_after_indicator(
                cond_index_list, sent_index, result
            )
            if srl_data:
                # Check for action from end of condition
                srl_sent = srl_data[1]
                begin_cond = srl_data[2]
                end_cond = srl_data[3]
                # select possible sentences
                conditional_keyword = " ".join(
                    result[sent_index]["words"][srl_data[4] : srl_data[5] + 1]
                )
                if conditional_keyword in empty_conditional_indicators:
                    # We have an empty conditional indicator, so we move the found
                    #   condition to the action.
                    action = [sent_index, srl_sent, begin_cond, end_cond]
                    # replace the condition with something empty, such that we don't get a text
                    srl_data[2] = srl_data[4]
                    # subtracting one to create an empty list when building the guard.
                    srl_data[3] = srl_data[5]
                    srl_data.append(
                        {"conditional_keyword": conditional_indicator.lower()}
                    )

                else:
                    # consider the one after the end and + 1 if there is a srl result
                    next_cond = [
                        x["tags"][end_cond + 1] for x in result[sent_index]["verbs"]
                    ]
                    begin_arg = [
                        [index, x] for index, x in enumerate(next_cond) if "B-ARG" in x
                    ]
                    if begin_arg:
                        action_result = get_action_srl_results(
                            begin_arg, end_cond, result[sent_index]["verbs"]
                        )
                        action = [
                            sent_index,
                            srl_sent,
                            action_result[0],
                            action_result[1],
                        ]
                    else:
                        if end_cond + 2 < len(result[sent_index]["words"]):
                            # make sure we don't go further than the index of the sentence
                            next_cond = [
                                x["tags"][end_cond + 2]
                                for x in result[sent_index]["verbs"]
                            ]
                            begin_arg = [
                                [index, x]
                                for index, x in enumerate(next_cond)
                                if "B-ARG" in x
                            ]
                            if begin_arg:
                                action_result = get_action_srl_results(
                                    begin_arg, end_cond, result[sent_index]["verbs"]
                                )
                                action = [
                                    sent_index,
                                    srl_sent,
                                    action_result[0],
                                    action_result[1],
                                ]
                            else:
                                action = []
                        else:
                            # Action not found
                            action = []
                if not action:
                    # action is empty
                    # so need to check in front of the conditional marker.
                    print(
                        "need to check in front of the conditional marker, not yet implemented."
                    )
                    action = []
                # Check for action in front of condition index
                condition_action_results.append([srl_data, action])
            elif conditional_indicator in empty_conditional_indicators:
                # Got a new sentence with an empty conditionional indicator.
                # without any SRL data, which is strange.
                # select the action
                print("Found no SRL data and an empty conditional indicator, check the SRL result for debugging.")
        # If there is another cond_index
    return condition_action_results


def extract_condition_action_data(texts: list, results: list) -> list:
    """Extract condition data for all the existing texts.

    Args:
        - texts (list(str)): list of texts to process.
        - results (list): list of all srl_results

    Returns:
        - condition_action_result (list): for each text a dictionary with
                sen_index as key and a condition, action pair
    """
    condition_action_result = []
    cond_extract_interface = ConditionExtractionInterface()
    sentences_with_condition_keywords = (
        cond_extract_interface.select_sentences_with_condition_keywords_for_texts(texts)
    )
    for index, text_condition_data in enumerate(sentences_with_condition_keywords):
        sentence_ids = text_condition_data["sen_ids"]
        keywords_per_sentence = text_condition_data["sen_id_data"]
        condition_action_per_sentence = {}
        for sent_id in sentence_ids:
            conditional_result = orchestration_condition_sentence(
                sent_id, keywords_per_sentence[sent_id], results[index]
            )
            condition_action_per_sentence[sent_id] = conditional_result
        condition_action_result.append(condition_action_per_sentence)
    return condition_action_result


def print_condition_action_data(condition_action_data, results):
    """Print the data created by the orchestration_condition_sentence function.

    Args:
        - condition_action_data (list(dict(list))): containing the index for the sentence,
            srlResult, start and end of a condition or action for each text.
        - results (list(list(dict))): containing all the SRL results for each text.

    Returns:
        - nothing.
    """
    for index, condition_action_result in enumerate(condition_action_data):
        sent_keys = condition_action_result.keys()
        for key in sent_keys:
            condition_action = condition_action_result[key]
            sent = results[index][key]["words"]
            if condition_action[0]:
                condition_data = condition_action[0]
                condition = " ".join(sent[condition_data[2] : condition_data[3] + 1])
                print("s{},c: {}".format(key, condition))
            if condition_action[1]:
                action_data = condition_action[1]
                action = " ".join(sent[action_data[2] : action_data[3] + 1])
                print("s{},a: {}".format(key, action))
