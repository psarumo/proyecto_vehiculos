%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Control de Vehiculos %
% MII/MIERA %
% Proyecto de curso: ACC con arduino %
% Sara Ruiz Moreno
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Identificacion del sistema

load('data_ejemplo');

x = zeros(size(v));
x_ref = zeros(size(v));

for i = 2:length(x)
    x(i) = x(i-1) + v(i-1)*0.1;
    x_ref(i) = x_ref(i-1) + v_ref(i-1)*0.1;
end

figure('color',[1,1,1]);
subplot(3,1,1);
plot(0:0.1:length(u)/10-0.1,u,'linewidth',2);
xlabel('Tiempo (s)');
ylabel('pwm');
set(gca,'fontsize',18);
subplot(3,1,2);
plot(0:0.1:length(u)/10-0.1,v,'linewidth',2);
hold on
plot(0:0.1:length(u)/10-0.1,v_ref,'linewidth',2);
xlabel('Tiempo (s)');
ylabel('Velocidad (m/s)');
set(gca,'fontsize',18);
% subplot(3,1,3);
% plot(0:0.1:length(u)/10-0.1,x,'linewidth',2);
% hold on
% plot(0:0.1:length(u)/10-0.1,x_ref,'linewidth',2);
% xlabel('Tiempo (s)');
% ylabel('Posición (m)');
% set(gca,'fontsize',18);
subplot(3,1,3);
plot(0:0.1:length(u)/10-0.1,x_ref-x,'linewidth',2);
xlabel('Tiempo (s)');
ylabel('Distancia (m)');
set(gca,'fontsize',18);