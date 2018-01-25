import nltk

class Analyzer():
    """Implements sentiment analysis."""
    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        positive_file = open(positives).readlines()[36:]
        negative_file = open(negatives).readlines()[36:]
        self.positive_words = [x.strip() for x in positive_file]
        self.negative_words = [x.strip() for x in negative_file]

        # TODO

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        for token in tokens:
            if token in self.positive_words:
                score += 1
            if token in self.negative_words:
                score -= 1
        # TODO
        return score
