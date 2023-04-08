#include "Task.h"
#include "tim.h"

uint8_t No_OperationTime = 200;
_Bool OLED_Mode = 1;

//UBaseType_t more[6] = {0};
//读取DS3231时间任务
void Read_Time(void const * argument)
{
	vTaskSuspend(Change_TaskHandle);
	vTaskSuspend(AlarmGo_TaskHandle);
	vTaskSuspend(AlarmSet_Task_Handle);
	vTaskSuspend(Menu_TaskHandle);
	vTaskSuspend(Game_TaskHandle);
	vTaskSuspend(Timing_TaskHandle);
	vTaskSuspend(Cartoon_TaskHandle);
	I2C_DS3231_Init();
	while(1)
	{
		//more[0] = uxTaskGetStackHighWaterMark(NULL);
		
		osMutexWait(Time_SignalHandle,osWaitForever);
		
		I2C_DS3231_getTime();
		
		osMutexRelease(Time_SignalHandle);
		osDelay(10);
	}
}

//显示Root界面
void Display_Root(void const * argument)
{
	static _Bool First_Flag = 0;
	while(1)
	{
		if (!First_Flag)
		{
			OLED_Clear();
			First_Flag = 1;
			Display_Choice(0);
			if (Alarm_Flag)
				OLED_ShowChar(50,7,'A',8);
			HAL_TIM_Base_Start_IT(&htim3);
		}
		
		//more[0] = uxTaskGetStackHighWaterMark(NULL);
		
		if (OLED_Mode && No_OperationTime == 255)
		{
			OLED_Mode = 0;
			vTaskSuspend(Root_TasHandle);
		}
		osMutexWait(SW_SignalHandle,osWaitForever);
		uint8_t x = sw;
		sw = 0;
		osMutexRelease(SW_SignalHandle);
		if (x >= 1 << 2)
		{
			First_Flag = 0;
			if (x == 1 << 3)
				vTaskResume(Change_TaskHandle);
			else
				vTaskResume(Menu_TaskHandle);
			
			HAL_TIM_Base_Stop_IT(&htim3);
			vTaskSuspend(Root_TasHandle);
		}
		osMutexWait(Time_SignalHandle,osWaitForever);
		Root_Motion();
		osMutexRelease(Time_SignalHandle);
		osDelay(15);
	}
}

//显示改变时间的界面
void Display_ChangeTime(void const * argument)
{
	static _Bool First_Flag = 0;
	static uint8_t Place = 0;
	static uint8_t flash = 0;
	while(1)
	{
		if (!First_Flag)
		{
			OLED_Clear();
			First_Flag = 1;
			Display_Choice(1);
			osMutexWait(Time_SignalHandle,osWaitForever);
			TimeNums_Init();
			osMutexRelease(Time_SignalHandle);
			ChangeTime_Motion();
		}
		
		//more[1] = uxTaskGetStackHighWaterMark(NULL);
		
		osMutexWait(SW_SignalHandle,osWaitForever);
		uint8_t x = sw;
		sw = 0;
		osMutexRelease(SW_SignalHandle);
		if (x == 1)
		{
			OLED_ShowTimes(Place < 2 ? Place : Place+1,ChangeNums[Place],1);
			Place++;
			Place %= 8;
		}
		else if (x == 1 << 1)
		{
			ChangeNums[Place]++;
			ChangeNumEffe(Place);
		}
		else if (x >= 1 << 2)
		{
			First_Flag = 0;
			if (x == 1 << 2)
				I2C_DS3231_SetTime(ChangeNums[4],ChangeNums[5],ChangeNums[6],ChangeNums[7],ChangeNums[0]*10+ChangeNums[1],ChangeNums[2]*10+ChangeNums[3],0);
			
			vTaskResume(Root_TasHandle);
			vTaskSuspend(Change_TaskHandle);
		}
		OLED_ShowTimes(Place < 2 ? Place : Place+1,ChangeNums[Place],(++flash/25) % 2);
		osDelay(20);
	}
}

//显示Menu界面
void Display_Menu(void const * argument)
{
	static _Bool First_Flag = 0;
	static uint8_t Place = 0;
	while(1)
	{
		if (!First_Flag)
		{
			OLED_Clear();
			First_Flag = 1;
			Menu_Static();
			Display_MenuLogo(Place);
		}
		
		//more[2] = uxTaskGetStackHighWaterMark(NULL);
		
		osMutexWait(SW_SignalHandle,osWaitForever);
		uint8_t x = sw;
		sw = 0;
		osMutexRelease(SW_SignalHandle);
		if (x < 1 << 2 & x > 0)
		{
			Place += x == 1 ? -1 : 1;
			Place = (Place + Menu_Num) % Menu_Num;
			Display_MenuLogo(Place);
		}
		else if (x >= 1 << 2)
		{
			if (x == 1 << 3)
			{
				Place = 0;
				vTaskResume(Root_TasHandle);
			}
			else
				vTaskResume(*Custom_Features[Place]);
			First_Flag = 0;
			vTaskSuspend(Menu_TaskHandle);
		}
		osDelay(30);
	}
}

//播放动画
void Display_Cartoon(void const * argument)
{
	static uint8_t nums = 0;
	while (1)
	{
		//more[1] = uxTaskGetStackHighWaterMark(NULL);
		Kunkun_dance(nums++);
		nums %= 18;
		osMutexWait(SW_SignalHandle,osWaitForever);
		uint8_t x = sw;
		sw = 0;
		osMutexRelease(SW_SignalHandle);
		if (x)
		{
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(Cartoon_TaskHandle);
		}
		osDelay(20);
	}
}

//读取陀螺仪数据
void Read_Gyro(void const * argument)
{
	while(!MPU6050_Init());
	while(1)
	{
		//more[2] = uxTaskGetStackHighWaterMark(NULL);
		osMutexWait(Gyro_SignalHandle,osWaitForever);
		MPU6050_Read_All(&angle);
		if (!OLED_Mode && (angle.Ay >= 0.80 | angle.Ay <= -1*0.80) && (angle.KalmanAngleY * angle.KalmanAngleY <= 500))
		{
			OLED_Display_On();
			OLED_Mode = 1;
			vTaskResume(Root_TasHandle);
			HAL_TIM_Base_Start_IT(&htim3);
		}
		osMutexRelease(Gyro_SignalHandle);
		osDelay(25);
	}
}

//设置闹钟
void Alarm_Set(void const * argument)
{
	static _Bool First_Flag = 0;
	static uint8_t Place = 0;
	static uint8_t flash = 0;
	static uint8_t AlarmTime[] = {0,0,0,0};
	while(1)
	{
		if (!First_Flag)
		{
			OLED_Clear();
			Place = 0;
			First_Flag = 1;
			Display_Choice(6);
			if (Alarm_Flag)
				I2C_DS3231_ReadAlarm(AlarmTime);
			else
			{
				for (uint8_t i = 0; i < 4; i++)
					AlarmTime[i] = 0;
			}
			Alarm_Init(AlarmTime);
		}
		
		//more[3] = uxTaskGetStackHighWaterMark(NULL);
		
		osMutexWait(SW_SignalHandle,osWaitForever);
		uint8_t x = sw;
		sw = 0;
		osMutexRelease(SW_SignalHandle);
		if (x == 1)
		{
			OLED_ShowTimes(Place < 2 ? Place : Place+1,AlarmTime[Place],1);
			Place++;
			Place %= 4;
		}
		else if (x == 1 << 1)
		{
			AlarmTime[Place]++;
			AlarmTime[Place] %= TimeNumEffe[Place];
		}
		else if (x >= 1 << 2)
		{
			First_Flag = 0;
			if (x == 1 << 2)
			{
				I2C_DS3231_SetAlarm(AlarmTime[0]*10+AlarmTime[1], AlarmTime[2]*10+AlarmTime[3]);
				Alarm_Flag = 1;
			}
			else
			{
				Alarm_Flag = 0;
				I2C_DS3231_ClearAlarm();
			}
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(AlarmSet_Task_Handle);
		}
		OLED_ShowTimes(Place < 2 ? Place : Place+1,AlarmTime[Place],(++flash/25) % 2);
		osDelay(20);
	}
}

_Bool IsAlarmGo = 0;
//闹钟处理
void Alarm_Go(void const * argument)
{
	uint8_t times = 0;
	while (1)
	{
		IsAlarmGo = 1;
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);
		osDelay(500);
		if (times++ >= 31)
		{
			I2C_DS3231_ByteWrite(0x0F, 0x88);
			IsAlarmGo = 0;
			vTaskSuspend(AlarmGo_TaskHandle);
		}
	}
}

//计时
void Timing(void const * argument)
{
	static _Bool On = 0;
	static _Bool First_Flag = 0;
	static _Bool Start = 0;
	while(1)
	{
		if (!First_Flag)
		{
			OLED_Clear();
			First_Flag = 1;
			Display_Choice(5);
			Timing_Init();
			Timing_Motion();
		}
		
		//more[4] = uxTaskGetStackHighWaterMark(NULL);
		
		osMutexWait(SW_SignalHandle,osWaitForever);
		uint8_t x = sw;
		sw = 0;
		osMutexRelease(SW_SignalHandle);
		if (x == 1 << 1)
		{
			Timing_Init();
			Timing_Motion();
		}
		else if (x == 1 << 2)
			On = !On;
		else if (x == 1 << 3)
		{
			On = 0;
			First_Flag = 0;
			vTaskResume(Menu_TaskHandle);
			vTaskSuspend(Timing_TaskHandle);
		}
		if (On)
		{
			if (!Start)
			{
				Start = 1;
				HAL_TIM_Base_Start_IT(&htim4);
			}
			Timing_Motion();
		}
		else
		{
			Start = 0;
			HAL_TIM_Base_Stop_IT(&htim4);
		}
		On ? OLED_ShowString(0,7,"stop ",8) : OLED_ShowString(0,7,"start",8);
		osDelay(20);
	}
}

//游戏
void Game(void const * argument)
{
	static _Bool First_Flag = 0;
	while(1)
	{
		if (!First_Flag)
		{
			OLED_Clear();
			Game_Init();
			First_Flag = 1;
		}
		
		//more[5] = uxTaskGetStackHighWaterMark(NULL);
		
		osMutexWait(SW_SignalHandle,osWaitForever);
		uint8_t x = sw;
		sw = 0; 
		osMutexRelease(SW_SignalHandle);
		if (x != 0) 
		{
			First_Flag = 0;
			goto EndGame;
		}
		if (My_Game())
		{
			Display_Choice(4);
			while (1)
			{	
				osMutexWait(SW_SignalHandle,osWaitForever);
				uint8_t x = sw;
				sw = 0; 
				osMutexRelease(SW_SignalHandle);
				if (x >= 1 << 2)
				{
					First_Flag = 0;
					if (x == 1 << 3)
					{
						EndGame:
						vTaskResume(Menu_TaskHandle);
						vTaskSuspend(Game_TaskHandle);
					}
					break;
				}
				osDelay(20);
			}
		}
		osDelay(20);
	}
}