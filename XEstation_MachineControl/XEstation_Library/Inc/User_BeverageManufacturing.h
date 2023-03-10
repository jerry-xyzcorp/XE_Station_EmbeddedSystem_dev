/*
 * User_BeverageManufacturing.h
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */

#ifndef INC_USER_BEVERAGEMANUFACTURING_H_
#define INC_USER_BEVERAGEMANUFACTURING_H_

#include "main.h"

#define EN 0
#define IN1 1
#define IN2 2

#define HEATER_MOTOR_STOP 0
#define HEATER_MOTOR_MAX  65534

#define POWDER_MILK 	0
#define POWDER_MALCHA 	1
#define POWDER_MILKTEA	2
#define POWDER_ICETEA	3
#define POWDER_CHOCO	4
#define POWDER_NUM		5
#define HEATER_NUM		6
#define HEATER_COFFEE	5

#define WATER_EXIT_TIME(hot_water_amout) (int)(((hot_water_amout/1000.0-0.45)/21.1)*1000)
//#define HEATER_MOTOR_SPEED_10 int(65535*0.1) // macro func.!!

// powder machine
void init_powderMachine(void);
bool motorShutdownAll_powderMachine(void);
uint8_t getStatus_powderMachine(void);
bool make_Beverage(uint16_t* hot_water_gram, uint16_t* powder_gram);

// water heater
void init_waterHeater(void);
bool motorShutdownAll_waterHeater(void);
uint8_t getStatus_waterHeater(void);
bool dispatch_water(uint8_t num_hole, uint16_t gram, uint16_t time_ms);

// beverage manufacturing
bool exitPowderWater(); // make powder recipe beverage
bool exitWater(); 		// 1. clean all, 2. exit coffee machine water

#endif /* INC_USER_BEVERAGEMANUFACTURING_H_ */
