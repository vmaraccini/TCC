import cv2
import recognize
import process
import tools
import numpy as np

class traffic_sign:

    frame = None
    lastKnownVelocity = None

    condition = None
    displayImage = np.zeros((300, 300))

    def __init__(self, condition):
        self.condition = condition

    def recognize(self):
        while 1:
            self.condition.acquire()
            self.condition.wait()
            image = self.frame
            self.condition.release()

            preprocessed = process.processImage(image)
            digits = recognize.recognize(preprocessed, image, image)
            self.lastKnownVelocity = tools.getSpeedLimitFromDigits(digits, self.lastKnownVelocity)

            self.displayImage = np.zeros((300, 300))
            font = cv2.FONT_HERSHEY_SIMPLEX
            cv2.putText(self.displayImage, str(self.lastKnownVelocity),
                        (10, 200),
                        font, 5, (200, 255, 155), 2, cv2.LINE_AA)

            #cv2.imshow('Velocity', self.displayImage)
            #cv2.waitKey(2)

            print(self.lastKnownVelocity)