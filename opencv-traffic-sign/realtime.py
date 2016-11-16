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

    digits = recognize.recognize(preprocessed, image, image)

    lastKnownVelocity = tools.getSpeedLimitFromDigits(digits, lastKnownVelocity)

    display = tools.printRecognizedDigits(frame, lastKnownVelocity)
    cv2.imshow("Capture", display)

cap = cv2.VideoCapture(0)

def calculate():	
    while True:
         captureAndRecognize(cap)
