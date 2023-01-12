"""Module to use text support methods, such as reading a folder."""
import os
from nltk.tokenize import sent_tokenize


class TextSupport:
    """Class with several text related support methods.

    Attributes:
       None
    """

    def __init__(self) -> None:
        """Intialiase the text support class."""
        pass

    def get_text_from_file_activity(self, path):
        """Get activity text from a file."""
        with open(path) as f:
            lines = f.read()
        sents = sent_tokenize(lines)
        return sents

    def get_all_texts_activity(self, folder):
        """Get all texts and return a list of different texts that are concatenated."""
        # Get all the text files
        all_txt_files = [
            os.path.join(folder, f)
            for f in os.listdir(folder)
            if os.path.isfile(os.path.join(folder, f)) and f[-3:] == "txt"
        ]

        # Get all contents
        all_texts = [self.get_text_from_file_activity(f) for f in all_txt_files]

        text_in = []
        for text in all_texts:
            t = " ".join(text)
            text_in.append(t)

        return text_in
