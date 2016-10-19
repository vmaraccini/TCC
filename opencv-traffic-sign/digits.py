import cv2

# Definitions
textSkeletonKernelPercentageX = 6
textSkeletonKernelPercentageY = 6

useSkeletonizationOnDigits = True
textSkeletonIterations = 1

def loadTemplates():
    digitTemplates = []
    for i in range(10):
        path = "./digits/" + str(i) + ".png"
        img = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
        _, img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY)
        digitTemplates.append(img)

    return digitTemplates


def skeletonizeTemplates(templates):
    skeletons = []
    for image in templates:
        height, width = image.shape
        kernX = max(width * textSkeletonKernelPercentageX / 100, 3)
        kernY = max(height * textSkeletonKernelPercentageY / 100, 3)

        element = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (kernX, kernY))

        digitTemplateSkeleton = []
        if (useSkeletonizationOnDigits):
            digitTemplateSkeleton = textSkeletonization(image, element, textSkeletonIterations)
        else:
            for i in range(textSkeletonIterations):
                digitTemplateSkeleton = cv2.erode(digitTemplateSkeleton, element)

        skeletons.append(digitTemplateSkeleton)

    return   skeletons


def textSkeletonization(image, kernel, numberIterations):
    if numberIterations < 1:
        return image

    result = image
    eroded = image
    for i in range(numberIterations):
        eroded = cv2.erode(eroded, kernel)
        temp = cv2.dilate(result, kernel)
        temp = cv2.subtract(result, temp)
        result = cv2.bitwise_or(temp, result)

    return result

templates = loadTemplates()
skeletons = skeletonizeTemplates(templates)

#Test code:

# templates = loadTemplates()
# skeletons = skeletonizeTemplates(templates)
#
# cv2.imshow("original", templates[0])
# cv2.imshow("skeleton", skeletons[0])
