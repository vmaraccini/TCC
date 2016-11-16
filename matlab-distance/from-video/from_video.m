videoFileLeft = 'l.mov';
videoFileRight = 'r.mov';

readerLeft = vision.VideoFileReader(videoFileLeft, 'VideoOutputDataType', 'uint8');
readerRight = vision.VideoFileReader(videoFileRight, 'VideoOutputDataType', 'uint8');
player = vision.DeployableVideoPlayer('Location', [20, 400]);
dispPlayer = vision.DeployableVideoPlayer('Location', [20, 400]);

readerRight.step();

player3D = pcplayer([-1, 2], [-3, 1], [0, 6], 'VerticalAxis', 'y', ...
    'VerticalAxisDir', 'down');

while ~isDone(readerLeft) && ~isDone(readerRight)
    % Read the frames.
    frameLeft = readerLeft.step();
    frameRight = readerRight.step();

    % Rectify the frames.
    [frameLeftRect, frameRightRect] = ...
        rectifyStereoImages(frameLeft, frameRight, stereoParams);

    % Convert to grayscale.
    frameLeftGray  = rgb2gray(frameLeftRect);
    frameRightGray = rgb2gray(frameRightRect);

    % Compute disparity.
    disparityMap = disparity(frameLeftGray, frameRightGray);

    % Reconstruct 3-D scene.
    points3D = reconstructScene(disparityMap, stereoParams);
    points3D = points3D ./ 1000;
    ptCloud = pointCloud(points3D, 'Color', frameLeftRect);
    view(player3D, ptCloud);

    % Display the frame.
    step(player, stereoAnaglyph(frameLeftRect, frameRightRect));
    step(dispPlayer, disparityMap ./ 64)
end

% Clean up.
reset(readerLeft);
reset(readerRight);
release(player);
