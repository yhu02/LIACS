'''
Usage:

    from classifier import clf
    clf.predict(test_data)
'''
from sklearn import svm
import pathlib
import joblib

FILENAME = 'clf.pkl'

try:
    CURRENT_PATH = str(pathlib.Path(__file__).parent.resolve()) + '/'
    clf = joblib.load(CURRENT_PATH + FILENAME)
except:
    import asl
    clf = svm.SVC(gamma=0.0001, C=50, probability=True)
    clf.fit(asl.data, asl.target)
    joblib.dump(clf,'clf.pkl')
