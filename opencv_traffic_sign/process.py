import cv2
import numpy as np
import tools

# Definitions
signalColorSegmentationLowerHue = 157
signalColorSegmentationLowerSaturation = 70
signalColorSegmentationLowerValue = 60

signalColorSegmentationUpperHue = 8
signalColorSegmentationUpperSaturation = 255
signalColorSegmentationUpperValue = 255

signalColorSegmentationMorphKernelSizeX = 2
signalColorSegmentationMorphKernelSizeY = 2
signalColorSegmentationMorphIterations = 1
signalColorSegmentationMorphType = 0


def processImage(image):
    preprocessedImage = image

    segmentedImage = segmentImageByTrafficSignColor(preprocessedImage)

    converted = cv2.cvtColor(segmentedImage, cv2.COLOR_GRAY2RGB)
    tools.displayIfNeeded(converted, "Preprocessed")

    return applyMorphologicalCorrections(segmentedImage)


def segmentImageByTrafficSignColor(image):
    image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    result = []

    if (signalColorSegmentationLowerHue < signalColorSegmentationUpperHue):
        lower = np.array([signalColorSegmentationLowerHue,
                          signalColorSegmentationLowerSaturation,
                          signalColorSegmentationLowerValue])
        upper = np.array([signalColorSegmentationUpperHue,
                          signalColorSegmentationUpperSaturation,
                          signalColorSegmentationUpperValue])

        result = cv2.inRange(image, lower, upper)
    else:  # color wrap
        lowerOne = np.array([0,
                             signalColorSegmentationLowerSaturation,
                             signalColorSegmentationLowerValue])
        upperOne = np.array([signalColorSegmentationUpperHue,
                             signalColorSegmentationUpperSaturation,
                             signalColorSegmentationUpperValue])

        one = cv2.inRange(image, lowerOne, upperOne)

        lowerOther = np.array([signalColorSegmentationLowerHue,
                               signalColorSegmentationLowerSaturation,
                               signalColorSegmentationLowerValue])
        upperOther = np.array([179,
                               signalColorSegmentationUpperSaturation,
                               signalColorSegmentationUpperValue])

        other = cv2.inRange(image, lowerOther, upperOther)

        result = cv2.bitwise_or(one, other)

    return result


def applyMorphologicalCorrections(image):
    if (signalColorSegmentationMorphKernelSizeX > 0 and
                signalColorSegmentationMorphKernelSizeY > 0 and
                signalColorSegmentationMorphIterations > 0):
        anchor = (signalColorSegmentationMorphKernelSizeX, signalColorSegmentationMorphKernelSizeY)
        element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,
                                            ((2 * signalColorSegmentationMorphKernelSizeX + 1),
                                             (2 * signalColorSegmentationMorphKernelSizeY + 1)),
                                            anchor)

        operation = cv2.MORPH_OPEN if signalColorSegmentationMorphType == 0 else cv2.MORPH_CLOSE

        return cv2.morphologyEx(src=image, op=operation, kernel=element, anchor=anchor,
                                iterations=signalColorSegmentationMorphIterations)
