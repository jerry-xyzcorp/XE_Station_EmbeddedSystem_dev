/*
 * User_BeverageManufacturing.c
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */
#include "User_beverageManufacturing.h"

extern TIM_HandleTypeDef htim4;


void init_powderMachine(void)
{
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
//	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

bool motorShutdownAll_powderMachine(void)
{

}

uint8_t getStatus_powderMachiner(void)
{

}

bool make_Beverage(uint8_t num_hole, uint16_t gram)
{

}

void L298Ntest(void)
{
	HAL_GPIO_WritePin(PD01_heat_IN1_GPIO_Port, PD01_heat_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PD01_heat_IN2_GPIO_Port, PD01_heat_IN2_Pin, GPIO_PIN_RESET);
	TIM4->CCR3 = 65534; // MAX :65535
	HAL_Delay(5000);
}
