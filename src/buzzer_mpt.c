#include <LPC21xx.h>
#include "types_t.h"
#include "buzzer_mpt.h"
#include "delay.h"

#define BUZZER 23       // P0.23

void Buzzer_Init(void)
{
    IODIR0 |= (1<<BUZZER);

    /* OFF initially */
    IOCLR0 = (1<<BUZZER);
}

void Buzzer_ON(void)
{
    IOSET0 = (1<<BUZZER);
}

void Buzzer_OFF(void)
{
    IOCLR0 = (1<<BUZZER);
}

void Buzzer_Beep(u32 times)
{
    u32 i;

    for(i=0; i<times; i++)
    {
        Buzzer_ON();
        delay_ms(200);

        Buzzer_OFF();
        delay_ms(200);
    }
}
