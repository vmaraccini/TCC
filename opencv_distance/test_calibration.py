import cv2
from stereovision import stereo_cameras, calibration, blockmatchers


cal = calibration.StereoCalibration(input_folder="./calibration")

maxDisparity = 16 * 5
matcher = cv2.StereoSGBM_create(0,maxDisparity,11)
#matcher = cv2.StereoBM_create(blockSize=17)

pair = stereo_cameras.CalibratedPair([1, 0], cal, None)

while 1:
    imgPair = pair.get_frames()
    imgPair = [cv2.cvtColor(f, cv2.COLOR_BGR2GRAY) for f in imgPair]

    disp = matcher.compute(imgPair[0], imgPair[1])

    cv2.imshow('L', imgPair[0])
    cv2.imshow('R', imgPair[1])
    cv2.imshow('disp', cv2.convertScaleAbs(disp))

    cv2.waitKey(10)