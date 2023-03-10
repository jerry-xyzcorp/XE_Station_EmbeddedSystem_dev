/*
 * User_Lidar.h
 */
#ifndef INC_USER_LIDAR_H_
#define INC_USER_LIDAR_H_

#include "main.h"

// Lidar mode setting
#define GET_DEVICE_INFO 				0
#define RUN_2D_MODE	 					1
#define RUN_3D_MODE	 					2
#define RUN_DUAL_MODE 					3
#define STOP 							4
#define SET_3D_PULSE_DURATION_REQUEST 	5
#define SET_FREQ_REQUEST 				6
#define SET_SENSITIVITY_REQUEST 		7
#define SET_BAUDRATE 					8

// Packet structure
#define PACKET_HEADER_1	 			0
#define PACKET_HEADER_2	 			1
#define PACKET_HEADER_3	 			2
#define PAYLOAD_LENGTH_LSB_INDEX 	3
#define PAYLOAD_LENGTH_MSB_INDEX 	4
#define PAYLOAD_HEADER			 	5
#define PACKET_LEN				 	6

#define FORMAT_CHECKSUM_LEN		1
#define FORMAT_3D_LEN			14400
#define FORMAT_3D_ARR			9600
#define NO_VALUE 				0

void init_Lidar(void);
void Lidar3dTest(void);
void requestPacket(uint8_t packet_cmd, uint32_t value);
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void response_packet(uint8_t recv);

uint8_t CalcChecksum(uint8_t* packet, uint32_t packet_size);
uint8_t process_lidarData_3d(uint8_t* lidarData_3d);

#endif /* INC_USER_LIDAR_H_ */
