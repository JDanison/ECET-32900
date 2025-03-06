/*
 * LCD.h
 *
 *  Created on: Mar 5, 2025
 *      Author: John
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

/* Define Pins */
#define LCD_EN GPIO_PIN_7
#define LCD_RS GPIO_PIN_6
#define LCD_D4 GPIO_PIN_8
#define LCD_D5 GPIO_PIN_9
#define LCD_D6 GPIO_PIN_10
#define LCD_D7 GPIO_PIN_11

/* Functions in LCD.c */
void LCD_STROBE(void);
void lcd_write_cmd(unsigned char);
void lcd_write_data(unsigned char);
void lcd_clear(void);
void lcd_puts(const char * s);
void lcd_putch(char c);
void lcd_goto(int,int);
void lcd_init(void);


#endif /* INC_LCD_H_ */
