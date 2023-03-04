/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "peripheral.h"
#include "StmFlash.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
typedef struct
{
	__IO uint32_t JIQIHAO;				//从机ID,
	__IO uint8_t  version;			//升级固件版本号	
}Memory_TypeDef;			 //数据结构体
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint8_t  Re485_Scan(void);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CL4_Pin GPIO_PIN_1
#define CL4_GPIO_Port GPIOB
#define Infrared_IN_CL5_Pin GPIO_PIN_10
#define Infrared_IN_CL5_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define RS485_DIR_Pin GPIO_PIN_8
#define RS485_DIR_GPIO_Port GPIOB
#define Infrared_IN_CL6_Pin GPIO_PIN_9
#define Infrared_IN_CL6_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
//#define MachineNumber 0X02   //出厂地址
#define	InfoAddr			0x8007400		//STM32_FLASH_BASE + 29 * 1024	升级标志地址占用30字节
#define VERSION	  		0x01 			//版本号

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
