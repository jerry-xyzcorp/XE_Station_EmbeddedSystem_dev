/*
 * User_BeverageManufacturing.c
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */
#include "User_beverageManufacturing.h"

//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim3;
//extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim7;
//extern TIM_HandleTypeDef htim9;
//extern TIM_HandleTypeDef htim10;
//extern TIM_HandleTypeDef htim11;
//extern TIM_HandleTypeDef htim12;

/*********************************** Pin configuration ***********************************/
GPIO_TypeDef * PD_DCpowder_GPIO_Port[][3] = {{PD01_DCpowder_EN_GPIO_Port, PD01_DCpowder_IN1_GPIO_Port, PD01_DCpowder_IN2_GPIO_Port},
											{PD02_DCpowder_EN_GPIO_Port, PD02_DCpowder_IN1_GPIO_Port, PD02_DCpowder_IN2_GPIO_Port},
											{PD03_DCpowder_EN_GPIO_Port, PD03_DCpowder_IN1_GPIO_Port, PD03_DCpowder_IN2_GPIO_Port},
											{PD04_DCpowder_EN_GPIO_Port, PD04_DCpowder_IN1_GPIO_Port, PD04_DCpowder_IN2_GPIO_Port},
											{PD05_DCpowder_EN_GPIO_Port, PD05_DCpowder_IN1_GPIO_Port, PD05_DCpowder_IN2_GPIO_Port}};

GPIO_TypeDef * PD_DCmix_GPIO_Port[][3] 	 = {{PD01_DCmix_EN_GPIO_Port, PD01_DCmix_IN1_GPIO_Port, PD01_DCmix_IN2_GPIO_Port},
											{PD02_DCmix_EN_GPIO_Port, PD02_DCmix_IN1_GPIO_Port, PD02_DCmix_IN2_GPIO_Port},
											{PD03_DCmix_EN_GPIO_Port, PD03_DCmix_IN1_GPIO_Port, PD03_DCmix_IN2_GPIO_Port},
											{PD04_DCmix_EN_GPIO_Port, PD04_DCmix_IN1_GPIO_Port, PD04_DCmix_IN2_GPIO_Port},
											{PD05_DCmix_EN_GPIO_Port, PD05_DCmix_IN1_GPIO_Port, PD05_DCmix_IN2_GPIO_Port}};

GPIO_TypeDef * heat_GPIO_Port[][3] 	 = {{PD01_heat_EN_GPIO_Port, PD01_heat_IN1_GPIO_Port, PD01_heat_IN2_GPIO_Port},
										{PD02_heat_EN_GPIO_Port, PD02_heat_IN1_GPIO_Port, PD02_heat_IN2_GPIO_Port},
										{PD03_heat_EN_GPIO_Port, PD03_heat_IN1_GPIO_Port, PD03_heat_IN2_GPIO_Port},
										{PD04_heat_EN_GPIO_Port, PD04_heat_IN1_GPIO_Port, PD04_heat_IN2_GPIO_Port},
										{PD05_heat_EN_GPIO_Port, PD05_heat_IN1_GPIO_Port, PD05_heat_IN2_GPIO_Port},
										{CF01_heat_EN_GPIO_Port, CF01_heat_IN1_GPIO_Port, CF01_heat_IN2_GPIO_Port}};

uint16_t* PD_DCpowder_Pin[][3] = 		{{PD01_DCpowder_EN_Pin, PD01_DCpowder_IN1_Pin, PD01_DCpowder_IN2_Pin},
										{PD02_DCpowder_EN_Pin, PD02_DCpowder_IN1_Pin, PD02_DCpowder_IN2_Pin},
										{PD03_DCpowder_EN_Pin, PD03_DCpowder_IN1_Pin, PD03_DCpowder_IN2_Pin},
										{PD04_DCpowder_EN_Pin, PD04_DCpowder_IN1_Pin, PD04_DCpowder_IN2_Pin},
										{PD05_DCpowder_EN_Pin, PD05_DCpowder_IN1_Pin, PD05_DCpowder_IN2_Pin}};

uint16_t* PD_DCmix_Pin[][3] =	{{PD01_DCmix_EN_Pin, PD01_DCmix_IN1_Pin, PD01_DCmix_IN2_Pin},
								{PD02_DCmix_EN_Pin, PD02_DCmix_IN1_Pin, PD02_DCmix_IN2_Pin},
								{PD03_DCmix_EN_Pin, PD03_DCmix_IN1_Pin, PD03_DCmix_IN2_Pin},
								{PD04_DCmix_EN_Pin, PD04_DCmix_IN1_Pin, PD04_DCmix_IN2_Pin},
								{PD05_DCmix_EN_Pin, PD05_DCmix_IN1_Pin, PD05_DCmix_IN2_Pin}};

uint16_t* heat_Pin[][3] = 		{{PD01_heat_EN_Pin, PD01_heat_IN1_Pin, PD01_heat_IN2_Pin},
								{PD02_heat_EN_Pin, PD02_heat_IN1_Pin, PD02_heat_IN2_Pin},
								{PD03_heat_EN_Pin, PD03_heat_IN1_Pin, PD03_heat_IN2_Pin},
								{PD04_heat_EN_Pin, PD04_heat_IN1_Pin, PD04_heat_IN2_Pin},
								{PD05_heat_EN_Pin, PD05_heat_IN1_Pin, PD05_heat_IN2_Pin},
								{CF01_heat_EN_Pin, CF01_heat_IN1_Pin, CF01_heat_IN2_Pin}};
/*********************************** Pin configuration ***********************************/
void delay_100us_powder (uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim7,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim7) < time);  // wait for the counter to reach the us input in the parameter
}

//void delay_100us_powder_short (uint16_t time)
//{
//	__HAL_TIM_SET_COUNTER(&htim9,0);  // set the counter value a 0
//	while (__HAL_TIM_GET_COUNTER(&htim9) < time);  // wait for the counter to reach the us input in the parameter
//}

//void delay_100us_PD03_milktea (uint16_t time)
//{
//	__HAL_TIM_SET_COUNTER(&htim10,0);  // set the counter value a 0
//	while (__HAL_TIM_GET_COUNTER(&htim10) < time);  // wait for the counter to reach the us input in the parameter
//}
//void delay_100us_PD04_icetea (uint16_t time)
//{
//	__HAL_TIM_SET_COUNTER(&htim11,0);  // set the counter value a 0
//	while (__HAL_TIM_GET_COUNTER(&htim11) < time);  // wait for the counter to reach the us input in the parameter
//}
//void delay_100us_PD05_choco (uint16_t time)
//{
//	__HAL_TIM_SET_COUNTER(&htim12,0);  // set the counter value a 0
//	while (__HAL_TIM_GET_COUNTER(&htim12) < time);  // wait for the counter to reach the us input in the parameter
//}




bool init_powderMachine(void)
{
//	 powder motor direction must be fixed!
	// init DCpowder motor direction & disable motor
	for(uint8_t i = 0; i<sizeof(PD_DCpowder_GPIO_Port) / sizeof(PD_DCpowder_GPIO_Port[0]); i++){
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][IN1], PD_DCpowder_Pin[i][IN1], GPIO_PIN_RESET);
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][IN2], PD_DCpowder_Pin[i][IN2], GPIO_PIN_SET);
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][EN], GPIO_PIN_RESET);
	}

	// init DCmix motor direction & disable motor
	for(uint8_t i = 0; i<sizeof(PD_DCmix_GPIO_Port) / sizeof(PD_DCmix_GPIO_Port[0]); i++){
		HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][IN1], PD_DCmix_Pin[i][IN1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][IN2], PD_DCmix_Pin[i][IN2], GPIO_PIN_RESET);
		HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][EN], PD_DCmix_Pin[i][EN], GPIO_PIN_RESET);
	}

	// init PWM
//	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); //PD01_heat
//	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4); //PD02_heat
//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //PD03_heat
//	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); //PD04_heat
//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //PD05_heat
//	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //CF01_heat


	// init heater motor direction
	for(uint8_t i = 0; i<sizeof(heat_GPIO_Port) / sizeof(heat_GPIO_Port[0]); i++){
		HAL_GPIO_WritePin(heat_GPIO_Port[i][IN1], heat_Pin[i][IN1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(heat_GPIO_Port[i][IN2], heat_Pin[i][IN2], GPIO_PIN_RESET);
	}

	// fan motor init(stop)
	HAL_GPIO_WritePin(PD_Fan01_relay_GPIO_Port, PD_Fan01_relay_Pin, GPIO_PIN_RESET);	// Relay Open
	HAL_GPIO_WritePin(PD_Fan02_relay_GPIO_Port, PD_Fan02_relay_Pin, GPIO_PIN_RESET);	// Relay Open

	return true;
}

bool motorShutdownAll_powderMachine(void)
{
	for(uint8_t i = 0; i<sizeof(PD_DCpowder_GPIO_Port); i++){
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][IN1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][IN2], GPIO_PIN_SET);

		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][EN], GPIO_PIN_RESET);
	}
	for(uint8_t i = 0; i<sizeof(PD_DCmix_GPIO_Port); i++){
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][IN1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][IN2], GPIO_PIN_SET);
		HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][EN], PD_DCmix_Pin[i][EN], GPIO_PIN_RESET);
	}
	for(uint8_t i = 0; i<sizeof(heat_GPIO_Port); i++){
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][IN1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][IN2], GPIO_PIN_SET);
		HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_RESET);
//		heater_control(i, HEATER_MOTOR_STOP);
	}

	// fan motor init(stop)
	HAL_GPIO_WritePin(PD_Fan01_relay_GPIO_Port, PD_Fan01_relay_Pin, GPIO_PIN_SET);	// Relay Open
	HAL_GPIO_WritePin(PD_Fan02_relay_GPIO_Port, PD_Fan02_relay_Pin, GPIO_PIN_SET);	// Relay Open

	return true;
}


bool run_powderMachine(void)
{
	// fan motor init(on)
	HAL_GPIO_WritePin(PD_Fan01_relay_GPIO_Port, PD_Fan01_relay_Pin, GPIO_PIN_SET);	// Relay Close
	HAL_GPIO_WritePin(PD_Fan02_relay_GPIO_Port, PD_Fan02_relay_Pin, GPIO_PIN_SET);	// Relay Close

	return true;
}

//uint8_t getStatus_powderMachine(void)
//{
//
//}

bool cleanPowderMachine(uint32_t* hot_water_gram, uint32_t* powder_gram)
{
	uint32_t water_exit_time[POWDER_NUM] = {0,};//(int)(((hot_water_amout-0.45)/21.1)*1000); // macro function.
	uint32_t water_time = 2500;
//	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MALCHA][EN], heat_Pin[POWDER_MALCHA][EN], GPIO_PIN_SET);
//	HAL_Delay(water_time);
//	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MALCHA][EN], heat_Pin[POWDER_MALCHA][EN], GPIO_PIN_RESET);

	// 0. heater on -> pre-wataer
	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MILK][EN], heat_Pin[POWDER_MILK][EN], GPIO_PIN_SET);
	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_CHOCO][EN], heat_Pin[POWDER_CHOCO][EN], GPIO_PIN_SET);
	HAL_Delay(water_time);

	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MILK][EN], heat_Pin[POWDER_MILK][EN], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_CHOCO][EN], heat_Pin[POWDER_CHOCO][EN], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MALCHA][EN], heat_Pin[POWDER_MALCHA][EN], GPIO_PIN_SET);
	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_ICETEA][EN], heat_Pin[POWDER_ICETEA][EN], GPIO_PIN_SET);
	HAL_Delay(water_time);

	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MALCHA][EN], heat_Pin[POWDER_MALCHA][EN], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_ICETEA][EN], heat_Pin[POWDER_ICETEA][EN], GPIO_PIN_RESET);

	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MILKTEA][EN], heat_Pin[POWDER_MILKTEA][EN], GPIO_PIN_SET);
	HAL_Delay(water_time);
	HAL_GPIO_WritePin(heat_GPIO_Port[POWDER_MILKTEA][EN], heat_Pin[POWDER_MILKTEA][EN], GPIO_PIN_RESET);

	return true;
}

bool make_powderbeverage(uint32_t* hot_water_gram, uint32_t* powder_gram)
{
	HAL_GPIO_WritePin(PD_Fan01_relay_GPIO_Port, PD_Fan01_relay_Pin, GPIO_PIN_SET);	// Relay Open
	HAL_GPIO_WritePin(PD_Fan02_relay_GPIO_Port, PD_Fan02_relay_Pin, GPIO_PIN_SET);	// Relay Open

	uint32_t water_exit_time[POWDER_NUM] = {0,};
	uint32_t powder_exit_time[POWDER_NUM] = {0,};
	uint32_t water_dutyRate[POWDER_NUM] = {0,};

	uint8_t recipes_idx[3] = {0};
	uint32_t powder_max[3] = {0};
	uint8_t powder_exit_num_holes = 0;

	printf(">> make_powderbeverage !!\n");

	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(powder_gram[i] != 0){
			powder_exit_num_holes ++;
		}
	}

	printf("powder holes : %d \n", powder_exit_num_holes);

	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(powder_max[0] < powder_gram[i]){
			powder_max[0] = powder_gram[i];
			recipes_idx[0] = i;
		}
	}

	for(uint8_t i=0; i<POWDER_NUM; i++){
		if((powder_max[1] < powder_gram[i]) && (i != recipes_idx[1])){
			powder_max[1] = powder_gram[i];
			recipes_idx[1] = i;
		}
	}

	for(uint8_t i=0; i<POWDER_NUM; i++){
		if((powder_max[2] < powder_gram[i]) && (i != recipes_idx[1]) && (i != recipes_idx[2])){
			powder_max[2] = powder_gram[i];
			recipes_idx[2] = i;
		}
	}

	for(int i=0; i<powder_exit_num_holes; i++){
		printf("recipes_idx[%d] : %d \n", i, recipes_idx[i]);
	}

	// 0. heater on -> pre-wataer
 	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(hot_water_gram[i] != 0){
			water_exit_time[i] = (int)(((hot_water_gram[i]-0.45)/21.1)*1000);
			printf("water[%d] : %d\n", i, water_exit_time[i]);

//			HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_SET);

		}
	}
 	HAL_GPIO_WritePin(heat_GPIO_Port[recipes_idx[0]][EN], heat_Pin[recipes_idx[0]][EN], GPIO_PIN_SET);

 	if(powder_exit_num_holes != 1){
		HAL_GPIO_WritePin(heat_GPIO_Port[recipes_idx[1]][EN], heat_Pin[recipes_idx[1]][EN], GPIO_PIN_SET);
 	}
	HAL_Delay(1000); // wait time

 	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(hot_water_gram[i] != 0){
			HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_RESET);

			HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_RESET);
		}
	}



	///////////////////////////////////////////////////////////////////////////////////////////
	// 1. exit powder
	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(powder_gram[i] != 0){
//			water_dutyRate[i] = 200;
			HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][EN], GPIO_PIN_SET);
			HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][EN], PD_DCmix_Pin[i][EN], GPIO_PIN_SET);
		}
	}

	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(powder_gram[i] != 0){
			printf("powder_gram[%d]: %d\n", i, powder_gram[i]);

			powder_exit_time[i] = ((powder_gram[i])*250);
			water_dutyRate[i] = (uint32_t)((water_exit_time[i]*100/powder_exit_time[i]));
			printf("powder[%d]: %d\n", i, powder_exit_time[i]);
			printf("dutyRate[%d]: %d\n", i, water_dutyRate[i]);
		}
	}

	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(powder_gram[i] != 0){
			HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][EN], GPIO_PIN_SET);
			HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][EN], PD_DCmix_Pin[i][EN], GPIO_PIN_SET);
		}
	}


	makeDutyRatio(recipes_idx[0], recipes_idx[1], water_dutyRate[recipes_idx[0]], water_dutyRate[recipes_idx[1]], powder_exit_time[recipes_idx[0]], powder_exit_time[recipes_idx[1]]);

	// 3. STOP EXIT powder
	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(powder_gram[i] != 0){
			HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][EN], GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][EN], PD_DCmix_Pin[i][EN], GPIO_PIN_RESET);
		}
	}
	if(powder_exit_num_holes == 3){
		makeDutyRatio(recipes_idx[2], recipes_idx[2], water_dutyRate[recipes_idx[2]], water_dutyRate[recipes_idx[2]], powder_exit_time[recipes_idx[2]], powder_exit_time[recipes_idx[2]]);

	}

	printf("finished! !!\n");

	return true;
}
//////////////////////////////// FOR TEST ///////////////////////////
bool dispatch_hot_coffee_water(uint16_t hot_water_gram)
{
	if(hot_water_gram != 0){
		HAL_GPIO_WritePin(heat_GPIO_Port[CF01_HEAT][EN], heat_Pin[CF01_HEAT][EN], GPIO_PIN_SET);
//		HAL_Delay(WATER_EXIT_TIME(180000)); //18sec
//		HAL_Delay(17300); //18sec -> 384g [hot americano] 17.750=> 383g / 17500=>378g /17000->367g
//		HAL_Delay(14000);
//		X=(Y-0.45)/21.1
		HAL_Delay(5600); //120G
//		if(hot_water_gram == 375)
//			HAL_Delay(17500); //18sec -> 384g [hot americano] 17.750=> 383g / 17500=>378g,376g,383g
	}

	HAL_GPIO_WritePin(heat_GPIO_Port[CF01_HEAT][EN], heat_Pin[CF01_HEAT][EN], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(heat_GPIO_Port[CF01_HEAT][IN1], heat_Pin[CF01_HEAT][IN1], GPIO_PIN_SET);
	HAL_GPIO_WritePin(heat_GPIO_Port[CF01_HEAT][IN2], heat_Pin[CF01_HEAT][IN2], GPIO_PIN_SET);
}

void makeDutyRatio(uint8_t powder_hole1, uint8_t powder_hole2, uint32_t dutyRate1, uint32_t dutyRate2, uint32_t exit_time1, uint32_t exit_time2) // 0~100
{
	printf(" %d, %d, %d, %d, %d, %d\n", powder_hole1, powder_hole2, dutyRate1, dutyRate2, exit_time1, exit_time2);
	// 1CYCLE : 100ms
	if(powder_hole1 == powder_hole2)
	{
//		dutyRate1 : 6 X, 15 OK
		for(int i=0; i<exit_time1/100; i++){

			HAL_GPIO_WritePin(heat_GPIO_Port[powder_hole1][EN], heat_Pin[powder_hole1][EN], GPIO_PIN_SET);
			HAL_Delay(dutyRate1);

			HAL_GPIO_WritePin(heat_GPIO_Port[powder_hole1][EN], heat_Pin[powder_hole1][EN], GPIO_PIN_RESET);
			HAL_Delay(100-dutyRate1);

			printf("100ms\n");
		}

	}
	else{
		uint32_t exit_time_long = exit_time1;
		uint32_t exit_time_short = exit_time2;
		uint32_t exit_duty_long = dutyRate1;
		uint32_t exit_duty_short = dutyRate2;
		uint8_t powder_hole_long = powder_hole1;
		uint8_t powder_hole_short = powder_hole2;

		if(exit_time_long < exit_time_short){
			uint32_t exit_time_tmp = exit_time_long;

			exit_time_long = exit_time_short;
			exit_time_short = exit_time_tmp;

		}
		bool reverse_duty = false;

		if(exit_duty_long < exit_duty_short){
			reverse_duty = true;
			uint8_t powder_hole_tmp = powder_hole_long;

			uint32_t exit_duty_tmp = exit_duty_long;

			exit_duty_long = exit_duty_short;
			exit_duty_short = exit_duty_tmp;
			powder_hole_long = powder_hole_tmp;
			powder_hole_tmp = powder_hole_short;
		}

		for(int i=0; i<exit_time_short/100; i++){

			HAL_GPIO_WritePin(heat_GPIO_Port[powder_hole_short][EN], heat_Pin[powder_hole_short][EN], GPIO_PIN_SET);
			HAL_GPIO_WritePin(heat_GPIO_Port[powder_hole_long][EN], heat_Pin[powder_hole_long][EN], GPIO_PIN_SET);

			HAL_Delay(exit_duty_short);
	//		printf("pwm start-short set, long set\n");
			HAL_GPIO_WritePin(heat_GPIO_Port[powder_hole_short][EN], heat_Pin[powder_hole_short][EN], GPIO_PIN_RESET);
			HAL_Delay(exit_duty_long-exit_duty_short);

	//		delay_100us_powder(exit_duty_short*100);
	//		delay_100us_powder(4800);
			HAL_GPIO_WritePin(heat_GPIO_Port[powder_hole_long][EN], heat_Pin[powder_hole_long][EN], GPIO_PIN_RESET);

			HAL_Delay(100-exit_duty_long);
	//		delay_100us_powder((100-exit_duty_short)*100);
			printf("100ms\n");
		}

		for(int i=0; i<(exit_time_long-exit_time_short)/100; i++){

			if(reverse_duty == true){
				HAL_GPIO_WritePin(heat_GPIO_Port[powder_hole_long][EN], heat_Pin[powder_hole_long][EN], GPIO_PIN_RESET);
				HAL_Delay(exit_duty_short);

			}

		}

	}

}

void caffelatte(uint16_t hot_water_gram)
{
//	uint32_t water_exit_time = 11116;//(int)(((hot_water_amout-0.45)/21.1)*1000); // macro function.
//	water_exit_time = 18000;//27500;//11000*3;//(int)(((hot_water_amout-0.45)/21.1)*1000); // macro function.
//
//	uint32_t powder_exit_time = 8000;//16500;//748->2g;//(int)(water_exit_time * 3.5);
//	HAL_GPIO_WritePin(heat_GPIO_Port[0][EN], heat_Pin[0][EN], GPIO_PIN_SET);
//
//	HAL_Delay(1000); // wait time
//	uint32_t dutyRate = 40;
//	HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[0][EN], PD_DCpowder_Pin[0][EN], GPIO_PIN_SET);
//	HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[0][EN], PD_DCmix_Pin[0][EN], GPIO_PIN_SET);
//
//	makeDutyRatio(POWDER_MILK, dutyRate, powder_exit_time);
//	HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[0][EN], PD_DCpowder_GPIO_Port[0][EN], GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[0][EN], PD_DCmix_Pin[0][EN], GPIO_PIN_RESET);
}

void heaterMultiControlTest(void)
{
	HAL_GPIO_WritePin(heat_GPIO_Port[0][EN], heat_Pin[0][EN], GPIO_PIN_SET);
	HAL_Delay(5000);
	HAL_GPIO_WritePin(heat_GPIO_Port[0][EN], heat_Pin[0][EN], GPIO_PIN_RESET);

//{
//	int i = 2;
//	int i2= 4;
//	int i3= 2;
//
//	HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_SET);
//	HAL_GPIO_WritePin(heat_GPIO_Port[i2][EN], heat_Pin[i2][EN], GPIO_PIN_SET);
////	HAL_GPIO_WritePin(heat_GPIO_Port[i3][EN], heat_Pin[i3][EN], GPIO_PIN_SET);
//
//	HAL_Delay(2500);
//	HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(heat_GPIO_Port[i2][EN], heat_Pin[i2][EN], GPIO_PIN_RESET);
////	HAL_GPIO_WritePin(heat_GPIO_Port[i3][EN], heat_Pin[i3][EN], GPIO_PIN_RESET);
//
//	HAL_Delay(20000);
//	HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_SET);
//	HAL_GPIO_WritePin(heat_GPIO_Port[i2][EN], heat_Pin[i2][EN], GPIO_PIN_SET);
////	HAL_GPIO_WritePin(heat_GPIO_Port[i3][EN], heat_Pin[i3][EN], GPIO_PIN_SET);
//
//	HAL_Delay(5000);
//	HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(heat_GPIO_Port[i2][EN], heat_Pin[i2][EN], GPIO_PIN_RESET);
////	HAL_GPIO_WritePin(heat_GPIO_Port[i3][EN], heat_Pin[i3][EN], GPIO_PIN_RESET);


}

void L298Ntest(void)
{
	HAL_GPIO_WritePin(PD01_DCpowder_EN_GPIO_Port, PD01_DCpowder_EN_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(PD01_DCmix_EN_GPIO_Port, PD01_DCmix_EN_Pin, GPIO_PIN_RESET);
	printf("off\n\n");
	HAL_Delay(5000);

	HAL_GPIO_WritePin(PD01_DCpowder_EN_GPIO_Port, PD01_DCpowder_EN_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(PD01_DCmix_EN_GPIO_Port, PD01_DCmix_EN_Pin, GPIO_PIN_SET);
	printf("on\n\n");
	HAL_Delay(5000);
}

void MalchaTest(uint32_t hot_water_amout)
{
	uint16_t water_exit_time = (int)(((hot_water_amout/1000.0-0.45)/21.1)*1000); // macro function.
	uint16_t powder_exit_time = 2244;//748->2g;//(int)(water_exit_time * 3.5);

	printf("\n\n===waiting ... [%d mg ][%d sec]===\n", hot_water_amout, water_exit_time);

	printf("\n\nwater_exit_time : %d sec, hot_water_amout : %d mg\n", water_exit_time, hot_water_amout);
	printf("powder_exit_time : %d\n\n", powder_exit_time);
	HAL_Delay(1000);
//	printf("water exit started!\n");
//
//	TIM4->CCR3 = HEATER_MOTOR_MAX;//->v//500,1000;//->8.8v //65534; // MAX :65535
//	HAL_Delay(water_exit_time);
//	TIM4->CCR3 = HEATER_MOTOR_STOP; // MAX :65535
//
//	HAL_Delay(100); // delay for water pumping time

	printf("powder start!\n");

	HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[0][EN], PD_DCpowder_Pin[0][EN], GPIO_PIN_SET);
	HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[0][EN], PD_DCmix_Pin[0][EN], GPIO_PIN_SET);

	HAL_Delay(powder_exit_time);

	HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[0][EN], PD_DCpowder_Pin[0][EN], GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[0][EN], PD_DCmix_Pin[0][EN], GPIO_PIN_RESET);
	printf("powder finished!\n");

	printf("-- all finished!\n");
}

void powderMotor_controlTest(void)
{
	for(uint8_t i = 0; i<sizeof(PD_DCpowder_GPIO_Port); i++){
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][EN], GPIO_PIN_SET);
	}
	for(uint8_t i = 0; i<sizeof(PD_DCmix_GPIO_Port); i++){
		HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][EN], PD_DCmix_Pin[i][EN], GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(PD02_DCpowder_EN_GPIO_Port, PD02_DCpowder_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PD02_DCpowder_EN_GPIO_Port, PD02_DCpowder_IN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PD02_DCpowder_EN_GPIO_Port, PD02_DCpowder_IN2_Pin, GPIO_PIN_SET);
}
