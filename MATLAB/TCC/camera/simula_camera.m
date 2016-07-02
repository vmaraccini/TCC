function image = simula_camera(vRel, dist, ang)
imgSize = 100;
dMax = 100;

road_template = rgb2gray(imread('road_template.png'));
image = imresize(road_template, [imgSize, imgSize]);

% Adicionar ruido simulando fundo
image = image + uint8(255*rand(imgSize)/10); %10% de dMax

% Adicionar carro
if dist < 1
    dist = 1;
end
if dist > dMax
    dist = dMax;
end
    
% Adicionar ruido branco sobre o carro
tamanho = ceil(imgSize*(1-dist/dMax));
carro = rand(tamanho)*0.1*(dist/dMax);

carro = carro + dist/dMax * ones(tamanho, tamanho);

image((end/2 - ceil(tamanho/2) + 1) : (end/2 + floor(tamanho/2)), ...
    (end/2 - ceil(tamanho/2) + 1) : end/2 + floor(tamanho/2)) = ...
    uint8(255*carro);

end