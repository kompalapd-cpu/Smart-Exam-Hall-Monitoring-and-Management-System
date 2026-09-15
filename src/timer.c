#include<LPC21XX.h>
#include "timer.h"
#include "7seg_mpt.h"
#include "led_mpt.h"
#include "buzzer_mpt.h"

unsigned int t_sec =0;
unsigned int duration =0;
 unsigned int timer_led =0;
void timer0_isr(void) __irq {
	T0IR = 1;
	
	if(duration > 0) {
	   t_sec++;
		if(t_sec >= 30000){
			t_sec=0;
			duration--;
		}
		Display_2Digit(duration);
		LED_CALL(timer_led,duration,t_sec);
	}
	else{
		 IOSET1 = 0xFF000000;
		 IOCLR0 = (3<<20);
		 LED_All_OFF();
		 Buzzer_OFF();
		 timer0_stop();
	}
	VICVectAddr = 0;
}


void timer0_init(void){
T0TCR = 0X02;
T0PR = 15000-1;
T0MR0 = 1;	

	T0MCR = 0X03;
	
	VICVectAddr2 = (unsigned int)timer0_isr;
	VICVectCntl2 = 0x20 |4;
	VICIntEnable = (1<<4); 
	
	
}

void timer0_start(void){
	T0TCR=0X01;
}

void timer0_stop(void){
	T0TCR=0X00;
}

