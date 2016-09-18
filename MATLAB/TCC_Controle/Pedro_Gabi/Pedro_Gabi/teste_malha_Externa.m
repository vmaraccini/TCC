% close all;
% clear all;
% clc;

Ta = 1/100;
Tsim = 10000;
t = 0:Ta:Tsim;
%vdes = zeros(1,length(t));
%vdes(t>0) = 50;
vdes=13.9;

% vl = zeros(1,length(t));
% dref = zeros(1, length(t));
% m = 1067;
% b = 50;

ident;

%calcula vl
% vl(t>2) = 50/3.6;
% for k = 400:1000
% vl(k) = vl(k-1) - .005;
% end
% 
% for k = 1000:100000
% vl(k) = vl(k-1);
% end
% for k = 3000:4000
% vl(k) = vl(k-1) + .01;
% end
% for k = 4000:6000
% vl(k) = vl(k-1);
% end
% for k = 6000:7000
% vl(k) = vl(k-1) + .02;
% end
% for k = 7000:9000
% vl(k) = vl(k-1) - .02;
% end
% for k = 9000:10000
% vl(k) = vl(k-1);
% end
% plot(vl)
% taux=t';
% Vl.time = taux;
% Vl.signals.values = vl';
% Vl.signals.dimensions = 1;

s=tf('s');
% Ti = 5.2;
% Kp = 2/0.6944;
% Cpi = (Kp/(Ti*s))*(Ti*s + 1);

sim('Curva_vl');

figure (99);
plot(tout2,vl,tout2,vc, 'r');
grid
figure (101);
plot(tout2, drelativa);
grid
%figure (102);
%plot(tout2,dist, tout2, ent, 'r');
%grid
%Gc.num{:}
%Gc.den{:}