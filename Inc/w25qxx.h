#ifndef _W25Q64_H_
#define _W25Q64_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
// W25Qxx device ID
#define W25Q80_ID   0x13   ///< W25Q80_ID
#define W25Q16_ID   0x14   ///< W25Q16_ID
#define W25Q32_ID   0x15   ///< W25Q32_ID
#define W25Q64_ID   0x16   ///< W25Q64_ID
#define W25Q128_ID  0x17   ///< W25Q128_ID
#define W25Q256_ID  0x18   ///< W25Q256_ID

#define W25X_WriteEnable            0x06 ///< 写使能
#define W25X_WriteDisable           0x04 ///< 写禁止
#define W25X_ReadStatusReg          0x05 ///< 读取状态寄存器
#define W25X_WriteStatusReg         0x01 ///< 写状态寄存器
#define W25X_ReadData               0x03 ///< 读取数据
#define W25X_FastReadData           0x0B ///< 快速读取数据
#define W25X_FastReadDual           0x3B ///< 快速读取双字
#define W25X_PageProgram            0x02 ///< 页编程
#define W25X_BlockErase             0xD8 ///< 块擦除
#define W25X_SectorErase            0x20 ///< 扇区擦除
#define W25X_ChipErase              0xC7 ///< 芯片擦除
#define W25X_PowerDown              0xB9 ///< 掉电
#define W25X_ReleasePowerDown       0xAB ///< 唤醒
#define W25X_DeviceID               0xAB ///< 读取设备号
#define W25X_ManufactDeviceID       0x90 ///< 读取厂商ID
#define W25X_JedecDeviceID          0x9F ///< JedecDeviceID

#define WIP_Flag                    0x01  /* Write In Progress (WIP) flag */

#define W25X_TIMEOUT_VALUE          1000 ///< 超时

#define W25Q64_CHIP_SIZE            0x400000  ///< 1M bytes = 8M bits
#define W25Q64_SECTOR_SIZE          4096      ///< 4k 4096bytes
#define W25Q64_PAGE_SIZE            256       ///< 256 bytes


///the Highest 4K byte as program Information sector
#define W25Q64_PROGRAM_INFORMATION_BLOCKSIZE    W25Q64_SECTOR_SIZE
#define W25Q64_PROGRAM_INFORMATION_START_ADDR   (W25Q64_CHIP_SIZE - W25Q64_PROGRAM_INFORMATION_BLOCKSIZE) 

///the first 384K byte as the programe sector
#define W25Q64_PROGRAM_SECTOR_BLOCKSIZE         0x60000
#define W25Q64_PROGRAM_SECTOR_START_ADDR        (0) 


///SPI Flash 片选信号有效
#define FLASH_SPI_CS_ON()     HAL_GPIO_WritePin(Flash_SPI_CS_OUT_GPIO_Port, Flash_SPI_CS_OUT_Pin, GPIO_PIN_RESET)
///SPI Flash 片选信号无效
#define FLASH_SPI_CS_OFF()    HAL_GPIO_WritePin(Flash_SPI_CS_OUT_GPIO_Port, Flash_SPI_CS_OUT_Pin, GPIO_PIN_SET)
///SPI Flash 使能写保护
#define FLASH_WP_ENABLE()     HAL_GPIO_WritePin(Flash_WP_OUT_GPIO_Port, Flash_WP_OUT_Pin, GPIO_PIN_RESET)
///SPI Flash 禁用写保护
#define FLASH_WP_DISABLE()    HAL_GPIO_WritePin(Flash_WP_OUT_GPIO_Port, Flash_WP_OUT_Pin, GPIO_PIN_SET)

/* Exported constants --------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/
uint8_t SPI_Flash_Init(void);
uint8_t SPI_Flash_ReadDeviceID(void);
uint32_t SPI_FLASH_ReadID(void);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void WriteImage2ExtFlash(uint32_t addr, uint8_t * data, uint16_t data_size);



#endif
