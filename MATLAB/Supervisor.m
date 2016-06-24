function [ pn, vn, an ] = Supervisor(p, v, a)
%SUPERVISOR Determina o proximo estado da simulacao baseado no estado
%atual.
%   Recebe os vetores de posicao, velocidade e aceleracao do(s) veiculo(s)
%   presente(s) na simulacao e calcula o estado no proximo instante de
%   tempo, retornando pn, vn an, respectivamente as posicoes, velocidades e
%   aceleracoes do proximo estado (next).


step = 33E-3; %Aprox. 30 fps.

pn, vn, an = aConstante(p,v,a,step);

end

function [ pn, vn, an] = aConstante(p,v,a, step)

pn = p + v.*(a*step);
vn = v + a*step;

end

