/*
 * User_CupLidDispenser.h
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */

#ifndef INC_USER_CUPLIDDISPENSER_H_
#define INC_USER_CUPLIDDISPENSER_H_

#include "main.h"


#define OFFSET 2
#define UP   0
#define DOWN 1

#define EN  0
#define STP 1
#define DIR 2

#define STATUS_CHECK 	0
#define DISPATCH_1_CUP 	1
#define IS_FINISHED 	2
#define IS_CUP_EXITED	6
#define RES_PACKET_BCC	10
#define CUP_EXITED		0x10
#define CUP_EXITING		0x01

#define STEPPING_MOTOR_OFF	false
#define STEPPING_MOTOR_ON	true

#define QUARTER 			5000
#define FREQ_FAST 			50	//10us * 50 = 500us

#define CLOCKWISE 			0x00
#define COUNTER_CLOCKWISE 	0x01

#define LEN 1

void SteppingMotorTest(void);
void pulseMaker(uint16_t time);
void controlSteppingMotor(uint8_t device_id, bool cmd, uint8_t dir, uint16_t angle, uint8_t freq);
bool motorShutdownAll_cupDispensor(void);
uint8_t getStatus_cupDispensor(void);
bool dispatch(uint8_t device_id);
void sendCommand(uint8_t device_id, uint8_t cmd);

#endif /* INC_USER_CUPLIDDISPENSER_H_ */
