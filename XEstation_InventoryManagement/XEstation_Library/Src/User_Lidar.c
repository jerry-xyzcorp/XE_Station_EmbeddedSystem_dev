/*
 * User_Lidar.c
 */
#include "User_Lidar.h"

extern UART_HandleTypeDef huart3; // PC
extern UART_HandleTypeDef huart5; // Lidar1
extern UART_HandleTypeDef huart6; // Lidar2

// lidar
uint8_t lidar_req_packet[8]; // lidar request packet

char uartBuf[4000];			//used as buffer for the string to be send to PC
uint8_t UART5_rxBuffer[200] = {0};

void init_Lidar(void)
{
	HAL_UART_Receive_IT(&huart5, UART5_rxBuffer, sizeof(UART5_rxBuffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == UART5)
	{
		// print to serial monitor
	    HAL_UART_Transmit(&huart3, UART5_rxBuffer, sizeof(UART5_rxBuffer), 100);

	    // enable interrupt again
	    HAL_UART_Receive_IT(&huart5, UART5_rxBuffer, sizeof(UART5_rxBuffer));
	}
}

void Lidar3dTest(void)
{
	uint32_t baud_rate = 115200;
//	requestPacket(SET_BAUDRATE, baud_rate);
//	 HAL_Delay(2000);
	requestPacket(GET_DEVICE_INFO, baud_rate);
	  HAL_Delay(5000);
	  requestPacket(RUN_3D_MODE, baud_rate);
	  HAL_Delay(2000);
}


void requestPacket(uint8_t packet_cmd, uint32_t value)
{
	uint8_t packet_value = 0x00;

	switch(packet_cmd){
	case GET_DEVICE_INFO: // Get Device Info
		packet_cmd = 0x10;
		break;
	case RUN_2D_MODE: // Run 2D Mode
		packet_cmd = 0x01;
		break;
	case RUN_3D_MODE: // Run 3D Mode
		packet_cmd = 0x08;
		break;
	case RUN_DUAL_MODE: // Run Dual Mode
		packet_cmd = 0x07;
		break;
	case STOP: // Stop
		packet_cmd = 0x02;
		break;
	case SET_3D_PULSE_DURATION_REQUEST: // Set 3D Pulse Duration Request
		packet_cmd = 0x0c;
		break;
	case SET_FREQ_REQUEST: // Frequency Setting Request
		packet_cmd = 0x0f;
		break;
	case SET_SENSITIVITY_REQUEST: // Sensitivity Setting Request
		packet_cmd = 0x11;
		break;
	case SET_BAUDRATE: // Set Serial Baud Rate
		packet_cmd = 0x12;
		switch(value){
		case 57600:
			packet_value = 0x39;
			huart5.Init.BaudRate = 57600;
			break;
		case 115200:
			packet_value = 0xaa;
			huart5.Init.BaudRate = 115200;
			break;
		case 250000:
			packet_value = 0x77;
			huart5.Init.BaudRate = 250000;
			break;
		case 3000000:
			packet_value = 0x55;
			huart5.Init.BaudRate = 3000000;
			break;
		}
		if (HAL_UART_Init(&huart5) != HAL_OK)
		{
			Error_Handler();
		}
		break;
	}

	uint8_t lidar_req_packet[] = {0x5a, 0x77, 0xff, 0x02, 0x00, packet_cmd, packet_value, 0x00};

	// calc checksum
	uint8_t CheckSum = 0;
	int PAYLOAD_LENGTH_LSB_INDEX = 3;
	for(int i = PAYLOAD_LENGTH_LSB_INDEX; i < sizeof(lidar_req_packet)- 2; i++)
	{
		CheckSum ^= lidar_req_packet[i];
	}
	lidar_req_packet[7] = CheckSum;

	 //print request packet
//	for(int i=0; i<sizeof(lidar_req_packet); i++){
//		sprintf(uartBuf, "%0.2x \r", lidar_req_packet[i]);			//convert to string
//		HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);
//	}
//
//	sprintf(uartBuf, "-> request packet\n");			//convert to string
//	HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);

	// request packet
	uint8_t tmpBuf[15] = {0,};
	HAL_UART_Transmit(&huart5, (uint8_t *)lidar_req_packet, sizeof(lidar_req_packet), 100);
	HAL_Delay(40);

//	 receive packet
//	while(1)
//	{
//		HAL_UART_Receive(&huart5, (uint8_t *)tmpBuf, sizeof(tmpBuf), 100);
//		for(int i=0; i<sizeof(tmpBuf); i++){
//			sprintf(uartBuf, "%0.2x \r", tmpBuf[i]);
//			HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);
//		}
//		sprintf(uartBuf, "\n");
//		HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);
//
//		uint8_t stx = 0x5a;
//		if(tmpBuf == stx)
//		{
//			sprintf(uartBuf, "received!\n");			//convert to string
//			HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, sizeof(uartBuf), 100);
//			break;
//		}
//	}

	HAL_Delay(1000);
}