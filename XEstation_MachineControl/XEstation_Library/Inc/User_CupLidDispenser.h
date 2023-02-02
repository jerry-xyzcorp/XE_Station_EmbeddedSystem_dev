/*
 * User_CupLidDispenser.h
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */

#ifndef INC_USER_CUPLIDDISPENSER_H_
#define INC_USER_CUPLIDDISPENSER_H_

#include "main.h"


#define OFFSET 0x02
#define UP 0X01
#define DOWN 0X02

#define STATUS_CHECK 	0x00
#define DISPATCH_1_CUP 	0x01

#define STEPPING_MOTOR_OFF	false
#define STEPPING_MOTOR_ON	true

#define QUARTER 			5000
#define FREQ_FAST 			50	//10us * 50 = 500us

void SteppingMotorTest(void);
void pulseMaker(uint16_t time);
void controlSteppingMotor(uint8_t device_id, bool cmd, uint16_t angle, uint8_t freq);
bool motorShutdownAll_cupDispensor(void);
uint8_t getStatus_cupDispensor(void);
bool dispatch(uint8_t device_id);
void sendCommand(uint8_t device_id, uint8_t* req_packet);

#endif /* INC_USER_CUPLIDDISPENSER_H_ */
