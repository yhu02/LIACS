#!/bin/bash

source venv/bin/activate
sudo bash ./kill.sh
sudo leapd &
(yarn --cwd ./frontend start&)
LD_PRELOAD=./sign/lib/libLeap.so python3 manage.py runserver &
LD_PRELOAD=./PyLeapMouse/Linux/libLeap.so python3 ./PyLeapMouse/PyLeapMouse.py 

# Dont forget to source venv/bin/activate
# If running individually, omit &
# When clf.pkl file is missing, manually go to /sign and enter LD_PRELOAD=./lib/libLeap.so python3 ./app.py

