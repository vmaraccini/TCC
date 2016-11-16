function A = getImageUDP(address, port, length)
u=udp(address, port);
fopen(u);
fwrite(u,0)
A=fread(u, length);
fclose(u);