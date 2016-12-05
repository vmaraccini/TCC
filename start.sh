#!/bin/bash

#./Controller/Controller &
cd web
sh ./start.sh &
cd .. 
python main.py
