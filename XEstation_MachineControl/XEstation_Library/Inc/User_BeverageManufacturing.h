/*
 * User_BeverageManufacturing.h
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */

#ifndef INC_USER_BEVERAGEMANUFACTURING_H_
#define INC_USER_BEVERAGEMANUFACTURING_H_

#include "main.h"

// powder machine
void init_powderMachine(void);
bool motorShutdownAll_powderMachine(void);
uint8_t getStatus_powderMachine(void);
bool make_Beverage(uint8_t num_hole, uint16_t gram);

// water heater
void init_waterHeater(void);
bool motorShutdownAll_waterHeater(void);
uint8_t getStatus_waterHeater(void);
bool dispatch_water(uint8_t num_hole, uint16_t gram, uint16_t time_ms);

// beverage manufacturing
bool exitPowderWater(); // make powder recipe beverage
bool exitWater(); 		// 1. clean all, 2. exit coffee machine water

#endif /* INC_USER_BEVERAGEMANUFACTURING_H_ */