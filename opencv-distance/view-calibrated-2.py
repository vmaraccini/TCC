import numpy as np
import cv2

# copy parameters to arrays
K = np.array([[2.93364400e+04,0,3.18677153e+02],[0,2.71170836e+04,2.39619525e+02],[0,0,1]])
d = np.array([ 1.94113721e+01,1.04040018e-03, 0, 0, 0]) # just use first two terms (no translation)

# read one of your images
cap = cv2.VideoCapture(2)
while(True):
	# Capture frame-by-frame
	ret, frame = cap.read()

	h, w = frame.shape[:2]
	# undistort
	newcamera, roi = cv2.getOptimalNewCameraMatrix(K, d, (w,h), 0) 
	newimg = cv2.undistort(frame, K, d, None, newcamera)

	# Display the resulting frame
	cv2.imshow('frame',newimg)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

cap.release()
cv2.destroyAllWindows()
