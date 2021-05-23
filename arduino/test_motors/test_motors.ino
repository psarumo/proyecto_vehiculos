// Prueba motores sin puete H

int motor_lu = 3;
int motor_ld = 9;
int motor_ru = 10;
int motor_rd = 11; 

void setup() {
//  Serial.begin (9600);
  pinMode(motor_lu, OUTPUT);
  pinMode(motor_ld, OUTPUT);
  pinMode(motor_ru, OUTPUT);
  pinMode(motor_rd, OUTPUT);
}

void loop() {
 // analogWrite(motor_lu, 255);
  analogWrite(motor_ld, 255);
 // analogWrite(motor_ru, 255);
 // analogWrite(motor_rd, 255);

  delay(10);
}
