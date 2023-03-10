/*
 * User_CupLidDispenser.c
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */
#include "User_CupLidDispenser.h"

extern TIM_HandleTypeDef htim1;

extern UART_HandleTypeDef huart3; //pc
extern UART_HandleTypeDef huart5; // cup dispenser
extern UART_HandleTypeDef huart6; // lid dispenser

extern uint8_t pc_res_packet[10];
extern uint8_t cupD_res_packet[11];
extern uint8_t LidD_res_packet[11];


uint8_t dispenser_req_packet_list[][7] =
		{
				{0x02, 0x01, 0x40, 0x03, 0x00, 0x00, 0x00},		// status check
				{0x02, 0x03, 0x41, 0x01, 0x01, 0x03, 0x00},		// extract 1 cup
				{0x02, 0x02, 0x42, 0x00, 0x03, 0x00, 0x00}		// finished
		};

GPIO_TypeDef* 	SteppingMotor_GPIO_Port[OFFSET][3] = 	{{CupD_EN_GPIO_Port, CupD_STP_GPIO_Port, CupD_DIR_GPIO_Port},
													{LidD_EN_GPIO_Port, LidD_STP_GPIO_Port, LidD_DIR_GPIO_Port}};
uint16_t* 		SteppingMotor_Pin[OFFSET][3] = 		{{CupD_EN_Pin, CupD_STP_Pin, CupD_DIR_Pin},
													{LidD_EN_Pin, LidD_STP_Pin, LidD_DIR_Pin}};

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
//	printf("ROTATE start!!\n");
//	controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
	controlSteppingMotor(LID_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
//	printf("ROTATE finished!!\n\n");

	HAL_Delay(1000);
//	dispatch(CUP_D);
	dispatch(LID_D);

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

void controlSteppingMotor(uint8_t device_id, bool cmd, uint8_t dir, uint16_t angle, uint8_t freq)
{
	uint8_t ID = device_id - OFFSET;

	if(cmd == STEPPING_MOTOR_ON){
		//EN : SET = enable MOTOR
		HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][EN], SteppingMotor_Pin[ID][EN], GPIO_PIN_SET);

		// DIR : Sset motor direction
		if(dir == CLOCKWISE)
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][DIR], SteppingMotor_Pin[ID][DIR], GPIO_PIN_RESET);
		else if(dir == COUNTER_CLOCKWISE)
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][DIR], SteppingMotor_Pin[ID][DIR], GPIO_PIN_SET);

		is_running[ID] = true;
		HAL_Delay(10);

		for(uint16_t step=0; step < angle/10; step++){
//			printf("%d \n",HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[0][UP], MotorSpeedSensor_Pin[0][UP]));

			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_SET);
			pulseMaker(freq);
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_RESET);
			pulseMaker(freq);
		}
		printf("detection mode on!\n");
		for(uint16_t step=0; step < angle*2; step++){
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_SET);
			pulseMaker(freq);
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_RESET);
			pulseMaker(freq);

			if(is_detected[ID][UP] == false){
				if(HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[ID][UP], MotorSpeedSensor_Pin[ID][UP]) & (is_detected[ID][DOWN] == true)){
					is_detected[ID][UP] = true;
					printf("up detected\n");
					break;
				}
			}
			if(is_detected[ID][DOWN] == false){
				if(HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[ID][DOWN], MotorSpeedSensor_Pin[ID][DOWN])){
					is_detected[ID][DOWN] = true;
					printf("down detected\n");
				}
			}
		}
	}
	// STEPPING_MOTOR_OFF
	HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][EN], SteppingMotor_Pin[ID][EN], GPIO_PIN_RESET); //EN - RESET = disable MOTOR

	is_running[ID] = false;
}

bool motorShutdownAll(void)
{
	for(uint8_t id=0; id<OFFSET; id++)
		controlSteppingMotor(id, STEPPING_MOTOR_OFF,0,0,0);

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
	HAL_UART_Receive_IT(&huart6, cupD_res_packet, sizeof(cupD_res_packet));
	sendCommand(device_id, DISPATCH_1_CUP);
//	sendCommand(device_id, STATUS_CHECK);
//
//	while(1)
//	{
//		HAL_UART_Receive_IT(&huart5, cupD_res_packet, sizeof(cupD_res_packet));
//		sendCommand(device_id, STATUS_CHECK);
//		if(cupD_res_packet[IS_CUP_EXITED] == CUP_EXITED)
//			break;
//	}

//	sendCommand(device_id, IS_FINISHED);
	return true;
}


void sendCommand(uint8_t device_id, uint8_t cmd)
{
	// calc checksum
	int BCC = dispenser_req_packet_list[cmd][LEN]+3;

	for(int i=1; i<BCC; i++){
		dispenser_req_packet_list[cmd][BCC] += dispenser_req_packet_list[cmd][i];
	}

	// request packet
	HAL_UART_Transmit(&huart6, (uint8_t *)dispenser_req_packet_list[cmd], sizeof(dispenser_req_packet_list[cmd]), 100);

	// for debug
	HAL_UART_Transmit(&huart3, (uint8_t *)dispenser_req_packet_list[cmd], sizeof(dispenser_req_packet_list[cmd]), 100);
}
