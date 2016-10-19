from functools import wraps
import errno
import os
import signal
import cv2

debug = True
downscaleDebugImage = False

def defaultDebugOperation(image, title):
    cv2.imshow(title, image)

debugOperation = defaultDebugOperation

class TimeoutError(Exception):
    pass

def displayIfNeeded(image, title="", resize=False, targetSize=200):
    if image == None:
        return
    if debug:
        h = image.shape[0]
        w = image.shape[1]

        shouldResize = (downscaleDebugImage and max(h, w) > targetSize) or resize
        if shouldResize:
            aspectRatio = float(w)/h
            image = cv2.resize(image, (int(aspectRatio * targetSize), targetSize))

        debugOperation(image, title)

def getSpeedLimitFromDigits(digits, lastKnownVelocity):
    for digitCollection in digits:
        if len(digitCollection) < 1: continue
        candidateVelocity = int(''.join([str(x) for x in digitCollection]))
        if  candidateVelocity > 10 and candidateVelocity < 120:
            return candidateVelocity
    return lastKnownVelocity

def printRecognizedDigits(image, digits):
    return cv2.putText(image, str(digits), (0, 50), cv2.FONT_HERSHEY_PLAIN, 2, (0, 0, 0))

def timeout(seconds=5, error_message=os.strerror(errno.ETIME)):
    def decorator(func):
        def _handle_timeout(signum, frame):
            raise TimeoutError(error_message)

        def wrapper(*args, **kwargs):
            signal.signal(signal.SIGALRM, _handle_timeout)
            signal.alarm(seconds)
            try:
                result = func(*args, **kwargs)
            finally:
                signal.alarm(0)
            return result

        return wraps(func)(wrapper)

    return decorator