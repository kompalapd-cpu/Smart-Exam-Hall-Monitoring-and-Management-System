//rtc_mpt.c

#include <LPC21xx.h>
#include "types_t.h"
#include "rtc_mpt.h"
#include "lcd_t.h"
#include "kpm_mp.h"
#include "interrupts_p.h"   
#include "delay.h"
#include "7seg_mpt.h"
#include "timer.h"
#include "led_mpt.h"

extern u32 examDuration;
extern u32 examRemaining;
extern int sec_value;
extern u32 duration;
extern u32 timer_led;
extern int click;
/* ---------------- DAY TABLE ---------------- */
char week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

/* ---------------- RTC INIT ---------------- */
void RTC_Init(void)
{
    CCR = (1<<1);   // reset RTC

    PREINT  = 0x000001C8;
    PREFRAC = 0x000061C0;

    CCR = (1<<0);   // enable RTC
}

/* ---------------- SET RTC ---------------- */
void SetRTCTimeInfo(u32 hour,u32 minute,u32 second)
{
    HOUR = hour;
    MIN  = minute;
    SEC  = second;
}

void SetRTCDateInfo(u32 date,u32 month,u32 year)
{
    DOM   = date;
    MONTH = month;
    YEAR  = year;
}

void SetRTCDay(u32 day)
{
    DOW = day;
}

/* ---------------- GET RTC ---------------- */
void GetRTCTimeInfo(s32 *hour,s32 *minute,s32 *second)
{
    *hour   = HOUR;
    *minute = MIN;
    *second = SEC;
}

void GetRTCDateInfo(s32 *date,s32 *month,s32 *year)
{
    *date  = DOM;
    *month = MONTH;
    *year  = YEAR;
}

void GetRTCDay(s32 *day)
{
    *day = DOW;
}

/* ---------------- DISPLAY TIME ---------------- */
void DisplayRTCTime(u32 hour,u32 minute,u32 second,u32 day)
{
    CmdLCD(0x80);

    CharLCD((hour/10)+'0');
    CharLCD((hour%10)+'0');
    CharLCD(':');

    CharLCD((minute/10)+'0');
    CharLCD((minute%10)+'0');
    CharLCD(':');

    CharLCD((second/10)+'0');
    CharLCD((second%10)+'0');

    CharLCD(' ');

    if(day > 6) day = 0;

    CharLCD(week[day][0]);
    CharLCD(week[day][1]);
    CharLCD(week[day][2]);
}

/* ---------------- DISPLAY DATE ---------------- */
void DisplayRTCDate(u32 date,u32 month,u32 year)
{
    CmdLCD(0xC0);

    CharLCD((date/10)+'0');
    CharLCD((date%10)+'0');
    CharLCD('/');

    CharLCD((month/10)+'0');
    CharLCD((month%10)+'0');
    CharLCD('/');

    CharLCD(((year%100)/10)+'0');
    CharLCD((year%10)+'0');
}



/* ---------------- EDIT TIME ---------------- */
void Edit_Time(void)
{
    char time[7];
    u8 hour, minute, second;

    CmdLCD(0x01);
    StrLCD("SET TIME");
    CmdLCD(0xC0);

    /* Enter HHMMSS then press '=' */
    Read_Input(time,6,0xC0,0);

    hour   = ((time[0]-'0')*10) + (time[1]-'0');
    minute = ((time[2]-'0')*10) + (time[3]-'0');
    second = ((time[4]-'0')*10) + (time[5]-'0');

    /* Validation */
    if(hour>23 || minute>59 || second>59)
    {
        CmdLCD(0x01);
        StrLCD("INVALID TIME");
        delay_ms(1000);
        return;
    }

    SetRTCTimeInfo(hour,minute,second);

    CmdLCD(0x01);
    StrLCD("TIME UPDATED");
    delay_ms(500);
}

/* ---------------- EDIT DATE ---------------- */
void Edit_Date(void)
{
    char date[7];
    u8 day,month,year;

    CmdLCD(0x01);
    StrLCD("SET DATE");
    CmdLCD(0xC0);

    /* Enter DDMMYY then press '=' */
    Read_Input(date,6,0xC0,0);

    day   = ((date[0]-'0')*10) + (date[1]-'0');
    month = ((date[2]-'0')*10) + (date[3]-'0');
    year  = ((date[4]-'0')*10) + (date[5]-'0');

    /* Validation */
    if(day<1 || day>31 || month<1 || month>12)
    {
        CmdLCD(0x01);
        StrLCD("INVALID DATE");
        delay_ms(1000);
        return;
    }

    SetRTCDateInfo(day,month,year);

    CmdLCD(0x01);
    StrLCD("DATE UPDATED");
    delay_ms(500);
}


/* ---------------- EDIT DAY ---------------- */
void Edit_Day(void)
{
    char day[2];

    CmdLCD(0x01);
    StrLCD("DAY(0-6)");
    CmdLCD(0xC0);

    Read_Input(day,1,0xC0,0);

    if((day[0]-'0')>6)
    {
        CmdLCD(0x01);
        StrLCD("INVALID DAY");
        delay_ms(1000);
        return;
    }

    SetRTCDay(day[0]-'0');

    CmdLCD(0x01);
    StrLCD("DAY UPDATED");
    delay_ms(500);
}

/* ---------------- EXAM DURATION ---------------- */
void Set_Exam_Duration(void)
{
    char dur[4];

    CmdLCD(0x01);
    StrLCD("DURATION MIN");
    CmdLCD(0xC0);

    Read_Input(dur,2,0xC0,0);

    if (dur[1] == '\0') {
        examDuration = dur[0] - '0';
    } else {
        examDuration = ((dur[0]-'0')*10) + ((dur[1]-'0'));
    }

    CmdLCD(0x01);
    StrLCD("DURATION SET");
    delay_ms(500);
    duration = examDuration;
    timer_led = examDuration;
    click = 0;
    PAUSED_CALLoff();
    timer0_start();
}
