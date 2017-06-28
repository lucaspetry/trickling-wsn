clearvars; % Clear variables in workspace
Temp = importdata('room_lighting.dat'); % Read data

% Input and Output
X1 = transpose(Temp(1:end-1));
Y1 = transpose(Temp(2:end));

% Normalize Input and Output (min and max)
minX1 = min(X1);
maxX1 = max(X1);
X2 = (X1 - minX1) / (maxX1 - minX1);
Y2 = (Y1 - minX1) / (maxX1 - minX1);

% Number of inputs and hidden units of the network
% (NumInputs = number of past values of the series)
NumInputs = 4;
NumHidden = 5;
TestSet = 0.3;

% Build new Input and Output
for i = NumInputs:length(X1)-NumInputs
    X(i-NumInputs+1,1:NumInputs) = X2(i-NumInputs+1:i);
    Y(i-NumInputs+1,1:1) = X2(i+1:i+1);
end
X = transpose(X);
Y = transpose(Y);

X_Test = X(1:NumInputs, 1:round(length(X)*TestSet));
Y_Test = Y(1:1, 1:round(length(Y)*TestSet));

X = X(1:NumInputs, round(length(X)*TestSet)+1:length(X));
Y = Y(1:1, round(length(Y)*TestSet)+1:length(Y));

% Create and initialize network
net = newff(X, Y, NumHidden, {'logsig', 'purelin'}, 'traingdm');
init(net);

% Set train parameters
net.trainParam.goal = 0.00005;
net.trainParam.epochs = 10000;
net.trainParam.lr = 0.2;
net.trainParam.mc = 0.7;
net.trainParam.max_fail = 50;

% Set train, validation and test sets size
net.divideParam.trainRatio=0.7;
net.divideParam.valRatio=0.15;
net.divideParam.testRatio=0.15;

% Train network
net = train(net, X, Y);

% Simulate the network
Saida = sim(net, X_Test);

% Plot network output vs. expected output
DataPoints = 5000; % Number of points to plot
plot(Y_Test(1:DataPoints) * (maxX1 - minX1) + minX1);
hold on;
plot(Saida(1:DataPoints) * (maxX1 - minX1) + minX1);
xlabel('Time(t)') % x-axis label
legend('Expected Output','Network Output')
title('Prediction of Room Lighting at UFSC Smart Solar Building')

% Get network weights
format long;
N_Iw = cell2mat(net.IW);
N_b1  = cell2mat(net.b(1));
N_Lw = cell2mat(net.Lw);
N_b2 = cell2mat(net.b(2));