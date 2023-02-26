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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define timer_ms(t) (int)(t/10)
#define ROBOT_ON  1
#define ROBOT_OFF 0
#define LED_RESET 2

// PC protocol
#define STX_idx 		0
#define LEN_idx 		1
#define SYNC_NUM_idx 	2
#define CMD_idx 		3
#define DEV_ID_idx 		4
#define DATA_idx 		5

#define STX_val 		0x02
#define ETX_val 		0x03
// DEV_LIST
#define ID_BOO	 		0x11

// PC_CMD_LIST
#define get_status 		0x00
#define connect 		0x01
#define disconnect 		0x02
#define get_connection 	0x03
#define is_connected	0x04
#define stop 			0x05
#define is_stop 		0x06
#define initialize 		0x07
#define get_error 		0x08
#define robot_boot 		0x09
#define led_boot 		0x0a
#define get_data 		0x0b

//
#define robotLifeCheck 0
#define Estop_Button 1
#define robotEstop_pendant 2
#define protective 3
#define Rail_Alram 4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t PC_CMD_arr[100];
bool 	ID_BOO_flag;
uint8_t process_idx;
uint32_t boo_timer_cnt;
bool boo_timer_flag;




GPIO_TypeDef * robotRail_GPIO_Port[] 	= {robotLifeCheck_GPIO_Port,
											Estop_Button_GPIO_Port,
											robotEstop_pendant_GPIO_Port,
											protective_GPIO_Port,
											Rail_Alram_GPIO_Port};

uint16_t* 		robotRail_Pin[] 		= {robotLifeCheck_Pin,
											Estop_Button_Pin,
											robotEstop_pendant_Pin,
											protective_Pin,
											Rail_Alram_Pin};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM11_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len)
{
	if(HAL_UART_Transmit(&huart3, (uint8_t *)ptr, len, 10) == HAL_OK)
		return len;
	return -1;
}

void receivePcCmd(void)
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
		for(int i=0; i<len+2; i++){ // for test
			PC_CMD_arr[i] = data_arr[i];
		}


		free(data_arr);
	}


}
void init_bootStop(void)
{
	HAL_GPIO_WritePin(robotOn_GPIO_Port, robotOn_Pin, GPIO_PIN_RESET);	// Relay Open
	HAL_GPIO_WritePin(robotOff_GPIO_Port, robotOff_Pin, GPIO_PIN_RESET);	// Relay Open
	HAL_GPIO_WritePin(ledReset_GPIO_Port, ledReset_Pin, GPIO_PIN_RESET);	// Relay Open
}

void BOO()
{
	switch(PC_CMD_arr[CMD_idx]){

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
		init_bootStop();
		break;

	case get_error:
		break;

	case robot_boot:
		switch(PC_CMD_arr[DATA_idx])
		{
			case ROBOT_ON:
				switch(process_idx){
					case 0:
						HAL_GPIO_WritePin(robotOn_GPIO_Port, robotOn_Pin, GPIO_PIN_SET);	// Relay Open
						process_idx ++;
						boo_timer_flag = true;
						break;

					case 1:
						if(boo_timer_flag == true && boo_timer_cnt > timer_ms(500)){
							HAL_GPIO_WritePin(robotOn_GPIO_Port, robotOn_Pin, GPIO_PIN_RESET);	// Relay Open
							process_idx = 0;
							uint8_t statusData[1] = {1};
							resonsePacket(statusData, sizeof(statusData));
							ID_BOO_flag = false;
						}
						break;
					}
				break;

			case ROBOT_OFF:
				switch(process_idx){
					case 0:
						HAL_GPIO_WritePin(robotOff_GPIO_Port, robotOff_Pin, GPIO_PIN_SET);	// Relay Open
						process_idx ++;
						boo_timer_flag = true;
						break;


					case 1:
						if(boo_timer_flag == true && boo_timer_cnt > timer_ms(500)){
							HAL_GPIO_WritePin(robotOff_GPIO_Port, robotOff_Pin, GPIO_PIN_RESET);	// Relay Open
							process_idx = 0;
							uint8_t statusData[1] = {1};
							resonsePacket(statusData, sizeof(statusData));
							ID_BOO_flag = false;
						}
						break;
					}
				break;
		}

	case led_boot:
		switch(process_idx){
			case 0:
				HAL_GPIO_WritePin(ledReset_GPIO_Port, ledReset_Pin, GPIO_PIN_SET);	// Relay Open
				process_idx ++;
				boo_timer_flag = true;
				break;


			case 1:
				if(boo_timer_flag == true && boo_timer_cnt > timer_ms(500)){
					HAL_GPIO_WritePin(ledReset_GPIO_Port, ledReset_Pin, GPIO_PIN_RESET);	// Relay Open
					process_idx = 0;

					uint8_t statusData[1] = {1};
					resonsePacket(statusData, sizeof(statusData));

					ID_BOO_flag = false;

					HAL_UART_Transmit(&huart5, data_arr, data_arr[len]+2, 100);
				}
				break;
			}
		break;

	case get_data:
		uint8_t statusData[5] = {0};

		for(int i=0; i<dataLen; i++){
			statusData[i] = HAL_GPIO_ReadPin(get_data, robotRail_GPIO_Port[i], robotRail_Pin[i]);
		}

		// make packet
		resonsePacket(statusData, sizeof(statusData));

		ID_BOO_flag = false;
		break;
	}
}


void resonsePacket(int8_t* dataPacket, uint8_t dataLen)
{
	// make packet
	uint8_t len = 5+dataLen;

	uint8_t *data_arr = (uint8_t*)malloc(sizeof(uint8_t) * (len+2));

	for(int i=0; i<len+2; i++){
		data_arr[i] = 0;
	}

	data_arr[STX_idx] = stx;
	data_arr[LEN_idx] = len;

	data_arr[SYNC_NUM_idx] = PC_CMD_arr[SYNC_NUM_idx];
	data_arr[CMD_idx] = PC_CMD_arr[CMD_idx];
	data_arr[DEV_ID_idx] = ID_BOO;

	// data init
	for(int i=0; i<dataLen; i++){
		data_arr[DATA_idx] = dataPacket[i];
	}
	// calc checksum
	for(int i = 2; i < len; i++)
		data_arr[len+1] ^= data_arr[i];

	HAL_UART_Transmit(&huart5, data_arr, data_arr[len]+2, 100);
	free(data_arr);
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
  MX_USART3_UART_Init();
  MX_TIM11_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  init_bootStop();
  ID_BOO_flag = false;
  process_idx = 0;
  boo_timer_cnt = 0;
  boo_timer_flag = false;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  receivePcCmd();

	  if(ID_BOO_flag == true){
		  BOO();
	  }

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  htim11.Init.Prescaler = 18-1;
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
  huart5.Init.BaudRate = 19200;
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
  huart3.Init.BaudRate = 115200;
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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, robotOff_Pin|robotOn_Pin|ledReset_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : robotLifeCheck_Pin Rail_Alram_Pin Estop_Button_Pin robotEstop_pendant_Pin
                           protective_Pin */
  GPIO_InitStruct.Pin = robotLifeCheck_Pin|Rail_Alram_Pin|Estop_Button_Pin|robotEstop_pendant_Pin
                          |protective_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : robotOff_Pin robotOn_Pin ledReset_Pin */
  GPIO_InitStruct.Pin = robotOff_Pin|robotOn_Pin|ledReset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
