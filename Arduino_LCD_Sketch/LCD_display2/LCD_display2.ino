/* NKC Electronics 16x2 Serial LCD test 16x2 Serial LCD pinout for UART serial communication pin 1: RX pin 2: GND pin 3: +5V connect pin1 to Arduino digital pin 1 connect pin2 to Arduino GND connect pin3 to Arduino +5V Default communication speed is 9600 */ 
char ESC = 0xFE; 
void setup() 
{ 
    Serial.begin(9600); // Initialize LCD module 
    Serial.write(ESC);
    Serial.write(0x41); 
    Serial.write(ESC); 
    Serial.write(0x51); // Set Contrast 
    Serial.write(ESC); 
    Serial.write(0x52); 
    Serial.write(40); // Set Backlight 
    Serial.write(ESC); 
    Serial.write(0x53); 
    Serial.write(8); 
    Serial.print(" NKC Electronics"); // Set cursor line 2, column 0 
    Serial.write(ESC); 
    Serial.write(0x45); 
    Serial.write(0x40); 
    Serial.print(" 16x2 Serial LCD"); 
} 

void loop() { } 
