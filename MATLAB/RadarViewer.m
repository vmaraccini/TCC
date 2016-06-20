for x = 1:100
    part1 = cellstr(char(judp('receive',25000,300, 10000))');
    part2 = cellstr(char(judp('receive',25001,300, 10000))');

    target1_json = parse_json(part1{1});
    target2_json = parse_json(part2{1});

    [r, theta, v] = RadarAdapter(target1_json, target2_json);
    [x, y] = pol2cart(pi/180*theta, r);


    [vx, vy] = pol2cart(pi/180*theta, v);


    scatter([0 x],[0 y]);
    axis([0 100 -50 50]);
    hold on
    quiver(x,y,vx, vy,'Autoscale','off');
    hold off
    drawnow
end
        