import recognize
import process
import tools
import cv2
import os
from copy import deepcopy

basePath = "./video/"
fileName = "80kmh"
fileExtension = ".mp4"

debugDirectory = "/home/tmep/Videos/video_debug/"
fullDebugDirectory = debugDirectory + fileName

if not os.path.exists(fullDebugDirectory):
    os.makedirs(fullDebugDirectory)

cap = cv2.VideoCapture(basePath + fileName + fileExtension)

h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
fps = int(cap.get(cv2.CAP_PROP_FPS))
codec = int(cap.get(cv2.CAP_PROP_FOURCC))

debugVideo = cv2.VideoWriter(filename=fullDebugDirectory + "/output.avi",
                             fourcc=codec,
                             fps=fps,
                             frameSize=(w, h))

debugCaps = {}
def writeVideoDebugOperation(image, title):
    if not title in debugCaps:
        h = image.shape[0]
        w = image.shape[1]
        cap = cv2.VideoWriter(fullDebugDirectory + "/" + title + ".avi",
                              fourcc=codec,
                              fps=fps,
                              frameSize=(w, h))
        debugCaps[title] = cap

    debugCaps[title].write(image)

tools.debugOperation = writeVideoDebugOperation

lastKnownVelocity = None
while cap.isOpened():
    ret, frame = cap.read()
    if frame == None:
        break

    preprocessed = process.processImage(frame)
    digits = recognize.recognize(preprocessed, frame, deepcopy(frame))

    lastKnownVelocity = tools.getSpeedLimitFromDigits(digits, lastKnownVelocity)

    result = tools.printRecognizedDigits(frame, str(lastKnownVelocity))
    debugVideo.write(result)

debugVideo.release()
