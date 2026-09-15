//interrupts_p.h

#include "types_t.h"

/* Global Flags */
extern volatile u8 adminFlag;
extern volatile u8 pauseFlag;

/* Interrupt Initialization */
void Interrupt_Init(void);

/* ISR Functions */
void EINT0_ISR(void)__irq;
void EINT1_ISR(void)__irq;

/* Admin Functions */
void Password_Check(void);
void Settings_Menu(void);

/* RTC Edit Functions (implemented in rtc_mpt.c) */
void Edit_Time(void);
void Edit_Date(void);
void Edit_Day(void);
void Set_Exam_Duration(void);
