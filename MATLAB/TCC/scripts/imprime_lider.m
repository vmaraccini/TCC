function imprime_lider(X1, Y1, Y2)
%CREATEFIGURE(X1, Y1, Y2)
%  X1:  vector of x data
%  Y1:  vector of y data
%  Y2:  vector of y data

%  Auto-generated by MATLAB on 29-Jun-2016 19:11:12

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
ylabel('Velocidade [m/s]','FontSize',15.4);

% Set the remaining axes properties
set(axes1,'YColor',[0 0 0]);
% Activate the right side of the axes
yyaxis(axes1,'right');
% Create plot
plot(X1,Y2,'LineWidth',1,'Color',[1 0 0]);

% Create ylabel
ylabel('Acelera��o [m/s^2]','FontSize',15.4);

% Set the remaining axes properties
set(axes1,'YColor',[0.85 0.325 0.098]);

ylim(axes1,[(min(Y2) - 0.1), (max(Y2) + 0.1)]);
% Create xlabel
xlabel('Tempo (s)','FontSize',15.4);

% Create title
title('Acelera��o e velocidade do ve�culo de refer�ncia','FontSize',15.4);

xlim(axes1,[95 110]);
box(axes1,'on');
% Set the remaining axes properties
set(axes1,'FontSize',14,'XColor',[0 0 0],'ZColor',[0 0 0]);