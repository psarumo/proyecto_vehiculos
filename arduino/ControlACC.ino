//Control por realimentación del vector de estados

const int SensorPin=1;
//Aqui irian los pines de los motores
const float T=0; //Tiempo de muestreo
const float r=0; //Radio de la rueda
const float K1=0; //Ganancia del controlador [K,KI]
const float K2=0;
const float K3=0;
const float K4=0;

float Dx3=0;
float x1=0;
float x2=0;
float x3=0;
float x4=0;
float w=0; //Velocidad angular
float U=0; //Señal de control x4
float U1=0; //Señal de control
int x2_read=0;
int x1_read=0;
void setup() {
  
  Serial.begin (9600);
  pinMode (SensorPin, INPUT);
 
}

void loop() {
  x2_read=analogRead(SensorPin);
  w=0; // Relación Tensión - Medida
  x2=w*r;
  x1_read=0; // Se pasa el valor por Bluetooth
  x1=0;
  Dx3=r-x1; //Cuando se calcule KI en Matlab hay que cambiar el signo
  x3+=Dx3*T;
  x4+=x3*T;

  U=-K1*x1-K2*x2+K3*x3+K4*x4;
  U1=U/4.0; //U1 es tracción. Hay que pasar a señal PWM.
  
  delay(300);
}
