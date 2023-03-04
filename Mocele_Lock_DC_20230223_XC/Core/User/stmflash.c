///****************
//					STM32G070 �ڲ�Flash��д����
//					
//*	STM32G070��Ƭ�����ڲ�Flash 128Kbytes����1bank��Page size��2Kbytes,������64ҳ��
//*	������С��λ��һҳ2K�ֽڣ�д����С��λ�� 64λ = 8�ֽڡ�
//*	д����֮ǰ������Ҫ����Ŀ���ַ������2Kҳ������ҳ�ռ���������ݣ�����ȡ�����ݴ浽һ
//*	��2K�ֽڴ�С�������У��������ж�Ӧλ�õ������滻���Լ�Ҫд�����ݣ��ٰ���������
//*	д�ظõ�ַ�ռ䡣
//*	165��·�У���������־д�����һҳ����ʼ��ַ�������������Ϣд��������־��100�ֽڳ���
//*	ͬ�������һҳ����������д��������165��·�����
//*****************/

//#include "StmFlash.h"
//#include <string.h>
//#include <stdio.h>

//#if STM32_FLASH_WREN	//���ʹ����д  


///**********************************************************************
//*							������д��Flash
//*	WriteAddr : ��ʼ��ַ
//*	pBuffer	  : ����ָ��
//*	NumToWrite: �ֽ�����������ʵ������FLASH_PAGE_SIZE��С��
//***********************************************************************/
//static void STMFLASH_Write_NoCheck(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)   
//{ 			 		 
//	u16 i = 0 ;
//	u16	f = 0 ;
//	f = NumToWrite / 8;					//8�ֽڷ�һ�飬��f������
//	
//	for( i = 0 ; i < f; i += 8)
//	{	
////		FLASH_ProgramWord(WriteAddr,*(u32 *)(pBuffer+i));
//		if(HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr, *(uint64_t *)(pBuffer + i) ) == HAL_OK)
//		{
//			WriteAddr += 8;							//��ַ����8
//			
//		}
//		else
//		{
//			printf("д���ڲ�Flash��ַ%x����ʱ����,\r\n", WriteAddr);
//		}
//	}	
//} 

///**********************************************************************
//*						��ָ����ַ��ʼд��ָ�����ȵ�����
//*	WriteAddr : ��ʼ��ַ
//*	pBuffer	  : ����ָ��
//*	NumToWrite: �ֽ�����������ż��
//***********************************************************************/
//u8 STMFLASH_BUF[FLASH_PAGE_SIZE];
//void STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)
//{
//	u16 i;    
//	u32 secpos;	   //ҳ��
//	u16 secoff;	   //ҳ��ƫ�Ƶ�ַ���ֽڼ��㣩
//	u16 secremain; //ҳ��ʣ���ַ���ֽڼ��㣩	    	
//	u32 offaddr;   //ȥ��0x08000000��ĵ�ַ
//	
//	uint32_t PageError = 0;
//	FLASH_EraseInitTypeDef EraseInitStruct = {0};	
//	
//	
//	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))	return;//�Ƿ���ַ
//	
//	HAL_FLASH_Unlock();
//	offaddr	=	WriteAddr-FLASH_BASE;					//ʵ��ƫ�Ƶ�ַ
//	secpos	=	offaddr / FLASH_PAGE_SIZE;				//ҳ��
//	secoff	=	(offaddr % FLASH_PAGE_SIZE);			//��ҳ�ڵ�ƫ�ƣ��ֽ�Ϊ������λ��
//	secremain =	FLASH_PAGE_SIZE - secoff;				//ҳʣ��ռ��С  
//	if(NumToWrite <= secremain)  secremain = NumToWrite;	//���ڸ�������Χ
//	
//	while(1) 
//	{	
//		STMFLASH_Read(secpos * FLASH_PAGE_SIZE + FLASH_BASE, STMFLASH_BUF, FLASH_PAGE_SIZE);	//������������������
//		
//		//������ҳ
//		EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
//		EraseInitStruct.Page        = secpos;
//		EraseInitStruct.NbPages     = 1;
//		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
//		{
//			printf("�����ڲ�Flashҳ%d����",secpos);
//		}
//		
//		//�滻��������
//		for(i = 0; i < secremain; i ++)
//		{
//			STMFLASH_BUF [ i + secoff ] = pBuffer [ i ] ;
//		}
//		//д�ص�Flash��
//		
//		STMFLASH_Write_NoCheck( secpos * FLASH_PAGE_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, FLASH_PAGE_SIZE);	//д������ҳ	
//		printf("1234567\r\n");
//		if( secremain == NumToWrite )	break;	//д�������
//		
//		else	//д��δ����
//		{
//			secpos ++ ;							//ҳ����1
//			secoff = 0 ;						//ƫ��λ��Ϊ0
//		 	pBuffer += secremain ;  			//ָ��ƫ��
//			WriteAddr += secremain ;			//дָ����ַƫ��
//		 	NumToWrite -= secremain ;			//�ֽ����ݼ�
//			
//			if( NumToWrite > FLASH_PAGE_SIZE)	secremain = FLASH_PAGE_SIZE;	//��һ��ҳ����д����			
//			else secremain = NumToWrite;										//��һ��ҳ����д����
//		}	 
//	}	
//	
//	HAL_FLASH_Lock();
//}
//#endif

///**********************************************************************
//*					��ָ����ַ��ʼ����ָ�����ȵ�����
//*	ReadAddr : ��ʼ��ַ
//*	pBuffer	  : ����ָ��
//*	NumToWrite: �ֽ���
//***********************************************************************/
//void STMFLASH_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)  	
//{
//		memcpy(pBuffer,(uint8_t*)ReadAddr,NumToRead);
//}











/*********************************************************************************
*							STM32�ڲ�Flash��д����
*
*�˴�����STM32G070ƽ̨��ʹ��HAL�ⷽʽʵ�֡�
*ע�⣬G070 128K��С��Page sizeΪ2K��HAL�ⷽʽ��дʱ��һ��д��64λ����
*************************************************************************************/
#include "StmFlash.h"
#include <stdio.h>

//��дΪ��˫��
uint64_t STMFLASH_ReadDoubleWord(u32 faddr)
{
	return *(__IO uint64_t*)faddr; 
}

//��дΪ��˫��
void STMFLASH_Read(u32 ReadAddr,uint64_t *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadDoubleWord(ReadAddr);	//��ȡ8���ֽ�
		ReadAddr += 8;									//ƫ��8���ֽ�
	}
}

#if STM32_FLASH_WREN	//���ʹ����д  

//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:˫�֣�64λ���� 
void STMFLASH_Write_NoCheck(u32 WriteAddr, uint64_t *pBuffer, u16 NumToWrite){
	uint16_t i;
	for(i = 0; i < NumToWrite; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr, pBuffer[i]);
		WriteAddr += 8;
	}
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ���˵�ַ����Ϊ2�ı�����
//pBuffer:����ָ��
//NumToWrite:˫�֣�64λ����������Ҫд���64λ���ݵĸ�����
//ͨ�������ֲ�鵽G070	flashһҳ2048�ֽ�
#define STM_SECTOR_SIZE	2048
		 
uint64_t STMFLASH_BUF[STM_SECTOR_SIZE/8];				//�����ݴ�������������ݡ����������������2K�ֽ�����

void STMFLASH_Write(u32 WriteAddr, uint64_t *pBuffer, u16 NumToWrite)	
{	
	u16 i;
	u32 secpos;	   //������
	u16 secoff;	   //������ƫ�Ƶ�ַ����˫�ּ��㣩
	u16 secremain; //������ʣ���ַ����˫�ּ��㣩	    	
	u32 offaddr;   //ȥ��0x08000000��ĵ�ַ
	uint32_t PageError = 0;
	
	static FLASH_EraseInitTypeDef EraseInitStruct = {0};
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;	//�жϵ��Ƿ���ַ���˳�
	
	HAL_FLASH_Unlock();									//����FLASH
	offaddr   = WriteAddr - STM32_FLASH_BASE;			//ʵ��ƫ�Ƶ�ַ
	secpos 	  = offaddr / STM_SECTOR_SIZE;				//������ַ�����ڵڼ�������
	secoff 	  = (offaddr % STM_SECTOR_SIZE) / 8;		//�������ڵ�ƫ�ƣ�8���ֽ�Ϊ������λ����Ҳ�������ڵڼ���8�ֽ���
	secremain = STM_SECTOR_SIZE / 8 - secoff;			//����ʣ��ռ��С��Ҳ����ʣ����ٸ�8�ֽڿռ�
	if(NumToWrite<=secremain)secremain=NumToWrite;		//���Ҫд�����ݲ����ڸ�����ʣ��ռ䷶Χ
	
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE/8);	//�����������������ݣ��ݴ浽BUF��
		for(i=0;i<secremain;i++)						//У������ ��������д���ݵ�ʣ��ռ�����з�OXFFFF��ֵ������Ҫ����
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFFFFFF)	break;	// ��Ҫ��������ǰ����ѭ��
		}
		if(i<secremain)//��Ҫ����	
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;		//ѡ�����һ��ҳ�ķ�ʽ
			EraseInitStruct.Page 	  = secpos;						//��secposҳ��ʼ����
			EraseInitStruct.NbPages   = 1;							//ֻ������һҳ
			HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);		//��ʼ�����������
//			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������	
			
			for(i=0;i<secremain;i++)					// ���� Ҫд������� �� �ݴ��Ÿն�����ȫ�������ݵ����� �ĺ�벿���У�����Ȼ��һ������д��������
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/8);	 //�������¾����ݵ�������д���������� 
		}
		else			//����Ҫ�����ģ�ֱ��д������ʣ������
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain/8);				   
		if(NumToWrite==secremain) break;//д�������
		else//д��δ����
		{
			secpos++;							//������ַ��1
			secoff=0;							//ƫ��λ��Ϊ0
		   	pBuffer	   += secremain;  			//ָ��ƫ��
			WriteAddr  += (secremain * 8);		//�����¸�д��ĵ�ַ
		   	NumToWrite -= secremain;			//ʣ��˫������������ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/8))secremain=STM_SECTOR_SIZE/8;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	HAL_FLASH_Lock();		//����
}
#endif

