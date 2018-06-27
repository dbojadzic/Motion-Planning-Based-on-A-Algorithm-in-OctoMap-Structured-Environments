axis([0 1000 -1000 0]);
title('Matrix: 1000 x 1000 From: (0,0) To: (999,999)');
xlabel('Width: 1000');
ylabel('Height: 1000');
X=[999,999,998,0];
Y=[-999,-999,-998,0];
hold on;
plot(X,Y,'r');
