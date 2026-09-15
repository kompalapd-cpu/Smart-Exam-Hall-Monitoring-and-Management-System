#include<LPC21xx.h>
#include "types_t.h"
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
	
	#include<stdlib.h>

u8 emergency_exit = 0;

#define check (key!=0 && key!='=' && key!='-' && key!='C')


void RTC_EDIT(void){
	char key,hour[3],min[3],sec[3],date[3],day[2],mon[3],year[5];  // Local buffers for time/date entries

	EDIT :  CmdLCD(0x01);             // Clear LCD
	        Goto(1,1);             // Go to first line
	        StrLCD("1.H 2.M 3.S 4.D"); // Display main edit menu
          Goto(2,1);
          StrLCD("5.DY 6.M 7.Y 8.E");  // Display remaining edit options
          
	 while(1){
		 
		 key = Keypad_GetKey();
		 if(key!=0){                  // If a key is pressed
			 switch(key){             // Check which option was selected

//=========================== EDIT HOUR ==============================
				 case '1' : 
					 rtc_hour:CmdLCD(0x01);       // Clear LCD
				            StrLCD("ENTER : HOUR"); // Prompt user to enter hour 
				            Goto(2,1);

				         while(1){                 // Wait for first digit
									 First:key = Keypad_GetKey();
									 if(check){        // Valid key check
				            hour[0]=key;           // Store first digit
				            CharLCD(hour[0]);     // Display it on LCD
										break; 
									 }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;} // Emergency exit
								 }
									 while(1){       // Wait for second digit
										 second:key = Keypad_GetKey();
										if(key != 0){
											if(key == '-'){    // Backspace operation
												Goto(2,1);
												CharLCD(' ');
												Goto(2,1);
												goto First;
											}else if(key == '='){ hour[1]='\0'; goto h;} // Enter pressed
											 else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											hour[1] = key;      // Store 2nd digit
											CharLCD(hour[1]);  // Display 2nd digit
											break;
										}
										}
										while(1){     // Wait for confirmation or error handling
											key = Keypad_GetKey();
											if(key != 0){
												if(key == '='){        // '=' means confirm entry
													hour[2] = '\0';
													h:if(atoi(hour)>=0 && atoi(hour)<24){ // Validate 0–23
													HOUR =atoi(hour);   // Update RTC hour register
													goto EDIT;}         // Go back to main menu
													else{               // Invalid hour
														CmdLCD(0x01);
														StrLCD("!!! ERROR !!!");
														Goto(2,1);
														StrLCD("ENTER : 1.BACK");
														while(1){
															key = Keypad_GetKey();
														if(key == '1')
															goto rtc_hour;  // Retry input
												 }
													}
												}else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
												 else if(key == '-'){   // Backspace again
													Goto(2,2);
													CharLCD(' ');
													Goto(2,2);
													goto second;
												}
											}
										}//case 1

//=========================== EDIT MINUTES ==============================
				 case '2': 
					 rtc_min:  CmdLCD(0x01);
				            StrLCD("ENTER : MIN"); 
				            Goto(2,1);
				         while(1){
									 First_min:
									 key = Keypad_GetKey();
									 if(check){
				            min[0]=key;
				            CharLCD(min[0]);
										break; 
									 }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
								 }
									 while(1){
										 second_min:
										 key = Keypad_GetKey();
										if(key != 0){
											if(key == '-'){
												Goto(2,1);
												CharLCD(' ');
												Goto(2,1);
												goto First_min;
											}else if(key == '='){ min[1]='\0'; goto m;}
											else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											min[1] = key;
											CharLCD(min[1]);
											break;
										}
										}
										while(1){
											key = Keypad_GetKey();
											if(key != 0){
												if(key == '='){
													min[2] = '\0';
													m:if(atoi(min)>=0 && atoi(min)<=59){
													MIN =atoi(min);  // Update RTC minutes
													goto EDIT;}
													else{            // Invalid minutes
														CmdLCD(0x01);
														StrLCD("!!! ERROR !!!");
														Goto(2,1);
														StrLCD("ENTER : 1.BACK");
														while(1){
														//key = Keypad_GetKey();
															key = Keypad_GetKey();
														if(key == '1')
															goto rtc_min;
												 }
													}}
												else if(key == '-'){
													Goto(2,2);
													CharLCD(' ');
													Goto(2,2);
													goto second_min;
												}else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											}
										}//case2			

//=========================== EDIT SECONDS ==============================
          case '3': 
					 rtc_sec:  CmdLCD(0x01);
				            StrLCD("ENTER : SEC"); 
				            Goto(2,1);
				         while(1){
									 First_sec:
									 key = Keypad_GetKey();
									 if(check){
				            sec[0]=key;
				            CharLCD(sec[0]);
										break; 
									 }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
								 }
									 while(1){
										 second_sec:
										 key = Keypad_GetKey();
										if(key != 0){
											if(key == '-'){
												Goto(2,1);
												CharLCD(' ');
												Goto(2,1);
												goto First_sec;
											}else if(key == '='){ sec[1]='\0'; goto s;}
										 	else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											sec[1] = key;
											CharLCD(sec[1]);
											break;
										}
										}
										while(1){
											key = Keypad_GetKey();
											if(key != 0){
												if(key == '='){
													sec[2] = '\0';
													s:	if(atoi(sec)>=0 && atoi(sec)<=59){
													SEC =atoi(sec);    // Update seconds
													goto EDIT;}
													else{              // Invalid seconds
														CmdLCD(0x01);
														StrLCD("!!! ERROR !!!");
														Goto(2,1);
														StrLCD("ENTER : 1.BACK");
														while(1){
														key = Keypad_GetKey();
														if(key == '1')
															goto rtc_sec;
												 }
													}}
												else if(key == '-'){
													Goto(2,2);
													CharLCD(' ');
													Goto(2,2);
													goto second_sec;
												}else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											}
										}//case3	

//=========================== EDIT DATE ==============================
           case '4': 
					 rtc_date:  CmdLCD(0x01);
				            StrLCD("ENTER : DATE"); 
				            Goto(2,1);
				         while(1){
									 First_date:
									 key = Keypad_GetKey();
									 if(check){
				            date[0]=key;
				            CharLCD(date[0]);
										break; 
									 }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
								 }
									 while(1){
										 second_date:
										 key = Keypad_GetKey();
										if(key != 0){
											if(key == '-'){
												Goto(2,1);
												CharLCD(' ');
												Goto(2,1);
												goto First_date;
											}else if(key == '='){ date[1]='\0'; goto d;}
											else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											date[1] = key;
											CharLCD(date[1]);
											break;
										}
										}
										while(1){
											key = Keypad_GetKey();
											if(key != 0){
												if(key == '='){
													date[2] = '\0';
													d:if(atoi(date)>0 && atoi(date)<=31){
													DOM =atoi(date);  // Update date
													goto EDIT;}
													else{             // Invalid date
														CmdLCD(0x01);
														StrLCD("!!! ERROR !!!");
														Goto(2,1);
														StrLCD("ENTER : 1.BACK");
														while(1){
														key = Keypad_GetKey();
														if(key == '1')
															goto rtc_date;
												 }
													}}
												else if(key == '-'){
													Goto(2,2);
													CharLCD(' ');
													Goto(2,2);
													goto second_date;
												}else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											}
										}//case4		

//=========================== EDIT DAY OF WEEK ==============================
					 case '5' :
						 rtc_day : CmdLCD(0x01);
					             StrLCD("   CHOOSE DAY"); 
					             delay_ms(400);
					             CmdLCD(0x01);
					             StrLCD("0.Su 1.M 2.T 3.W"); // Day options
					             Goto(2,1);
					             StrLCD("4.T 5.F 6.Sa");
					             while(1){
												 key = Keypad_GetKey();
												 if(check){
													 day[0]=key;
													 day[1]='\0';
													 if(atoi(day)>=0 && atoi(day)<7){ // Valid day 0–6
														 DOW = atoi(day);
														 goto EDIT;}
													 else{
														 CmdLCD(0x01);
														 StrLCD("!!! ERROR !!!");
														 goto rtc_day;
													 }
												 }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											 }//case 5

//=========================== EDIT MONTH ==============================
						case '6': 
					 rtc_mon:  CmdLCD(0x01);
				            StrLCD("ENTER : MONTH"); 
				            Goto(2,1);
				         while(1){
									 First_mon:key = Keypad_GetKey();
									 if(check){
				            mon[0]=key;
				            CharLCD(mon[0]);
										break; }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
									 }
									 while(1){
										 second_mon:key = Keypad_GetKey();
										if(key != 0){
											if(key == '-'){
												Goto(2,1);
												CharLCD(' ');
												Goto(2,1);
												goto First_mon;
											}else if(key== '='){mon[1]='\0'; goto M;}
											else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											mon[1] = key;
											CharLCD(mon[1]);
											break;
										}
										}
										while(1){
											key = Keypad_GetKey();
											if(key != 0){
												if(key == '='){
													mon[2] = '\0';
													M:		if(atoi(mon)>0 && atoi(mon)<=12){
													MONTH =atoi(mon); // Update month
													goto EDIT;}
													else{             // Invalid month
														CmdLCD(0x01);
														StrLCD("!!! ERROR !!!");
														Goto(2,1);
														StrLCD("ENTER : 1.BACK");
														while(1){
														key = Keypad_GetKey();
														if(key == '1')
															goto rtc_mon;
												 }
													}}
												else if(key == '-'){
													Goto(2,2);
													CharLCD(' ');
													Goto(2,2);
													goto second_mon;
												}else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											}
										}//case6 

//=========================== EDIT YEAR ==============================
						case '7':
						rtc_year:  CmdLCD(0x01);
				               StrLCD("ENTER : YEAR"); 
				               Goto(2,1);
										while(1){
				  	First_year: key = Keypad_GetKey();
					             if(check){
                       year[0]=key;
                       CharLCD(year[0]);	
                       break;}
											 else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
										 }
                   while(1){
										  second_year:key = Keypad_GetKey();
										if(key != 0){
											if(key == '-'){
												Goto(2,1);
												CharLCD(' ');
												Goto(2,1);
												goto First_year;
											}else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											year[1] = key;
											CharLCD(year[1]);
											break;
										}
										}
									 while(1){
										 Third_year:key = Keypad_GetKey();
										 if(key!=0){
											 if(key == '-'){
												 Goto(2,2);
												 CharLCD(' ');
												 Goto(2,2);
												 goto second_year;
											 }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											 year[2]= key;
											 CharLCD(year[2]);
											 break;
										 }
									 }
									 while(1){
										 Fourth_year:key = Keypad_GetKey();
										 if(key !=0){
											 if(key == '-'){
												 Goto(2,3);
												 CharLCD(' ');
												 Goto(2,3);
												 goto Third_year;
											 }else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											 year[3] = key;
											 CharLCD(year[3]);
											 break;
										 }
									 }
									 while(1){
											key = Keypad_GetKey();
											if(key != 0){
												if(key == '='){
													year[4] = '\0';
													if(atoi(year)<=2025){ 
													 CmdLCD(0x01);StrLCD("! WARNING !");
													 Goto(2,1);
													 StrLCD("GOING TO PAST");
													 YEAR =atoi(year);
													 delay_s(1);
													 goto EDIT;}
													if(atoi(year)>0 && atoi(year)<=4095){
													YEAR =atoi(year);   // Update year
													goto EDIT;
													}
													else{
														CmdLCD(0x01);
														StrLCD("!!! ERROR !!!");
														Goto(2,1);
														StrLCD("ENTER : 1.BACK");
														while(1){
														key = Keypad_GetKey();
														if(key == '1')
															goto rtc_year;
												   }
													}
												}
												else if(key == '-'){
													Goto(2,4);
													CharLCD(' ');
													Goto(2,4);
													goto Fourth_year;
												}else if(key=='C'){emergency_exit=1;CmdLCD(0x01); return;}
											}
										} //case7

//=========================== EXIT FROM EDIT MODE ==============================
						case '8' : 
              return;                  // Exit menu normally
						case 'C' :CmdLCD(0x01); emergency_exit =1; return; // Emergency exit via 'C'						
									 
}//switch
}//waiting for key
		 }//while
	 }//rtc_edit
