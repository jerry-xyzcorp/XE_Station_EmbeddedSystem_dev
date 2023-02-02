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
	hx711_coef_set(&loadcellA, 345); // read afer calibration
	hx711_tare(&loadcellA, 10);


//	hx711_init(&loadcellB,  PD01_sck_GPIO_Port, PD01_sck_Pin,  PD01_dout_GPIO_Port, PD01_dout_Pin);
//	hx711_coef_set(&loadcellA, 354.5); // read afer calibration
//	hx711_tare(&loadcellB, 10);

	while(1){
		printf("A:%d g\n", (int)(hx711_weight(&loadcellA, 10)));
		HAL_Delay(100);
	}


}
void hx711Test(void)
{
	hx711_t loadcellA, loadcellB;
	float weight;

	int offsetA = -4597;
	int offsetB = 21325-14202-7119;
	while(1){
		printf("while ...\n");
//		int weightA = (int)(hx711_weight(&loadcellA, 10)) - offsetA;
	//	int weightB = (int)(hx711_weight(&loadcellB, 10)) - offsetB;

//		sprintf(uartBuf, "A:%d g \rB:%d g\n", weightA, weightB);			//convert to string
//		HAL_UART_Transmit(&huart3, (uint8_t *)uartBuf, strlen(uartBuf), 100);
		printf("A:%d g\n", (int)(hx711_weight(&loadcellA, 10)));
		HAL_Delay(1000);
	}
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
