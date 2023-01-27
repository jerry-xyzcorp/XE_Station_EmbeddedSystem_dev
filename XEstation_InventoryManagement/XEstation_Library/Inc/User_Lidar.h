#ifndef INC_USER_LIDAR_H_
#define INC_USER_LIDAR_H_
/*
 * User_Lidar.h
 */
#ifdef __cplusplus
extern "C" {
#endif


#ifndef INC_USER_LIDAR_H_
#define INC_USER_LIDAR_H_

#include "main.h"
/*
 * Lidar mode setting
 */
#define GET_DEVICE_INFO 0
#define RUN_2D_MODE	 1
#define RUN_3D_MODE	 2
#define RUN_DUAL_MODE 3
#define STOP 4
#define SET_3D_PULSE_DURATION_REQUEST 5
#define SET_FREQ_REQUEST 6
#define SET_SENSITIVITY_REQUEST 7
#define SET_BAUDRATE 8

void init_Lidar(void);
void Lidar3dTest(void);
void requestPacket(uint8_t packet_cmd, uint32_t value);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* INC_USER_LIDAR_H_ */