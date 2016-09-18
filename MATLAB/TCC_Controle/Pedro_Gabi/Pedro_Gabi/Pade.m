e = exp(1);
Mp = .05;
Tp = 30;




lnMp = log(Mp)/log(e);
csi = (lnMp^2)/(pi^2 + lnMp^2);
csi = sqrt(csi);
omegan = 4/(Tp*csi);

s= tf('s');
tau = 5.2;
K = 0.6994/tau;
G = K/(s + 1/tau);
G = G*(s-4)/(s+4);

A = [4, -K; 4, 4*K];
B = [2*csi*omegan; omegan^2];
X = B\A;
beta = X(1);
Kc = X(2);
Gc = -Kc*(s + 1/tau)/(s + beta);
