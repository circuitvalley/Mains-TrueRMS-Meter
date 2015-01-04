
#include "nokia_3315.h"
#include "english_6x8_pixel.h"
#include "eusci_b0_spi_5959.h"
/*-----------------------------------------------------------------------
LCD_init
-----------------------------------------------------------------------*/

void delay_1us(void)
{
   unsigned int i;
  for(i=0;i<100;i++);
   
}

  void delay_1ms(void)
  {
   unsigned int i;
   for (i=0;i<1140;i++);
  }
  
void delay_nms(unsigned int n)
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1ms();
  }

void LCD_init(void)
  {

    LCD_DC_DIR |=   LCD_DC ;
	LCD_MOSI_DIR |= LCD_MOSI;
	LCD_CLK_DIR |=   LCD_CLK;
	LCD_CS_DIR |= LCD_CS;
	EUSCI_B_SPI_masterInit(EUSCI_B_SPI_CLOCKSOURCE_SMCLK,16000000, 8000000, EUSCI_B_SPI_MSB_FIRST,EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, EUSCI_B_SPI_3PIN);
	 EUSCI_B_SPI_enable();
    
    LCD_CS_OUT &= ~LCD_CS;
    delay_1us();

   LCD_CS_OUT |=  LCD_CS;

    LCD_write_byte(0x21, 0);
    LCD_write_byte(0xc8, 0);
    LCD_write_byte(0x06, 0);
    LCD_write_byte(0x13, 0);
    LCD_write_byte(0x20, 0);
  LCD_write_byte(0x0c, 0);
  LCD_write_byte(0x40, 0);
  LCD_write_byte(0x80, 0);
  }

/*-----------------------------------------------------------------------
LCD_clear
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

 //  LCD_write_byte(0x0C, 0);
 //   LCD_goto_XY(0,0);
    for (i=0; i<504; i++)
      LCD_write_byte(0xAA, 1);
  }

/*-----------------------------------------------------------------------
LCD_goto_XY
-----------------------------------------------------------------------*/
void LCD_goto_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);		// row
    LCD_write_byte(0x80 | X, 0);          	// cloumn
  }

/*-----------------------------------------------------------------------
LCD_write_char

-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

    c -= 32;

    for (line=0; line<6; line++)
      LCD_write_byte(font6x8[c][line], 1);
  }

/*-----------------------------------------------------------------------
LCD_write_english_String

-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_goto_XY(X,Y);
    while (*s) 
      {
	 LCD_write_char(*s);
	 s++;
      }
  }

/*-----------------------------------------------------------------------
LCD_write_byte

-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {

	LCD_CS_OUT &= ~LCD_CS;	// msp430

	   if (command == 0)
		  LCD_DC_OUT &= ~LCD_DC;
	    else
		 LCD_DC_OUT |= LCD_DC;


    		// insert boundary check here
    while(!EUSCI_B_SPI_getInterruptStatus(EUSCI_B_SPI_TRANSMIT_INTERRUPT));
    		   EUSCI_B_SPI_transmitData(dat);
    	while(EUSCI_B_SPI_isBusy()); 			// wait for all to finish
	 LCD_CS_OUT |=  LCD_CS;
  }
void printV(uint16_t voltsXten , uint8_t x,uint8_t y){
		printbigc(10,x+67,y);
		printbigc(voltsXten%10,x+52,y);
		LCD_goto_XY(x+46,y+1);

		LCD_write_byte(0x3C,1);
		LCD_write_byte(0x3C,1);
		LCD_write_byte(0x3C,1);
		LCD_write_byte(0x3C,1);
		voltsXten = voltsXten/10;
		printbigc(voltsXten%10,x+30,y);
		voltsXten = voltsXten/10;
		printbigc(voltsXten%10,x+15,y);
		voltsXten = voltsXten/10;
		printbigc(voltsXten%10,x,y);
}
void printHz(uint16_t HzXhundred,uint8_t x,uint8_t y)
{
	printbigc(11,x+67,y);
	printbigc(HzXhundred%10,x+52,y);
	HzXhundred = HzXhundred/10;
	printbigc(HzXhundred%10,x+37,y);
	LCD_goto_XY(x+31,y+1);
	LCD_write_byte(0x3C,1);
	LCD_write_byte(0x3C,1);
	LCD_write_byte(0x3C,1);
	LCD_write_byte(0x3C,1);
	HzXhundred = HzXhundred/10;
	printbigc(HzXhundred%10,x+15,y);
	HzXhundred = HzXhundred/10;
	printbigc(HzXhundred%10,x,y);

}

void printbigc(unsigned char r,unsigned char x,unsigned char y)
{
	unsigned char i,j;
	    	for(j=0;j<2;j++)
	    	{
	    		LCD_goto_XY(x,y+j);
			for ( i = 0; i < 13; i++ ){
				LCD_write_byte(FONTBIG[r][j][i],1);
	    	        }
	    	}
	//doSPI(LCD_DATA | 0); // 1px gap after each letter

}


