// minimal MPU-6050 tilt and roll (sjr). Works with MPU-9250 too.
// works perfectly with GY-521, pitch and roll signs agree with arrows on sensor module 7/2019
//
// Tested with 3.3V eBay Pro Mini with no external pullups on I2C bus (worked with internal pullups)
// Add 4.7K pullup resistors to 3.3V if required. A4 = SDA, A5 = SCL

#include<Wire.h>
const int MPU_addr1 = 0x68;
float xa, ya, za, roll, pitch;

void setup() {
  Wire.setClock(400000);
  Wire.begin();                                      //begin the wire communication
  Wire.beginTransmission(MPU_addr1);                 //begin, send the slave adress (in this case 68)
  Wire.write(0x6B);                                  //make the reset (place a 0 into the 6B register)
  Wire.write(0);
  Wire.endTransmission(true);                        //end the transmission
  Serial.begin(9600);
}

void loop() {

  Wire.beginTransmission(MPU_addr1);
  Wire.write(0x3B);  //send starting register address, accelerometer high byte
  Wire.endTransmission(false); //restart for read
  Wire.requestFrom(MPU_addr1, 6); //get six bytes accelerometer data
  int t = Wire.read();
  xa = (t << 8) | Wire.read();
  t = Wire.read();
  ya = (t << 8) | Wire.read();
  t = Wire.read();
  za = (t << 8) | Wire.read();
// formula from https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing
  roll = atan2(ya , za) * 180.0 / PI;
  pitch = atan2(-xa , sqrt(ya * ya + za * za)) * 180.0 / PI; //account for roll already applied

  Serial.print("roll = ");
  Serial.print(roll,1);
  Serial.print(", pitch = ");
  Serial.println(pitch,1);
  //delay(400);
}
