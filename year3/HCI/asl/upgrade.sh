#!/bin/bash

python3 -m venv venv
source venv/bin/activate

cd frontend
yarn install
cd ..

pip3 install -r requirements.txt