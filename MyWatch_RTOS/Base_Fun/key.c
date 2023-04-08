#include "Base.h"

uint8_t sw = 0;
_Bool Alarm_Flag = 0;

//按键中断检测
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//HAL_Delay(10);
	//power_mode更改，系统关机
	if (SW0 == 0 | SW1 == 0 | SW2 == 0 | SW3 == 0)
	{
		xSemaphoreGiveFromISR(Key_StateHandle,0);
		No_OperationTime = 200;
	}
//	else if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) && Alarm_Flag)
//		xTaskResumeFromISR(AlarmGo_TaskHandle);
}

void Key_Scan(void const * argument)
{
	while (1)
	{
		if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) && Alarm_Flag && !IsAlarmGo)
			vTaskResume(AlarmGo_TaskHandle);
		if (osSemaphoreWait(Key_StateHandle,10) == osOK)
		{
			osDelay(20);
			osMutexWait(SW_SignalHandle,osWaitForever);
			
			if (SW0 == 0)
				sw = 1;
			else if (SW1 == 0)
				sw = 1<<1;
			else if (SW2 == 0)
				sw = 1<<2;
			else if (SW3 == 0)
				sw = 1<<3;
			
			osMutexRelease(SW_SignalHandle);
		}
	}
}
