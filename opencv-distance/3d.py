import numpy as np
import cv2
import cv2.ximgproc as imgproc

cap1 = cv2.VideoCapture(0)
cap2 = cv2.VideoCapture(1)


def nothing(x):
    pass


cv2.namedWindow('controls')

# Good values are (1, 12)
cv2.createTrackbar('numDisparities', 'controls', 0, 255, nothing)
cv2.createTrackbar('blockSize', 'controls', 0, 128, nothing)

cv2.setTrackbarPos('numDisparities', 'controls', 0)
cv2.setTrackbarPos('blockSize', 'controls', 12)


def getImages():
    K1 = np.array([[3.61647514e+03, 0, 4.81198272e+02], [0, 3.15695096e+03, 2.30954541e+02], [0, 0, 1]])
    d1 = np.array([-1.34456146e+00, 6.38536567e+01, 0, 0, 0])  # just use first two terms (no translation)

    ret1, frame1 = cap1.read()
    frame1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2GRAY)
    # cap1.release()

    K2 = np.array([[2.93364400e+04, 0, 3.18677153e+02], [0, 2.71170836e+04, 2.39619525e+02], [0, 0, 1]])
    d2 = np.array([1.94113721e+01, 1.04040018e-03, 0, 0, 0])  # just use first two terms (no translation)

    ret2, frame2 = cap2.read()
    frame2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2GRAY)
    # cap2.release()

    h, w = frame1.shape[:2]

    # undistort
    newcamera1, roi1 = cv2.getOptimalNewCameraMatrix(K1, d1, (w, h), 0)
    undist1 = cv2.undistort(frame1, K1, d1, None, newcamera1)

    newcamera2, roi2 = cv2.getOptimalNewCameraMatrix(K2, d2, (w, h), 0)
    undist2 = cv2.undistort(frame2, K2, d2, None, newcamera2)

    return (undist1, undist2)


while True:
    imgL, imgR = getImages()
    vis = np.concatenate((imgL, imgR), axis=1)
    cv2.imshow('concat', vis)

    numDisp = 16 * cv2.getTrackbarPos('numDisparities', 'controls')
    blockSize = 5 + 2 * cv2.getTrackbarPos('blockSize', 'controls')

    stereo = cv2.StereoBM_create(numDisparities=numDisp, blockSize=blockSize)
    right_matcher = imgproc.createRightMatcher(stereo)

    lDisp = stereo.compute(imgL, imgR)
    rDisp = right_matcher.compute(imgR, imgL)

    filter = imgproc.createDisparityWLSFilter(stereo)
    filter.setLambda(8000)
    filter.setSigmaColor(1)

    filtered_disp = imgL
    filter.filter(lDisp, imgL, filtered_disp, rDisp)

    disps = np.concatenate((lDisp, rDisp), axis=1)
    cv2.imshow('disparities', cv2.convertScaleAbs(disps))
    cv2.imshow('filter', cv2.convertScaleAbs(filtered_disp))

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
