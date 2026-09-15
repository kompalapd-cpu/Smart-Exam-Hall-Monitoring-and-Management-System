#include "types_t.h"
#include "adc_mpt.h"
#include "adc_defines_mpt.h"
#include "lm35_mpt.h"

f32 Read_LM35DegC(void)
{
    u32 dval;
    f32 eAR;

    Read_ADC(CH1, &dval, &eAR);

    /* LM35 gives 10mV per Degree Celsius */
    return (eAR * 100);
}

f32 Read_LM35DegF(void)
{
    f32 tempC;

    tempC = Read_LM35DegC();

    return ((tempC * 1.8) + 32);
}

/* Reads two LM35 sensors (CH0 & CH1) */
void Read_LM35NT(f32 *tDegC, f32 *tDegF)
{
    u32 dval1, dval2;
    f32 eAR1, eAR2;

    Read_ADC(CH1, &dval1, &eAR1);
    Read_ADC(CH1, &dval2, &eAR2);

    *tDegC = (eAR1 - eAR2) * 100;
    *tDegF = (*tDegC * 1.8) + 32;
}
