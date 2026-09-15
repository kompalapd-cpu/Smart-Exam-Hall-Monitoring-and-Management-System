#include <LPC21xx.h>
#include "types_t.h"
#include "adc_mpt.h"
#include "adc_defines_mpt.h"
#include "delay.h"
//#include "pin_connect_block.h"

void Init_ADC(void)
{
    /* Configure P0.27 as AD0.0 */
PINSEL1 &= ~(3 << 22);
PINSEL1 |=  (1 << 22);

/* Configure P0.28 as AD0.1 */
PINSEL1 &= ~(3 << 24);
PINSEL1 |=  (1 << 24);

    /* ADC Operational Mode */
    ADCR = PDN_BIT | CLKDIV_VALUE;
}

void Read_ADC(u32 CHNO, u32 *AdcDval, f32 *eAR)
{
    u32 timeout = 50000;

    ADCR &= ~(0xFF);
    ADCR |= CHNO;
    ADCR |= START_CONV;

    delay_us(3);

    while((((ADDR >> DONE_BIT) & 1) == 0) && (--timeout));

    ADCR &= ~(START_CONV);

    *AdcDval = (ADDR >> RESULT) & 0x3FF;
    *eAR = ((*AdcDval) * 3.3) / 1024.0;
}
