#include "types_t.h"

void LED_Init(void);
void LED_All_OFF(void);

void Green_LED_ON(void);
void Yellow_LED_ON(void);
void Red_LED_ON(void);

void LED_Status(u32 examDuration, u32 examRemaining);
void PAUSED_CALLin(void);
void PAUSED_CALLoff(void);
void LED_CALL(unsigned int , unsigned int, unsigned int  );
