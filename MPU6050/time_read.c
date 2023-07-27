#include "mpu6050.h"

MPU6050_t angle = {
	  .Accel_X_RAW = 0,
    .Accel_Y_RAW = 0,
    .Accel_Z_RAW = 0,
    .Ax = 0.0,
    .Ay = 0.0,
    .Az = 0.0,

    .Gyro_X_RAW = 0,
    .Gyro_Y_RAW = 0,
    .Gyro_Z_RAW = 0,
    .Gx = 0.0,
    .Gy = 0.0,
    .Gz = 0.0,

    .Temperature = 0.0,

    .KalmanAngleX = 0.0,
    .KalmanAngleY = 0.0
};

float ay_thre = 0.85;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	MPU6050_Read_All(&angle);
	if (angle.Ay >= ay_thre | angle.Ay <= -1*ay_thre)
	{
		HAL_GPIO_WritePin(GPIOB,LED_Pin,1);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,LED_Pin,0);
	}
}	