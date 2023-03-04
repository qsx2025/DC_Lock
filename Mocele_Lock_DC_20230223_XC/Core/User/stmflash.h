///****************
//					STM32G070 内部Flash读写程序
//					
//*	STM32G070单片机，内部Flash 128Kbytes，共1bank，Page size：2Kbytes,所以有64页。
//*	擦除最小单位是一页2K字节；写入最小单位是 64位 = 8字节。
//*	写数据之前，必须要擦除目标地址所属的2K页，而此页空间可能有内容，所以取出来暂存到一
//*	个2K字节大小的数组中，把数组中对应位置的数据替换成自己要写的数据，再把数组整个
//*	写回该地址空间。
//*	165电路中，将升级标志写在最后一页的起始地址；将程序加密信息写在升级标志后100字节出，
//*	同属于最后一页。这里程序简单写，仅面向165电路情况。参考官方例程修改。
//*****************/
//#ifndef __STMFLASH_H__
//#define __STMFLASH_H__

//#include "main.h"

////用户根据自己的需要设置
//#define STM32_FLASH_BASE 			0x08000000 				//STM32 FLASH的起始地址
//#define STM32_FLASH_SIZE 			*(u16*)0x1fff75e0 	 	//所选STM32的FLASH容量大小(单位为K)，不同芯片地址不同，G070地址为此
//#define STM32_FLASH_WREN 			1              			//使能FLASH写入(0，不是能;1，使能) 
//#define u8 			uint8_t
//#define u16 		uint16_t
//#define u32     uint32_t
//void STMFLASH_Read(u32 ReadAddr, u8 *pBuffer, u16 NumToRead);	  		//从指定地址开始读出指定长度的数据
//void STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite);			//从指定地址开始写入指定长度的数据
//							   
//#endif





/*****************************************************
*				使用HAL库方式封装FLASH读写函数
*
*	用于STM32G070CBT6，128K flash
*****************************************************/
#ifndef __STMFLASH_H
#define __STMFLASH_H

#include "stm32g0xx_hal.h"

#define u32 uint32_t
#define u16 uint16_t

#define STM32_FLASH_BASE 	0x08000000 		//STM32 FLASH的起始地址
#define STM32_FLASH_SIZE 	128 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 	1            	//使能FLASH写入(0，不是能;1，使能)

uint64_t 	STMFLASH_ReadDobuleWord(uint32_t faddr);									// 读出双字数据一次
void 		STMFLASH_Read(uint32_t ReadAddr, uint64_t *pBuffer, uint16_t NumRead);		// 读出指定长度的双字数据
void 		STMFLASH_Write_NoCheck(u32 WriteAddr, uint64_t *pBuffer, u16 NumToWrite);	// 往指定地址写指定长度数据的核心操作
void 		STMFLASH_Write(uint32_t WriteAddr, uint64_t *pBuffer, uint16_t NumToWrite);	// 从指定地址开始写入指定长度的数据

#endif

