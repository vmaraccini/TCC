%Create reference
refCar = rgb2gray(imread('car.png'));
refPoints = detectSURFFeatures(refCar);
[refFeatures, validPts] = extractFeatures(refCar, refPoints);

%Load videos
colorReader = vision.VideoFileReader('camera_color.avi');
depthReader = vision.VideoFileReader('camera_depth.avi');

%Process frames
for i = 1:100
    step(colorReader);
    step(depthReader);
end
depth = [];
for i = 1:3
    currCar = rgb2gray(step(colorReader));
    currPoints = detectSURFFeatures(currCar);
    [currFeatures, currValidPoints] = extractFeatures(currCar, currPoints);

    matches = matchFeatures(refFeatures, currFeatures);
    
    matchesReference = refPoints(matches(:,1));
    matchesCurrent = currPoints(matches(:,2));
    
    [transform, inlierReference, inlierCurrent] = ...
    estimateGeometricTransform(matchesReference, matchesCurrent, 'Similarity');
    
    intensity = rgb2gray(step(depthReader));    
    currDepth = mean(mean(intensity(round(inlierCurrent.Location))));
    
    depth = [depth currDepth];
end