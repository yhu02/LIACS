from django.shortcuts import render

# Create your views here.
from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
from sign.hand_data import get_hand_position
from sign.lib import Leap
from sign.classifier import clf
from django.views.decorators.csrf import csrf_exempt
from rest_framework.decorators import action
from rest_framework.decorators import api_view
from rest_framework import viewsets
import random
import pickle
import random
import redis
import json
import base64
import pathlib
import os
 
# Get the list of all files and directories
CURRENT_PATH = str(pathlib.Path(__file__).parent.resolve()) + '/'
TOPIC_PATH = CURRENT_PATH + '../static/topic/'

controller = Leap.Controller()
controller.set_policy(Leap.Controller.POLICY_BACKGROUND_FRAMES)

past_symbol = 'a'
prev_prediction = None
r = redis.StrictRedis(host='localhost', port=6379, db=0)

class SignViewSet(viewsets.ReadOnlyModelViewSet):

    def translate(request):
        return render(request, 'ui.html')

    def home(request):
        return render(request, 'home.html')

    def about(request):
        return render(request, 'about.html')

    def application(request):
        return render(request, 'application.html')

    @csrf_exempt
    def add_score(request):
        body_unicode = request.body.decode('utf-8')
        body = json.loads(body_unicode)

        try:
            record = json.dumps({'user': body['user'], 'score': int(body['score'])})
            print(record)
            result = r.lpush('scoreboard', record)
            return JsonResponse({"error":result})
        except KeyError:
            return JsonResponse({"error":True})

    def get_scores(request):
        scores = [json.loads(i) for i in r.lrange('scoreboard', 0, 100)]
        scores.sort(key=lambda s: s['score'], reverse=True)
        return JsonResponse({"scores":scores[:10]})

    def current(request):
        global past_symbol
        global prev_prediction

        # Is there a hand?
        hand_pos = get_hand_position(controller)
        if not hand_pos:
            new = past_symbol != ' '
            past_symbol = ' '
            
            thisdict = {
                "new": new,
                "symbol": " "
            }
            return JsonResponse(thisdict)
        features = [[v for k, v in hand_pos.items()]]
        #features = [[0]*60 for i in range(2)]
        # Do we have a new symbol?
        prediction = ''.join(clf.predict(features))
        print(prediction, ',', prev_prediction)
        if prediction == prev_prediction:
            # We good fam
            thisdict = {
                "new": False,
                "symbol": prediction
            }
            return JsonResponse(thisdict)
        else:
            prev_prediction = prediction
            thisdict = {
                "new": True,
                "symbol": prediction
            }
            return JsonResponse(thisdict)

    def splash(request):
        return render(request, 'splash.html')

    def themes(request):
        return render(request, 'themes.html')

    def scoreboard(request):
        return JsonResponse({"user_score":100})

    @csrf_exempt
    @api_view(["POST"])
    def quiz_data(request):
        data = dict(request.data)
        #print("Files and directories in '", CURRENT_PATH, "' :")
        # prints all files
        #print(topic_list)
        topics = data['topic[]']
        n_answers = int(data['n_answers'][0])
        n = 50

        list2 = []
        for i in topics:
            files = os.listdir(TOPIC_PATH + i)
            questions = []
            for j in range(n):
                questions.append(random.sample(files, 1)[0])
            for j in questions:
                dict2 = {}
                files = os.listdir(TOPIC_PATH + i)
                #pick 5 answers from same topic
                answers = random.sample(files, n_answers) 
                #answer may contain question so maybe remove later
                if not j in answers: 
                    answers[0] = j

                random.shuffle(answers)
                dict2['topic'] = i
                dict2['question'] = j
                l = []
                for k in answers:
                    dict3 = {}
                    dict3['answerText'] = k[0:-4]
                    
                    if k == j:
                       dict3['isCorrect'] = True
                    else:
                       dict3['isCorrect'] = False
                    l.append(dict3)
                random.shuffle(l)

                dict2['answers'] = l
                list2.append(dict2)

        return JsonResponse(list2, safe=False)
    
    @csrf_exempt
    @api_view(["GET"])
    def dictionary(request):
        topics = os.listdir(TOPIC_PATH)
        test = []

        data = {}
        for i in topics:
            files = os.listdir(TOPIC_PATH + i)
            data[i] = {}
            for j in files:
                with open(TOPIC_PATH + i + '/' + j, "rb") as f:
                    data[i][j[:-4]] = base64.b64encode(f.read()).decode("utf-8")

        
            #return JsonResponse(f.read(),content_type="image/svg", safe=False)
        #print(TOPIC_PATH + 'alphabet/a.svg')
        return JsonResponse(data, safe=False)
