/*
 * User_PC_serial.c
 */
#include "User_PC_serial.h"

extern UART_HandleTypeDef huart3; // PC


// PC-baord communication
void pcSerialTest(void)
{
	uint8_t buf = 0;
	uint8_t stx = 0;
	uint8_t len = 0;

	uint8_t buf2 = 0;
	uint8_t rxbuf[16] = {0,};
	uint8_t txbuf[16] = {0,};
	uint8_t i=0;

	// receive packet
//	HAL_UART_Receive(&huart3, &buf1, sizeof(buf1), 10);
//	while(1){
//		HAL_UART_Receive(&huart3, &buf2, sizeof(buf2), 10);
//
//		if (buf1 == 0x02 && buf2 == 0x01){
//			rxbuf[0] = buf1;
//			rxbuf[1] = buf2;
//			i += 2;
//			break;
//		}
//		buf1 = buf2;
//	}

	while(1){
		HAL_UART_Receive(&huart3, &stx, sizeof(stx), 10);
		// STX
		if (stx == 0x02){
			rxbuf[0] = stx;

			break;
		}
	}
	// LEN
	HAL_UART_Receive(&huart3, &len, sizeof(len), 10);
	rxbuf[1] = len;
	uint8_t *data_arr = (uint8_t*)malloc(sizeof(uint8_t) * (len+2));
	for(int i=0; i<len+2; i++){
		data_arr[i] = 0;
	}
	i = 2;

	while(1){
		HAL_UART_Receive(&huart3, &buf, sizeof(buf), 10);

		if(buf != NULL){
			rxbuf[i] = buf;
			data_arr[i] = buf;
			i += 1;
			if (buf == 0x03){
				HAL_UART_Receive(&huart3, &buf, sizeof(buf), 10);
				rxbuf[i] = buf;
				data_arr[i] = buf;
				break;
			}
		}
	}
	data_arr[0] = stx;
	data_arr[1] = len;

	// check checksum
	uint8_t CheckSum = 0;
	for(int i = 2; i < len; i++)
	{
		CheckSum ^= data_arr[i];
	}
	data_arr[len+1] = CheckSum;

	// transmit packet
	HAL_UART_Transmit(&huart3, (uint8_t *)data_arr, (len+2), 100);
	free(data_arr);
}