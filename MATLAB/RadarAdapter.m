function [ r, theta, v ] = RadarAdapter( radar_target1, radar_target2 )
%RADARADAPTER Adapts the radar input data into an r, theta, v format
%   Transforms the radar input into a set of arrays containing radius,
%   angle and velocity information.

r = [];
v = [];
theta = [];
for k=1:length(radar_target1{1})
    currTarget1 = radar_target1{1}{k};
    currTarget2 = radar_target2{1}{k};
    
    r = [r, currTarget1.Tar_Dist];
    v = [v, currTarget1.Tar_Vrel];
    theta = [theta, currTarget2.Tar_Ang];
end

end

