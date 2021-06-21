/*--------------------------------------------------------------------------*/
/* Manejo de motores con L293D*/
/*--------------------------------------------------------------------------*/

// Pines Motores
int enable = 13; 
int motorI1 = 3;
int motorI2 = 10;
int motorD3 = 9;
int motorD4 = 11;


/*     Direcciones motores   */
/*     / Atras    /  Alante  */
/* I1 /     0     /    1     */
/* I2 /     1     /    0     */
/* D3 /     0     /    1     */
/* D4 /     1     /    0     */

void setup() {
  
  //Serial.begin (9600);
  
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

  delay (1000);
}
 
void loop() {
  // Enabling motors
  //Serial.println ("Enabling Motors");
  
  digitalWrite (enable, HIGH);

  // Motion Forward
  //Serial.println ("Motion Forward");

  
//  for (int i = 150; i <= 255; i++){
//
//  analogWrite (motorI1, i);
//  analogWrite (motorD3, i);
//
//  delay (50);
//  }

  analogWrite (motorI1, 255);
  analogWrite (motorD3, 255);
  
  delay (5000); // 0.2s

//     analogWrite (motorI1, 0);
//
//   delay (5000); // 0.2s

   analogWrite (motorI1, 100);
   analogWrite (motorD3, 100);

   delay (5000); // 0.2s

  // Stop
  Serial.println ("Stoping motors");
  
//  digitalWrite (enable, LOW);
//  
delay (50); //0.3s

}
