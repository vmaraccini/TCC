function PlotAnimate(d_target, v_rel_target, v_abs, a_ref, d_segura, a_target, t, tRange, axes_spec)
% Animates the plotting of x,y data at a specified framerate

warning off MATLAB:plot:IgnoreImaginaryXYPart % Suppress annoying warnings

xmin = axes_spec(1); xmax = axes_spec(2);
ymin = axes_spec(3); ymax = axes_spec(4);

frameTime = 1/10;
Video = VideoWriter('output_video');
Video.FrameRate = round(1/frameTime);
open(Video);figure(1);

ts = [];
for i = tRange(1) : frameTime : tRange(end);
    [c index] = min(abs(t - i));
    ts = [ts index];
end

for ii = ts
   %Plot animation above
   subplot(3,1,1);
   scatter(-d_target(ii),0); 
   hold on
   quiver(-d_target(ii),0, v_abs(ii), 0, 0.3, 'Autoscale', 'off');
   scatter(0, 0)
   quiver(0,0,v_abs(ii) + v_rel_target(ii), 0, 0.3, 'Autoscale', 'off');
   axis([xmin xmax ymin ymax]); 
   hold off
   
   %Plot real data below
   subplot(3,1,2);
   plot(t(ts), d_target(ts), 'Color', 'b');
   hold on
   
   plot(t(ts), v_rel_target(ts), 'Color', 'r');
   plot(t(ts), a_ref(ts), 'Color', 'k');
   plot(t(ts), d_segura(ts), '--', 'Color', 'g');
   
   %Scatter
   scatter(t(ii), a_ref(ii), 'k');
   scatter(t(ii), v_rel_target(ii), 'r');
   scatter(t(ii), d_target(ii), 'b')
   
   legend('Distancia (m)','Velocidade relativa (m/s)', 'Aceleracao (%)', 'Distancia Segura (m)', 'Location', 'bestoutside')
   xlim([tRange(1), tRange(end)]);
   hold off
   
   %Plot real data below
   subplot(3,1,3);
   plot(t(ts), v_rel_target(ts) + v_abs(ts), 'Color', 'k');
   hold on
   plot(t(ts), a_target(ts), 'Color', 'r');
   
   %Scatter
   scatter(t(ii), v_rel_target(ii) + v_abs(ii), 'k');
   scatter(t(ii), a_target(ii), 'r');
   
   legend('Velocidade do lider (m/s)','Acel. do lider (m/s^2)', 'Location', 'bestoutside')
   xlim([tRange(1), tRange(end)]);
   hold off
   
   writeVideo(Video,getframe(gcf));
end
close(Video); 
warning on MATLAB:plot:IgnoreImaginaryXYPart % Suppress annoying warnings