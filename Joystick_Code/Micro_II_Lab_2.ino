//Authors: Dana Maloney, Thomas Moeller
// Micro 2 FA24 Lab 2
#include <time.h>

int VRx = A0;
int VRy = A1;
int xPosition;
int yPosition;
int incomingByte = 0;
unsigned long startMillis;

void setup() {
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  // Serial.println("");
  // Serial.println("X Position");
  // Serial.println(xPosition);
  // Serial.println("Y Position");
  // Serial.println(yPosition);
  // delay(3000);

  if (xPosition >= 400 && xPosition <= 700 && yPosition <= 100){
    Serial.println("W");
    //delay(1000);
  }
  else if (xPosition >= 400 && xPosition <= 700 && yPosition >= 900){
    Serial.println("S");
    //delay(1000);
  }
  else if (yPosition >= 400 && yPosition <= 700 && xPosition <= 100){
    Serial.println("A");
    //delay(1000);
  }
  else if (yPosition >= 400 && yPosition <= 700 && xPosition >= 900){
    Serial.println("D");
    //delay(1000);
  }

  //check if apple was eaten
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (incomingByte == 'A'){
      startMillis = millis();
      digitalWrite(3, HIGH);
    }
    Serial.println(incomingByte);
  }

  if (millis() > startMillis + 250) {
    digitalWrite(3, LOW);
  }

}