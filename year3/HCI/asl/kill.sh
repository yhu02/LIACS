#!/bin/bash

sudo killall leapd
fuser -k 5000/tcp
fuser -k 3000/tcp
fuser -k 8000/tcp
