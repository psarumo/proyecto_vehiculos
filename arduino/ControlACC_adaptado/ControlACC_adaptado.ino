/*--------------------------------------------------------------------------*/
/* Control por realimentación del vector de estados.
   Control de Vehiculos (MII/MIERA. 2020-2021) */
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
const float K1 = -771.8034279089700; //Ganancia del controlador [K,KI]
const float K2 = 455.7879384367343;
const float K3 = -322.4745512600699;
const float K4 = -51.4071480389171;
const float Kp = 0; // PI
const float Ki = 0; // PI
const float Aimax=0; // Antiwindup
const float Aimin=0; // Antiwindup

/* Variables de estados y señal de control */
float Dx3 = 0; // Variable auxiliar
float x1 = 0;
float x2 = 0;
float x3 = 0;
float x4 = 0;
float w = 0; // Velocidad angular
float w_leida = 0;
float v = 0;
float U = 0; // Señal de control x4
float U1 = 0; // Señal de control
float error = 0;
float int_error = 0;
int pwm = 0;
float Ai = 0;
float ref_d = 5;  // Referencia distancia con vehiculo de delante
float ref_v = 0; // Velocidad vehiculo de delante

/* Variables auxiliares */
unsigned long t_0 = 0;
unsigned long t_1 = 0;
unsigned long t_start = 0;
int count = 0;
bool change= false; //La idea es pasar esto por bluetooh o progamarlo para que cambie al cabo de cierto tiempo
               //para asegurarse que se ha alcanzado la velocidad de consigna

String data = "";
String last_data = "0";
String linea = ",,,";

/* Setup */
void setup() {

  BT.begin(38400); //IMPORTANTE

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
      ref_v = data.toFloat();
      last_data = data;
      }
      data = "";
    }
  }


  t_1 = millis();

  if ((t_1 - t_0) >= T) { // En cada instante de muestreo

    /* Obtener velocidad */
    // Lectura de la velocidad y conversion a rev/s
    w_leida = ((float) count / 20.0) / ((t_1 - t_0) / 1000.0);
    //count = 0;

    // Filtrar lectura
    if (w - w_leida > 1 || w_leida - w > 1) { //No se si este filtro puede dar problemas en caso de que la primera lectura no sea cercana a 0
      w = 0.2 * w_leida + 0.8 * w;
    } else w = w_leida;

    // Velocidad lineal
    v = w * r;

    count = 0;

    /* Control */
    if(change==false){
      x1=0; //Se resetean las variables acumulativas del control de distancia
      x3=0;
      x4=0;
      error=ref_v-(w*r);
      int_error+=error*((t_1-t_0)/1000.0);
      Ai=Ki*int_error;
      //Antiwindup
      if (Ai>=Aimax){
        Ai=Aimax;}
      else if(Ai<=Aimin){
        Ai=Aimin;} 
      //Señal de control
      pwm=Kp*error+Ai;
    }
    else{
      int_error=0;//Se resetea la integral del error de velocidad
      x2=w*r-ref_v; //x2 debe ser 0 al inicio
      x1 += ((t_1-t_0)/1000.0)*(vel_cp-(w*r)); //Calculo de x1 realizando la integral
      Dx3=ref_d-x1; //Cuando se calcule KI en Matlab hay que cambiar el signo
      x3+=Dx3*((t_1-t_0)/1000.0);
      x4+=x3*((t_1-t_0)/1000.0);
  
      pwm=-K1*x1-K2*x2+K3*x3+K4*x4;}

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