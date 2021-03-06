/*--------------------------------------------------------------------------*/
/* Test motores + encoder + bluetooth */
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

/* Otras variables */
const float r = 64 * 0.001; //Radio de la rueda (m)
float w = 0; //Velocidad angular
float w_leida = 0;
const float T = 100; //Tiempo de muestreo en milisegundos
float v = 0;
float vel_cp = 0;
float pwm = 0;

unsigned long t_0 = 0;
unsigned long t_1 = 0;
unsigned long t_start = 0;
int count = 0;

String data = "";
String last_data = "0";
String linea = ",,,";

/* Setup */
void setup() {

  BT.begin(9600);

  BT.setTimeout(10);

  attachInterrupt(digitalPinToInterrupt(SensorPin), counter, RISING);

  pinMode (enable, OUTPUT);
  pinMode (motorI1, OUTPUT);
  pinMode (motorI2, OUTPUT);
  pinMode (motorD3, OUTPUT);
  pinMode (motorD4, OUTPUT);

  digitalWrite (enable, LOW);
  analogWrite (motorI1, 0);
  analogWrite (motorI2, 0);
  analogWrite (motorD3, 0);
  analogWrite (motorD4, 0);

  // Enabling motors
  digitalWrite (enable, HIGH);

  delay (1000);
  t_start = millis();
}

void counter() {
  count++;
}

void loop() {
  /* Lectura Bluetooth. Paso de la referencia desde el ordenador hasta el vehiculo */

  if (BT.available()) {
    // Leer un dato (esto seria vref). No podemos printear pedir d ese guarda en el archivo y no es lo que queremos
    char c = BT.read();
    if (c >= '0' && c <= '9') data += c; // Control de error

    if (c == '\r') {
      
      if (data.length() > 0){
      vel_cp = data.toFloat();
      last_data = data;
      }
      data = "";
    }
  }


  t_1 = millis();

  if ((t_1 - t_0) >= T) { // En cada instante de muestreo

    /* Obtener velocidad */
    // Lectura de la velocidad y conversion a rev/s
    w_leida = ((float) count / 20.0) / ((t_1 - t_0) / 1000.0); // he metido aqui el pi

    // Filtrar lectura
    if (w - w_leida > 1 || w_leida - w > 1) {
      w = 0.2 * w_leida + 0.8 * w;
    } else w = w_leida;

    // Velocidad lineal
    v = w * r;

    count = 0;

    /* Control */
    if (t_1 - t_start < 5000) {
      pwm = 255;
    } else if (t_1 - t_start < 7000) {
      pwm = 200;
    } else {
      pwm = 150;
    }

    /* Aplicar seniales */
    analogWrite (motorI1, pwm);
    analogWrite (motorD3, pwm);

    /* Escribir datos */
    linea = String(pwm) + "," + String(v, 4) + "," + last_data + "," + String(t_1 - t_start) + "\n\r"; // pwm, v, vref, t
    BT.write(linea.c_str());

    t_0 = t_1; //Actualizamos el valor

  }


  delay (10); //0.3s

}
