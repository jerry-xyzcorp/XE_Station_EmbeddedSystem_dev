/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef enum { false, true } bool;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define robotLifeCheck_Pin GPIO_PIN_2
#define robotLifeCheck_GPIO_Port GPIOE
#define Rail_Alram_Pin GPIO_PIN_3
#define Rail_Alram_GPIO_Port GPIOE
#define Estop_Button_Pin GPIO_PIN_4
#define Estop_Button_GPIO_Port GPIOE
#define robotEstop_pendant_Pin GPIO_PIN_5
#define robotEstop_pendant_GPIO_Port GPIOE
#define protective_Pin GPIO_PIN_6
#define protective_GPIO_Port GPIOE
#define robotOff_Pin GPIO_PIN_7
#define robotOff_GPIO_Port GPIOF
#define robotOn_Pin GPIO_PIN_8
#define robotOn_GPIO_Port GPIOF
#define ledReset_Pin GPIO_PIN_9
#define ledReset_GPIO_Port GPIOF
#define PC_TX_Pin GPIO_PIN_10
#define PC_TX_GPIO_Port GPIOC
#define PC_RX_Pin GPIO_PIN_11
#define PC_RX_GPIO_Port GPIOC
#define PC_TX2_Pin GPIO_PIN_12
#define PC_TX2_GPIO_Port GPIOC
#define PC_RX2_Pin GPIO_PIN_2
#define PC_RX2_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
