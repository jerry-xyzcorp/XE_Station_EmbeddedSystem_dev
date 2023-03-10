/*
 * User_HC_SR04.h
 */

#ifndef INC_USER_HC_SR04_H_
#define INC_USER_HC_SR04_H_

#include "main.h"

#define CKCP01 0x41
#define CKCF01 0x42
#define CKPD01 0x43
#define CKJG01 0x44
#define CKJG02 0x45
#define CKLD01 0x46
#define CJL01  0x47
#define CJL02  0x48

#define OFFSET 0x41

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


void delay (uint16_t time);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HCSR04_Read(uint8_t sensor_id);

uint8_t getStatus(void);
bool setFrequency(uint16_t time_ms);
bool getData(uint8_t sensor_id);
bool setThresh(uint8_t sensor_id, uint16_t);


#endif /* INC_USER_HC_SR04_H_ */
