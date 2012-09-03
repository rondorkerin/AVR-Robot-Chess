/*
NKC Electronics 20x4 Serial LCD test

20x4 Serial LCD pinout for UART serial communication
pin 1: RX
pin 2: GND
pin 3: +5V

connect pin1 to Arduino digital pin 1
connect pin2 to Arduino GND
connect pin3 to Arduino +5V

More info:
[...]

*/

#include <SoftwareSerial.h>

// since the LCD does not send data back to the Arduino, we should only define the txPi
#define txPin 1 // White wire from Serial LCD screen
const int LCDdelay = 2; // conservative, 2 actually works

SoftwareSerial LCD(0, txPin);

void lcdPosition(int row, int col) {
LCD.write(0xFE); //command flag
LCD.write((col + row*64 + 128)); //position
delay(LCDdelay);
}

/*
void lcdPositionLine1() {
LCD.write(0xFE); //command flag
LCD.write(0x45);
LCD.write(0x00);
delay(LCDdelay);
}
*/

void lcdPositionLine2() {
LCD.write(0xFE); //command flag
LCD.write(0x45);
LCD.write(0x40);
delay(LCDdelay);
}

void lcdPositionLine3() {
LCD.write(0xFE); //command flag
LCD.write(0x45);
LCD.write(0x14);
delay(LCDdelay);
}

void lcdPositionLine4() {
LCD.write(0xFE); //command flag
LCD.write(0x45);
LCD.write(0x54);
delay(LCDdelay);
}

void clearLCD(){
LCD.write(0xFE); //command flag
LCD.write(0x51); //clear command.
delay(LCDdelay);
}

void serCommand(){ //a general function to call the command flag for issuing all other commands
LCD.write(0xFE);
}

void setLCDContrast() {
LCD.write(0xFE); //command flag
LCD.write(0x52);
LCD.write(40); //value 1 to 50 (50 is highest contrast)
delay(LCDdelay);
}

void setLCDBrightness() {
LCD.write(0xFE); //command flag
LCD.write(0x53);
LCD.write(5); //value 1 to 8
delay(LCDdelay);
}

void setup()
{
Serial.begin(57600);

pinMode(txPin, OUTPUT);
LCD.begin(9600);

//lcdPosition(0,0);
clearLCD();
setLCDContrast();
setLCDBrightness();

LCD.print("Hello line 1");
lcdPositionLine2();
LCD.print("W33333333333333rrrrrrrrrrrrdddddddd");
lcdPositionLine3();
LCD.print("Pop it like its hot");

}

void loop()
{
}

