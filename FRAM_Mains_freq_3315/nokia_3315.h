

#ifndef __nokia_3315_h_
#define __nokia_3315_h_
#include <stdint.h>

#include <msp430.h>

#define LCD_CLK_DIR 	P2DIR
#define LCD_MOSI_DIR 	P1DIR
#define LCD_CS_DIR		P3DIR
#define LCD_DC_DIR		P1DIR

#define LCD_CLK_OUT		P2OUT
#define LCD_MOSI_OUT	P1OUT
#define LCD_CS_OUT		P3OUT
#define LCD_DC_OUT		P1OUT

#define LCD_CLK			BIT2	//port 2
#define LCD_MOSI		BIT6	//port 2
#define LCD_CS			BIT0	//port 3
#define LCD_DC			BIT5	//port 1


void LCD_init(void);
void LCD_clear(void);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void LCD_goto_XY(unsigned char X, unsigned char Y);
void printHz(uint16_t HzXhundred,uint8_t x,uint8_t y);

void LCD_write_char(unsigned char c);
void LCD_write_byte(unsigned char dat, unsigned char dc);
void delay_1us(void);                 
void printV(uint16_t voltsXten , uint8_t x,uint8_t y);
void printbigc(unsigned char r,unsigned char x,unsigned char y);
#endif
