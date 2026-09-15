#include <LPC21xx.h>
#include "types_t.h"
#include "led_mpt.h"
#include "lcd_t.h"
#include "delay.h"
#include "buzzer_mpt.h"

#define PAUSED 0
#define GREEN_LED   2
#define YELLOW_LED  3
#define RED_LED     4
#define PAUSE_LED   25

extern unsigned int duration; 
int t_sec_01=0;
	
void LED_Init(void)
{
	//PINSEL0 &= ~(0x3F << 6);
  PINSEL0 &= ~0x00000003;

	IODIR0 |= (1<<GREEN_LED) |
	          (1<<YELLOW_LED) |
	          (1<<RED_LED) |(1<<PAUSED) | (1 << PAUSE_LED);

	LED_All_OFF();
}

void LED_All_OFF(void)
{
	IOCLR0 = (1<<GREEN_LED) |
	         (1<<YELLOW_LED) |
	         (1<<RED_LED);
}

void Green_LED_ON(void)
{
	LED_All_OFF();
	IOSET0 = (1<<GREEN_LED);
	IOCLR0 = (1<<YELLOW_LED);
	IOCLR0 = (1<<RED_LED);
}

void Yellow_LED_ON(void)
{
	LED_All_OFF();
	IOSET0 = (1<<YELLOW_LED);
	IOCLR0 = (1<<GREEN_LED);
	IOCLR0 = (1<<RED_LED);
}

void Red_LED_ON(void)
{
	LED_All_OFF();
	IOSET0 = (1<<RED_LED);
	IOCLR0 = (1<<YELLOW_LED);
	IOCLR0 = (1<<GREEN_LED);
	
}

void LED_Status(u32 examDuration, u32 examRemaining)
{
	u32 percent = 0;

	if(examDuration == 0)
	{
		LED_All_OFF();
		return;
	}

	percent = (examRemaining * 100UL) / examDuration;

	if(percent > 50)
	{
		Green_LED_ON();
	}
	else if(percent > 30)
	{
		Yellow_LED_ON();
	}
	else
	{
		Red_LED_ON();
	}
}
void PAUSED_CALLin(void){
	 IOSET0 = (1<<PAUSED);
}
void PAUSED_CALLoff(void){
	IOCLR0 = (1<<PAUSED);
}


void LED_CALL(unsigned int whole, unsigned int part,  unsigned int sec){
	u32 percentage=0;
	u32 total_time_ms;
	u32 remain_time_ms;

	if (whole == 0) {
		LED_All_OFF();
		Buzzer_OFF();
		return;
	}

	total_time_ms = whole * 30000;
	if (part > 0) {
		remain_time_ms = (part * 30000) - sec;
	} else {
		remain_time_ms = 0;
	}

	percentage = (remain_time_ms * 100) / total_time_ms;
	
	if(percentage > 50){
		Green_LED_ON();	
		Buzzer_OFF();
	}
	else if(percentage > 30){
		Yellow_LED_ON();
		Buzzer_OFF();
	}
	else if(percentage > 10){
		Red_LED_ON();
		Buzzer_OFF();
	}
	else if(percentage > 0 ){
		if ((sec % 500) < 250) {
			IOSET0 = (1<<RED_LED);
			IOCLR0 = (1<<YELLOW_LED);
			IOCLR0 = (1<<GREEN_LED);
			Buzzer_ON();
		} else {
			LED_All_OFF();
			Buzzer_OFF();
		}
	}
	else{
		 LED_All_OFF();
		 Buzzer_OFF();
	}
}


