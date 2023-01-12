# from booknlp.booknlp import BookNLP
import os
import pandas as pd
import csv
import numpy as np
from nltk.tokenize import sent_tokenize
import uuid


class Pipeline:
    # def __init__(self):
    # Attach modules
    # self.bucketing = Bucketizer(self.tokens, self.get_saved_text)

    def generate_text_data(self, input_text):
        # This system works on plaintext files, create a plaintext file from input text
        sentences = sent_tokenize(input_text)

        # Write all sentences to txt file
        file_id = uuid.uuid4()
        input_file = f'input/{file_id}.txt'
        with open(input_file, 'w') as f:
            for line in sentences:
                f.write('%s\n' % line)

        # Initialize BookNLP
        booknlp = BookNLP(
            'en',
            {
                'pipeline': 'entity,quote,supersense,event,coref',
                'model': 'small',
            },
        )

        # Process source text
        output_dir = f'output/{file_id}'
        booknlp.process(input_file, output_dir, file_id)

        # Aggregate all process data tables into one
        tokens = pd.read_csv(f'{output_dir}/{file_id}.tokens', sep='\t', quoting=csv.QUOTE_NONE)
        tokens = self.get_supersenses(tokens, output_dir, file_id)
        tokens = self.get_detected_entities(tokens, output_dir, file_id)

        return {
            'tokens': tokens,
            'entities': self.entities,
        }

    def get_supersenses(self, tokens, output_dir, file_id):
        # Get supersense table
        supersenses = pd.read_csv(f'{output_dir}/{file_id}.supersense', sep='\t')

        # Merge supersenses with main token table
        tokens['supersense_category'] = ''

        # Loop over supersense df
        for index, row in supersenses.iterrows():
            # Add supersense to all rows in range of starting and ending tokens
            for token in range(row.start_token, row.end_token + 1):
                tokens.loc[tokens.token_ID_within_document == token, 'supersense_category'] = row['supersense_category']

        return tokens

    def get_detected_entities(self, tokens, output_dir, file_id):
        # Get entities table
        entities = pd.read_csv(f'{output_dir}/{file_id}.entities', sep='\t')

        # Merge entities with main token table
        tokens['entity'] = np.nan
        tokens['entity_type'] = ''
        tokens['entity_category'] = ''

        # Loop over entity df
        for index, row in entities.iterrows():
            # Add entity data to all rows in range of starting and ending tokens
            for token in range(row.start_token, row.end_token + 1):
                tokens.loc[tokens.token_ID_within_document == token, 'entity'] = row.COREF
                tokens.loc[tokens.token_ID_within_document == token, 'entity_type'] = row.prop
                tokens.loc[tokens.token_ID_within_document == token, 'entity_category'] = row['cat']

        # Only return the entities as a dictionary
        self.entities = {row['COREF']: row['text'] for index, row in entities.drop_duplicates('COREF').iterrows()}

        return tokens
