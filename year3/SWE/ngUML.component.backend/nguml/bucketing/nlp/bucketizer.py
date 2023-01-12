class Bucketizer:
    def __init__(self, tokens):
        self.tokens = tokens

    def is_important_for_class(self, supersense_df):
        # Rule 1: there needs to be at least two nouns in the sentences
        if (
            supersense_df['fine_POS_tag'].tolist().count('NN')
            + supersense_df['fine_POS_tag'].tolist().count('NNP')
            + supersense_df['fine_POS_tag'].tolist().count('NNS')
            > 1
        ):
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['aux', 'auxpass', 'conj'])
                and 'verb.cognition' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if all(item in supersense_df['dependency_relation'].tolist() for item in ['pobj', 'prep', 'nsubj']) and all(
                item in supersense_df['supersense_category'].tolist() for item in ['verb.stative', 'noun.relation']
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['pobj', 'nummod'])
                and all(
                    item in supersense_df['supersense_category'].tolist() for item in ['verb.stative', 'noun.artifact']
                )
                and supersense_df['supersense_category'].tolist().count('verb.stative') > 1
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['nsubj', 'dobj', 'amod'])
                and 'verb.stative' in supersense_df['supersense_category'].tolist()
                and 'JJ' in supersense_df['fine_POS_tag'].tolist()
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['det', 'nsubj', 'dobj'])
                and 'verb.possession' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['det', 'nsubj', 'pobj', 'aux'])
                and 'verb.stative' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['det', 'nsubj', 'pobj', 'aux'])
                and 'verb.communication' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['det', 'nsubj', 'dobj', 'cc'])
                and 'verb.change' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if all(item in supersense_df['dependency_relation'].tolist() for item in ['det', 'nsubj', 'dobj']) and all(
                item in supersense_df['supersense_category'].tolist() for item in ['verb.perception', 'noun.artifact']
            ):
                return True
            if all(
                item in supersense_df['dependency_relation'].tolist()
                for item in ['prep', 'nsubjpass', 'auxpass', 'nummod', 'cc']
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['nsubjpass', 'auxpass'])
                and 'require' in supersense_df['lemma'].tolist()
            ):
                return True
            if (
                all(
                    item in supersense_df['dependency_relation'].tolist()
                    for item in ['nsubjpass', 'auxpass', 'det', 'aux']
                )
                and 'verb.change' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if (
                all(
                    item in supersense_df['dependency_relation'].tolist()
                    for item in ['det', 'nsubjpass', 'aux', 'auxpass', 'pobj']
                )
                and 'verb.contact' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['det', 'nsubj', 'pobj', 'prep'])
                and 'verb.stative' in supersense_df['supersense_category'].tolist()
            ):
                return True
            if (
                all(item in supersense_df['dependency_relation'].tolist() for item in ['pobj', 'prep'])
                and all(
                    item in supersense_df['supersense_category'].tolist() for item in ['verb.stative', 'noun.artifact']
                )
                and supersense_df['dependency_relation'].tolist().count('pobj') > 1
            ):
                return True
            if (
                all(
                    item in supersense_df['dependency_relation'].tolist() for item in ['det', 'nsubj', 'dobj', 'predet']
                )
                and 'verb.social' in supersense_df['supersense_category'].tolist()
            ):
                return True
            else:
                return False
        else:
            return False

    def is_important_for_activity(self, supersense_df):
        # Rule 1: there needs to be at least two nouns in the sentences
        if (
            supersense_df['fine_POS_tag'].tolist().count('NN')
            + supersense_df['fine_POS_tag'].tolist().count('NNP')
            + supersense_df['fine_POS_tag'].tolist().count('NNS')
            > 1
        ):
            if all(
                item in supersense_df['dependency_relation'].tolist() for item in ['nsubj', 'acl', 'agent', 'ccomp']
            ) and all(
                item in supersense_df['supersense_category'].tolist() for item in ['verb.communication', 'verb.stative']
            ):
                return True
            if (
                len(
                    supersense_df[
                        (supersense_df['lemma'].isin(['when', 'second', 'if', 'then', 'first']))
                        & (supersense_df['dependency_relation'] == 'advmod')
                    ]
                )
                > 0
            ):
                return True
            else:
                return False
        else:
            return False

    def is_important_for_usecase(self, supersense_df):
        # Rule 1: there needs to be at least two nouns in the sentences
        if (
            supersense_df['fine_POS_tag'].tolist().count('NN')
            + supersense_df['fine_POS_tag'].tolist().count('NNP')
            + supersense_df['fine_POS_tag'].tolist().count('NNS')
            > 1
        ):
            if (
                len(
                    supersense_df[
                        supersense_df['lemma'].isin(
                            ['system', 'facilitate', 'module', 'interface', 'functionality', 'capability']
                        )
                    ]
                )
                > 0
            ):
                return True
            if len(supersense_df[supersense_df['supersense_category'] == 'verb.contact']) > 0:
                return True
            if (
                len(
                    supersense_df[
                        (supersense_df['supersense_category'].isin(['verb.creation', 'verb.social']))
                        & (supersense_df['lemma'] == 'execute')
                    ]
                )
                > 0
            ):
                return True
            if (
                len(
                    supersense_df[
                        (supersense_df['supersense_category'].isin(['noun.animal', 'noun.person', 'noun.plant']))
                        & (supersense_df['dependency_relation'] == 'nsubj')
                    ]
                )
                > 0
            ):
                return True
            else:
                return False
        else:
            return False

    def apply_bucketing(self):
        print(self.tokens)
        focus_text_dfs = list(map(lambda x: x[1], self.tokens))
        print(focus_text_dfs)
        bucketed_data = {
            'class': list(filter(lambda x: self.is_important_for_class(x), focus_text_dfs)),
            'activity': list(filter(lambda x: self.is_important_for_activity(x), focus_text_dfs)),
            'usecase': list(filter(lambda x: self.is_important_for_usecase(x), focus_text_dfs)),
        }

        return {
            bucket: [' '.join(df['word'].values) for df in bucketed_data[bucket]] for bucket in bucketed_data.keys()
        }
