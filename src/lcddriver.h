#ifndef    LCD_H 

// Driver for HD44780 controller. R/W pin is gnd'ed. 

#define    LCD_H 
   
   
// This implementation uses Port A for the Data port, all 8 bits of it
#define DATA_PORT PORTF


// This implementation uses Port B for the Control port
#define LCD_RS PIN5_bm		/* RS on pin PB3 */
#define LCD_E  PIN4_bm		/* E on pin PB1 */
#define COMM_PORT PORTE


   
// lcd.h
// Created by Frank Van Hooft Sept 2010
// to support the lcd.c file

void lcd_set_write_instruction();
void lcd_set_write_data();
void lcd_write_byte (char c);
void lcd_clear_and_home();
void lcd_home();
void lcd_goto(uint8_t line, uint8_t pos);
void lcd_line_one();
void lcd_line_two();
void lcd_write_data(char c);
void lcd_write_string(char *x, uint8_t len );
void lcd_write_string_0(char *x);
void lcd_write_string_p(const char *s);
void lcd_init();

#endif  // End  #ifndef    LCD_H
