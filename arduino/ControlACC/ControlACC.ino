//Control por realimentación del vector de estados. Control de Vehiculos (MII/MIERA. 2020-2021)

//Pines
const int SensorPin=2;

const int enable = 1;   // Enable motores (uno para todos)
const int motorI1 = 3;  // PWM1 motores izquierda
const int motorI2 = 10; // PWM2 motores izquierda
const int motorD3 = 9;  // PWM3 motores derecha
const int motorD4 = 11; // PWM4 motores derecha

/*     Direcciones motores   */
/*     / Atras    /  Alante  */
/* I1 /     0     /    1     */
/* I2 /     1     /    0     */
/* D3 /     0     /    1     */
/* D4 /     1     /    0     */

//Constantes de control y del sistema
const float T=0; //Tiempo de muestreo en milisegundos
const float r=0; //Radio de la rueda
const float K1=0; //Ganancia del controlador [K,KI]
const float K2=0;
const float K3=0;
const float K4=0;
const float ref_d = 0;
const float ref_v=0;
const float Kp=0;
const float Ki=0;
const float Aimax=0;
const float Aimin=0;
//Variables de estados y señal de control
float Dx3=0;
float x1=0;
float x2=0;
float x3=0;
float x4=0;
float w=0; //Velocidad angular
float U=0; //Señal de control x4
float U1=0; //Señal de control
int vel_cp=0;
float error=0;
float int_error=0;
int pwm=0;
float Ai=0;
//Variables auxiliares
unsigned long t_0=0;
unsigned long t_1=0;
int count = 0;
bool change= false; //La idea es pasar esto por bluetooh o progamarlo para que cambie al cabo de cierto tiempo
               //para asegurarse que se ha alcanzado la velocidad de consigna
void setup() {
  
  Serial.begin (9600);
  attachInterrupt(digitalPinToInterrupt(SensorPin),counter,RISING);

  // Motores
  pinMode (enable, OUTPUT);
  pinMode (motorI1, OUTPUT);
  pinMode (motorI2, OUTPUT);
  pinMode (motorD3, OUTPUT);
  pinMode (motorD4, OUTPUT);

  digitalWrite (enable, LOW);
  analogWrite (motorI1, 0);
  analogWrite (motorI2, 0);
  analogWrite (motorD3, 0);at
  analogWrite (motorD4, 0);

  delay (1000);
}

void counter(){
   count++;
}

void loop() {
  // Habilitar motores
  digitalWrite (enable, HIGH);
  
  //Lectura Bluetooth. Paso de la referencia desde el ordenador hasta el vehiculo
  if (Serial.available() > 0) {
      String buff = "";
      while (Serial.available() > 0) {
        buff += (char)Serial.read();
      }
      vel_cp = buff.toInt();
    }

  
  t_1=millis();
  
  if((t_1-t_0)>=T){
    //Lectura de la velocidad y conversion a rev/s
    w = ((float) count/20.0)/((t_1-t_0)/1000.0);
    count = 0;
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
      U=Kp*error+Ai;
    }
    else{
      int_error=0;//Se resetea la integral del error de velocidad
      x2=w*r-ref_v; //x2 debe ser 0 al inicio
      x1 += ((t_1-t_0)/1000.0)*(vel_cp-(w*r)); //Calculo de x1 realizando la integral
      Dx3=ref_d-x1; //Cuando se calcule KI en Matlab hay que cambiar el signo
      x3+=Dx3*((t_1-t_0)/1000.0);
      x4+=x3*((t_1-t_0)/1000.0);
  
      U=-K1*x1-K2*x2+K3*x3+K4*x4;}

    //Habria que ver como se implementa el PWM
    U1=U/4.0; //U1 es tracción. Hay que pasar a señal PWM.

    // Enviar PWM
    pwm = 0; // TODO: esto hay que implementarlo arriba y borrar el 0 este
    analogWrite (motorI1, pwm);
    analogWrite (motorD3, pwm);

    t_0=t_1; //Actualizamos el valor
  }
  delay(10); //No se si esto habria que quitarlo
}
