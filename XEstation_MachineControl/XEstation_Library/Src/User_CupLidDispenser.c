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
extern uint8_t cupD_res_packet[20];
extern uint8_t LidD_res_packet[20];


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

bool is_cup_lid_running(uint8_t device_id)
{
	uint8_t ID = device_id - OFFSET;
	if(is_running[ID] == true)
		return true;
	else
		return false;
}

void SteppingMotorControl(void)
{
	// 90 degree rotation test
//	printf("ROTATE start!!\n");
//	controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
	controlSteppingMotor(LID_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
//	printf("ROTATE finished!!\n\n");

	HAL_Delay(1000);
//	cup_lid_dispatch(CUP_D);
	cup_lid_dispatch(LID_D);
}
void cupRotate(void)
{
	controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);

}

void pulseMaker (uint16_t time)	//delay function in 10us
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < time);
}

bool initCupLidDispensor(void)
{
	HAL_TIM_Base_Start(&htim1);
	return true;
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
			printf("%d \n",HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[0][UP], MotorSpeedSensor_Pin[0][UP]));

			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_SET);
			pulseMaker(freq);
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_RESET);
			pulseMaker(freq);
		}

//		printf("detection mode on!\n");
		for(uint16_t step=0; step < angle*2; step++){
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_SET);
			pulseMaker(freq);
			HAL_GPIO_WritePin(SteppingMotor_GPIO_Port[ID][STP], SteppingMotor_Pin[ID][STP], GPIO_PIN_RESET);
			pulseMaker(freq);

			if(is_detected[ID][UP] == false){
				if(HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[ID][UP], MotorSpeedSensor_Pin[ID][UP]) & (is_detected[ID][DOWN] == true)){
					is_detected[ID][UP] = true;
//					printf("up detected\n");
					break;
				}
			}

			if(is_detected[ID][DOWN] == false){
				if(HAL_GPIO_ReadPin(MotorSpeedSensor_GPIO_Port[ID][DOWN], MotorSpeedSensor_Pin[ID][DOWN])){
					is_detected[ID][DOWN] = true;
//					printf("down detected\n");
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
bool cup_lid_dispatch(uint8_t device_id)
{
//	sendCommand(device_id, IS_FINISHED);
//	HAL_Delay(3000);

//	sendCommand(device_id, IS_FINISHED);
//	  if (HAL_UART_Init(&huart6) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
//	HAL_Delay(3000);

	sendCommand(device_id, STATUS_CHECK);
	HAL_Delay(3000);

	sendCommand(device_id, DISPATCH_1_CUP);

	sendCommand(device_id, IS_FINISHED);


//	HAL_Delay(3000);

//	sendCommand(device_id, DISPATCH_1_CUP);

//	sendCommand(device_id, IS_FINISHED);


	//	printf("send dispatch\n");

//
//	while(1)
//	{
////		HAL_UART_Receive_IT(&huart5, cupD_res_packet, sizeof(cupD_res_packet));
////		HAL_UART_Receive_IT(&huart6, LidD_res_packet, sizeof(LidD_res_packet));
//
//		sendCommand(device_id, STATUS_CHECK);
//		if(LidD_res_packet[IS_CUP_EXITED] == CUP_EXITED)
//			break;
////		if(lidD_res_packet[IS_LID_EXITED] == LID_EXITED)
////			break;
//	}
//	HAL_Delay(2000);
//	sendCommand(device_id, DISPATCH_1_CUP);
//	sendCommand(device_id, IS_FINISHED);
//
////	printf("send dispatch\n");
//	HAL_Delay(2000);
//	sendCommand(device_id, DISPATCH_1_CUP);
//	sendCommand(device_id, IS_FINISHED);


//	printf("send dispatch\n");
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
	if (device_id == CUP_D){
//		HAL_UART_Receive_IT(&huart5, cupD_res_packet, sizeof(cupD_res_packet));
		HAL_UART_Transmit(&huart5, (uint8_t *)dispenser_req_packet_list[cmd], dispenser_req_packet_list[cmd][1]+4, 100);

		uint8_t stx = 0;
		uint8_t len = 0;
		uint8_t etx = 0;
		uint8_t buf = 0;
		uint8_t chk = 0;

		// receive packet
		HAL_UART_Receive(&huart5, &stx, sizeof(stx), 10);
		if (stx == 0x02){
			HAL_UART_Receive(&huart5, &len, sizeof(len), 10);
			for(int i=0; i<len; i++){
				HAL_UART_Receive(&huart5, &buf, sizeof(buf), 10);
			}
			HAL_UART_Receive(&huart5, &etx, sizeof(etx), 10);
			HAL_UART_Receive(&huart5, &chk, sizeof(chk), 10);


		}
	}
	else if (device_id == LID_D){
//		HAL_UART_Receive_IT(&huart6, LidD_res_packet, sizeof(LidD_res_packet));
		HAL_UART_Transmit(&huart6, (uint8_t *)dispenser_req_packet_list[cmd], sizeof(dispenser_req_packet_list[cmd]), 100);
	}

	uint8_t buf = 0;
	uint8_t stx = 0;
	uint8_t len = 0;

	// receive packet
	HAL_UART_Receive(&huart6, &stx, sizeof(stx), 10);

	// STX
	if (stx == 0x02)
	{
		// LEN
		HAL_UART_Receive(&huart6, &len, sizeof(len), 10);
		uint8_t *data_arr = (uint8_t*)malloc(sizeof(uint8_t) * (len+2));

		for(int i=0; i<len+2; i++){
			data_arr[i] = 0;
		}

		data_arr[0] = stx;
		data_arr[1] = len;

		for(int i=2; i<len+1; i++)
		{
			HAL_UART_Receive(&huart6, &buf, sizeof(buf), 10);

			if(buf != NULL)
				data_arr[i] = buf;
		}

		// check checksum
//		for(int i = 2; i < len; i++)
//			data_arr[len+1] ^= data_arr[i];

		// EXECUTION CODE
//		for(int i=0; i<len+2; i++){ // for test
//			LidD_res_packet[i] = data_arr[i];
//		}
		HAL_UART_Transmit(&huart3, (uint8_t *)data_arr, 7+4, 100);

		free(data_arr);
	}

	// for debug
//	HAL_UART_Transmit(&huart3, (uint8_t *)dispenser_req_packet_list[cmd], sizeof(dispenser_req_packet_list[cmd]), 100);
}
