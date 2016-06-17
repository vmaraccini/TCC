target1_json = parse_json('[{"NoOfTarget_1": 0,"Tar_Dist_rms": 0.000000,"Tar_Ang_rms": 0.000000,"Tar_Vrel_rms": -0.020000,"Tar_Vrel": 14.989998,"Tar_Dist": 100.000000},{"NoOfTarget_1": 0,"Tar_Dist_rms": 0.000000,"Tar_Ang_rms": 0.000000,"Tar_Vrel_rms": -0.020000,"Tar_Vrel": 14.989998,"Tar_Dist": 120.000000}]');
target2_json = parse_json('[{"NoOfTarget_2": 0,"Tar_PdH0": 1.000000,"Tar_Length": 0.000000,"Tar_Width": 30.900000,"Tar_Type": 0,"Tar_Ang": 20.000000,"Tar_RCSValue": 824}{"NoOfTarget_2": 0,"Tar_PdH0": 1.000000,"Tar_Length": 0.000000,"Tar_Width": 30.900000,"Tar_Type": 0,"Tar_Ang": 30.000000,"Tar_RCSValue": 0}]');

[r, theta, v] = RadarAdapter(target1_json, target2_json);
[x, y] = pol2cart(theta, r);


[vx, vy] = pol2cart(theta, v);

scatter([0 x],[0 y]);
hold on
quiver(x,y,vx, vy);
hold off