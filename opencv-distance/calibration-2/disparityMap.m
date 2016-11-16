function d = disparityMap(leftFilename, rightFilename)

camera1 = cameraParameters();
camera2 = cameraParameters();
rotation2 = [0.998312890923293,0.056331759799223,-0.014074965516831;-0.056339079047066,0.998411685057954,-1.237411209919837e-04;0.014045639483687,9.165029510611157e-04,0.999900935109991];
translation2 = [-1.466096041019824e+02,-8.906916505315461,5.119473869277364];
stereoParams = stereoParameters(camera1, camera2, rotation2, translation2);

I1 = imrotate(readppm(leftFilename), 90);
I2 = imrotate(readppm(rightFilename), 90);
[J1, J2] = rectifyStereoImages(I1,I2,stereoParams);
d = disparity(rgb2gray(J1), rgb2gray(J2));
end