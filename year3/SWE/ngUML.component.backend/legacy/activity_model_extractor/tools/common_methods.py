import spacy
from nltk.tokenize import word_tokenize

spacy_nlp = spacy.load('en_core_web_sm')


class CommonFunctions:
   """Functions that are often used.

   Functions that are often used in different classes.
   So we have added them here to make reuse more easy.

   Attributes:
      None
   """
   def __init__(self) -> None:
      """Init the common functions class."""
      pass

   def get_list_sent_lengths(self, text:str) -> list:
      """Create list of lengths of words per sentence."""
      doc = spacy_nlp(text)
      sents = [str(sent) for sent in doc.sents]
      sent_words = [word_tokenize(sen) for sen in sents]
      last = 0
      sen_lens = []
      for words in sent_words:
         length = last + len(words)
         sen_lens.append(length)
         last = length
      return sen_lens
   
   def order_avo_on_sent_index(self,agent_verb_object_results:list) -> dict:
      """Order the agent_verb_object results per sentence index. To make it easy to access them."""
      avo_result_per_sent = {}
      for index, result in enumerate(agent_verb_object_results):
         sent_index = result['sent_index']
         result['avo_result_index'] = index
         if sent_index in avo_result_per_sent.keys():
            avo_result_per_sent[sent_index].append(result)
         else:
            avo_result_per_sent[sent_index] = [result]
      return avo_result_per_sent
   
   def find_sent_begin_text_index(self,avo_result_index:int,sentence_lengths:list) -> int:
      """Find the index for the begin of a sentence."""
      sen_begin_text_index = 0
      if avo_result_index > 0:
         sen_begin_text_index = sentence_lengths[avo_result_index-1]
      return sen_begin_text_index
   
   def write_to_status_code_file(self,text:str) -> None:
      """Write to the status code file."""
      try:
        status_file = open("./extractor/activity_model_extractor/last_action.txt","w",encoding="utf8")
      except OSError:
         print("file does not exist.")
         return
      status_file.write(text)
      status_file.close()
      return