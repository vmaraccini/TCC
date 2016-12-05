#!/bin/bash

./Controller/Controller &
sh ./web/start.sh &

python opencv-camera/distance.py &
python opencv-traffic-sign/traffic-sign.py &