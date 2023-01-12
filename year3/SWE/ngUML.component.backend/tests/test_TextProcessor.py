import unittest
import src.rules.processors.TextProcessor

class TestTextProcessor(unittest.TestCase):

    def test_wordToNumber(self):
        self.assertEqual(TextProcessor.wordToNumber("fifty"), "50")
        # self.assertEqual(TextProcessor.wordToNumber(-1, 1), 0)
        # self.assertEqual(TextProcessor.wordToNumber(-1, -1), -2)


if __name__ == '__main__':
    unittest.main()
