clearvars; % Clear variables in workspace
Temp = importdata('data1.dat'); % Read data

% Input and Output
X1 = transpose(Temp(1:end-1));
Y1 = transpose(Temp(2:end));

% Normalize Input and Output (min and max)
minX1 = min(X1);
maxX1 = max(X1);
X2 = (X1 - minX1) / (maxX1 - minX1);
Y2 = (Y1 - minX1) / (maxX1 - minX1);

% Number of inputs of the network
% (number of past values of the series)
NumInputs = 3;

% Build new Input and Output
for i = NumInputs:length(X1)-NumInputs
    X(i-NumInputs+1,1:NumInputs) = X2(i-NumInputs+1:i);
    Y(i-NumInputs+1,1:1) = X2(i+1:i+1);
end
X = transpose(X);
Y = transpose(Y);

% Create and initialize network
net = newff(X, Y, 5, {'logsig', 'purelin'}, 'traingdm');
init(net);

% Set train parameters
net.trainParam.goal = 0.0005;
net.trainParam.epochs = 10000;
net.trainParam.lr = 0.2;
net.trainParam.mc = 0.1;
net.trainParam.max_fail = 50;

% Set train, validation and test sets size
net.divideParam.trainRatio=0.7;
net.divideParam.valRatio=0.15;
net.divideParam.testRatio=0.15;

% Train network
net = train(net, X, Y);

% Simulate the network
Test = X;
Saida = sim(net, Test);

% Plot network output vs. expected output
DataPoints = 2000; % Number of points to plot
plot(Y(1:DataPoints) * (maxX1 - minX1) + minX1);
hold on;
plot(Saida(1:DataPoints) * (maxX1 - minX1) + minX1);

% Get network weights
N_Iw = cell2mat(net.IW)
N_b1  = cell2mat(net.b(1))
N_Lw = cell2mat(net.Lw)
N_b2 = cell2mat(net.b(2))