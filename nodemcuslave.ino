#include <Wire.h>

#define SDA_PIN 4
#define SCL_PIN 5
//red=sda=d1
const int16_t I2C_SLAVE = 0x10;

void setup() {

  Serial.begin(9600);
  Wire.begin(SDA_PIN,SCL_PIN,I2C_SLAVE);
  Wire.onReceive(receiveD);
  Wire.onRequest(sendD);
  Serial.println("Ready");
}

void loop() { 
}
void receiveD(size_t h) {
  (void) h;
  while (Wire.available()) { // loop through all but the last
    int c = Wire.read(); // receive byte as a character
    Serial.println(c);         // print the character
  }
}
void sendD()
{
  Wire.write(5);
}
