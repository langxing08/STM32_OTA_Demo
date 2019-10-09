/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#ifdef DEBUG
  #define log(format,...) printf("FILE: "__FILE__", LINE: %d: "format"\r\n", __LINE__, ##__VA_ARGS__)
#else
  #define log(format,...)
#endif
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OTA_UART_RX_Pin GPIO_PIN_0
#define OTA_UART_RX_GPIO_Port GPIOC
#define OTA_UART_TX_Pin GPIO_PIN_1
#define OTA_UART_TX_GPIO_Port GPIOC
#define Debug_UART_TX_Pin GPIO_PIN_2
#define Debug_UART_TX_GPIO_Port GPIOA
#define Debug_UART_RX_Pin GPIO_PIN_3
#define Debug_UART_RX_GPIO_Port GPIOA
#define Flash_SPI_CS_OUT_Pin GPIO_PIN_12
#define Flash_SPI_CS_OUT_GPIO_Port GPIOB
#define Flash_SPI_SCK_Pin GPIO_PIN_13
#define Flash_SPI_SCK_GPIO_Port GPIOB
#define Flash_SPI_MISO_Pin GPIO_PIN_14
#define Flash_SPI_MISO_GPIO_Port GPIOB
#define Flash_SPI_MOSI_Pin GPIO_PIN_15
#define Flash_SPI_MOSI_GPIO_Port GPIOB
#define Flash_WP_OUT_Pin GPIO_PIN_6
#define Flash_WP_OUT_GPIO_Port GPIOC
#define Key_EXTI_Pin GPIO_PIN_10
#define Key_EXTI_GPIO_Port GPIOA
#define Key_EXTI_EXTI_IRQn EXTI15_10_IRQn
#define LED_OUT_Pin GPIO_PIN_11
#define LED_OUT_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
