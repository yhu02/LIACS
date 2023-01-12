local = False
nlp_urls = {}
if local:
    nlp_urls = {
        "srl": "http://allen_nlp:5000/predict/srl",
        "coref": "http://allen_nlp:5000/predict/coref",
        "entail": "http://allen_nlp:5000/predict/entail",
    }
else:
    nlp_urls = {
        "srl": "https://allennlp.semax.nguml.com/predict/srl",
        "coref": "https://allennlp.semax.nguml.com/predict/coref",
        "entail": "https://allennlp.semax.nguml.com/predict/entail",
    }
