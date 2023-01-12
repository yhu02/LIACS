from django.test import TestCase
from rules.processors.ValidatorProcessor import get_validator_function_reference, get_validator_function_definition


class ValidatorsTestCase(TestCase):
    def test_reference_exists(self):
        reference = get_validator_function_reference(1)
        self.assertEqual(reference, "rule_2", "Function reference is not correct.")

    def test_function_exists(self):
        function = get_validator_function_definition(2, "")
        self.assertEqual(function.strip(), "def rule_2(value):", "Function definition is not correct.")
