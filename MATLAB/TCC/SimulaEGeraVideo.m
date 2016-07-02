maxVel = max(v_rel);
minPos = -max(d_target);

range = maxVel - minPos;

PlotAnimate(d_target, v_rel, v_abs, a_ref, d_segura, ...
    a_leader, ...
    t, [95 115], [minPos maxVel -range/30 range/30]);