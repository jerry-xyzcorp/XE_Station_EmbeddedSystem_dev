/*
 * User_IceDispenser.h
 *
 *  Created on: 2023. 1. 25.
 *      Author: Ethan
 */

#ifndef INC_USER_ICEDISPENSER_H_
#define INC_USER_ICEDISPENSER_H_

#include "main.h"



void relayTest(void);
void iceDispenser_SetMode(uint16_t water_time, uint16_t ice_time);


void init_iceDispensor(void);
bool stopAll(void);
uint8_t getStatus_iceDispensor(void);
bool startExit(uint32_t water_time, uint32_t ice_time);



//bool dispatch_water(uint8_t num_hole, uint16_t gram, uint16_t time); ->온수기

#endif /* INC_USER_ICEDISPENSER_H_ */
