#include <LPC21xx.h>
#include "types_t.h"
#include "7seg_mpt.h"
#include "delay.h"
#include "interrupts_p.h"
#include "defines.h"
#include "led_mpt.h"
#include "lcd_t.h"
#include "lcd_defines_t.h"

#define CA7SEG_2_MUX   8
#define DSEL1          20
#define DSEL2          21

u8 examRunning = 0;
u8 examCompleted = 0;
u32 percent;
u32 lastExamSecond = 0;
u8 examPaused=0;

int flag_pass = 1;


extern u32 examDuration;
extern u32 examRemaining;
extern u32 last_h;
extern u32 last_m;
extern u32 last_s;
extern u32 pausedcounter;
const u8 segLUT[10] =
{
    0xFC,   //0
    0x60,   //1
    0xDA,   //2
    0xF2,   //3
    0x66,   //4
    0xB6,   //5
    0xBE,   //6
    0xE0,   //7
    0xFE,   //8
    0xF6    //9
};

/*const u8 segLUT[10] =
{
    0xC0,   //0
    0xF9,   //1
    0xA4,   //2
    0xB0,   //3
    0x99,   //4
    0x92,   //5
    0x82,   //6
    0xF8,   //7
    0x80,   //8
    0x90    //9
};*/

void Init_7Segs(void)
{
	PINSEL2 &= ~(0x0000003F);

	IODIR1 |= 0xFF000000;

	IODIR0 |= (1<<20);
	IODIR0 |= (1<<21);

	IOCLR0 = (1<<20);
	IOCLR0 = (1<<21);

	IOSET1 = 0xFF000000;
}

void Display_2Seg(u8 num)
{
	IOCLR0 = (1<<20);
	IOCLR0 = (1<<21);

	IOSET1 = 0xFF000000;

	IOCLR1 = ((u32)(segLUT[num] & 0xFF)) << 24;

	IOSET0 = (1<<21);
}

void Display_1Seg(u8 num)
{
    IOCLR0 = (1<<20);
    IOCLR0 = (1<<21);

    IOSET1 = 0xFF000000;

    IOCLR1 = ((u32)(segLUT[num] & 0xFF)) << 24;

    IOSET0 = (1<<20);
}

void Display_2Digit(u8 value)
{	
    static u8 mux = 0;

    u8 tens;
    u8 ones;
	
//		if(examRunning == 0){
//			IOCLR0 = (1<<20);
//			IOCLR0 = (1<<21);

//			IOSET1 = 0xFF000000;

//			return;
//		}

    tens = value / 10;
    ones = value % 10;

    if(mux == 0)
    {
        Display_1Seg(tens);
        mux = 1;
    }
    else
    {
        Display_2Seg(ones);
        mux = 0;
    }
}

void StartExamTimer(void)
{
    if(examDuration == 0)
        return;

    examRemaining = examDuration;

    lastExamSecond =
        (HOUR * 3600UL) +
        (MIN * 60UL) +
        SEC;

    examRunning = 1;
    examCompleted = 0;

    LED_Status(examDuration, examRemaining);
}

void ExamTimer_Task(void)
{
    u32 currentSecond;
    u32 elapsed;

    if(examRunning == 0) // flag
        return;
    
	
    currentSecond =
        (HOUR * 3600UL) +
        (MIN * 60UL) +
        SEC; //present time 

    if(currentSecond >= lastExamSecond){
				
        elapsed = currentSecond - lastExamSecond;
		}
    else
        elapsed = (86400UL - lastExamSecond) + currentSecond;
		
    while(elapsed >= 60)
    {
        elapsed = 0;

        lastExamSecond += 60;

        /*if(lastExamSecond >= 86400UL)
            lastExamSecond -= 86400UL;*/

        if(examRemaining > 0)
            examRemaining--;

        LED_Status(examDuration, examRemaining);

        if(examRemaining == 0)
        {
            examRunning = 0;
            examCompleted = 1;
            break;
        }
    }
}
