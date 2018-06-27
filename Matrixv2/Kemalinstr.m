axis([0 1000 -1000 0]);
title('Matrix: 1000 x 1000 From: (1,1) To: (500,500)');
xlabel('Width: 1000');
ylabel('Height: 1000');
X=[500,500,499,1];
Y=[-500,-500,-499,-1];
hold on;
plot(X,Y,'r');
