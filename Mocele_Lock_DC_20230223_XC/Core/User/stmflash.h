///****************
//					STM32G070 �ڲ�Flash��д����
//					
//*	STM32G070��Ƭ�����ڲ�Flash 128Kbytes����1bank��Page size��2Kbytes,������64ҳ��
//*	������С��λ��һҳ2K�ֽڣ�д����С��λ�� 64λ = 8�ֽڡ�
//*	д����֮ǰ������Ҫ����Ŀ���ַ������2Kҳ������ҳ�ռ���������ݣ�����ȡ�����ݴ浽һ
//*	��2K�ֽڴ�С�������У��������ж�Ӧλ�õ������滻���Լ�Ҫд�����ݣ��ٰ���������
//*	д�ظõ�ַ�ռ䡣
//*	165��·�У���������־д�����һҳ����ʼ��ַ�������������Ϣд��������־��100�ֽڳ���
//*	ͬ�������һҳ����������д��������165��·������ο��ٷ������޸ġ�
//*****************/
//#ifndef __STMFLASH_H__
//#define __STMFLASH_H__

//#include "main.h"

////�û������Լ�����Ҫ����
//#define STM32_FLASH_BASE 			0x08000000 				//STM32 FLASH����ʼ��ַ
//#define STM32_FLASH_SIZE 			*(u16*)0x1fff75e0 	 	//��ѡSTM32��FLASH������С(��λΪK)����ͬоƬ��ַ��ͬ��G070��ַΪ��
//#define STM32_FLASH_WREN 			1              			//ʹ��FLASHд��(0��������;1��ʹ��) 
//#define u8 			uint8_t
//#define u16 		uint16_t
//#define u32     uint32_t
//void STMFLASH_Read(u32 ReadAddr, u8 *pBuffer, u16 NumToRead);	  		//��ָ����ַ��ʼ����ָ�����ȵ�����
//void STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite);			//��ָ����ַ��ʼд��ָ�����ȵ�����
//							   
//#endif





/*****************************************************
*				ʹ��HAL�ⷽʽ��װFLASH��д����
*
*	����STM32G070CBT6��128K flash
*****************************************************/
#ifndef __STMFLASH_H
#define __STMFLASH_H

#include "stm32g0xx_hal.h"

#define u32 uint32_t
#define u16 uint16_t

#define STM32_FLASH_BASE 	0x08000000 		//STM32 FLASH����ʼ��ַ
#define STM32_FLASH_SIZE 	128 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 	1            	//ʹ��FLASHд��(0��������;1��ʹ��)

uint64_t 	STMFLASH_ReadDobuleWord(uint32_t faddr);									// ����˫������һ��
void 		STMFLASH_Read(uint32_t ReadAddr, uint64_t *pBuffer, uint16_t NumRead);		// ����ָ�����ȵ�˫������
void 		STMFLASH_Write_NoCheck(u32 WriteAddr, uint64_t *pBuffer, u16 NumToWrite);	// ��ָ����ַдָ���������ݵĺ��Ĳ���
void 		STMFLASH_Write(uint32_t WriteAddr, uint64_t *pBuffer, uint16_t NumToWrite);	// ��ָ����ַ��ʼд��ָ�����ȵ�����

#endif

