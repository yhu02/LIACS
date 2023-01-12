from django.urls import path
from .views import SignViewSet
from rest_framework_nested import routers
from rest_framework.routers import APIRootView

app_name = 'sign'
router = routers.SimpleRouter()

router.register('',SignViewSet, basename='viewset')

urlpatterns = ([
    path('translate/', SignViewSet.translate),
    path('application/', SignViewSet.application),
    path('score/', SignViewSet.add_score),
    path('scores/', SignViewSet.get_scores),
    path('current/', SignViewSet.current),
    path('scoreboard/', SignViewSet.scoreboard),
    path('quiz_data/', SignViewSet.quiz_data),
    path('dictionary/', SignViewSet.dictionary),
])

