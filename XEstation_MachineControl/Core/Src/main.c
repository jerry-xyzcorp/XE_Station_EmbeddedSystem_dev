/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>    //library to send string to usart
#include <stdio.h>

#include "User_beverageManufacturing.h"
#include "User_CupLidDispenser.h"
#include "User_IceDispenser.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// PC protocol
#define STX_idx 		0
#define LEN_idx 		1
#define SYNC_NUM_idx 	2
#define CMD_idx 		3
#define DEV_ID_idx 		4
#define DATA_idx 		5

#define STX_val 		0x02
#define ETX_val 		0x03

#define DEV_NUM			5
// DEV_LIST
#define ID_POW	 		0x00
#define ID_ICE 			0x01
#define ID_CUP 			0x02
#define ID_LID 			0x03
#define ID_HOT 			0x04

// PC_CMD_LIST
#define get_status 		0x00
#define connect 		0x01
#define disconnect 		0x02
#define get_connection 	0x03
#define is_connected	0x04
#define stop 			0x05
#define is_stop 		0x06
#define initialize 		0x07
#define is_ready		0x08
#define test 			0x09
#define get_error 		0x0a
#define is_busy 		0x0b
#define is_idle 		0x0c
#define make_beverage 	0x0d
#define dispatch 		0x0d
#define clean 			0x0e
#define rotate 			0x0e
#define get_sensor 		0x0f
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim12;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* Definitions for Task_pcCommunic */
osThreadId_t Task_pcCommunicHandle;
const osThreadAttr_t Task_pcCommunic_attributes = {
  .name = "Task_pcCommunic",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task_powder */
osThreadId_t Task_powderHandle;
const osThreadAttr_t Task_powder_attributes = {
  .name = "Task_powder",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for Task_ice */
osThreadId_t Task_iceHandle;
const osThreadAttr_t Task_ice_attributes = {
  .name = "Task_ice",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for Task_cup */
osThreadId_t Task_cupHandle;
const osThreadAttr_t Task_cup_attributes = {
  .name = "Task_cup",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for Task_lid */
osThreadId_t Task_lidHandle;
const osThreadAttr_t Task_lid_attributes = {
  .name = "Task_lid",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for Task_hot */
osThreadId_t Task_hotHandle;
const osThreadAttr_t Task_hot_attributes = {
  .name = "Task_hot",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* USER CODE BEGIN PV */
uint8_t pc_res_packet[100] = {0};
uint8_t cupD_res_packet[20] = {0};
uint8_t LidD_res_packet[20] = {0};
bool pc_comm_flag = false;

bool 		DEV_ID_flag[DEV_NUM] 	  = {false, false, false, false, false};
uint8_t 	PC_CMD_arr[DEV_NUM][100]  = {0, };
uint8_t 	POW_CMD_arr[100] = {0, };
uint8_t 	TEST_CMD_arr[100] = {0, };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM12_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM11_Init(void);
void thread_pcCommunication(void *argument);
void thread_powder(void *argument);
void thread_ice(void *argument);
void thread_cup(void *argument);
void thread_lid(void *argument);
void thread_hot(void *argument);

/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len)
{
	if(HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, 10) == HAL_OK)
		return len;
	return -1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
	{
//		HAL_UART_Transmit(&huart3, cupD_res_packet, sizeof(cupD_res_packet), 100);

//		pc_comm_flag = true;
//
//		HAL_UART_Receive_IT(&huart3, pc_res_packet, sizeof(pc_res_packet));
	}
	else if (huart->Instance == UART5)
	{
		HAL_UART_Transmit(&huart3, cupD_res_packet, sizeof(cupD_res_packet), 100);
//		for(int i=0; i<20; i++){
//			printf("")
//		}
		// calc checksum
		int chksum = 0x00;
		for(int i=1; i<RES_PACKET_BCC; i++){
			chksum += cupD_res_packet[i];
		}
		if (cupD_res_packet[RES_PACKET_BCC] == chksum)
			printf("check sum error!");

	    HAL_UART_Receive_IT(&huart5, cupD_res_packet, sizeof(cupD_res_packet));
	}
	else if (huart->Instance == USART6)
	{
		HAL_UART_Transmit(&huart3, LidD_res_packet, sizeof(LidD_res_packet), 100);

		// calc checksum
		int chksum = 0x00;
		for(int i=1; i<RES_PACKET_BCC; i++){
			chksum += LidD_res_packet[i];
		}
		if (LidD_res_packet[RES_PACKET_BCC] == chksum)
			printf("check sum error!");

	    HAL_UART_Receive_IT(&huart6, LidD_res_packet, sizeof(LidD_res_packet));
	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_UART5_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM12_Init();
  MX_TIM7_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

//  HAL_UART_Receive_IT(&huart3, pc_res_packet, sizeof(pc_res_packet));
	init_iceDispensor();
	initCupLidDispensor();
	init_powderMachine();
//	heaterMultiControlTest();

//	uint32_t* t1, t2;
//	cleanPowderMachine(t1,t2);
//	controlSteppingMotor(LID_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);

//	uint32_t powder_gram[] = {35,10,0,0,35};
//	uint32_t water_gram[] = {70,20,0,0,70};
//	uint32_t powder_gram[] = {0,0,35,0,0};
//	uint32_t water_gram[] = {0,0,70,0,0};

//	uint8_t arr[] = {2, 25, 0, 13, 0, 4, 121, 1, 253, 4, 121, 0, 0, 0, 0, 1, 194, 0, 200, 1, 194, 0, 0, 0, 0, 3, 57};
//	for(int i=0; i<POWDER_NUM; i++){
//		water_gram[i] = (int)(((arr[DATA_idx+2*i] << 8) + arr[DATA_idx+2*i+1])/10);
//		powder_gram[i] = (int)(((arr[DATA_idx+2*i+2*POWDER_NUM] << 8) + arr[DATA_idx+2*i+1+2*POWDER_NUM])/10);
//
//		printf("water_gram[%d] : %d\n", i, water_gram[i]);
//		printf("powder_gram[%d] : %d\n", i, powder_gram[i]);
//	}
//	make_powderbeverage(water_gram, powder_gram);


//  MalchaTest((int)(16.25*1000)); //unit : mg<hot_water_amout>

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_pcCommunic */
//  Task_pcCommunicHandle = osThreadNew(thread_pcCommunication, NULL, &Task_pcCommunic_attributes);
//
//  /* creation of Task_powder */
//  Task_powderHandle = osThreadNew(thread_powder, NULL, &Task_powder_attributes);
//
//  /* creation of Task_ice */
//  Task_iceHandle = osThreadNew(thread_ice, NULL, &Task_ice_attributes);
//
//  /* creation of Task_cup */
//  Task_cupHandle = osThreadNew(thread_cup, NULL, &Task_cup_attributes);
//
//  /* creation of Task_lid */
//  Task_lidHandle = osThreadNew(thread_lid, NULL, &Task_lid_attributes);
//
//  /* creation of Task_hot */
//  Task_hotHandle = osThreadNew(thread_hot, NULL, &Task_hot_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
//  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		uint8_t buf = 0;
		uint8_t stx = 0;
		uint8_t len = 0;

		// receive packet
		HAL_UART_Receive(&huart3, &stx, sizeof(stx), 10);

		// STX
		if (stx == 0x02)
		{
			// LEN
			HAL_UART_Receive(&huart3, &len, sizeof(len), 10);
			uint8_t *data_arr = (uint8_t*)malloc(sizeof(uint8_t) * (len+2));

			for(int i=0; i<len+2; i++){
				data_arr[i] = 0;
			}

			data_arr[0] = stx;
			data_arr[1] = len;

			for(int i=2; i<len+1; i++)
			{
				HAL_UART_Receive(&huart3, &buf, sizeof(buf), 10);

				if(buf != NULL)
					data_arr[i] = buf;
			}

			// check checksum
			for(int i = 2; i < len; i++)
				data_arr[len+1] ^= data_arr[i];

			// EXECUTION CODE
			for(int i=0; i<len+2; i++){ // for test
				PC_CMD_arr[data_arr[DEV_ID_idx]][i] = data_arr[i];
				TEST_CMD_arr[i] = data_arr[i];
			}

			DEV_ID_flag[data_arr[DEV_ID_idx]] = true;

			free(data_arr);
		}

		if(DEV_ID_flag[ID_POW] == true){
			switch(PC_CMD_arr[ID_POW][CMD_idx]){

			case get_status:
				break;

			case connect:
				break;

			case disconnect:
				break;

			case get_connection:
				break;

			case is_connected:
				break;

			case stop:
//				if (motorShutdownAll_powderMachine() == true)
//					is_cmd_success = true;
				break;

			case is_stop:
				break;

			case initialize:
//				if (init_powderMachine() == true)
//					is_cmd_success = true;
				break;

			case is_ready:
				break;
			case test:
				break;
			case get_error:
				break;
			case is_busy:
				break;
			case is_idle:
				break;

			case make_beverage:
			{
				uint32_t water_gram[POWDER_NUM] = {0};
				uint32_t powder_gram[POWDER_NUM] = {0};

				for(int i=0; i<POWDER_NUM; i++){
					water_gram[i] = (int)(((PC_CMD_arr[ID_POW][DATA_idx+2*i] << 8) + PC_CMD_arr[ID_POW][DATA_idx+2*i+1])/10);
					powder_gram[i] = (int)(((PC_CMD_arr[ID_POW][DATA_idx+2*i+2*POWDER_NUM] << 8) + PC_CMD_arr[ID_POW][DATA_idx+2*i+1+2*POWDER_NUM])/10);
				}
				run_powderMachine();
				make_powderbeverage(water_gram, powder_gram);


				// hot
//				caffelatte(235);
				// ice
//				caffelatte(120);
				break;
			}

			case clean:
			{
				uint32_t water_gram[POWDER_NUM] = {0};
				uint32_t powder_gram[POWDER_NUM] = {0};

				for(int i=0; i<POWDER_NUM; i++){
					water_gram[i] = (int)(((PC_CMD_arr[ID_POW][DATA_idx+2*i] << 8) + PC_CMD_arr[ID_POW][DATA_idx+2*i+1])/10);
					powder_gram[i] = (int)(((PC_CMD_arr[ID_POW][DATA_idx+2*i+2*POWDER_NUM] << 8) + PC_CMD_arr[ID_POW][DATA_idx+2*i+1+2*POWDER_NUM])/10);
				}
				cleanPowderMachine(water_gram, powder_gram);

				break;
			}

			case get_sensor:
				break;
			}
			DEV_ID_flag[ID_POW] = false;

			uint8_t* pointer;
			pointer = PC_CMD_arr;
//			HAL_UART_Transmit(&huart3, pointer+ID_POW*100, PC_CMD_arr[ID_POW][LEN_idx]+4, 100); // SUCCESS
			HAL_UART_Transmit(&huart3, (uint8_t *)TEST_CMD_arr, TEST_CMD_arr[LEN_idx]+2, 100); // SUCCESS

			// transmit packet
	//		HAL_UART_Transmit(&huart3, (uint8_t *)POW_CMD_arr, POW_CMD_arr[LEN_idx]+2, 100); // SUCCESS
		}

		if(DEV_ID_flag[ID_ICE] == true){
			switch(PC_CMD_arr[ID_ICE][CMD_idx]){
			case get_status:
				break;
			case connect:
				break;
			case disconnect:
				break;
			case get_connection:
				break;
			case is_connected:
				break;
			case stop:
				break;
			case is_stop:
				break;

			case initialize:
//				if (init_iceDispensor() == true)
//					is_cmd_success = true;
				break;

			case is_ready:
				break;
			case test:
//				printf("ice dispenser test start !\n");

	//			HOT AMERICANO
//				uint16_t water_time = 0;//3500;
//				uint16_t ice_time = 10000;//3500;
//				startExit(water_time, ice_time);

//				HAL_Delay(1000);//0.8sec x 0.85sec
//				water_time = 0;
//				ice_time = 500;
//				startExit(water_time, ice_time);

//				printf("ice dispenser test finished !\n\n");
				break;

			case get_error:
				break;
			case is_busy:
//				is_iceDispensor_busy;
				break;

			case is_idle:
				break;

			case dispatch:

//				uint32_t water_gram; //= 100; // ice americano
//				uint32_t ice_gram; //= 264; // ice americano
//				water_gram = 100;
//				ice_gram = 264;
//				startExit(100, 264);

				//	HOT AMERICANO
//				startExit(0, 3500);

				//CAFFELATTE
				startExit(0, 2700);
				break;

			case clean:
				break;
			case get_sensor:
				break;
			}
			DEV_ID_flag[ID_ICE] = false;

			// transmit packet
			uint8_t* pointer;
			pointer = PC_CMD_arr;
			HAL_UART_Transmit(&huart3, pointer+ID_ICE*100, PC_CMD_arr[ID_ICE][LEN_idx]+2, 100); // SUCCESS
		}

		if(DEV_ID_flag[ID_CUP] == true){
			switch(PC_CMD_arr[ID_CUP][CMD_idx]){
			case get_status:
				break;
			case connect:
				break;
			case disconnect:
				break;
			case get_connection:
				break;
			case is_connected:
				break;
			case stop:
				break;
			case is_stop:
				break;
			case initialize:
				break;
			case is_ready:
				break;
			case test:
				break;
			case get_error:
				break;

			case is_busy:
//				if(is_cup_lid_running(CUP_D) == true)
//					is_xxx_return_val = true;
				break;

			case is_idle:
//				if(is_cup_lid_running(CUP_D) == false)
//					is_xxx_return_val = true;
				break;

			case dispatch:
				cup_lid_dispatch(CUP_D);
				if (HAL_UART_Init(&huart5) != HAL_OK)
				  {
				    Error_Handler();
				  }
				break;

			case rotate:
	//			= PC_CMD_arr[ID_CUP][DATA_idx];
				controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
				break;
			}
			DEV_ID_flag[ID_CUP] = false;

			// transmit packer
			uint8_t* pointer;
			pointer = PC_CMD_arr;
//			HAL_UART_Transmit(&huart3, pointer+ID_CUP*100, PC_CMD_arr[ID_CUP][LEN_idx]+4, 100); // SUCCESS
			HAL_UART_Transmit(&huart3, (uint8_t *)TEST_CMD_arr, TEST_CMD_arr[LEN_idx]+2, 100); // SUCCESS

		}
		if(DEV_ID_flag[ID_LID] == true){
			switch(PC_CMD_arr[ID_LID][CMD_idx]){
			case get_status:
				break;
			case connect:
				break;
			case disconnect:
				break;
			case get_connection:
				break;
			case is_connected:
				break;
			case stop:
				break;
			case is_stop:
				break;

			case initialize:
//				if (initCupLidDispensor() == true)
//					is_cmd_success = true;
				break;

			case is_ready:
				break;

			case test:
				break;

			case get_error:
				break;

			case is_busy:
//				if(is_cup_lid_running(LID_D) == true)
//					is_xxx_return_val = true;
				break;

			case is_idle:
//				if(is_cup_lid_running(LID_D) == false)
//					is_xxx_return_val = true;
				break;

			case dispatch:
				cup_lid_dispatch(LID_D);
				if (HAL_UART_Init(&huart6) != HAL_OK)
				  {
				    Error_Handler();
				  }
				break;

			case rotate:
				controlSteppingMotor(LID_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
				break;
			}
			DEV_ID_flag[ID_LID] = false;

			// transmit packer
			uint8_t* pointer;
			pointer = PC_CMD_arr;
//			HAL_UART_Transmit(&huart3, pointer+ID_LID*100, PC_CMD_arr[ID_LID][LEN_idx]+4, 100); // SUCCESS
			HAL_UART_Transmit(&huart3, (uint8_t *)TEST_CMD_arr, TEST_CMD_arr[LEN_idx]+2, 100); // SUCCESS
		}


		if(DEV_ID_flag[ID_HOT] == true){
			switch(PC_CMD_arr[ID_HOT][CMD_idx]){
			case get_status:
				break;
			case connect:
				break;
			case disconnect:
				break;
			case get_connection:
				break;
			case is_connected:
				break;
			case stop:
				break;
			case is_stop:
				break;
			case initialize:
				break;
			case is_ready:
				break;
			case test:
				break;
			case get_error:
				break;
			case is_busy:
				break;
			case is_idle:
				break;
			case dispatch:
				// for test
//				uint16_t hot_water_gram_arr[5] = {0,};
//				uint16_t powder_gram_arr[6] = {0,};
//
//				hot_water_gram[CF01_HEAT] = 375;
//				make_Beverage(hot_water_gram_arr, powder_gram_arr);
//				dispatch_hot_coffee_water(375);
//				dispatch_hot_coffee_water(120);

				break;
			}
			DEV_ID_flag[ID_HOT] = false;

			// transmit packer

			uint8_t* pointer;
			pointer = PC_CMD_arr;
//			HAL_UART_Transmit(&huart3, pointer+ID_HOT*100, PC_CMD_arr[ID_HOT][LEN_idx]+4, 100); // SUCCESS
			HAL_UART_Transmit(&huart3, (uint8_t *)TEST_CMD_arr, TEST_CMD_arr[LEN_idx]+2, 100); // SUCCESS

		}

	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////

//	  if (pc_comm_flag == true)
//	  {
//		uint8_t test_arr[4] = {0,0,2,3};
//		HAL_UART_Transmit(&huart3, (uint8_t *)test_arr, sizeof(test_arr), 100);
//
//	  // STX
//		if (pc_res_packet[STX_idx] == STX_val){
//			// LEN
//			uint8_t *data_arr = (uint8_t*)malloc(sizeof(uint8_t) * (pc_res_packet[LEN_idx]+2));
//
//			// init data_arr array
//			for(int i=0; i<pc_res_packet[LEN_idx]+2; i++){
//				data_arr[i] = pc_res_packet[i];
//			}
//
//			// check checksum
//			uint8_t CheckSum = 0;
//			for(int i = 2; i < pc_res_packet[LEN_idx]; i++)
//			{
//				CheckSum ^= data_arr[i];
//			}
//			data_arr[pc_res_packet[LEN_idx]+1] = CheckSum;
//
//			if (CheckSum == pc_res_packet[LEN_idx+1] && ETX_val == pc_res_packet[LEN_idx+2])
//			{
//				// transmit packet
//				HAL_UART_Transmit(&huart3, (uint8_t *)data_arr, (data_arr[LEN_idx]+2), 100);
//			}
//			else{
//				for(int i=2; i<pc_res_packet[LEN_idx]; i++){
//					data_arr[i] = 0x00;
//				} // error msg
//				HAL_UART_Transmit(&huart3, (uint8_t *)data_arr, (data_arr[LEN_idx]+2), 100);
//			}
//
//			free(data_arr);
//		}
//		pc_comm_flag = false;
//		HAL_UART_Receive_IT(&huart3, pc_res_packet, sizeof(pc_res_packet));
//	  }
//	  HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1800-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim3, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 1800-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim4, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 9000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 18000-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 65535;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 18000-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 18000-1;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief TIM12 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM12_Init(void)
{

  /* USER CODE BEGIN TIM12_Init 0 */

  /* USER CODE END TIM12_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM12_Init 1 */

  /* USER CODE END TIM12_Init 1 */
  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 18000-1;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 65535;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM12_Init 2 */

  /* USER CODE END TIM12_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 19200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, PD02_DCmix_EN_Pin|PD04_DCpowder_IN2_Pin|PD02_DCpowder_EN_Pin|PD04_DCpowder_IN1_Pin
                          |PD01_DCmix_IN2_Pin|PD01_DCmix_IN1_Pin|CupD_STP_Pin|PD03_DCmix_IN1_Pin
                          |CupD_DIR_Pin|PD03_DCmix_IN2_Pin|LidD_STP_Pin|PD03_DCmix_EN_Pin
                          |PD05_DCmix_IN1_Pin|PD04_DCmix_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, PD02_DCpowder_IN1_Pin|PD01_DCpowder_IN1_Pin|PD01_DCpowder_IN2_Pin|PD05_DCpowder_IN1_Pin
                          |PD04_DCpowder_EN_Pin|PD05_DCpowder_IN2_Pin|PD01_DCpowder_EN_Pin|CF01_heat_IN2_Pin
                          |CupD_EN_Pin|LidD_EN_Pin|LidD_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ICE01_water_Pin|ICE01_mix_Pin|PD01_heat_IN1_Pin|PD04_heat_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PD04_DCmix_IN1_Pin|ICE01_ice_Pin|CF01_heat_IN1_Pin|PD03_heat_IN1_Pin
                          |PD03_heat_IN2_Pin|PD04_heat_IN2_Pin|PD03_heat_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PD04_DCmix_IN2_Pin|PD05_DCmix_IN2_Pin|PD05_DCmix_EN_Pin|PD02_heat_IN2_Pin
                          |PD02_heat_IN1_Pin|PD01_heat_IN2_Pin|PD05_heat_EN_Pin|CF01_heat_EN_Pin
                          |PD04_heat_IN1_Pin|PD01_heat_EN_Pin|PD02_heat_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, PD03_DCpowder_EN_Pin|PD05_DCpowder_EN_Pin|PD02_DCpowder_IN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, PD02_DCmix_IN2_Pin|PD02_DCmix_IN1_Pin|PD01_DCmix_EN_Pin|PD05_heat_IN1_Pin
                          |PD05_heat_IN2_Pin|PD03_DCpowder_IN1_Pin|PD03_DCpowder_IN2_Pin|PD_Fan02_relay_Pin
                          |PD_Fan01_relay_Pin|ICE01_relay_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD02_DCmix_EN_Pin PD04_DCpowder_IN2_Pin PD02_DCpowder_EN_Pin PD04_DCpowder_IN1_Pin
                           PD01_DCmix_IN2_Pin PD01_DCmix_IN1_Pin CupD_STP_Pin PD03_DCmix_IN1_Pin
                           CupD_DIR_Pin PD03_DCmix_IN2_Pin LidD_STP_Pin PD03_DCmix_EN_Pin
                           PD05_DCmix_IN1_Pin PD04_DCmix_EN_Pin */
  GPIO_InitStruct.Pin = PD02_DCmix_EN_Pin|PD04_DCpowder_IN2_Pin|PD02_DCpowder_EN_Pin|PD04_DCpowder_IN1_Pin
                          |PD01_DCmix_IN2_Pin|PD01_DCmix_IN1_Pin|CupD_STP_Pin|PD03_DCmix_IN1_Pin
                          |CupD_DIR_Pin|PD03_DCmix_IN2_Pin|LidD_STP_Pin|PD03_DCmix_EN_Pin
                          |PD05_DCmix_IN1_Pin|PD04_DCmix_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD02_DCpowder_IN1_Pin PD01_DCpowder_IN1_Pin PD01_DCpowder_IN2_Pin PD05_DCpowder_IN1_Pin
                           PD04_DCpowder_EN_Pin PD05_DCpowder_IN2_Pin PD01_DCpowder_EN_Pin CF01_heat_IN2_Pin
                           CupD_EN_Pin LidD_EN_Pin LidD_DIR_Pin */
  GPIO_InitStruct.Pin = PD02_DCpowder_IN1_Pin|PD01_DCpowder_IN1_Pin|PD01_DCpowder_IN2_Pin|PD05_DCpowder_IN1_Pin
                          |PD04_DCpowder_EN_Pin|PD05_DCpowder_IN2_Pin|PD01_DCpowder_EN_Pin|CF01_heat_IN2_Pin
                          |CupD_EN_Pin|LidD_EN_Pin|LidD_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : Lid_up_Pin */
  GPIO_InitStruct.Pin = Lid_up_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Lid_up_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ICE01_water_Pin ICE01_mix_Pin PD01_heat_IN1_Pin PD04_heat_EN_Pin */
  GPIO_InitStruct.Pin = ICE01_water_Pin|ICE01_mix_Pin|PD01_heat_IN1_Pin|PD04_heat_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : Cup_down_Pin */
  GPIO_InitStruct.Pin = Cup_down_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Cup_down_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD04_DCmix_IN1_Pin ICE01_ice_Pin CF01_heat_IN1_Pin PD03_heat_IN1_Pin
                           PD03_heat_IN2_Pin PD04_heat_IN2_Pin PD03_heat_EN_Pin */
  GPIO_InitStruct.Pin = PD04_DCmix_IN1_Pin|ICE01_ice_Pin|CF01_heat_IN1_Pin|PD03_heat_IN1_Pin
                          |PD03_heat_IN2_Pin|PD04_heat_IN2_Pin|PD03_heat_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD04_DCmix_IN2_Pin PD05_DCmix_IN2_Pin PD05_DCmix_EN_Pin PD02_heat_IN2_Pin
                           PD02_heat_IN1_Pin PD01_heat_IN2_Pin PD05_heat_EN_Pin CF01_heat_EN_Pin
                           PD04_heat_IN1_Pin PD01_heat_EN_Pin PD02_heat_EN_Pin */
  GPIO_InitStruct.Pin = PD04_DCmix_IN2_Pin|PD05_DCmix_IN2_Pin|PD05_DCmix_EN_Pin|PD02_heat_IN2_Pin
                          |PD02_heat_IN1_Pin|PD01_heat_IN2_Pin|PD05_heat_EN_Pin|CF01_heat_EN_Pin
                          |PD04_heat_IN1_Pin|PD01_heat_EN_Pin|PD02_heat_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Cup_up_Pin Lid_down_Pin */
  GPIO_InitStruct.Pin = Cup_up_Pin|Lid_down_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD03_DCpowder_EN_Pin PD05_DCpowder_EN_Pin PD02_DCpowder_IN2_Pin */
  GPIO_InitStruct.Pin = PD03_DCpowder_EN_Pin|PD05_DCpowder_EN_Pin|PD02_DCpowder_IN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PD02_DCmix_IN2_Pin PD02_DCmix_IN1_Pin PD01_DCmix_EN_Pin PD05_heat_IN1_Pin
                           PD05_heat_IN2_Pin PD03_DCpowder_IN1_Pin PD03_DCpowder_IN2_Pin PD_Fan02_relay_Pin
                           PD_Fan01_relay_Pin ICE01_relay_Pin */
  GPIO_InitStruct.Pin = PD02_DCmix_IN2_Pin|PD02_DCmix_IN1_Pin|PD01_DCmix_EN_Pin|PD05_heat_IN1_Pin
                          |PD05_heat_IN2_Pin|PD03_DCpowder_IN1_Pin|PD03_DCpowder_IN2_Pin|PD_Fan02_relay_Pin
                          |PD_Fan01_relay_Pin|ICE01_relay_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_thread_pcCommunication */
/**
  * @brief  Function implementing the Task_pcCommunic thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_thread_pcCommunication */
void thread_pcCommunication(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	uint8_t buf = 0;
	uint8_t stx = 0;
	uint8_t len = 0;

	// receive packet
	HAL_UART_Receive(&huart3, &stx, sizeof(stx), 10);

	// STX
	if (stx == 0x02)
	{
		// LEN
		HAL_UART_Receive(&huart3, &len, sizeof(len), 10);
		uint8_t *data_arr = (uint8_t*)malloc(sizeof(uint8_t) * (len+2));

		for(int i=0; i<len+2; i++){
			data_arr[i] = 0;
		}

		data_arr[0] = stx;
		data_arr[1] = len;

		for(int i=2; i<len+1; i++)
		{
			HAL_UART_Receive(&huart3, &buf, sizeof(buf), 10);

			if(buf != NULL)
				data_arr[i] = buf;
		}

		// check checksum
		for(int i = 2; i < len; i++)
			data_arr[len+1] ^= data_arr[i];

		// EXECUTION CODE
		for(int i=0; i<len+2; i++) // for test
			PC_CMD_arr[data_arr[DEV_ID_idx]][i] = data_arr[i];

		DEV_ID_flag[data_arr[DEV_ID_idx]] = true;

		free(data_arr);
	}
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_thread_powder */
/**
* @brief Function implementing the Task_powder thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread_powder */
void thread_powder(void *argument)
{
  /* USER CODE BEGIN thread_powder */
	bool is_cmd_success = false;
	init_powderMachine();
//	powderMotor_controlTest();
//	heaterMultiControlTest();

  /* Infinite loop */
  for(;;)
  {
	if(DEV_ID_flag[ID_POW] == true){
		switch(PC_CMD_arr[ID_POW][CMD_idx]){

		case get_status:
			break;

		case connect:
			break;

		case disconnect:
			break;

		case get_connection:
			break;

		case is_connected:
			break;

		case stop:
			if (motorShutdownAll_powderMachine() == true)
				is_cmd_success = true;
			break;

		case is_stop:
			break;

		case initialize:
			if (init_powderMachine() == true)
				is_cmd_success = true;
			break;

		case is_ready:
			break;
		case test:
			break;
		case get_error:
			break;
		case is_busy:
			break;
		case is_idle:
			break;
		case make_beverage:
			break;
		case clean:
			break;
		case get_sensor:
			break;

		}
		DEV_ID_flag[ID_POW] = false;

		uint8_t* pointer;
		pointer = PC_CMD_arr;
		HAL_UART_Transmit(&huart3, pointer+ID_POW*100, PC_CMD_arr[ID_POW][LEN_idx]+4, 100); // SUCCESS

		// transmit packet
//		HAL_UART_Transmit(&huart3, (uint8_t *)POW_CMD_arr, POW_CMD_arr[LEN_idx]+2, 100); // SUCCESS
	}
    osDelay(1);
  }
  /* USER CODE END thread_powder */
}

/* USER CODE BEGIN Header_thread_ice */
/**
* @brief Function implementing the Task_ice thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread_ice */
void thread_ice(void *argument)
{
  /* USER CODE BEGIN thread_ice */
	bool is_cmd_success = false;

	init_iceDispensor();

  /* Infinite loop */
  for(;;)
  {
	if(DEV_ID_flag[ID_ICE] == true){
		switch(PC_CMD_arr[ID_ICE][CMD_idx]){
		case get_status:
			break;
		case connect:
			break;
		case disconnect:
			break;
		case get_connection:
			break;
		case is_connected:
			break;
		case stop:
			break;
		case is_stop:
			break;

		case initialize:
			if (init_iceDispensor() == true)
				is_cmd_success = true;
			break;

		case is_ready:
			break;
		case test:
			printf("ice dispenser test start !\n");

//			HOT AMERICANO
			uint16_t water_time = 0;//3500;
			uint16_t ice_time = 10000;//3500;
			startExit(water_time, ice_time);

			HAL_Delay(1000);//0.8sec x 0.85sec
			water_time = 0;
			ice_time = 500;
			startExit(water_time, ice_time);

			printf("ice dispenser test finished !\n\n");
			break;

		case get_error:
			break;
		case is_busy:
			break;
		case is_idle:
			break;
		case dispatch:

			startExit(0, ice_time);

			break;
		case clean:
			break;
		case get_sensor:
			break;
		}
		DEV_ID_flag[ID_ICE] = false;
		// transmit packer
		uint8_t tmp_arr = {1,2,3,4,5};
	//		HAL_UART_Transmit(&huart3, (uint8_t *)tmp_arr, 5, 100);

		HAL_UART_Transmit(&huart3, (uint8_t *)PC_CMD_arr[ID_ICE][0], (PC_CMD_arr[ID_ICE][LEN_idx]+2), 100);
	}
    osDelay(1);
  }
  /* USER CODE END thread_ice */
}

/* USER CODE BEGIN Header_thread_cup */
/**
* @brief Function implementing the Task_cup thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread_cup */
void thread_cup(void *argument)
{
  /* USER CODE BEGIN thread_cup */
	bool is_xxx_return_val = false;

	// FOR TEST
//	controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
//	cup_lid_dispatch(CUP_D);

  /* Infinite loop */
  for(;;)
  {
	if(DEV_ID_flag[ID_CUP] == true){
		switch(PC_CMD_arr[ID_CUP][CMD_idx]){
		case get_status:
			break;
		case connect:
			break;
		case disconnect:
			break;
		case get_connection:
			break;
		case is_connected:
			break;
		case stop:
			break;
		case is_stop:
			break;
		case initialize:
			break;
		case is_ready:
			break;
		case test:
			break;
		case get_error:
			break;

		case is_busy:
			if(is_cup_lid_running(CUP_D) == true)
				is_xxx_return_val = true;
			break;

		case is_idle:
			if(is_cup_lid_running(CUP_D) == false)
				is_xxx_return_val = true;
			break;

		case dispatch:
			cup_lid_dispatch(CUP_D);
			break;

		case rotate:
//			= PC_CMD_arr[ID_CUP][DATA_idx];
			controlSteppingMotor(CUP_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
			break;
		}
		DEV_ID_flag[ID_CUP] = false;

		// transmit packer
		HAL_UART_Transmit(&huart3, (uint8_t *)PC_CMD_arr[ID_CUP][0], (PC_CMD_arr[ID_CUP][LEN_idx]+2), 100);
	}
	osDelay(1);

  }
  /* USER CODE END thread_cup */
}

/* USER CODE BEGIN Header_thread_lid */
/**
* @brief Function implementing the Task_lid thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread_lid */
void thread_lid(void *argument)
{
  /* USER CODE BEGIN thread_lid */
	bool is_xxx_return_val = false;
	bool is_cmd_success = false;

  /* Infinite loop */
  for(;;)
  {
	if(DEV_ID_flag[ID_LID] == true){
		switch(PC_CMD_arr[ID_LID][CMD_idx]){
		case get_status:
			break;
		case connect:
			break;
		case disconnect:
			break;
		case get_connection:
			break;
		case is_connected:
			break;
		case stop:
			break;
		case is_stop:
			break;

		case initialize:
			if (initCupLidDispensor() == true)
				is_cmd_success = true;
			break;

		case is_ready:
			break;

		case test:
			break;

		case get_error:
			break;

		case is_busy:
			if(is_cup_lid_running(LID_D) == true)
				is_xxx_return_val = true;
			break;

		case is_idle:
			if(is_cup_lid_running(LID_D) == false)
				is_xxx_return_val = true;
			break;

		case dispatch:
			cup_lid_dispatch(LID_D);
			break;

		case rotate:
			controlSteppingMotor(LID_D, STEPPING_MOTOR_ON, CLOCKWISE, QUARTER, FREQ_FAST);
			break;
		}
		DEV_ID_flag[ID_LID] = false;

		// transmit packer
		HAL_UART_Transmit(&huart3, (uint8_t *)PC_CMD_arr[ID_LID][0], (PC_CMD_arr[ID_LID][LEN_idx]+2), 100);
	}
	osDelay(1);
  }
  /* USER CODE END thread_lid */
}

/* USER CODE BEGIN Header_thread_hot */
/**
* @brief Function implementing the Task_hot thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread_hot */
void thread_hot(void *argument)
{
  /* USER CODE BEGIN thread_hot */
  /* Infinite loop */
  for(;;)
  {
	if(DEV_ID_flag[ID_HOT] == true){
		switch(PC_CMD_arr[ID_HOT][CMD_idx]){
		case get_status:
			break;
		case connect:
			break;
		case disconnect:
			break;
		case get_connection:
			break;
		case is_connected:
			break;
		case stop:
			break;
		case is_stop:
			break;
		case initialize:
			break;
		case is_ready:
			break;
		case test:
			break;
		case get_error:
			break;
		case is_busy:
			break;
		case is_idle:
			break;
		case dispatch:
			break;
		}
		DEV_ID_flag[ID_HOT] = false;

		// transmit packer
		HAL_UART_Transmit(&huart3, (uint8_t *)PC_CMD_arr[ID_HOT][0], (PC_CMD_arr[ID_HOT][LEN_idx]+2), 100);
	}
    osDelay(1);
  }
  /* USER CODE END thread_hot */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM13 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM13) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
