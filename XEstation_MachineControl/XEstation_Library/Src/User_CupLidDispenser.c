/*
 * User_CupLidDispenser.c
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */
#include "User_CupLidDispenser.h"

extern TIM_HandleTypeDef htim1;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;


uint8_t dispenser_req_packet[][OFFSET] = {{0x02, 0x01, 0x40, 0x03, 0x44, 0X00, 0X00}, // status check
								  	  	  {0x02, 0x03, 0x41, 0x01, 0x01, 0x03, 0x49}}; // dispatch 1 cup
GPIO_TypeDef* 	SteppingMotor_GPIO_Port[OFFSET] = 	{CupD_STP_GPIO_Port, LidD_STP_GPIO_Port};

uint16_t* 		SteppingMotor_Pin[OFFSET] = 		{CupD_STP_Pin, LidD_STP_Pin};

GPIO_TypeDef* 	MotorSpeedSensor_GPIO_Port[][OFFSET] = {{Cup_up_GPIO_Port, Cup_down_GPIO_Port},
														{Lid_up_GPIO_Port, Lid_down_GPIO_Port}};
uint16_t* 		MotorSpeedSensor_Pin[][OFFSET] = 		{{Cup_up_Pin, Cup_down_Pin},
														{Lid_up_Pin, Lid_down_Pin}};

bool 	is_detected[][OFFSET] 			= {{false, false},
											{false, false}};

bool 	is_running[OFFSET] 				= {false, false};
char uartBuf[4000];

void SteppingMotorTest(void)
{
	// 90 degree rotation test

	printf("ROTATE start!!\n");
//	controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, QUARTER, FREQ_FAST);
	HAL_GPIO_WritePin(CupD_DIR_GPIO_Port, CupD_DIR_Pin, GPIO_PIN_SET);

	controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, 4800, FREQ_FAST);

//	controlSteppingMotor(LID_D, STEPPING_MOTOR_ON, QUARTER, FREQ_FAST);
	printf("ROTATE finished!!\n\n %d \n",HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[0][UP], MotorSpeedSensor_Pin[0][UP]));

//	HAL_GPIO_WritePin(CupD_DIR_GPIO_Port, CupD_DIR_Pin, GPIO_PIN_RESET);
//
//	controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, 4800, FREQ_FAST);

	HAL_Delay(2000);


}

void pulseMaker (uint16_t time)	//delay function in 10us
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < time);
}

void initCupLidDispensor(void)
{
	HAL_TIM_Base_Start(&htim1);
}

void controlSteppingMotor(uint8_t device_id, bool cmd, uint16_t angle, uint8_t freq)
{
	uint8_t ID = device_id - OFFSET;

	if(cmd == STEPPING_MOTOR_ON){
		is_running[ID] = true;
		HAL_GPIO_WritePin(CupD_EN_GPIO_Port, CupD_EN_Pin, GPIO_PIN_SET); //EN - SET = enable MOTOR
		HAL_Delay(10);

		for(uint16_t step=0; step < angle; step++){
			printf("%d \n",HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[0][UP], MotorSpeedSensor_Pin[0][UP]));

			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID], SteppingMotor_Pin[ID], GPIO_PIN_SET);
			pulseMaker(freq);
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID], SteppingMotor_Pin[ID], GPIO_PIN_RESET);
			pulseMaker(freq);

			if(is_detected[ID][UP] == false){
				if(HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[ID][UP], MotorSpeedSensor_Pin[ID][UP]) != GPIO_PIN_SET){
					is_detected[ID][UP] = true;
					printf("up detected\n");
					break;
				}
			}
			if(is_detected[ID][DOWN] == false){
				if(HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[ID][DOWN], MotorSpeedSensor_Pin[ID][DOWN]) != GPIO_PIN_SET){
					is_detected[ID][DOWN] = true;
					printf("down detected\n");
					break;
				}
			}
		}
	}
	// STEPPING_MOTOR_OFF
	HAL_GPIO_WritePin(CupD_EN_GPIO_Port, CupD_EN_Pin, GPIO_PIN_RESET); //EN - RESET = disable MOTOR

	is_running[ID] = false;
}

bool motorShutdownAll(void)
{
	for(uint8_t id=0; id<OFFSET; id++)
		controlSteppingMotor(id, STEPPING_MOTOR_OFF,0,0);

	return true;
}
uint8_t getStatus_cup_lid_Dispensor(void)
{
	for(uint8_t id=0; id<OFFSET; id++){
		if(is_running[id] == true)
			return RUNNING;
	}
}
bool dispatch(uint8_t device_id)
{
	sendCommand(device_id, dispenser_req_packet[DISPATCH_1_CUP]);
	return true;
}

void sendCommand(uint8_t device_id, uint8_t* req_packet)
{
	// init packet
	// calc checksum
	for(int i=1; i<1+req_packet[1]+1;i++){
		req_packet[6] += req_packet[i];
	}

	// request packet
	HAL_UART_Transmit(&huart5, (uint8_t *)req_packet, sizeof(req_packet), 100);

	// for debug
	HAL_UART_Transmit(&huart3, (uint8_t *)dispenser_req_packet, sizeof(dispenser_req_packet), 100);

}
