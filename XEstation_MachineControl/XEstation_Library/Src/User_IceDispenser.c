/*
 * User_IceDispenser.c
 *
 *  Created on: 2023. 1. 25.
 *      Author: Jerry
 */


#include "User_IceDispenser.h"

void init_iceDispensor(void)
{
	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_SET);	// Relay Open

	HAL_GPIO_WritePin(ICE01_water_GPIO_Port, ICE01_water_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ICE01_ice_GPIO_Port, ICE01_ice_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ICE01_mix_GPIO_Port, ICE01_mix_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
}

void lever_control(uint32_t exit_time)
{
	// lever on
	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_RESET);	// Relay Close
	HAL_Delay(exit_time); // unit : ms

	// lever off
	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_SET);	// Relay Open
}

void iceDispenser_SetMode(uint16_t water_time, uint16_t ice_time)
{
	uint16_t wait_time = 500; // 5sec

	if(ice_time == 0){
		HAL_GPIO_WritePin(ICE01_water_GPIO_Port, ICE01_water_Pin, GPIO_PIN_RESET);
		HAL_Delay(wait_time);
		HAL_GPIO_WritePin(ICE01_water_GPIO_Port, ICE01_water_Pin, GPIO_PIN_SET);
		printf("mode : water\n");
	}
	else if(water_time == 0){
		HAL_GPIO_WritePin(ICE01_ice_GPIO_Port, ICE01_ice_Pin, GPIO_PIN_RESET);
		HAL_Delay(wait_time);
		HAL_GPIO_WritePin(ICE01_ice_GPIO_Port, ICE01_ice_Pin, GPIO_PIN_SET);
		printf("mode : ice\n");
	}
	else{
		HAL_GPIO_WritePin(ICE01_mix_GPIO_Port, ICE01_mix_Pin, GPIO_PIN_RESET);
		HAL_Delay(wait_time);
		HAL_GPIO_WritePin(ICE01_mix_GPIO_Port, ICE01_mix_Pin, GPIO_PIN_SET);
		printf("mode : mix\n");
	}
}

bool stopAll(void)
{
	// lever off
	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_SET);	// Relay Open

	return true;
}

uint8_t getStatus_iceDispensor(void)
{
	return ERROR;
}

bool startExit(uint32_t water_time, uint32_t ice_time)
{
	// set mode - ice, water, mix
	iceDispenser_SetMode(water_time, ice_time);

	// set exit time
	uint32_t exit_time = 0;
	if(water_time > ice_time)
		exit_time = water_time;
	else
		exit_time = ice_time;
	printf("exit_time : %d\n", exit_time);

	// lever control
	lever_control(exit_time);

	return true;
}

