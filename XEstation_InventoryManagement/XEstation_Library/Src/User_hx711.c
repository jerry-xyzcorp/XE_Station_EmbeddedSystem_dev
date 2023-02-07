/*
 * User_hx711.c
 */
#include "User_hx711.h"
#include "hx711Config.h"
#include "hx711.h"

hx711_t hx711_arr[HX711_NUM][2];
int hx711_weight_arr[HX711_NUM][2];
int hx711_cali_factor[HX711_NUM][3] = {{700, 1700, 2415},//{122, 115, 7100}
										{220, 0, 0},
										{225, 0, 0},
										{215, 0, 0},
										{89, 118, 0},
										{0, 0, 0},
										{700, 1700, 2415},
										{0, 0, 0},
										{0, 0, 0}};

GPIO_TypeDef* HX711_GPIO_Port[HX711_NUM][2] = {{PD01_sck_GPIO_Port, PD01_dout_GPIO_Port},
												{PD02_sck_GPIO_Port, PD02_dout_GPIO_Port},
												{PD03_sck_GPIO_Port, PD03_dout_GPIO_Port},
												{PD04_sck_GPIO_Port, PD04_dout_GPIO_Port},
												{PD05_sck_GPIO_Port, PD05_dout_GPIO_Port},
												{EXIC01_sck_GPIO_Port, EXIC01_dout_GPIO_Port},
												{EXPD01_sck_GPIO_Port, EXPD01_dout_GPIO_Port},
												{SYR01_sck_GPIO_Port, SYR01_dout_GPIO_Port},
												{SYR02_sck_GPIO_Port, SYR02_dout_GPIO_Port}};

uint16_t HX711_Pin[HX711_NUM][2] = {{PD01_sck_Pin, PD01_dout_Pin},
									{PD02_sck_Pin, PD02_dout_Pin},
									{PD03_sck_Pin, PD03_dout_Pin},
									{PD04_sck_Pin, PD04_dout_Pin},
									{PD05_sck_Pin, PD05_dout_Pin},
									{EXIC01_sck_Pin, EXIC01_dout_Pin},
									{EXPD01_sck_Pin, EXPD01_dout_Pin},
									{SYR01_sck_Pin, SYR01_dout_Pin},
									{SYR02_sck_Pin, SYR02_dout_Pin}};

void initHx711(void)
{
	// init hx711
	// it takes time(5s)
	for(uint8_t i=0; i<HX711_NUM; i++){
		hx711_init(&hx711_arr[i][CHANNEL_A], HX711_GPIO_Port[i][SCK], HX711_Pin[i][SCK],  HX711_GPIO_Port[i][DOUT], HX711_Pin[i][DOUT], 'A');
		hx711_coef_set(&hx711_arr[i][CHANNEL_A], hx711_cali_factor[i][CHANNEL_A]); // read afer calibration
		hx711_tare(&hx711_arr[i][CHANNEL_A], 10);
		printf("[%d th] a completed\n", i);

		if(hx711_cali_factor[i][CHANNEL_B] != 0){
			hx711_init(&hx711_arr[i][CHANNEL_B],  HX711_GPIO_Port[i][SCK], HX711_Pin[i][SCK],  HX711_GPIO_Port[i][DOUT], HX711_Pin[i][DOUT], 'B');
			hx711_coef_set(&hx711_arr[i][CHANNEL_B], hx711_cali_factor[i][CHANNEL_B]); // read afer calibration
			hx711_tare(&hx711_arr[i][CHANNEL_B], 10);
			printf("b completed\n");
		}
	}
	printf("hx711 initialization finished!\n");
}

void runHx711(uint8_t i)
{
	hx711_weight_arr[i][CHANNEL_A] = (int)(hx711_weight(&hx711_arr[i][CHANNEL_A], 10));
	hx711_weight_arr[i][CHANNEL_B] = (int)(hx711_weight(&hx711_arr[i][CHANNEL_B], 10));

	if(hx711_cali_factor[i][CHANNEL_B] != 0){
		hx711_weight_arr[i] = hx711_weight_arr[i][CHANNEL_A]+hx711_weight_arr[i][CHANNEL_B]+hx711_cali_factor[i][ZERO_VAL];
		printf("[%d th] A:%d g, B:%d g ====> %d g\n\n", i, hx711_weight_arr[i][CHANNEL_A], hx711_weight_arr[i][CHANNEL_B], hx711_weight_arr[i]);
	}
	else{
		hx711_weight_arr[i] = hx711_weight_arr[i][CHANNEL_A];
		printf("[%d th] A:%d g\n", i, hx711_weight_arr[i][CHANNEL_A]);
	}
	HAL_Delay(100);
}

int get_data(uint8_t sensor_id)
{
	return hx711_weight_arr[sensor_id];
}

bool set_threash(uint8_t sensor_id, int data)
{
	hx711_cali_factor[sensor_id][ZERO_VAL] = data;

	return true;
}
