%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Control de Vehiculos %
% MII/MIERA %
% Practica 3: Analisis de la dinamica longitudinal del vehiculo %
% Sara Ruiz Moreno
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Se establecen los parametros del vehiculo (Audi A5)

m = 1500;
Cd = 0.31;
A = 2.5437; 
f = 0.02;
rho = 1.202;
g = 9.81;
W = m*g;

%%% Linealizacion
v0 = 20;
w0 = 2;
theta0 = 0;
d0 = 200;
F0 = m*g*sin(theta0)+f*m*g*cos(theta0)+(rho*Cd*A*(v0+w0)^2)/2;

K = 1/(rho*Cd*A*(v0+w0));
tau = m/(rho*Cd*A*(v0+w0));

sys = tf(K,[tau 1]);

%%% PI
tau_bc = 45/3;
Kp = tau/(tau_bc*K);
Ti = tau;

%%% Control por realimentacion espacio de estados
Ac = [0,-1,0,0;0,-1/tau,0,0;1,0,0,0;0,0,1,0];
Bc = [0,K/tau,0,0]';
p = [-0.36+0.1744i , -0.36-0.1744i , -1.08 , -1.18]
K_acc = place(Ac,Bc,p)

