/*--------------------------------------------------------------------------*/
/* Control por realimentación del vector de estados.
   Control de Vehiculos (MII/MIERA. 2020-2021)
   Version para protoboard */
/*--------------------------------------------------------------------------*/
#include <SoftwareSerial.h>

/* Pines */
// Pines Motores
int enable = 13;
int motorI1 = 3;
int motorI2 = 10;
int motorD3 = 9;
int motorD4 = 11;

// Pin encoder
const int SensorPin = 2;

// Pines bluetooth
SoftwareSerial BT(7, 4); // RX | TX del arduino

/* Constantes */
const float T = 100; //Tiempo de muestreo en milisegundos
const float r = 64 * 0.001; //Radio de la rueda (m)
const float K1 = -197.7510; //Ganancia del controlador [K,KI]
const float K2 = 145.1253;
const float K3 = -35.1774;
const float K4 = -3.0775;
const float Kp = 0; // PI
const float Ki = 0; // PI
const float Aimax = 0; // Antiwindup
const float Aimin = 0; // Antiwindup
const float ref_d = 0; // Referencia incr distancia con vehiculo de delante
//const float ref_d = 5; // Referencia distancia con vehiculo de delante
const int x3lim=-6;
const int x4lim=-55;

/* Variables de estados y señal de control */
float Dx3 = 0; // Variable auxiliar
float x1 = 0;
float x2 = 0;
float x3 = 0;
float x4 = 0;
float w = 0; // Velocidad angular
float w_leida = 0;
float v = 0.4;
float U = 0; // Señal de control x4
float U1 = 0; // Señal de control
float error = 0;
float int_error = 0;
int pwm = 200;
float Ai = 0;
float ref_v = 0.4; // Velocidad vehiculo de delante
float V0=0.4;
/* Variables auxiliares */
unsigned long t_0 = 0;
unsigned long t_1 = 0;
unsigned long t_start = 0;
int count = 0;
bool arranque = false;
bool change = false;
bool flag=true;
String data = "";
String last_data = "0";
String linea = ",,,";

/* Setup */
void setup() {

  BT.begin(9600);

  BT.setTimeout(10);

  attachInterrupt(digitalPinToInterrupt(SensorPin), counter, RISING);

  pinMode(enable, OUTPUT);
  pinMode(motorI1, OUTPUT);
  pinMode(motorI2, OUTPUT);
  pinMode(motorD3, OUTPUT);
  pinMode(motorD4, OUTPUT);

  digitalWrite(enable, LOW);
  analogWrite(motorI1, 0);
  analogWrite(motorI2, 0);
  analogWrite(motorD3, 0);
  analogWrite(motorD4, 0);

  // Enabling motors
  digitalWrite(enable, HIGH);

  delay(1000);
  t_start = millis();
}

void counter() {
  count++;
}

void loop() {
  if (arranque == false) {
    pwm = 255;
    /* Aplicar seniales */
    analogWrite(motorI1, pwm);
    analogWrite(motorD3, pwm);
    t_1 = millis();
    //if(((t1-t0)>=T) && millis()<5000 ){
    //   t0=t1;
    //   count=0;
    //}
    if (millis() - t_start >= 5000) {
      arranque = true;
      pwm = 150;
      count=0;
      t_0=t_1;
      
    }
  } else {
    /* Lectura Bluetooth. Paso de la referencia desde el ordenador hasta el vehiculo */

    if (BT.available()) {
      // La velocidad se envia en cm/s
      char c = BT.read();
      if (c >= '0' && c <= '9') data += c; // Control de error

      if (c == '\r') {
        if (data.length() > 0) {
          ref_v = data.toFloat();
          ref_v = ref_v / 100; // Pasar a m/s
          last_data = data;
        }
        data = "";
      }
    }

    t_1 = millis();

    if ((t_1 - t_0) >= T) { // En cada instante de muestreo

      /* Obtener velocidad */
      // Lectura de la velocidad y conversion a rev/s
      w_leida = ((float) count * 2 * 3.1416 / 20.0) / ((t_1 - t_0) / 1000.0);
      //count = 0;

      // Filtrar lectura
      if (w - w_leida > 1 || w_leida - w > 1) {
        w = 0.2 * w_leida + 0.8 * w;
      } else w = w_leida;

      // Velocidad lineal
      v = w * r;

      count = 0;
      
      x2 = w * r-V0; //x2 debe ser 0 al inicio
      x1 += ((t_1 - t_0) / 1000.0) * (ref_v - (w * r)); //Calculo de x1 realizando la integral
      Dx3 = ref_d - x1;
      x3 += Dx3 * ((t_1 - t_0) / 1000.0);
       if (x3<x3lim){
          x3=x3lim;}
      x4 += x3 * ((t_1 - t_0) / 1000.0);
       if (x4<x4lim){
          x4=x4lim;}

      pwm = (int)(-K1 * x1 - K2 * x2 + K3 * x3 + K4 * x4 + 150.0);

      if (pwm > 255) pwm = 255;
      if (pwm < 100) pwm = 100;
      //}

      /* Aplicar seniales */
      analogWrite(motorI1, pwm);
      analogWrite(motorD3, pwm);

      /* Escribir datos */
      linea = String(pwm) + "," + String(v, 4) + "," + String(ref_v,4) + "," + String(x1,4) + "," + String(t_1 - t_start) + "\n\r"; // pwm, v, vref, t
      BT.write(linea.c_str());

      t_0 = t_1; //Actualizamos el valor

    }

    delay(10); //0.1s
  }
}
