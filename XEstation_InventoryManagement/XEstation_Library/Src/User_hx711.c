/*
 * User_hx711.c
 */
#include "User_hx711.h"
#include "hx711Config.h"
#include "hx711.h"


int hx711_cali_factor[][2] = {{122, 115},
										{210, 0},
										{210, 0},
										{210, 0},
										{210, 0},
										{210, 0},
										{210, 0}};

//GPIO_TypeDef* HX711_GPIO_Port[][2] = {{PD01_sck_GPIO_Port, PD01_dout_GPIO_Port},
//										{PD02_sck_GPIO_Port, PD02_dout_GPIO_Port},
//										{PD03_sck_GPIO_Port, PD03_dout_GPIO_Port},
//										{PD04_sck_GPIO_Port, PD04_dout_GPIO_Port},
//										{PD05_sck_GPIO_Port, PD05_dout_GPIO_Port},
//										{EXIC01_sck_GPIO_Port, EXIC01_dout_GPIO_Port},
//										{EXPD01_sck_GPIO_Port, EXPD01_dout_GPIO_Port}};
//
//uint16_t HX711_Pin[][2] = {{PD01_sck_Pin, PD01_dout_Pin},
//							{PD02_sck_Pin, PD02_dout_Pin},
//							{PD03_sck_Pin, PD03_dout_Pin},
//							{PD04_sck_Pin, PD04_dout_Pin},
//							{PD05_sck_Pin, PD05_dout_Pin},
//							{EXIC01_sck_Pin, EXIC01_dout_Pin},
//							{EXPD01_sck_Pin, EXPD01_dout_Pin}};

void initHx711(void)
{
	// load cell
	// hx711
	hx711_t* hx711_arr[HX711_NUM][2];
	printf("gpio port : %x\n",PD01_sck_GPIO_Port);
//	printf("%x\n", HX711_GPIO_Port[0][SCK]);

	printf("pin :%x\n",PD01_sck_Pin );
//	printf("%x\n", HX711_Pin[0][SCK] );

	hx711_t loadcellA;
//	hx711_init(&loadcellA, PD01_sck_GPIO_Port, PD01_sck_Pin, PD01_dout_GPIO_Port, PD01_dout_Pin, 'A');

	for(uint8_t i=0; i<HX711_NUM; i++){
//		hx711_init(hx711_arr[i][CHANNEL_A], HX711_GPIO_Port[i][SCK], HX711_Pin[i][SCK], HX711_GPIO_Port[i][DOUT], HX711_Pin[i][DOUT], 'A');
		hx711_init(hx711_arr[i][CHANNEL_A], i, 'A');

		hx711_coef_set(hx711_arr[i][CHANNEL_A], hx711_cali_factor[i][CHANNEL_A]);
		hx711_tare(hx711_arr[i][CHANNEL_A], 10);
			printf("a completed\n");

		if(hx711_cali_factor[i][CHANNEL_B] != 0){
//			hx711_init(hx711_arr[i][CHANNEL_B],  HX711_GPIO_Port[i][SCK], HX711_Pin[i][SCK], HX711_GPIO_Port[i][DOUT], HX711_Pin[i][DOUT], 'B');
			hx711_init(hx711_arr[i][CHANNEL_A], i, 'B');

			hx711_coef_set(hx711_arr[i][CHANNEL_B], hx711_cali_factor[i][CHANNEL_B]);
			hx711_tare(hx711_arr[i][CHANNEL_B], 10);
			printf("b completed\n");
		}
	}


//
//	hx711_t loadcellA, loadcellB;
//
//	hx711_init(&loadcellA,  PD01_sck_GPIO_Port, PD01_sck_Pin,  PD01_dout_GPIO_Port, PD01_dout_Pin, 'A');
//	hx711_coef_set(&loadcellA, 122); // read afer calibration
//	hx711_tare(&loadcellA, 10);
//
//	hx711_init(&loadcellB,  PD01_sck_GPIO_Port, PD01_sck_Pin,  PD01_dout_GPIO_Port, PD01_dout_Pin, 'B');
//	hx711_coef_set(&loadcellB, 115); // read afer calibration
//	hx711_tare(&loadcellB, 10);
////

	while(1){
		int A = (int)(hx711_weight(hx711_arr[0][CHANNEL_A], 10));
		int B = (int)(hx711_weight(hx711_arr[0][CHANNEL_B], 10));
		printf("A:%d g\n", A);
		printf("B:%d g\n", B);
		printf("A+B:%d g\n\n\n", A+B+2515);
		HAL_Delay(100);
	}
}
//void initHx711(void)
//{
//	// load cell
//	// init hx711
//	hx711_t* hx711_arr[HX711_NUM];
//
//	for(uint8_t i=0; i<HX711_NUM; i++){
//		hx711_init(hx711_arr[i],  HX711_GPIO_Port[i][SCK], HX711_Pin[i][SCK], HX711_GPIO_Port[i][DOUT], HX711_Pin[i][DOUT]);
//		hx711_coef_set(hx711_arr[i], hx711_cali_factor[i]);
//		hx711_tare(hx711_arr[i], 10);
//	}
//
//
//	hx711_t loadcellA;
////
////	float weight;
////
//	int i=0;
//
//	hx711_init(&loadcellA,  PD01_sck_GPIO_Port, PD01_sck_Pin,  PD01_dout_GPIO_Port, PD01_dout_Pin);
//	hx711_coef_set(&loadcellA, 210.0); // read afer calibration - -1000
//	hx711_tare(&loadcellA, 10);
//	printf("test started1\n");
//
////	hx711_init(&loadcellA,  HX711_GPIO_Port[i][SCK], HX711_Pin[i][SCK], HX711_GPIO_Port[i][DOUT], HX711_Pin[i][DOUT]);//
////	hx711_coef_set(&loadcellA, 210); // read afer calibration
////	hx711_tare(&loadcellA, 10);
//
//	while(1){
//		printf("A:%d g\n", (int)(hx711_weight(&loadcellA, 10)));
////		printf("A:%d g\n", (int)(hx711_weight(hx711_arr[0], 10)));
//
//		HAL_Delay(1000);
//	}
//}
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
