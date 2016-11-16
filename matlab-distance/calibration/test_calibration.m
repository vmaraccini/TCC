I1 = imread('l.bmp');
I2 = imread('r.bmp');
[J1, J2] = rectifyStereoImages(I1,I2,stereoParams);

figure(1)
imshow(J1);

figure(2)
imshow(J2);

disparityMap = disparity(rgb2gray(J1), rgb2gray(J2));

figure(3)
imshow(disparityMap,[0,64],'InitialMagnification',50);

xyzPoints = reconstructScene(disparityMap,stereoParams);
ptCloud = pointCloud(xyzPoints./1000, 'Color', J1);

% Create a streaming point cloud viewer
player3D = pcplayer([-3, 3], [-3, 3], [0, 5], 'VerticalAxis', 'y', ...
    'VerticalAxisDir', 'down');

view(player3D, ptCloud);
