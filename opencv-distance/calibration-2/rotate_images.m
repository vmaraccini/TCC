imageDir = 'calibration';
leftImages = imageDatastore(fullfile(imageDir,'left'));
rightImages = imageDatastore(fullfile(imageDir,'right'));
images1 = leftImages.Files;
images2 = rightImages.Files;

for image = images2'
    imwrite(imrotate(imread(char(image)), 90), char(image))
end