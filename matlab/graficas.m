%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Control de Vehiculos %
% MII/MIERA %
% Proyecto de curso: ACC con arduino %
% Sara Ruiz Moreno
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Identificacion del sistema

data=load('../log/suave.log');

u = data(:,1);
v = data(:,2);
v_ref = data(:,3);

if size(data,2) == 4
x = zeros(size(v));
x_ref = zeros(size(v));

for i = 2:length(x)
    x(i) = x(i-1) + v(i-1)*0.1;
    x_ref(i) = x_ref(i-1) + v_ref(i-1)*0.1;
end
x_ref = x_ref + 5; % Añadir punto de equilibrio
d = x_ref-x;
else
d = data(:,4) + 5;
end

figure('color',[1,1,1]);
subplot(3,1,1);
plot(0:0.1:length(u)/10-0.1,u),'linewidth',2);
grid on;
ylim([100,257]);
xlim([0,length(u)/10-0.1]);
xlabel('Tiempo (s)');
ylabel('PWM');
set(gca,'fontsize',18);
subplot(3,1,2);
plot(0:0.1:length(u)/10-0.1,v_ref,'linewidth',2);
hold on
plot(0:0.1:length(u)/10-0.1,v,'linewidth',2);
ylim([0.2,0.65]);
xlim([0,length(u)/10-0.1]);
xlabel('Tiempo (s)');
ylabel('Velocidad (m/s)');
grid on
legend('Vehículo de delante','Vehículo');
set(gca,'fontsize',18);
% subplot(3,1,3);
% plot(0:0.1:length(u)/10-0.1,x,'linewidth',2);
% hold on
% plot(0:0.1:length(u)/10-0.1,x_ref,'linewidth',2);
% xlabel('Tiempo (s)');
% ylabel('Posición (m)');
% set(gca,'fontsize',18);
subplot(3,1,3);
plot(0:0.1:length(u)/10-0.1,5*ones(size(u)),'linewidth',2);
hold on
plot(0:0.1:length(u)/10-0.1,d,'linewidth',2);
grid on
xlabel('Tiempo (s)');
ylim([4.5,5.5]);
xlim([0,length(u)/10-0.1]);
ylabel('Distancia (m)');
legend('Referencia','Valor real');
set(gca,'fontsize',18);