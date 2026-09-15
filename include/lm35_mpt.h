#include "types_t.h"

/* Read Temperature in Degree Celsius */
f32 Read_LM35DegC(void);

/* Read Temperature in Degree Fahrenheit */
f32 Read_LM35DegF(void);

/* Read Temperature from two LM35 sensors */
void Read_LM35NT(f32 *tDegC, f32 *tDegF);

