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
#define ON 		0x01
#define READY 	0x02
#define RUNNING	0x03
#define ERROR 	0x04

#define CUP_D 0x02
#define LID_D 0x03
/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PD02_DCmix_EN_Pin GPIO_PIN_2
#define PD02_DCmix_EN_GPIO_Port GPIOE
#define PD04_DCpowder_IN2_Pin GPIO_PIN_3
#define PD04_DCpowder_IN2_GPIO_Port GPIOE
#define PD04_DCpowder_IN1_Pin GPIO_PIN_6
#define PD04_DCpowder_IN1_GPIO_Port GPIOE
#define PD02_DCpowder_IN1_Pin GPIO_PIN_2
#define PD02_DCpowder_IN1_GPIO_Port GPIOF
#define PD01_DCpowder_IN1_Pin GPIO_PIN_3
#define PD01_DCpowder_IN1_GPIO_Port GPIOF
#define Lid_up_Pin GPIO_PIN_4
#define Lid_up_GPIO_Port GPIOF
#define PD01_DCpowder_IN2_Pin GPIO_PIN_5
#define PD01_DCpowder_IN2_GPIO_Port GPIOF
#define PD05_DCpowder_IN1_Pin GPIO_PIN_7
#define PD05_DCpowder_IN1_GPIO_Port GPIOF
#define PD04_DCpowder_EN_Pin GPIO_PIN_8
#define PD04_DCpowder_EN_GPIO_Port GPIOF
#define PD05_DCpowder_IN2_Pin GPIO_PIN_9
#define PD05_DCpowder_IN2_GPIO_Port GPIOF
#define PD01_DCpowder_EN_Pin GPIO_PIN_10
#define PD01_DCpowder_EN_GPIO_Port GPIOF
#define PD02_DCpowder_EN_Pin GPIO_PIN_0
#define PD02_DCpowder_EN_GPIO_Port GPIOH
#define PD02_DCpowder_IN2_Pin GPIO_PIN_1
#define PD02_DCpowder_IN2_GPIO_Port GPIOH
#define ICE01_water_Pin GPIO_PIN_0
#define ICE01_water_GPIO_Port GPIOC
#define Cup_down_Pin GPIO_PIN_2
#define Cup_down_GPIO_Port GPIOC
#define ICE01_mix_Pin GPIO_PIN_3
#define ICE01_mix_GPIO_Port GPIOC
#define PD04_DCmix_IN1_Pin GPIO_PIN_0
#define PD04_DCmix_IN1_GPIO_Port GPIOA
#define ICE01_ice_Pin GPIO_PIN_3
#define ICE01_ice_GPIO_Port GPIOA
#define CF01_heat_IN1_Pin GPIO_PIN_4
#define CF01_heat_IN1_GPIO_Port GPIOA
#define PD03_heat_IN1_Pin GPIO_PIN_5
#define PD03_heat_IN1_GPIO_Port GPIOA
#define PD03_heat_IN2_Pin GPIO_PIN_6
#define PD03_heat_IN2_GPIO_Port GPIOA
#define PD04_heat_IN2_Pin GPIO_PIN_7
#define PD04_heat_IN2_GPIO_Port GPIOA
#define PD04_DCmix_IN2_Pin GPIO_PIN_0
#define PD04_DCmix_IN2_GPIO_Port GPIOB
#define Cup_up_Pin GPIO_PIN_1
#define Cup_up_GPIO_Port GPIOB
#define CF01_heat_IN2_Pin GPIO_PIN_12
#define CF01_heat_IN2_GPIO_Port GPIOF
#define CupD_EN_Pin GPIO_PIN_13
#define CupD_EN_GPIO_Port GPIOF
#define LidD_EN_Pin GPIO_PIN_14
#define LidD_EN_GPIO_Port GPIOF
#define LidD_DIR_Pin GPIO_PIN_15
#define LidD_DIR_GPIO_Port GPIOF
#define PD03_DCpowder_EN_Pin GPIO_PIN_0
#define PD03_DCpowder_EN_GPIO_Port GPIOG
#define PD05_DCpowder_EN_Pin GPIO_PIN_1
#define PD05_DCpowder_EN_GPIO_Port GPIOG
#define PD01_DCmix_IN2_Pin GPIO_PIN_7
#define PD01_DCmix_IN2_GPIO_Port GPIOE
#define PD01_DCmix_IN1_Pin GPIO_PIN_8
#define PD01_DCmix_IN1_GPIO_Port GPIOE
#define CupD_STP_Pin GPIO_PIN_9
#define CupD_STP_GPIO_Port GPIOE
#define PD03_DCmix_IN1_Pin GPIO_PIN_10
#define PD03_DCmix_IN1_GPIO_Port GPIOE
#define CupD_DIR_Pin GPIO_PIN_11
#define CupD_DIR_GPIO_Port GPIOE
#define PD03_DCmix_IN2_Pin GPIO_PIN_12
#define PD03_DCmix_IN2_GPIO_Port GPIOE
#define LidD_STP_Pin GPIO_PIN_13
#define LidD_STP_GPIO_Port GPIOE
#define PD03_DCmix_EN_Pin GPIO_PIN_14
#define PD03_DCmix_EN_GPIO_Port GPIOE
#define PD05_DCmix_IN1_Pin GPIO_PIN_15
#define PD05_DCmix_IN1_GPIO_Port GPIOE
#define PD05_DCmix_IN2_Pin GPIO_PIN_10
#define PD05_DCmix_IN2_GPIO_Port GPIOB
#define PD05_DCmix_EN_Pin GPIO_PIN_11
#define PD05_DCmix_EN_GPIO_Port GPIOB
#define PD02_heat_IN2_Pin GPIO_PIN_12
#define PD02_heat_IN2_GPIO_Port GPIOB
#define PD02_heat_IN1_Pin GPIO_PIN_13
#define PD02_heat_IN1_GPIO_Port GPIOB
#define PD01_heat_IN2_Pin GPIO_PIN_15
#define PD01_heat_IN2_GPIO_Port GPIOB
#define PC_TX_Pin GPIO_PIN_8
#define PC_TX_GPIO_Port GPIOD
#define PC_RX_Pin GPIO_PIN_9
#define PC_RX_GPIO_Port GPIOD
#define PD02_DCmix_IN2_Pin GPIO_PIN_11
#define PD02_DCmix_IN2_GPIO_Port GPIOD
#define PD02_DCmix_IN1_Pin GPIO_PIN_12
#define PD02_DCmix_IN1_GPIO_Port GPIOD
#define PD01_DCmix_EN_Pin GPIO_PIN_13
#define PD01_DCmix_EN_GPIO_Port GPIOD
#define PD05_heat_IN1_Pin GPIO_PIN_14
#define PD05_heat_IN1_GPIO_Port GPIOD
#define PD05_heat_IN2_Pin GPIO_PIN_15
#define PD05_heat_IN2_GPIO_Port GPIOD
#define PD01_heat_IN1_Pin GPIO_PIN_6
#define PD01_heat_IN1_GPIO_Port GPIOC
#define PD04_heat_EN_Pin GPIO_PIN_7
#define PD04_heat_EN_GPIO_Port GPIOC
#define PD03_heat_EN_Pin GPIO_PIN_15
#define PD03_heat_EN_GPIO_Port GPIOA
#define CupD_TX_Pin GPIO_PIN_12
#define CupD_TX_GPIO_Port GPIOC
#define PD03_DCpowder_IN1_Pin GPIO_PIN_0
#define PD03_DCpowder_IN1_GPIO_Port GPIOD
#define PD03_DCpowder_IN2_Pin GPIO_PIN_1
#define PD03_DCpowder_IN2_GPIO_Port GPIOD
#define CupD_RX_Pin GPIO_PIN_2
#define CupD_RX_GPIO_Port GPIOD
#define PD_Fan02_relay_Pin GPIO_PIN_5
#define PD_Fan02_relay_GPIO_Port GPIOD
#define PD_Fan01_relay_Pin GPIO_PIN_6
#define PD_Fan01_relay_GPIO_Port GPIOD
#define ICE01_relay_Pin GPIO_PIN_7
#define ICE01_relay_GPIO_Port GPIOD
#define LidD_RX_Pin GPIO_PIN_9
#define LidD_RX_GPIO_Port GPIOG
#define LidD_TX_Pin GPIO_PIN_14
#define LidD_TX_GPIO_Port GPIOG
#define PD05_heat_EN_Pin GPIO_PIN_3
#define PD05_heat_EN_GPIO_Port GPIOB
#define CF01_heat_EN_Pin GPIO_PIN_4
#define CF01_heat_EN_GPIO_Port GPIOB
#define PD04_heat_IN1_Pin GPIO_PIN_5
#define PD04_heat_IN1_GPIO_Port GPIOB
#define Lid_down_Pin GPIO_PIN_6
#define Lid_down_GPIO_Port GPIOB
#define PD01_heat_EN_Pin GPIO_PIN_8
#define PD01_heat_EN_GPIO_Port GPIOB
#define PD02_heat_EN_Pin GPIO_PIN_9
#define PD02_heat_EN_GPIO_Port GPIOB
#define PD04_DCmix_EN_Pin GPIO_PIN_0
#define PD04_DCmix_EN_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
