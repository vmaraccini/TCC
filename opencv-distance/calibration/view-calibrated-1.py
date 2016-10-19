import numpy as np
import cv2

# copy parameters to arrays
K = np.array([[  3.61647514e+03, 0, 4.81198272e+02],[ 0,3.15695096e+03,2.30954541e+02],[0,0,1]])
d = np.array([ -1.34456146e+00, 6.38536567e+01, 0, 0, 0]) # just use first two terms (no translation)

# read one of your images
cap = cv2.VideoCapture(1)
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
