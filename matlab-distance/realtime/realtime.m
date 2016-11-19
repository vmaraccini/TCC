load('../calibration/stereoParams.mat')
player = vision.DeployableVideoPlayer('Location', [20, 400]);

while 1
    I1 = imrotate(imread('l.bmp'), 90);
    I2 = imrotate(imread('r.bmp'), 90);
    [J1, J2] = rectifyStereoImages(I1,I2,stereoParams);
    disparityMap = disparity(imresize(rgb2gray(J1), 0.5), imresize(rgb2gray(J2), 0.5));
    step(player, disparityMap ./ 64)
end