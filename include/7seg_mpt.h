#include "types_t.h"

void Init_7Segs(void);
void Display_2Seg(u8 num);
void Display_1Seg(u8 num);
void DisplayDigit(u8 digit);
void Display_2Digit(u8 value);
void StartExamTimer(void);
void ExamTimer_Task(void);
void ExamStatusLED(void);

/* Countdown Functions */
void Countdown_Init(void);
void Countdown_Task(void);
