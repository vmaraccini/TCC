tRange = [17 40];

maxVel = 10*max(v_rel);
minPos = -max(d_target);

range = maxVel - minPos;

PlotAnimate(p_leader - p_abs, v_leader - v_abs, v_abs, a_ref, d_segura, ...
    a_leader, ...
    t, tRange, [-35 maxVel -range/30 range/30]);