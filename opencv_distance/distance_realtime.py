import cv2
import numpy as np


class distance:
    currDistance = 0

    imgPair = None
    disp_to_depth_mat = None
    condition = None

    maxDisparity = 16 * 10

    #matcher = cv2.StereoBM_create(numDisparities=maxDisparity, blockSize=31)
    matcher = cv2.StereoSGBM_create(0, maxDisparity, 17)

    minY = 100
    maxY = 300

    def __init__(self, disp_to_depth_mat, condition):
        self.condition = condition
        self.disp_to_depth_mat = disp_to_depth_mat

    def recognize_distance(self):
        while 1:
            self.condition.acquire()
            self.condition.wait()
            imgPair = [cv2.cvtColor(f, cv2.COLOR_BGR2GRAY) for f in self.imgPair]
            self.condition.release()

            #Compute disparity map
            disp = self.matcher.compute(imgPair[0], imgPair[1])

            #Get 3d points
            points = cv2.reprojectImageTo3D(disp, self.disp_to_depth_mat)
            z = -points[:, :, 2]

            z = z[self.minY:self.maxY, :]

            #Setup k-means
            criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
            flags = cv2.KMEANS_RANDOM_CENTERS

            nClusters = 8
            cols, rows = z.shape
            arr = np.float32(z.reshape((cols*rows, 1)))
            arr[arr >= self.maxDisparity] = 0
            arr[arr < 0] = 0

            #Compute clusters
            compactness, labels, centers = cv2.kmeans(arr, nClusters, None, criteria, 1, flags)

            reshapedLabels = labels.reshape(z.shape)

            printMe = cv2.cvtColor(np.copy(imgPair[0][self.minY:self.maxY, :]), cv2.COLOR_GRAY2BGR)
            font = cv2.FONT_HERSHEY_SIMPLEX

            boundsList = []
            distancesList = []
            costsList = []
            #Find potential candidates
            for i in range(nClusters):
                mask = np.zeros(z.shape)
                mask[reshapedLabels == i] = 1

                #Find all contours within the segment
                contours = cv2.findContours(np.uint8(mask), mode=cv2.RETR_CCOMP, method=cv2.CHAIN_APPROX_SIMPLE)
                for contour in contours[1]:
                    if len(contour) < 4: continue
                    area = cv2.contourArea(contour)
                    if area < 2000: continue

                    #Find contour bounds
                    bounds = cv2.boundingRect(contour)
                    x, y, w, h = bounds

                    #Calculate average distance
                    internalMask = np.uint8(np.zeros(z.shape))
                    cv2.drawContours(internalMask, contour, 0, color=255)
                    averageDistance = np.average(z[internalMask == 255])
                    if averageDistance < 1: continue
                    if averageDistance > 20: continue

                    #Compute X distance to center
                    centerX = x + w / 2
                    centerDiff = abs(centerX - rows / 2)

                    #Save to list
                    distancesList.append(averageDistance)
                    boundsList.append(bounds)
                    costsList.append(centerDiff + 0.3 * averageDistance)

                    #Display for debug
                    cv2.rectangle(printMe, (x, y), (x + w, y + h), (0, 0, 0), 2)
                    cv2.putText(printMe,
                                str(averageDistance),
                                (x + 10, y + 20),
                                font, 0.5, (200,255,155), 2, cv2.LINE_AA)

            if len(costsList) == 0: continue

            #Print best match in red
            _, idx = min((val, idx) for (idx, val) in enumerate(costsList))
            x, y, w, h = boundsList[idx]
            cv2.rectangle(printMe, (x, y), (x + w, y + h), (0, 0, 255), 2)

            #Set best match to global variable
            self.currentDistance = distancesList[idx]

            cv2.imshow('ref', cv2.pyrUp(printMe))
            cv2.waitKey(2)
