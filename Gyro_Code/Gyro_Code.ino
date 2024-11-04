//Gyro controller by Dana Maloney and Thomas Moeller: Micro II FA24
//Based on code by Michael Schoeffler: http://www.mschoeffler.de
//I2C MP board registers identified using https://invensense.tdk.com/wp-content/uploads/2015/02/RM-MPU-9250A-00-v1.6.pdf

#include "Wire.h" // I2C Library.

const int MPU_ADDR = 0x68; // I2C address of the MP9250 with AD0 set LOW.

//int16_t accelerometer_x, accelerometer_y, accelerometer_z; //Accelerometer data: We don't use this.
int16_t gyro_x, gyro_y, gyro_z; // Gyro data. These are the bytes we care about.
int16_t temperature; // Used for temperature, again not used for our purposes.
int incomingByte = 0; // Used to store data coming from computer for buzzer.
unsigned long startMillis = 0; // Used for buzzer timing.



char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  pinMode(3, OUTPUT); // Buzzer out
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to gyro (slave/secondary)
  Wire.write(0x6B); // Register 107 = power management reg
  Wire.write(0); // 0 = not in sleep mode.
  Wire.endTransmission(true);
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); // Reg 67 = gyro data start. Use address 59 for accelerometer.
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 3*2, true); // request a total of 3*2=6 registers
  
 
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  // Check for forces strong enough to trigger up/down movement.
  if (gyro_x > 8000) {
    Serial.print("S\n");
  } else if (gyro_x <  -8000) {
    Serial.print("W\n");
  }

  // Check for forces strong enough to trigger left/right movement.
  if(gyro_y > 8000) {
    Serial.print("D\n");
  } else if (gyro_y < -8000) {
    Serial.print("A\n");
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

  //Shut off buzzer if it's been on for 250ms+
  if (millis() > startMillis + 250) {
    digitalWrite(3, LOW);
  }

  // delay before polling again
  delay(100);
}