//kpm_mp.c

#include <lpc21xx.h>
#include "kpm_mp.h"
#include "lcd_t.h"
#include "types_t.h"
#include "7seg_mpt.h"
#include "delay.h"

#define R1 (1<<16)
#define R2 (1<<17)
#define R3 (1<<18)
#define R4 (1<<19)

#define C1 (1<<20)
#define C2 (1<<21)
#define C3 (1<<22)
#define C4 (1<<23)

//for proteus connections

/*char keymap[4][4] =
{
    {'7','8','9','/'},
    {'4','5','6','*'},
    {'1','2','3','-'},
    {'C','0','=','+'}
};*/

extern char pass[5];
extern u32 examRemaining;


//for board connections
char keymap[4][4]=
{
    {'1','2','3','/'},
    {'4','5','6','*'},
    {'7','8','9','-'},
    {'C','0','=','+'}
};

void Keypad_Init(void)
{
    // Rows as Output
    IODIR1 |= (R1|R2|R3|R4);

    // Columns as Input
    IODIR1 &= ~(C1|C2|C3|C4);


    // Pull rows High
    IOSET1 = (R1|R2|R3|R4);
}

char Keypad_GetKey(void)
{
    unsigned int row;
 //unsigned int col;

    while(1)
    {
        for(row=0;row<4;row++)
        {
            IOSET1 = (R1|R2|R3|R4);

            switch(row)
            {
                case 0: IOCLR1=R1; break;
                case 1: IOCLR1=R2; break;
                case 2: IOCLR1=R3; break;
                case 3: IOCLR1=R4; break;
            }

            if(!(IOPIN1&C1))
            {
                while(!(IOPIN1&C1));
                delay_ms(20);
                return keymap[row][0];
            }

            if(!(IOPIN1&C2))
            {
                while(!(IOPIN1&C2));
                delay_ms(20);
                return keymap[row][1];
            }

            if(!(IOPIN1&C3))
            {
                while(!(IOPIN1&C3));
                delay_ms(20);
                return keymap[row][2];
            }

            if(!(IOPIN1&C4))
            {
                while(!(IOPIN1&C4));
                delay_ms(20);
                return keymap[row][3];
            }
        }
			}
		}

/*---------------------------------------------------------
 Function : Read_Input()
 Purpose  : Read numbers from keypad with Backspace
            '*' -> Backspace
 startPos  : LCD starting address (0x80 or 0xC0)
---------------------------------------------------------*/

void Read_Input(char *buf,u8 len,u8 startPos,u8 mode)
{
    u8 idx = 0;
    char key;
	
    CmdLCD(startPos);

    while(1)
    {
			
        key = Keypad_GetKey();

        /* Digit Pressed */
        if((key >= '0') && (key <= '9'))
        {
            if(idx < len)
            {
                buf[idx] = key;

                CmdLCD(startPos + idx);
							
                if(mode == 1)
                    CharLCD('*');
                else
                    CharLCD(key);

                idx++;
            }
        }

        /* Backspace */
        else if(key == '*')
        {
            if(idx > 0)
            {
                idx--;

                CmdLCD(startPos + idx);
                CharLCD(' ');
                CmdLCD(startPos + idx);
            }
        }

        /* Finish Input */
        else if(key == '=')
        {
            if(idx > 0)
            {
                buf[idx] = '\0';
                return;
            }
        }
    }
}

		

