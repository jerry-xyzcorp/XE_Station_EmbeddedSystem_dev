/*
 * User_hx711.c
 */
#include "User_hx711.h"
#include "hx711Config.h"
#include "hx711.h"

void initHx711(void)
{
	// load cell
	// hx711
	hx711_t loadcellA, loadcellB;
	float weight;

	hx711_init(&loadcellA,  PD01_sck_GPIO_Port, PD01_sck_Pin,  PD01_dout_GPIO_Port, PD01_dout_Pin);
	hx711_coef_set(&loadcellA, 354.5); // read afer calibration
	hx711_tare(&loadcellA, 10);

	hx711_init(&loadcellB,  PD01_sck_GPIO_Port, PD01_sck_Pin,  PD01_dout_GPIO_Port, PD01_dout_Pin);
	hx711_coef_set(&loadcellA, 354.5); // read afer calibration
	hx711_tare(&loadcellB, 10);
}

uint8_t get_data(uint8_t sensor_id)
{
	hx711_t loadcellA, loadcellB;

	return 0;
}

uint8_t set_threash(uint8_t sensor_id)
{


	return 0;
}
