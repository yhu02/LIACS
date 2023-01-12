"""Module to configure the different keyword indicators."""
friedrich_conditional_indicators = [
    "if",
    "whether",
    "in case of",
    "in the case of",
    "in case",
    "for the case",
    # "whereas",
    "optionally",
]

empty_conditional_indicators = [
    "otherwise",
]

ferreira_conditional_indicators = [
    # 'if not',
    # 'or',
    # 'either',
    # 'only',
    # 'till',
    # 'until',
    # 'unless',
    # 'when'
    # 'only if'
]

self_conditional_indicators = []

friedrich_parallel_indicators = [
    "while",
    "meanwhile",
    "in parallel",
    "concurrently",
    "meantime",
    # 'in the meantime'
]

ferreira_parallel_indicators = [
    # 'in parallel with this',
    # 'in addition to',
    "simultaneously",
    # 'at the same time',
    # 'whereas'
]

self_parallel_indicators = []

friedrich_sequence_indicators = [
    "then",
    "after",
    "afterward",
    "afterwards",
    "subsequently",
    # 'based on this',
    "thus",
]

ferreira_sequence_indicators = [
    # todo - look at teh others
]

termination_indicators = [
    "cancel",
    "finish",
]

self_sequence_indicators = []

conditional_indicators = (
    friedrich_conditional_indicators
    + ferreira_conditional_indicators
    + self_conditional_indicators
    + empty_conditional_indicators
)
parallel_indicators = (
    friedrich_parallel_indicators
    + ferreira_parallel_indicators
    + self_parallel_indicators
)
sequence_indicators = (
    friedrich_sequence_indicators
    + ferreira_sequence_indicators
    + self_sequence_indicators
)
