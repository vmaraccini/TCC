import numpy as np
import threading
from opencv_traffic_sign import traffic_realtime
from opencv_distance import distance_realtime
import time
from stereovision import stereo_cameras, calibration

cal = calibration.StereoCalibration(input_folder="./opencv_distance/calibration")
pair = stereo_cameras.CalibratedPair([1, 0], cal, None)

condition = threading.Condition()

#Global variables
lastKnownVelocity = 0
currentDistance = 0

imgPair = None

#Threads
distance = distance_realtime.distance(pair.calibration.disp_to_depth_mat, condition)
trafficSign = traffic_realtime.traffic_sign(condition)

trafficThread = threading.Thread(target=trafficSign.recognize)
distanceThread = threading.Thread(target=distance.recognize_distance)

trafficThread.start()
distanceThread.start()

while 1:
    condition.acquire()
    imgPair = pair.get_frames()

    distance.imgPair = np.copy(imgPair)
    trafficSign.frame = np.copy(imgPair[0])

    condition.notifyAll()
    condition.release()

    time.sleep(0.1)
