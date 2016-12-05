capture_chessboards --columns 6 --rows 9 1 0 40 calibration_pictures
calibrate_cameras --columns 6 --rows 9 --square-size 2.5 calibration_pictures calibration_results
python test_calibration.py
