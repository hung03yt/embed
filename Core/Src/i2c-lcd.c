
#include "i2c-lcd.h"
#include "i2c.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"


#define SLAVE_ADDRESS_LCD 0x27 // change this according to ur setup

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	
	for(uint8_t i = 0; i < 4; i++){
		i2c1_Write_Byte (SLAVE_ADDRESS_LCD, data_t[i]);
	}
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	
	for(uint8_t i = 0; i < 4; i++){
		i2c1_Write_Byte (SLAVE_ADDRESS_LCD, data_t[i]);
	}
}

void lcd_init (void)
{
	lcd_send_cmd (0x33); /* set 4-bits interface */
	lcd_send_cmd (0x32);
	HAL_Delay(50);
	lcd_send_cmd (0x28); /* start to set LCD function */
	HAL_Delay(50);
	lcd_send_cmd (0x01); /* clear display */
	HAL_Delay(50);
	lcd_send_cmd (0x06); /* set entry mode */
	HAL_Delay(50);
	lcd_send_cmd (0x0c); /* set display to on */	
	HAL_Delay(50);
	lcd_send_cmd (0x02); /* move cursor to home and set data address to 0 */
	HAL_Delay(50);
	lcd_send_cmd (0x80);
}

void lcd_send_string(char *str, ...)
{
	char StrBuff[20];
	va_list ap;
	va_start(ap, str);
	vsnprintf(StrBuff, sizeof(StrBuff), (char*)str, ap);
	va_end(ap);
	
	str = StrBuff;
	while(*str)
	{
		lcd_send_data(*str ++);
	}
}

void lcd_clear_display (void)
{
	lcd_send_cmd (0x01); //clear display
	HAL_Delay(5);
}

void lcd_goto_XY (int row, int col)
{
	uint8_t pos_Addr;
	if(row == 1) 
	{
		pos_Addr = 0x80 + row - 1 + col;
	}
	else
	{
		pos_Addr = 0x80 | (0x40 + col);
	}
	lcd_send_cmd(pos_Addr);
	HAL_Delay(5);
}
