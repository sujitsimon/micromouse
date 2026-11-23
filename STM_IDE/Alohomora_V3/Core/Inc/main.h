/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DISPLAY_RS_Pin GPIO_PIN_13
#define DISPLAY_RS_GPIO_Port GPIOC
#define LED_SOLVE_Pin GPIO_PIN_14
#define LED_SOLVE_GPIO_Port GPIOC
#define VOLTMETER_Pin GPIO_PIN_0
#define VOLTMETER_GPIO_Port GPIOC
#define LED_SPRUN_Pin GPIO_PIN_1
#define LED_SPRUN_GPIO_Port GPIOC
#define LED_RF_Pin GPIO_PIN_2
#define LED_RF_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_3
#define LED_R_GPIO_Port GPIOC
#define RM_ENC_G_Pin GPIO_PIN_0
#define RM_ENC_G_GPIO_Port GPIOA
#define RM_ENC_Y_Pin GPIO_PIN_1
#define RM_ENC_Y_GPIO_Port GPIOA
#define RF_RECEIVER_Pin GPIO_PIN_2
#define RF_RECEIVER_GPIO_Port GPIOA
#define R_RECEIVER_Pin GPIO_PIN_3
#define R_RECEIVER_GPIO_Port GPIOA
#define IMU_CS_Pin GPIO_PIN_4
#define IMU_CS_GPIO_Port GPIOA
#define IMU_CLK_Pin GPIO_PIN_5
#define IMU_CLK_GPIO_Port GPIOA
#define IMU_SDO_Pin GPIO_PIN_6
#define IMU_SDO_GPIO_Port GPIOA
#define IMU_SDI_Pin GPIO_PIN_7
#define IMU_SDI_GPIO_Port GPIOA
#define LED_L_Pin GPIO_PIN_4
#define LED_L_GPIO_Port GPIOC
#define LED_LF_Pin GPIO_PIN_5
#define LED_LF_GPIO_Port GPIOC
#define L_RECEIVER_Pin GPIO_PIN_0
#define L_RECEIVER_GPIO_Port GPIOB
#define LF_RECEIVER_Pin GPIO_PIN_1
#define LF_RECEIVER_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_10
#define BUZZER_GPIO_Port GPIOB
#define LED_CALIB_Pin GPIO_PIN_12
#define LED_CALIB_GPIO_Port GPIOB
#define LED_BATTERY_Pin GPIO_PIN_13
#define LED_BATTERY_GPIO_Port GPIOB
#define SELECT_Pin GPIO_PIN_14
#define SELECT_GPIO_Port GPIOB
#define BACK_Pin GPIO_PIN_15
#define BACK_GPIO_Port GPIOB
#define LED_STRUN_Pin GPIO_PIN_6
#define LED_STRUN_GPIO_Port GPIOC
#define LF_EMITTER_Pin GPIO_PIN_7
#define LF_EMITTER_GPIO_Port GPIOC
#define L_EMITTER_Pin GPIO_PIN_8
#define L_EMITTER_GPIO_Port GPIOC
#define R_EMITTER_Pin GPIO_PIN_9
#define R_EMITTER_GPIO_Port GPIOC
#define RF_EMITTER_Pin GPIO_PIN_8
#define RF_EMITTER_GPIO_Port GPIOA
#define LM_ENC_Y_Pin GPIO_PIN_15
#define LM_ENC_Y_GPIO_Port GPIOA
#define DISPLAY_RESET_Pin GPIO_PIN_11
#define DISPLAY_RESET_GPIO_Port GPIOC
#define DISPLAY_CE_Pin GPIO_PIN_2
#define DISPLAY_CE_GPIO_Port GPIOD
#define LM_ENC_G_Pin GPIO_PIN_3
#define LM_ENC_G_GPIO_Port GPIOB
#define IMU_INT1_Pin GPIO_PIN_4
#define IMU_INT1_GPIO_Port GPIOB
#define IMU_FSYNC_Pin GPIO_PIN_5
#define IMU_FSYNC_GPIO_Port GPIOB
#define L_PWM_B_Pin GPIO_PIN_6
#define L_PWM_B_GPIO_Port GPIOB
#define L_PWM_A_Pin GPIO_PIN_7
#define L_PWM_A_GPIO_Port GPIOB
#define R_PWM_B_Pin GPIO_PIN_8
#define R_PWM_B_GPIO_Port GPIOB
#define R_PWM_A_Pin GPIO_PIN_9
#define R_PWM_A_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
