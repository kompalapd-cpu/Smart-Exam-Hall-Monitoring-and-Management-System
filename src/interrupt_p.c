//interrupt_p.c

#include <LPC21xx.h>
#include "types_t.h"
#include "lcd_t.h"
#include "lcd_defines_t.h"
#include "kpm_mp.h"
#include "rtc_mpt.h"
#include "delay.h"
#include "interrupts_p.h"
#include "led_mpt.h"
//#include "7seg_mpt.c"

/* ---------------- GLOBAL FLAGS ---------------- */
u32 last_h=0;
u32 last_m=0;
u32 last_s=0;

volatile u8 adminFlag = 0;
volatile u8 pauseFlag = 0;

u32 examDuration = 0;
u32 examRemaining = 0;
char pass[5];
extern u8 examPaused;
extern u32 duration;
/* ---------------- PASSWORD ---------------- */

static char password[5] = {'2','2','2','2','\0'};

/* ---------------- INTERRUPT INIT ---------------- */

void Interrupt_Init(void)
{
    PINSEL0 &= ~((3<<2) | (3<<14));
    PINSEL0 |= ((3<<2) | (3<<14));

    EXTMODE = (1<<0) | (1<<2);
    EXTPOLAR = 0x00;
    EXTINT = 0x0F;

    VICIntSelect &= ~((1<<14)|(1<<16));

    VICVectAddr0 = (u32)EINT0_ISR;
    VICVectCntl0 = (1<<5)|14;

    VICVectAddr1 = (u32)EINT1_ISR;
    VICVectCntl1 = (1<<5)|16;

    VICIntEnable = (1<<14)|(1<<16);
}

/* ---------------- EINT0 ---------------- */

void EINT0_ISR(void)__irq
{
    EXTINT = (1<<0);
    adminFlag = 1;
    VICVectAddr = 0;
}

/* ---------------- EINT1 ---------------- */

void EINT1_ISR(void)__irq //po.7 int2
{
    EXTINT = (1<<2);
    pauseFlag =1;
    VICVectAddr = 0;
}


/* ---------------- PASSWORD ---------------- */
void Password_Check(void)
{
    //char pass[5];
    u8 i;
    u8 ok = 1;

    CmdLCD(CLEAR_LCD);
    StrLCD("ENTER PASS");
    CmdLCD(GOTO_LINE2_POS0);
	
    /* Read password */
    Read_Input(pass,4,GOTO_LINE2_POS0,1);
	
    /* Compare entered password */
    for(i=0; i<4; i++)
    {
        if(pass[i] != password[i])
        {
            ok = 0;
            break;
        }
    }

    if(ok)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("ACCESS OK");
        delay_ms(500);

        /* Open Settings Menu */
        Settings_Menu();
    }
    else
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("WRONG PASS");
        delay_ms(500);
    }
}

/* ---------------- SETTINGS MENU ---------------- */
void Settings_Menu(void)
{
    u8 key;
    u8 page = 1;
   
    while(1)
    {
        CmdLCD(CLEAR_LCD);
        delay_ms(2);
			
			

        /* -------- PAGE 1 -------- */
        if(page == 1)
        {
            CmdLCD(0x80);
            StrLCD("1.RTC EDIT");

            CmdLCD(0xC0);
            StrLCD("2.DURATION");
					
        }

        /* -------- PAGE 2 -------- */
        else if(page == 2)
        {
            CmdLCD(0x80);
            StrLCD("3.PASSWORD");

            CmdLCD(0xC0);
            StrLCD("4.EXIT");
        }

        key = Keypad_GetKey();

        switch(key)
        {
            /* -------- Navigation -------- */

            case '/':
                if(page < 2)
                    page++;
                break;

            case '-':
                if(page > 1)
                    page--;
                break;

            /* -------- Menu Options -------- */

            case '1':
                if(page == 1)
                    RTC_EDIT();
                break;

            case '2':
                if(page == 1)
                    Set_Exam_Duration();
                break;
								
						case '3':
                if(page == 2)
                    Password_Change();
                break;

            case '4':
                if(page == 2)
                    return;
                break;
						case 'C':return;
        }
    }
}

void Password_Change(void){
	CmdLCD(CLEAR_LCD);
  delay_ms(2);
	StrLCD("ENTER PASSWORD:");
	Read_Input(password, 4, GOTO_LINE2_POS0, 1);
}
