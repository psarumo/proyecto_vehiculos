const int SensorPin=2;

const float r=64*0.001; //Radio de la rueda (m)
float w=0; //Velocidad angular
const float T=500; //Tiempo de muestreo en milisegundos
float v = 0;

unsigned long t_0=0;
unsigned long t_1=0;
int count = 0;

String msg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  attachInterrupt(digitalPinToInterrupt(SensorPin),counter,RISING);

  delay (1000);
}

void counter(){
   count++;
}

void loop() {
  t_1=millis();

  
    if((t_1-t_0)>=T){
    //Lectura de la velocidad y conversion a rev/s
    w = ((float) count * 2 * 3.1416 /20.0)/((t_1-t_0)/1000.0); // he metido aqui el pi
    v = w*r;
    count = 0;

    msg = "velocidad: " + String(w);
    Serial.println(msg);
    t_0=t_1; //Actualizamos el valor
    }

    delay(10);
}
