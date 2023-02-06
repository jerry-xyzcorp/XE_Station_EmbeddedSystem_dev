#ifndef INC_USER_HX711_H_
#define INC_USER_HX711_H_
/*
 * User_hx711.h
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

#define HX711_NUM 7

#define CHANNEL_A 0
#define CHANNEL_B 1

void initHx711(void);

uint8_t get_data(uint8_t sensor_id);
uint8_t set_threash(uint8_t sensor_id);


#ifdef __cplusplus
}
#endif

#endif

