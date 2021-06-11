/* Manejo de motores con L293D*/

// Pines Motores
int enable = 13; 
int motorI1 = 3;
int motorI2 = 10;
int motorD3 = 9;
int motorD4 = 11;

const int SensorPin = 2;

const float r=64*0.001; //Radio de la rueda (m)
float w=0; //Velocidad angular
float w_leida = 0;
const float T=100; //Tiempo de muestreo en milisegundos
float v = 0;

unsigned long t_0=0;
unsigned long t_1=0;
unsigned long t_start=0;
int count = 0;

String msg;

void setup() {
  
  Serial.begin (9600);

  attachInterrupt(digitalPinToInterrupt(SensorPin),counter,RISING);
  
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
  Serial.println ("Enabling Motors");
  digitalWrite (enable, HIGH);

      analogWrite (motorI1, 0);
  analogWrite (motorD3, 0);

  delay (1000);
  t_start=millis();
}

void counter(){
   count++;
}
 
void loop() {
  t_1=millis();
  

  analogWrite (motorI1, 255);
  analogWrite (motorD3, 255);
  
    if (t_1-t_start >= 5000){
   analogWrite (motorI1, 150);
   analogWrite (motorD3, 150);
   
    }


    if((t_1-t_0)>=T){
    //Lectura de la velocidad y conversion a rev/s
    w_leida = ((float) count * 2 * 3.1416 /20.0)/((t_1-t_0)/1000.0); // he metido aqui el pi

    // Filtrar lectura
    if (w-w_leida > 1 || w_leida-w>1){
    w = 0.2*w_leida + 0.8*w;
    } else w = w_leida;
    
    v = w*r;
    count = 0;

    msg = "velocidad: " + String(w);
    Serial.println(msg);

    t_0=t_1; //Actualizamos el valor

    
    }
    

delay (10); //0.3s

}
