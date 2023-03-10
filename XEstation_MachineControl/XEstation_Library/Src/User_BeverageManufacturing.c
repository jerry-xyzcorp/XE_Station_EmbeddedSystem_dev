/*
 * User_BeverageManufacturing.c
 *
 *  Created on: 2023. 1. 26.
 *      Author: Ethan
 */
#include "User_beverageManufacturing.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

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

void init_powderMachine(void)
{
	// powder motor direction must be fixed!
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

	// init heater motor direction
	for(uint8_t i = 0; i<sizeof(heat_GPIO_Port) / sizeof(heat_GPIO_Port[0]); i++){
		HAL_GPIO_WritePin(heat_GPIO_Port[i][IN1], heat_Pin[i][IN1], GPIO_PIN_SET);
		HAL_GPIO_WritePin(heat_GPIO_Port[i][IN2], heat_Pin[i][IN2], GPIO_PIN_RESET);
	}

	// init PWM
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); //PD01_heat
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4); //PD02_heat
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //PD03_heat
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); //PD04_heat
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); //PD05_heat
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); //CF01_heat

	// motor stop
	TIM4->CCR3 = HEATER_MOTOR_STOP;
	TIM4->CCR4 = HEATER_MOTOR_STOP;
	TIM2->CCR1 = HEATER_MOTOR_STOP;
	TIM4->CCR2 = HEATER_MOTOR_STOP;
	TIM2->CCR2 = HEATER_MOTOR_STOP;
	TIM4->CCR3 = HEATER_MOTOR_STOP;
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
//	TIM4->CCR3 = HEATER_MOTOR_MAX;//->v//500,1000;//->8.8v //65534;//65534; // MAX :65535
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

bool motorShutdownAll_powderMachine(void)
{
	for(uint8_t i = 0; i<sizeof(PD_DCpowder_GPIO_Port); i++){
		HAL_GPIO_WritePin(PD_DCpowder_GPIO_Port[i][EN], PD_DCpowder_Pin[i][EN], GPIO_PIN_RESET);
	}
	for(uint8_t i = 0; i<sizeof(PD_DCmix_GPIO_Port); i++){
		HAL_GPIO_WritePin(PD_DCmix_GPIO_Port[i][EN], PD_DCmix_Pin[i][EN], GPIO_PIN_RESET);
	}
	for(uint8_t i = 0; i<sizeof(heat_GPIO_Port); i++){
		HAL_GPIO_WritePin(heat_GPIO_Port[i][EN], heat_Pin[i][EN], GPIO_PIN_RESET);
	}

	TIM4->CCR3 = HEATER_MOTOR_STOP;
	TIM4->CCR4 = HEATER_MOTOR_STOP;
	TIM2->CCR1 = HEATER_MOTOR_STOP;
	TIM4->CCR2 = HEATER_MOTOR_STOP;
	TIM2->CCR2 = HEATER_MOTOR_STOP;
	TIM4->CCR3 = HEATER_MOTOR_STOP;
}

uint8_t getStatus_powderMachine(void)
{

}

bool make_Beverage(uint16_t* hot_water_gram, uint16_t* powder_gram)
{
	// 1. heater on
	for(uint8_t i=0; i<HEATER_NUM; i++){
		if(hot_water_gram[i] != 0){

		}
	}

	// 2, exit powder
	for(uint8_t i=0; i<POWDER_NUM; i++){
		if(powder_gram[i] != 0){

		}
	}
}

//////////////////////////////// FOR TEST ///////////////////////////
void heaterTest(void)
{
	int max = 65534;
	// HEATER

	TIM4->CCR3 = 0;//(int)max/2; // MAX :65535
	printf("water exit finished!\n");
	HAL_Delay(5000);

	TIM4->CCR3 = max; // MAX :65535
	printf("water exit started!\n");
	HAL_Delay(5000);

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









