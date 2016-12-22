imageDir = '.';
leftImages = imageDatastore(fullfile(imageDir,'l'));
rightImages = imageDatastore(fullfile(imageDir,'r'));
images1 = leftImages.Files;
images2 = rightImages.Files;

[imagePoints, boardSize] = detectCheckerboardPoints(images1,images2);

squareSizeInMM = 25;
worldPoints = generateCheckerboardPoints(boardSize,squareSizeInMM);

im = readimage(leftImages,1);
stereoParams = estimateCameraParameters(imagePoints,worldPoints);