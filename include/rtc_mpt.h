//rtc_mpt.h

#include "types_t.h"

/* RTC */
void RTC_Init(void);

/* Set RTC */
void SetRTCTimeInfo(u32 hour,u32 minute,u32 second);
void SetRTCDateInfo(u32 date,u32 month,u32 year);
void SetRTCDay(u32 day);

/* Get RTC */
void GetRTCTimeInfo(s32 *hour,s32 *minute,s32 *second);
void GetRTCDateInfo(s32 *date,s32 *month,s32 *year);
void GetRTCDay(s32 *day);

/* LCD Display */
void DisplayRTCTime(u32 hour,u32 minute,u32 second,u32 day);
void DisplayRTCDate(u32 date,u32 month,u32 year);

/* Admin Functions */
void Edit_Time(void);
void Edit_Date(void);
void Edit_Day(void);
void Set_Exam_Duration(void);
void RTC_EDIT(void);
