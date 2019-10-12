/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_H__
#define __BOARD_H__
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported macro ------------------------------------------------------------*/

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

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif


