function imprime_simulacao(X1, Y1, Y2)
%imprime_simulacao(X1, Y1, Y2)
%  X1:  vector of time data
%  Y1:  vector of distance data
%  Y2:  vector of relative velocity data

% Create figure
figure1 = figure('PaperOrientation','landscape','PaperType','A4',...
    'PaperSize',[11.692913 8.267716]);

% Create axes
axes1 = axes('Parent',figure1);
hold(axes1,'on');

% Activate the left side of the axes
yyaxis(axes1,'left');
% Create plot
plot(X1,Y1,'LineWidth',1,'Color',[0 0 0]);

% Create ylabel
ylabel('Distância [m]','FontSize',15.4);

% Set the remaining axes properties
set(axes1,'YColor',[0 0 0]);
% Activate the right side of the axes
yyaxis(axes1,'right');
% Create plot
plot(X1,Y2,'LineWidth',1,'Color',[1 0 0]);

% Create ylabel
ylabel('Velocidade relativa [m/s]','FontSize',15.4);

% Set the remaining axes properties
set(axes1,'YColor',[0.85 0.325 0.098]);

% ylim(axes1,[(min(Y2) - 0.1), (max(Y2) + 0.1)]);
% Create xlabel
xlabel('Tempo (s)','FontSize',15.4);

% Create title
title('Distância e velocidade relativa do veículo de referência',...
    'FontSize',15.4);

xlim(axes1,[95 140]);
box(axes1,'on');
% Set the remaining axes properties
set(axes1,'FontSize',14,'XColor',[0 0 0],'ZColor',[0 0 0]);
