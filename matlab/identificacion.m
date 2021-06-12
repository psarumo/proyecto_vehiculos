%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Control de Vehiculos %
% MII/MIERA %
% Proyecto de curso: ACC con arduino %
% Sara Ruiz Moreno
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Identificacion del sistema

%% Cargar datos
load('../log/data1.log'); % Identificacion
load('../log/data2.log'); % Validacion
% Datos: pwm | w | vref | t (milis)

y = data1(:,2)*0.064; % Pasada a velocidad lineal
y_suav = smooth(y);
y2 = data2(:,2)*0.064; % Pasada a velocidad lineal
u = data1(:,1);
u2 = data2(:,1);

figure; subplot(2,1,1); plot(u); subplot(2,1,2); plot(y_suav)
figure; subplot(2,1,1); plot(u2); subplot(2,1,2); plot(y2)

%% Linealizacion
v0 = 0.1933;
F0 = 120;

y = y - v0;
y_suav = y_suav - v0;
u = u - F0;
y2 = y2 - v0;
u2 = u2 - F0;

% systemIdentification;

%% Funcion de transferencia
load('tf_incremental');
sys = tf(tf6.Numerator,tf6.Denominator,0.1);
sys = d2c(sys);
num = cell2mat(sys.Numerator);
den = cell2mat(sys.Denominator);
num = num/den(2);
den = den/den(2);

y_sim = lsim(sys,u,0:0.1:length(u)/10-0.1);
y_sim2 = lsim(sys,u2,0:0.1:length(u2)/10-0.1);

figure('color',[1,1,1]);
subplot(2,1,1);
plot(0:0.1:length(u)/10-0.1,u+F0,'linewidth',2);
xlabel('Tiempo (s)');
ylabel('pwm');
set(gca,'fontsize',18);
subplot(2,1,2);
plot(0:0.1:length(u)/10-0.1,y+v0,'linewidth',2);
hold on
plot(0:0.1:length(u)/10-0.1,y_sim+v0,'linewidth',2);
xlabel('Tiempo (s)');
ylabel('Velocidad (m/s)');
legend('Real','Modelo');
sgtitle('Datos para identificación','fontsize',20);
set(gca,'fontsize',18);

figure('color',[1,1,1]);
subplot(2,1,1);
plot(0:0.1:length(u2)/10-0.1,u2+F0,'linewidth',2);
xlabel('Tiempo (s)');
ylabel('pwm');
set(gca,'fontsize',18);
subplot(2,1,2);
plot(0:0.1:length(u2)/10-0.1,y2+v0,'linewidth',2);
hold on
plot(0:0.1:length(u2)/10-0.1,y_sim2+v0,'linewidth',2);
xlabel('Tiempo (s)');
ylabel('Velocidad (m/s)');
legend('Real','Modelo');
sgtitle('Datos para validación','fontsize',20);
set(gca,'fontsize',18);

%% Controlador
K = num(2);
tau = den(1);

Ac = [0,-1,0,0;0,-1/tau,0,0;1,0,0,0;0,0,1,0];
Bc = [0,K/tau,0,0]';
p = [-0.36+0.1744i , -0.36-0.1744i , -1.08 , -1.18];
K_acc = place(Ac,Bc,p);

