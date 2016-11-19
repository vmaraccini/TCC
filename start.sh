#!/bin/bash

python ./opencv-traffic-sign & \
python ./opencv-distance/server.py & \
./Controller/Controller & \
sh ./web/start.sh