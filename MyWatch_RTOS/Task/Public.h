#ifndef _Public_H
#define _Public_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Base.h"

extern	osThreadId defaultTaskHandle;
extern	osThreadId RTC_TaskHandle;
extern	osThreadId Root_TasHandle;
extern	osThreadId Change_TaskHandle;
extern	osThreadId Menu_TaskHandle;
extern	osThreadId Cartoon_TaskHandle;
extern	osThreadId Gyro_TaskHandle;
extern	osThreadId AlarmSet_Task_Handle;
extern	osThreadId AlarmGo_TaskHandle;
extern	osThreadId Timing_TaskHandle;
extern	osThreadId Game_TaskHandle;
extern	osMutexId Time_SignalHandle;
extern	osMutexId Gyro_SignalHandle;
extern osMutexId SW_SignalHandle;
extern osSemaphoreId Key_StateHandle;

extern void Read_Time(void const * argument);
extern void Display_Root(void const * argument);
extern void Display_ChangeTime(void const * argument);
extern void Display_Menu(void const * argument);
extern void Display_Cartoon(void const * argument);
extern void Read_Gyro(void const * argument);
extern void Alarm_Set(void const * argument);
extern void Alarm_Go(void const * argument);
extern void Timing(void const * argument);
extern void Game(void const * argument);
extern void Key_Scan(void const * argument);

extern osThreadId *Custom_Features[];

#endif

