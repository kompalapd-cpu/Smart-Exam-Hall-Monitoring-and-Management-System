//lcd_t.c

#include <LPC21xx.h>
#include "types_t.h"
#include "lcd_defines_t.h"
#include "defines.h"
#include "delay.h"

//void WriteLCD(u8 byte)
//{
// //write to data pins
// WRITEBYTE(IOPIN0,LCD_DATA,byte);
// //select write operations
// IOCLR0=1<<LCD_RW;
// //provide high to low enable pulse
// IOSET0=1<<LCD_EN;
// delay_us(1);
// IOCLR0=1<<LCD_EN;
// delay_ms(2);
//}

void WriteLCD(u8 byte)
{
    IOCLR0 = (0xFF << LCD_DATA);

    IOSET0 = ((u32)byte << LCD_DATA);

    IOCLR0 = (1<<LCD_RW);

    IOSET0 = (1<<LCD_EN);
    delay_us(10);
    IOCLR0 = (1<<LCD_EN);

    delay_ms(2);
}

void CmdLCD(u8 opcode)
{
  IOCLR0=1<<LCD_RS;
  WriteLCD(opcode); 
}

void InitLCD(void)
{
  IODIR0|=((0xFF<<LCD_DATA)|
          (1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
 
  delay_ms(50);
  CmdLCD(0x30);
  delay_ms(5);

  delay_us(200);
  CmdLCD(0x30);
  delay_us(200);
  CmdLCD(0x30);
  CmdLCD(MODE_8BIT_2LINE);
  CmdLCD(DSP_ON_CUR_OFF);
  CmdLCD(CLEAR_LCD);
  delay_ms(2);
  CmdLCD(SHIFT_CUR_RIGHT);
}
void CharLCD(u8 asciiVal)
{
 //set rs pin for data register select
 IOSET0=1<<LCD_RS;
 //write to ddram via data reg via data pins
 WriteLCD(asciiVal);
}

void StrLCD(s8 *str)
{
 while(*str)
  CharLCD(*str++);
}

void U32LCD(u32 num)
{
  u8 a[10];
  s32 i=0;
  if(num==0)
  {
  CharLCD('0');
  }  
 else
 {
  while(num>0)
  {
   a[i++]=(num%10)+48;
   num/=10;
  }
  for(--i;i>=0;i--)
   CharLCD(a[i]);
 }
}

void Goto( unsigned char Row, unsigned char Col){
	 unsigned char Address;
	 if(Row == 1){
		 Address = GOTO_LINE1_POS0 + (Col - 1); }
	 else {
	  Address = GOTO_LINE2_POS0 + (Col - 1); }
	 CmdLCD(Address);
}

//void S32LCD(s32);
//void F32LCD(f32,u32);
//void BuildCGRAM(s8 *,u32 );
