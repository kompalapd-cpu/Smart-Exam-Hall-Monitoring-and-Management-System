	//main.c

	#include <LPC21xx.h>
	#include "types_t.h"
	#include "lcd_t.h"
	#include "lcd_defines_t.h"
	#include "kpm_mp.h"
	#include "rtc_mpt.h"
	#include "interrupts_p.h"
	#include "delay.h"
	#include "7seg_mpt.h"
	#include "adc_mpt.h"
	#include "lm35_mpt.h"
	#include "led_mpt.h"
	#include "buzzer_mpt.h"
  #include "timer.h"

	extern u32 examDuration;
	extern u32 examRemaining;
  extern u32 t_sec;
	extern u32 duration;
	extern volatile u8 pauseFlag;

	s32 hour, min, sec;
	s32 date, month, year;
	s32 day;
	f32 tempC;
	
	// Not needed right now
	//int sec_value = 0;
	//int sec_value_captured = 1;
	
  int click = 0;
	s32 last_sec = -1;

	int main(){
		InitLCD();
		Keypad_Init();
		RTC_Init();
		Init_ADC();
	  timer0_init();
		
		SetRTCTimeInfo(10,30,00);
		SetRTCDateInfo(16,07,26);
		SetRTCDay(4);

		Interrupt_Init();
		Init_7Segs();
		LED_Init();
		Buzzer_Init();

		CmdLCD(CLEAR_LCD);
		StrLCD("EXAM MONITORING");
		CmdLCD(0xC0);
		StrLCD("     SYSTEM");
		delay_ms(1500);
		CmdLCD(CLEAR_LCD);
		
		while(1){
		
			
			// Not needed right now
			/*if(sec_value_captured){
				last_sec = SEC;
				while(SEC < (last_sec + 1)){
					sec_value += 1;
				}
				sec_value_captured = 0;
			}*/
		//	CmdLCD(CLEAR_LCD);
			GetRTCTimeInfo(&hour,&min,&sec);
			GetRTCDateInfo(&date,&month,&year);
			GetRTCDay(&day);
			
			if(adminFlag==0){
				if (sec != last_sec) {
					last_sec = sec;
					tempC = Read_LM35DegC();
					
					DisplayRTCTime(hour,min,sec,day);
					StrLCD("    ");
					DisplayRTCDate(date,month,year);
							CharLCD(' ');
			
					U32LCD((u32)tempC);        // Display temperature
					CharLCD(0xDF);             // Degree symbol
					CharLCD('C');
					StrLCD("   ");
				}
			}

			if(adminFlag){
				adminFlag=0;
				Password_Check();
				CmdLCD(CLEAR_LCD);
				last_sec = -1;
			}
			
			if(pauseFlag){
				pauseFlag = 0;
				if(duration > 0){
					click = !click;
					if(click){
						timer0_stop();
						IOSET0 = 1 << 25;
						PAUSED_CALLin();
					}
					else{
						timer0_start();
						IOCLR0 = 1 << 25;
						PAUSED_CALLoff();
					}
				}
			}

			if(duration == 0 && click != 0){
				timer0_stop();
				PAUSED_CALLoff();
				click = 0;
			}  
	}
}

