#include "w25qxx.h"
#include "spi.h"


/**
  * 函数功能: 从串行Flash读取数据
  * 输入参数: pBuffer：存放读取到数据的指针
  *           ReadAddr：读取数据目标地址
  *           NumByteToRead：读取数据长度
  * 返 回 值: 无
  * 说    明：该函数可以设置任意读取数据长度
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  uint8_t cmd[4] = {W25X_ReadData,0x00,0x00,0x00};	
  uint8_t byte1;
  uint8_t byte2;
  uint8_t byte3;
	
	byte1= ((ReadAddr & 0xFF0000) >> 16);
	byte2= ((ReadAddr & 0xFF00) >> 8);
	byte3= ReadAddr & 0xFF;
      
  /* 选择串行FLASH: CS低电平 */
  FLASH_SPI_CS_ON();

  /* 发送 读 指令 */
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);	

  /* 发送 读 地址高位 */
  HAL_SPI_Transmit(&hspi2, &byte1, 1, W25X_TIMEOUT_VALUE);
  /* 发送 读 地址中位 */
  HAL_SPI_Transmit(&hspi2, &byte2, 1, W25X_TIMEOUT_VALUE);
  /* 发送 读 地址低位 */
  HAL_SPI_Transmit(&hspi2, &byte3, 1, W25X_TIMEOUT_VALUE);

  while (NumByteToRead--) /* 读取数据 */
  {
    /* 读取一个字节*/
		HAL_SPI_Receive(&hspi2, pBuffer, 1, W25X_TIMEOUT_VALUE);
    /* 指向下一个字节缓冲区 */
    pBuffer++;
  }

  /* 禁用串行FLASH: CS 高电平 */
  FLASH_SPI_CS_OFF();
}


/**
  * 函数功能: 擦除整片
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：擦除串行Flash整片空间
  */
void SPI_FLASH_BulkErase(void)
{
  uint8_t cmd[4] = {W25X_ChipErase,0x00,0x00,0x00};	
  
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 整片擦除 Erase */
  /* 选择串行FLASH: CS低电平 */
  FLASH_SPI_CS_ON();

  /* 发送整片擦除指令*/
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);	

  /* 禁用串行FLASH: CS高电平 */
  FLASH_SPI_CS_OFF();

  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * 函数功能: 往串行FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * 输入参数: pBuffer：待写入数据的指针
  *           WriteAddr：写入地址
  *           NumByteToWrite：写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * 返 回 值: 无
  * 说    明：串行Flash每页大小为256个字节
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t cmd[4] = {W25X_PageProgram,0x00,0x00,0x00};	
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	
	byte1 = ((WriteAddr & 0xFF0000) >> 16);
	byte2 = ((WriteAddr & 0xFF00) >> 8);
	byte3 = WriteAddr & 0xFF;
	
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 寻找串行FLASH: CS低电平 */
  FLASH_SPI_CS_ON();
  /* 写送写指令*/
   HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);
  /*发送写地址的高位*/
   HAL_SPI_Transmit(&hspi2, &byte1, 1, W25X_TIMEOUT_VALUE);
  /*发送写地址的中位*/
  HAL_SPI_Transmit(&hspi2, &byte2, 1, W25X_TIMEOUT_VALUE);
  /*发送写地址的低位*/
  HAL_SPI_Transmit(&hspi2, &byte3, 1, W25X_TIMEOUT_VALUE);

  if(NumByteToWrite > W25Q64_PAGE_SIZE)
  {
     NumByteToWrite = W25Q64_PAGE_SIZE;
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    HAL_SPI_Transmit(&hspi2, pBuffer, 1, W25X_TIMEOUT_VALUE);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 禁用串行FLASH: CS 高电平 */
  FLASH_SPI_CS_OFF();

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

/**
  * 函数功能: 往串行FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * 输入参数: pBuffer：待写入数据的指针
  *           WriteAddr：写入地址
  *           NumByteToWrite：写入数据长度
  * 返 回 值: 无
  * 说    明：该函数可以设置任意写入数据长度
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % W25Q64_PAGE_SIZE;
  count = W25Q64_PAGE_SIZE - Addr;
  NumOfPage =  NumByteToWrite / W25Q64_PAGE_SIZE;
  NumOfSingle = NumByteToWrite % W25Q64_PAGE_SIZE;

  if (Addr == 0) /* 若地址与 SPI_FLASH_PageSize 对齐  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, W25Q64_PAGE_SIZE);
        WriteAddr +=  W25Q64_PAGE_SIZE;
        pBuffer += W25Q64_PAGE_SIZE;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* 若地址与 SPI_FLASH_PageSize 不对齐 */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25Q64_PAGE_SIZE;
      NumOfSingle = NumByteToWrite % W25Q64_PAGE_SIZE;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, W25Q64_PAGE_SIZE);
        WriteAddr +=  W25Q64_PAGE_SIZE;
        pBuffer += W25Q64_PAGE_SIZE;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}


/**
  * 函数功能: 使能串行Flash写操作
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void SPI_FLASH_WriteEnable(void)
{
  uint8_t cmd[4] = {W25X_WriteEnable,0x00,0x00,0x00};
  /* 选择串行FLASH: CS低电平 */
  FLASH_SPI_CS_ON();

  /* 发送命令：写使能 */
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);	

  /* 禁用串行Flash：CS高电平 */
  FLASH_SPI_CS_OFF();
}



/**
  * 函数功能: 等待数据写入完成
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：Polls the status of the Write In Progress (WIP) flag in the
  *           FLASH's status  register  and  loop  until write  opertaion
  *           has completed.
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;
  uint8_t cmd[4] = {W25X_ReadStatusReg,0x00,0x00,0x00};

  /* Select the FLASH: Chip Select low */
	
   FLASH_SPI_CS_ON();

  /* Send "Read Status Register" instruction */
 
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);	
  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
      HAL_SPI_Receive(&hspi2, &FLASH_Status, 1, W25X_TIMEOUT_VALUE);;	 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  FLASH_SPI_CS_OFF();
}


/**
 * @brief  擦除一个扇区
 *
 * 擦除一个山区的最少时间:150ms
 *
 * @param[in] Dst_Addr 扇区地址 根据实际容量设置
 *
 * @return   无
 */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{ 
  uint8_t cmd[4] = {W25X_SectorErase,0x00,0x00,0x00};
  uint8_t byte1=0;
  uint8_t byte2=0;
  uint8_t byte3=0;

  byte1 = ((SectorAddr & 0xFF0000) >> 16);
  byte2 = ((SectorAddr & 0xFF00) >> 8);
  byte3 = SectorAddr & 0xFF;
  
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();
  
  /* 等待写入完毕*/  
  SPI_FLASH_WaitForWriteEnd();

  /* 寻找串行FLASH: CS低电平 */  
  FLASH_SPI_CS_ON();
  
  /* 写送写指令*/
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);	
  /*发送写地址的高位*/	
  HAL_SPI_Transmit(&hspi2, &byte1, 1, W25X_TIMEOUT_VALUE);	
  /*发送写地址的中位*/  
  HAL_SPI_Transmit(&hspi2, &byte2, 1, W25X_TIMEOUT_VALUE);
  /*发送写地址的低位*/	
  HAL_SPI_Transmit(&hspi2, &byte3, 1, W25X_TIMEOUT_VALUE);	

  /* 禁用串行FLASH: CS 高电平 */
  FLASH_SPI_CS_OFF();

  /* 等待写入完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

//#define FLASH_HEADTAIL
//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00


/**
  * 函数功能: 读取串行Flash型号的ID
  * 输入参数: 无
  * 返 回 值: uint32_t：串行Flash的型号ID
  * 说    明：  FLASH_ID      IC型号      存储空间大小         
                0xEF3015      W25X16        2M byte
                0xEF4015	    W25Q16        4M byte
                0XEF4017      W25Q64        8M byte
                0XEF4018      W25Q128       16M byte  (YS-F1Pro开发板默认配置)
  */
uint32_t SPI_FLASH_ReadID(void)
{
  uint8_t cmd[4] = {W25X_JedecDeviceID,0x00,0x00,0x00};
  uint8_t byte1=0;
	uint8_t byte2=0;
	uint8_t byte3=0;
	uint32_t temp=0;

  /* 选择串行FLASH: CS低电平 */
  FLASH_SPI_CS_ON();

  /* Send the read ID command */
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);	
  
  HAL_SPI_Receive(&hspi2, &byte1, 1, W25X_TIMEOUT_VALUE);
  HAL_SPI_Receive(&hspi2, &byte2, 1, W25X_TIMEOUT_VALUE);
  HAL_SPI_Receive(&hspi2, &byte3, 1, W25X_TIMEOUT_VALUE);
 
  /* 禁用串行Flash：CS高电平 */
  FLASH_SPI_CS_OFF();
  
  temp = (byte1 << 16) | (byte2 << 8) | byte3;
  return temp;
}

/**
 * @brief  读取设备ID
 *
 * @return   设备ID
 */
uint8_t SPI_Flash_ReadDeviceID(void)
{  
  uint8_t cmd[4] = {W25X_DeviceID,0x00,0x00,0x00};
  uint8_t byte = 0;

  FLASH_SPI_CS_ON();
  /* Send the read ID command */
  HAL_SPI_Transmit(&hspi2, cmd, 4, W25X_TIMEOUT_VALUE);	
  /* Reception of the data */
  HAL_SPI_Receive(&hspi2, &byte, 1, W25X_TIMEOUT_VALUE);
  FLASH_SPI_CS_OFF();
  
  return byte;
}


/**
 * @brief  进入掉电模式
 *
 * @return   无
 */
void SPI_Flash_PowerDown(void)
{
  uint8_t cmd[] = {W25X_PowerDown};
  FLASH_SPI_CS_ON();
  /* Send the read status command */
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);
  FLASH_SPI_CS_OFF();
}


/**
 * @brief  Flash唤醒
 *
 * @return   无
 */
void SPI_Flash_WAKEUP(void)
{
  uint8_t cmd[] = {W25X_ReleasePowerDown};
  FLASH_SPI_CS_ON();
  /* Send the read status command */
  HAL_SPI_Transmit(&hspi2, cmd, 1, W25X_TIMEOUT_VALUE);
  FLASH_SPI_CS_OFF();
}


/**
 * @brief  Flash 初始化
 *
 * @return   Flash ID
 */
uint8_t SPI_Flash_Init(void)
{
  FLASH_WP_DISABLE();

  return SPI_Flash_ReadDeviceID();    
}



/**
  * @brief  保存文件到外部Flash
  *
  * @param[in]    addr    Ext Flash address
  * @param[in]    data    data buffer address
  * @param[in]    data_size  data buffer size
  *
  * @return       none
  */
void WriteImage2ExtFlash(uint32_t addr, uint8_t * data, uint16_t data_size)
{
  if(((addr) & (W25Q64_SECTOR_SIZE-1))==0)
  {
    SPI_FLASH_SectorErase(addr);
  }
  else if(((addr & (W25Q64_SECTOR_SIZE-1)) + data_size) > W25Q64_SECTOR_SIZE)
  {
    SPI_FLASH_SectorErase((addr+data_size)&(~(W25Q64_SECTOR_SIZE-1)));
  }
  
  SPI_FLASH_BufferWrite(data, addr, data_size);
}


