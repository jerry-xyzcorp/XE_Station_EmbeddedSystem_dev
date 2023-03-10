#ifndef INC_USER_HX711_H_
#define INC_USER_HX711_H_
/*
 * User_hx711.h
 */
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"



#define CHANNEL_A 0
#define CHANNEL_B 1
#define ZERO_VAL  2
#define SUM 	  2

void initHx711(void);
void runHx711(uint8_t i);

int get_data(uint8_t sensor_id);
bool set_threash(uint8_t sensor_id, int data);


#ifdef __cplusplus
}
#endif

#endif

