load('Sistema')
[num,den]=tfdata(sys,'v');

% Matrices de espacio de estados
Ac = [1,-0.1,0,0;0,-den(2),0,0;0.1,0,1,0;0,0,0.1,1];
Bc = [0,num(2),0,0]';

% Matrices de ponderación
Q=[10000,0,0,0;0,0.0001,0,0;0,0,10,0;0,0,0,10];
R=1;

% Matriz de ganancia de realimentación
K=dlqr(Ac,Bc,Q,R);
K1=K(1);
K2=K(2);
K3=K(3);
K4=K(4);
K=[K1 K2 K3 K4]

% Condiciones iniciales
v0 = 0.1933;
d0= 5;
F0 = 120;

% Sistema en ss
A=Ac;
B=Bc;
C=eye(length(Ac));
D=zeros(length(Bc),1);
sys=ss(A,B,C,D,0.1);