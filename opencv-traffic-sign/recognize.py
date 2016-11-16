import cv2
import math
import process
import digits
import numpy as np
from tools import *
from copy import deepcopy

# PARAMETERS
PARAM_FIT_ELLIPSIS_SCALE_FOR_HOUGH_CIRCLE = 1.50
PARAM_TEXT_MIN_AREA_PERCENTAGE_IN_SIGN = 0.01

# Definitions
houghCirclesMinDistanceCenters = 10
houghCirclesMinRadius = 1
houghCirclesMaxRadius = 100
houghCirclesDP = 1

houghCirclesCannyHigherThreshold = 100
houghCirclesAccumulatorThreshold = 25

circlesMinDistanceCenters = 2

textColorSegmentationLowerHue = 0
textColorSegmentationLowerSaturation = 0
textColorSegmentationLowerValue = 0

textColorSegmentationUpperHue = 140
textColorSegmentationUpperSaturation = 255
textColorSegmentationUpperValue = 100

textSkeletonKernelPercentageX = 3
textSkeletonKernelPercentageY = 3
textSkeletonIterations = 1

def recognize(segmentedImage, preprocessedImage, imageToDraw=None):
    houghCircles = recognizeTrafficSignEllipsis(segmentedImage, imageToDraw)

    if len(houghCircles) < 1:
        return []

    filteredCircles = filterTrafficSignEllipsis(houghCircles)

    filteredCirclesImage = deepcopy(imageToDraw)
    for circle in filteredCircles:
        filteredCirclesImage = cv2.circle(filteredCirclesImage, (circle[0], circle[1]), circle[2], (0,255,255), 5)

    displayIfNeeded(filteredCirclesImage, "Filtered circles")
    internalTexts = segmentImageByTrafficSignTextUsingCircles(filteredCircles,preprocessedImage, deepcopy(preprocessedImage))

    digits = []
    for text, bounding in internalTexts:
        digit = recognizeTrafficSignTextUsingCircles(text, bounding, imageToDraw)
        digits.append(digit)

    return digits


def recognizeTrafficSignEllipsis(segmentedImage, imageToDraw=None):
    height, width = segmentedImage.shape

    circlesMinDistanceCenters = height * houghCirclesMinDistanceCenters / 100
    circlesMinRadius = height * houghCirclesMinRadius / 100
    circlesMaxRadius = height * houghCirclesMaxRadius / 100
    circles = cv2.HoughCircles(image=segmentedImage,
                               method=cv2.HOUGH_GRADIENT,
                               dp=max(1, houghCirclesDP),
                               minDist=max(1, circlesMinDistanceCenters),
                               param1=max(1, houghCirclesCannyHigherThreshold),
                               param2=max(1, houghCirclesAccumulatorThreshold),
                               minRadius=circlesMinRadius,
                               maxRadius=circlesMaxRadius)
    if circles == None:
        return []

    circles = circles[0, :]

    #Filter only the first 5
    circles = circles[0:4]

    circlesImage = deepcopy(imageToDraw)
    for circle in circles:
        circlesImage = cv2.circle(circlesImage, (circle[0], circle[1]), circle[2], (0,255,255), 5)
        displayIfNeeded(circlesImage, "Detected circles")

    return circles


# Aggregate circles closer to each other (with centers inside each other forming a cluster)
def filterTrafficSignEllipsis(houghCircles):
    houghCirclesClusters = [[houghCircles[0]]]
    if len(houghCircles) > 1:
        for allCirclesPos in range(len(houghCircles)):
            centerToAdd = houghCircles[allCirclesPos]
            if not aggregateCircleIntoClusters(houghCirclesClusters, centerToAdd):
                newCluster = [centerToAdd]
                houghCirclesClusters.append(newCluster)

    return flatClustersByMeanCenter(houghCirclesClusters)


def aggregateCircleIntoClusters(clusters, centerToAdd):
    for clusterPos in range(len(clusters)):
        for centerToTest in clusters[clusterPos]:
            maxRadius = max(centerToTest[2], centerToAdd[2])
            dx = centerToTest[0] - centerToAdd[0]
            dy = centerToTest[1] - centerToAdd[1]
            distance = math.sqrt(pow(dx, 2) + pow(dy, 2))

            # Aggregate if overlap
            if distance < maxRadius:
                clusters[clusterPos].append(centerToAdd)
                return True
    return False


def flatClustersByMeanCenter(circleClusters):
    result = []

    for cluster in circleClusters:
        if len(cluster) > 1:
            meanCircle = [0, 0, 0]
            for circle in cluster:
                meanCircle[0] += circle[0]
                meanCircle[1] += circle[1]
                meanCircle[2] += circle[2]

            meanCircle[0] = int(meanCircle[0] / len(cluster))
            meanCircle[1] = int(meanCircle[1] / len(cluster))
            meanCircle[2] = int(meanCircle[2] / len(cluster))

            result.append(meanCircle)
        else:
            result.append(cluster[0])

    return result


def retrieveEllipsisFromHoughCircles(segmentedImage, filteredCircles, imageToDraw=None):
    result = []

    height, width = segmentedImage.shape

    for circle in filteredCircles:
        x, y, radius = circle

        maxRadiusAllowed = min(min(round(x), round(y)), min(width - round(x), height - round(y)))
        radiusWithOffset = min(round(radius * PARAM_FIT_ELLIPSIS_SCALE_FOR_HOUGH_CIRCLE), maxRadiusAllowed)

        roiWidth = min(radiusWithOffset * 2, width)
        roiHeight = min(radiusWithOffset * 2, height)

        roiX = int(max(x - radiusWithOffset, 0))
        roiY = int(max(y - radiusWithOffset, 0))

        # Make sure ROI is inside image
        roiWidth = int(min(roiWidth, width - roiX))
        roiHeight = int(min(roiHeight, height - roiY))

        segmentedCopy = deepcopy(segmentedImage)
        _, contours, _ = cv2.findContours(segmentedCopy[roiX:roiX + roiWidth, roiY:roiY + roiHeight],
                                          mode=cv2.RETR_EXTERNAL,
                                          method=cv2.CHAIN_APPROX_SIMPLE,
                                          offset=(roiX, roiY))

        if len(contours) > 0:

            areas = [cv2.contourArea(x) for x in contours]
            biggestContour = contours[areas.index(max(areas))]

            if len(biggestContour) > 4:
                ellipse = cv2.fitEllipse(biggestContour)

                # Make sure ellipse is inside Hough circle
                ellipseCenter = ellipse[0]
                dx = round(ellipseCenter[0] - circle[0])
                dy = round(ellipseCenter[1] - circle[1])

                dst = math.sqrt(pow(dx, 2) + pow(dy, 2))

                if dst < radius:
                    result.append(ellipse)

    drawing = deepcopy(segmentedImage)
    for ellipse in result:
        drawing = cv2.ellipse(drawing, ellipse, (255,0,0), 3)

    displayIfNeeded(drawing, "Ellipsis")

    return result

def segmentImageByTrafficSignTextUsingCircles(recognizedCircles, preprocessedImage, imageToDraw=None):
    image = cv2.cvtColor(preprocessedImage, cv2.COLOR_BGR2HSV)

    digits = []
    for circle in recognizedCircles:
        h, w, depth = image.shape
        mask = np.zeros((h, w, depth), np.uint8)
        mask = cv2.circle(mask, (circle[0], circle[1]), circle[2], (255,255,255), thickness=-1)

        antimask = np.full((h, w, depth), 255, np.uint8)
        antimask = cv2.circle(antimask, (circle[0], circle[1]), circle[2], (0, 0, 0), thickness=-1)

        masked = cv2.bitwise_and(preprocessedImage, mask)

        # Remove black background
        masked = cv2.add(masked, antimask)

        displayIfNeeded(masked, "Masked digits")

        # Filter text
        lowerB = np.array([textColorSegmentationLowerHue,
                           textColorSegmentationLowerSaturation,
                           textColorSegmentationLowerValue])

        upperB = np.array([textColorSegmentationUpperHue,
                           textColorSegmentationUpperSaturation,
                           textColorSegmentationUpperValue])

        digits.append((cv2.inRange(masked, lowerB, upperB), circle))

    return digits

def segmentImageByTrafficSignText(recognizedEllipsis, preprocessedImage, imageToDraw=None):
    image = cv2.cvtColor(preprocessedImage, cv2.COLOR_BGR2HSV)

    digits = []
    for ellipse in recognizedEllipsis:
        h, w, depth = image.shape
        mask = np.zeros((h, w, depth), np.uint8)
        mask = cv2.ellipse(mask, ellipse, (255, 255, 255), thickness=-1)

        antimask = np.full((h, w, depth), 255, np.uint8)
        antimask = cv2.ellipse(antimask, ellipse, (0, 0, 0), thickness=-1)

        masked = cv2.bitwise_and(preprocessedImage, mask)

        # Remove black background
        masked = cv2.add(masked, antimask)

        displayIfNeeded(masked, "Masked digits")

        # Filter text
        lowerB = np.array([textColorSegmentationLowerHue,
                           textColorSegmentationLowerSaturation,
                           textColorSegmentationLowerValue])

        upperB = np.array([textColorSegmentationUpperHue,
                           textColorSegmentationUpperSaturation,
                           textColorSegmentationUpperValue])

        digits.append((cv2.inRange(masked, lowerB, upperB), ellipse))

    return digits


def recognizeTrafficSignTextUsingCircles(filteredText, circle, imageToDraw=None):
    _, contours, _ = cv2.findContours(deepcopy(filteredText),
                                      mode=cv2.RETR_EXTERNAL,
                                      method=cv2.CHAIN_APPROX_SIMPLE)

    withContours = cv2.drawContours(imageToDraw, contours, contourIdx=-1, color=(0, 255, 255), thickness=5)
    displayIfNeeded(withContours, "Text contours")

    sizes = map(cv2.contourArea, contours)

    # Sort contours by size
    zipped = zip(sizes, contours)
    zipped.sort(cmp=lambda x, y: cmp(x[0], y[0]), reverse=True)

    # Filter-out small contours
    radius = circle[2]
    boundingSize = math.pi * pow(radius, 2)
    minSize = PARAM_TEXT_MIN_AREA_PERCENTAGE_IN_SIGN * boundingSize
    filtered = filter(lambda x: x[0] > minSize, zipped)

    if len(filtered) == 0:
        return []

    # Get at most the 3 biggest ones
    sizes, contours = zip(*filtered)
    selectedContours = list(contours[0:3])

    # Sort by x position
    selectedContours.sort(cmp=lambda x, y: cmp(x[0][0][0], y[0][0][0]))

    recognizedDigits = []
    for contour in selectedContours:
        x, y, w, h = cv2.boundingRect(contour)

        digitROI = filteredText[y:y + h, x:x + w]
        digit = recognizeDigit(digitROI)
        if digit is not None:
            recognizedDigitImage = digits.templates[digit]

            h, w = recognizedDigitImage.shape
            resizedDigit = cv2.resize(digitROI, (w, h))
            displayImage = np.concatenate((resizedDigit, recognizedDigitImage), axis=1)
            converted = cv2.cvtColor(displayImage, cv2.COLOR_GRAY2RGB)
            displayIfNeeded(converted, "Digit=" + str(digit), resize=True)

            recognizedDigits.append(digit)

    return recognizedDigits

def recognizeTrafficSignText(filteredText, ellipse, imageToDraw=None):
    _, contours, _ = cv2.findContours(deepcopy(filteredText),
                                      mode=cv2.RETR_EXTERNAL,
                                      method=cv2.CHAIN_APPROX_SIMPLE)

    withContours = cv2.drawContours(imageToDraw, contours, contourIdx=-1, color=(0,255,255), thickness=5)
    displayIfNeeded(withContours, "Text contours")

    sizes = map(cv2.contourArea, contours)

    # Sort contours by size
    zipped = zip(sizes, contours)
    zipped.sort(cmp=lambda x, y: cmp(x[0], y[0]), reverse=True)

    # Filter-out small contours
    elWidth, elHeight = ellipse[1]
    boundingSize = elWidth * elHeight * math.pi / 4
    minSize = PARAM_TEXT_MIN_AREA_PERCENTAGE_IN_SIGN * boundingSize
    filtered = filter(lambda x: x[0] > minSize, zipped)

    if len(filtered) == 0:
        return []

    # Get at most the 3 biggest ones
    sizes, contours = zip(*filtered)
    selectedContours = list(contours[0:3])

    # Sort by x position
    selectedContours.sort(cmp=lambda x, y: cmp(x[0][0][0], y[0][0][0]))

    digits = []
    for contour in selectedContours:
        x, y, w, h = cv2.boundingRect(contour)

        digitROI = filteredText[y:y + h, x:x + w]
        digit = recognizeDigit(digitROI)
        if digit is not None:
            digits.append(digit)

    return digits


def recognizeDigit(image):
    h, w = image.shape
    roiKernelSizeX = max(w * textSkeletonKernelPercentageX / 100, 3)
    roiKernelSizeY = max(w * textSkeletonKernelPercentageY / 100, 3)

    element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (roiKernelSizeX, roiKernelSizeY))
    digitROI = digits.textSkeletonization(image, element, textSkeletonIterations)

    def matchDigit(digitTemplate):
        # Resize to match template
        templateH, templateW = digitTemplate.shape
        resized = cv2.resize(digitROI, (templateW, templateH))

        return cv2.matchTemplate(resized, digitTemplate, cv2.TM_CCORR_NORMED)[0, 0]

    scores = map(matchDigit, digits.skeletons)

    zipped = zip(scores, range(10))
    zipped.sort(lambda x, y: cmp(x[0], y[0]), reverse=True)

    scores, indexes = zip(*zipped)
    return indexes[0] if scores[0] > 0.5 else None


# Test code
#
# def allImagePaths():
#     from os import listdir
#     from os.path import isfile, join
#
#     path = "./signs/"
#     return [path + f for f in listdir(path) if isfile(join(path, f))]
#
#
# def recognizeImageAtPath(imagePath):
#     image = cv2.imread(imagePath)
#     preprocessed = process.processImage(image)
#     recognized = recognize(preprocessed, image, deepcopy(image))
#     print(recognized)
#
#     cv2.waitKey(10000)
#
# recognizeImageAtPath("./signs/real-world-camera.png")

# for imagePath in allImagePaths():
#     recognizeImageAtPath(imagePath)
