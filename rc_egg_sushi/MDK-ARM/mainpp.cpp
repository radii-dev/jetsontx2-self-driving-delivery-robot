/*
 * main.cpp

 *
 *  Created on: 2018/01/02
 *      Author: yoneken
 */
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"
#include "mainpp.h"
#include <queue>
#include <memory>
#include "ROS/ros.h"
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>

uint32_t IC1Value_lin, IC2Value_lin;
float cycle_lin, width_lin, dutycycle_lin;
uint32_t IC1Value_ang, IC2Value_ang;
float cycle_ang, width_ang, dutycycle_ang;
char buffer[30];

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

ros::NodeHandle nh;

sensor_msgs::Imu imu;
ros::Publisher pub_imu("imu", &imu);

void setup(void)
{
  nh.initNode();
  nh.advertise(pub_imu);

	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
}

void loop(void)
{

//	sprintf(buffer, "cycle_lin: %.2f\r\n", cycle_lin);
//	CDC_Transmit_FS((uint8_t *)buffer, 20);
//	sprintf(buffer, "width_lin: %.2f\r\n", width_lin);
//	CDC_Transmit_FS((uint8_t *)buffer, 20);
//	sprintf(buffer, "dutycycle_lin: %.2f\r\n", dutycycle_lin);
//	CDC_Transmit_FS((uint8_t *)buffer, 30);
//	sprintf(buffer, "frequency1: %.2f\r\n", frequency1);
//	CDC_Transmit_FS((uint8_t *)buffer, 30);
//	HAL_Delay(500);
	sprintf(buffer, "cycle_ang: %.2f\r\n", cycle_ang);
	CDC_Transmit_FS((uint8_t *)buffer, 20);
	sprintf(buffer, "width_ang: %.2f\r\n", width_ang);
	CDC_Transmit_FS((uint8_t *)buffer, 20);
	sprintf(buffer, "dutycycle_ang: %.2f\r\n", dutycycle_ang);
	CDC_Transmit_FS((uint8_t *)buffer, 30);
	nh.spinOnce();
	HAL_Delay(10);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if(htim -> Instance == TIM2) {
    if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_1) { // if CH1 is rising
      IC1Value_lin = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // Read CH1 value
      TIM2 ->CNT = 0; // Reset Count
      cycle_lin = (float)IC1Value_lin; // cycle_lin of PWM
		}
    else if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_2) { // if CH2 is falling
      IC2Value_lin = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // Read CH2 value
      width_lin = (float) IC2Value_lin; // width_lin of PWM
      dutycycle_lin = 100 * width_lin / cycle_lin; // dutycycle_lin
		}
    else {
      cycle_lin = 0;
      dutycycle_lin = 0;
		}
	}
	
  if(htim -> Instance == TIM3) {
    if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_1) { // if CH1 is rising
      IC1Value_ang = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // Read CH1 value
      TIM3 ->CNT = 0; // Reset Count
      cycle_ang = (float)IC1Value_ang; // cycle_lin of PWM
		}
    else if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_2) { // if CH2 is falling
      IC2Value_ang = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // Read CH2 value
      width_ang = (float) IC2Value_ang; // width_lin of PWM
      dutycycle_ang = 100 * width_ang / cycle_ang; // dutycycle_lin
		}
    else {
      cycle_ang = 0;
      dutycycle_ang = 0;
		}
	}
}

