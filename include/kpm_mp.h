//kpm_mp.h

#include "types_t.h"

void Keypad_Init(void);
char Keypad_GetKey(void);
void Read_Input(char *buf, u8 len, u8 startPos, u8 mode);
