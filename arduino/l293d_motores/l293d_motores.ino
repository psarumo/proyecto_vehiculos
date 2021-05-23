/* Manejo de motores con L293D*/

// Pines Motores
int enable = 8; 
int motorI = 9;
int motorD = 10;


void setup() {
  
  Serial.begin (9600);
  
  pinMode (enable, OUTPUT);
  pinMode (motorI, OUTPUT);
  pinMode (motorD, OUTPUT);

  digitalWrite (enable, LOW);

  analogWrite (motorI, 0);
  analogWrite (motorD, 0);

  delay (1000);
}
 
void loop() {
  // Enabling motors
  Serial.println ("Enabling Motors");
  
  digitalWrite (enable, HIGH);

  // Motion Forward
  Serial.println ("Motion Forward");

  
  for (int i = 0; i <= 255; i++){

  analogWrite (motorI, i);
  analogWrite (motorD, i);

  delay (50);
  }
  
  //delay (2000); // 0.2s

  // Stop
  Serial.println ("Stoping motors");
  
  digitalWrite (enable, LOW);
  
  //delay (3000); //0.3s

}
