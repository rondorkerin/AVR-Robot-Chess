
#include <SoftwareSerial.h>

// Hook ground of port D of the Xmega to ground of the arduino
// Hook pin 2 of the arduino to pin 3 of port D on the Xmega and run this sketch
// Hook pin 3 of the arduino to pin 2 of port D on the Xmega
// then open serial monitor for debug logs

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()  
{
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
//  if (Serial.avialable())
//    mySerial.write(Serial.read());
  
}
