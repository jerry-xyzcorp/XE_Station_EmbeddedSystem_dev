/*
 * User_IceDispenser.c
 *
 *  Created on: 2023. 1. 25.
 *      Author: Ethan
 */


#include "User_IceDispenser.h"

void relayTest(void)
{
	uint32_t time = 5000; // 5sec
	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_SET);	// Relay Close
	HAL_Delay(time);
//	sprintf(uartBuf, "Relay Close\r\n\n");			//convert to string
//	HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);		//transfer the string to PC // (interface, address, buffer size, timeout)

	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_RESET);	// Relay Open
	HAL_Delay(time);
//	sprintf(uartBuf, "Relay Open\r\n\n");			//convert to string
//	HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);		//transfer the string to PC // (interface, address, buffer size, timeout)
}
void iceDispenser_SetMode(void)
{
	uint32_t time = 1000; // 5sec

	HAL_GPIO_WritePin(ICE01_water_GPIO_Port, ICE01_water_Pin, GPIO_PIN_RESET);
//	sprintf(uartBuf, "water\r\n\n");			//convert to string
//	HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);		//transfer the string to PC // (interface, address, buffer size, timeout)
	HAL_Delay(500);
	HAL_GPIO_WritePin(ICE01_water_GPIO_Port, ICE01_water_Pin, GPIO_PIN_SET);


	HAL_GPIO_WritePin(ICE01_mix_GPIO_Port, ICE01_mix_Pin, GPIO_PIN_RESET);
//	sprintf(uartBuf, "MIX\r\n\n");			//convert to string
//	HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);		//transfer the string to PC // (interface, address, buffer size, timeout)
	HAL_Delay(500);
	HAL_GPIO_WritePin(ICE01_mix_GPIO_Port, ICE01_mix_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(ICE01_ice_GPIO_Port, ICE01_ice_Pin, GPIO_PIN_RESET);
//	sprintf(uartBuf, "ice\r\n\n");			//convert to string
//	HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);		//transfer the string to PC // (interface, address, buffer size, timeout)
	HAL_Delay(500);
	HAL_GPIO_WritePin(ICE01_ice_GPIO_Port, ICE01_ice_Pin, GPIO_PIN_SET);
}


void init_iceDispensor(void)
{

}
bool stopAll(void)
{
	return true;
}
uint8_t getStatus_iceDispensor(void)
{
	return ERROR;
}

bool startExit(uint16_t water_time, uint16_t ice_time)
{
	//set mode - ice, water, mix
	if(water_time == 0){

	}
	else if(ice_time == 0){

	}
	else{

	}
	// lever on
	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_SET);	// Relay Close

	HAL_Delay(1); // unit : ms
	// lever off
	HAL_GPIO_WritePin(ICE01_relay_GPIO_Port, ICE01_relay_Pin, GPIO_PIN_RESET);	// Relay Open
}

