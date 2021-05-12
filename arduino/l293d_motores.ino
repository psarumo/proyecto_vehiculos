/* Manejo de motores con L293D*/

// Pines Motor A
int enableA = 5;
int MotorA1 = 6; //IN1A L293D
int MotorA2 = 7; //IN2A L293D
 
// Pines Motor B
int enableB = 8; //IN1B L293D
int MotorB1 = 9; //IN2B L293D
int MotorB2 = 10;

/*     / Motors directions */
/*     / Right    /  Left  */
/* IN1 /    0     /   1    */
/* IN2 /    1     /   0    */
 
void setup() {
  
  Serial.begin (9600);
  
  pinMode (enableA, OUTPUT);
  pinMode (MotorA1, OUTPUT);
  pinMode (MotorA2, OUTPUT);  
  
  pinMode (enableB, OUTPUT);
  pinMode (MotorB1, OUTPUT);
  pinMode (MotorB2, OUTPUT);  
  
}
 
void loop() {
  // Enabling motors
  Serial.println ("Enabling Motors");
  
  digitalWrite (enableA, HIGH);
  digitalWrite (enableB, HIGH);
  digitalWrite (MotorA1, LOW);
  digitalWrite (MotorA2, LOW);
  digitalWrite (MotorB1, LOW);
  digitalWrite (MotorB2, LOW);

  // Motion Forward
  Serial.println ("Motion Forward");
  
  digitalWrite (MotorA1, LOW);
  digitalWrite (MotorA2, HIGH);
 
  digitalWrite (MotorB1, LOW);
  digitalWrite (MotorB2, HIGH);
  
  delay (200); // 0.2s

  // Stop
  Serial.println ("Stoping motors");
  
  digitalWrite (enableA, LOW);
  digitalWrite (enableB, LOW);
  
  delay (300); //0.3s

  // Enabling motors
  Serial.println ("Running motors");
  
  digitalWrite (enableA, HIGH);
  digitalWrite (enableB, HIGH);

  // Motion Backwards
  Serial.println ("Motion Backwards");
  
  digitalWrite (MotorA1,HIGH);
  digitalWrite (MotorA2,LOW);  
  
  digitalWrite (MotorB1,HIGH);
  digitalWrite (MotorB2,LOW);  
 
  delay (200); //0.2s

  // Stop
  Serial.println ("Stoping motors");
  
  digitalWrite (enableA, LOW);
  digitalWrite (enableB, LOW);
  
  delay (300); //0.3s
}