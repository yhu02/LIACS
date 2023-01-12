"""Enter the Pipeline through the flask app to access and test different API methods."""
from flask import Flask, request
import numpy as np
from pipeline import run_latest_demo, test_condition_extraction, test_model_building
import tools.text_support as text_sup

app = Flask(__name__)


@app.route("/")
def index():
    """Return hello world as example."""
    return "hello Flask!"


@app.route("/run_test")
def run_test():
    """Run the latest demo for testing purposes."""
    text_support = text_sup.TextSupport()
    input_texts = text_support.get_all_texts_activity("test-data")
    test_text = input_texts[4]
    avo_sents = run_latest_demo("order_coref_test", test_text, post_model=False)
    return "Finished correctly"


@app.route("/run_condition_test")
def run_condition_test():
    """Run the condition process for testing."""
    text_support = text_sup.TextSupport()
    input_texts = text_support.get_all_texts_activity("test-data")
    test_text = input_texts[3]
    res = test_condition_extraction(test_text)
    condition_res = res[0]
    avo_sents = res[1]
    return "Finished correctly"


@app.route("/run_model_build_test", methods=["POST"])
def run_model_build_test():
    """Run a model test, with a saved set of avo_sents."""
    data = request.get_json()
    file_name = data["file_name"]
    model_name = data["model_name"]
    avo_sents = np.load(file_name, allow_pickle="TRUE")
    test_model_building(model_name, avo_sents)
    return "Finished correctly"


@app.route("/run_model_and_post", methods=["POST"])
def run_model_and_post():
    """Run the whole model and post to backend and overwrite current file."""
    data = request.get_json()
    text_id = int(data["text_id"])
    name = data["name"]
    text_support = text_sup.TextSupport()
    input_texts = text_support.get_all_texts_activity("test-data")
    test_text = input_texts[text_id]
    avo_sents = run_latest_demo(name, test_text, True)
    np.save(name + ".npy", avo_sents)
    return "Finished correctly"


@app.route("/run_model_and_post_text", methods=["POST"])
def run_model_and_post_with_text():
    """Run the whole model and post to backend, with a given text."""
    data = request.get_json()
    model_name = data["model_name"]
    text = data["text"]
    avo_sents = run_latest_demo(model_name, text, True)
    np.save(model_name + ".npy", avo_sents)
    return "Finished correctly"


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
