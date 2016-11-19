import cv2
import recognize
import process
import tools
from copy import deepcopy

lastKnownVelocity = None

def captureAndRecognize(videoCapture):
    ret, frame = videoCapture.read()

    image = deepcopy(frame)
    preprocessed = process.processImage(image)

    cv2.waitKey(10)

    global digits
    digits = recognize.recognize(preprocessed, image, image)

    lastKnownVelocity = tools.getSpeedLimitFromDigits(digits, lastKnownVelocity)

    display = tools.printRecognizedDigits(frame, lastKnownVelocity)
    cv2.imshow("Capture", display)

def calculate():	
    cap = cv2.VideoCapture(0)
    while True:
         captureAndRecognize(cap)


