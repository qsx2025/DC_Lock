///****************
//					STM32G070 内部Flash读写程序
//					
//*	STM32G070单片机，内部Flash 128Kbytes，共1bank，Page size：2Kbytes,所以有64页。
//*	擦除最小单位是一页2K字节；写入最小单位是 64位 = 8字节。
//*	写数据之前，必须要擦除目标地址所属的2K页，而此页空间可能有内容，所以取出来暂存到一
//*	个2K字节大小的数组中，把数组中对应位置的数据替换成自己要写的数据，再把数组整个
//*	写回该地址空间。
//*	165电路中，将升级标志写在最后一页的起始地址；将程序加密信息写在升级标志后100字节出，
//*	同属于最后一页。这里程序简单写，仅面向165电路情况。
//*****************/

//#include "StmFlash.h"
//#include <string.h>
//#include <stdio.h>

//#if STM32_FLASH_WREN	//如果使能了写  


///**********************************************************************
//*							不检查地写入Flash
//*	WriteAddr : 起始地址
//*	pBuffer	  : 数据指针
//*	NumToWrite: 字节数，这里其实锁死在FLASH_PAGE_SIZE大小。
//***********************************************************************/
//static void STMFLASH_Write_NoCheck(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)   
//{ 			 		 
//	u16 i = 0 ;
//	u16	f = 0 ;
//	f = NumToWrite / 8;					//8字节分一组，共f组数据
//	
//	for( i = 0 ; i < f; i += 8)
//	{	
////		FLASH_ProgramWord(WriteAddr,*(u32 *)(pBuffer+i));
//		if(HAL_FLASH_Program( FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr, *(uint64_t *)(pBuffer + i) ) == HAL_OK)
//		{
//			WriteAddr += 8;							//地址增加8
//			
//		}
//		else
//		{
//			printf("写入内部Flash地址%x数据时出错,\r\n", WriteAddr);
//		}
//	}	
//} 

///**********************************************************************
//*						从指定地址开始写入指定长度的数据
//*	WriteAddr : 起始地址
//*	pBuffer	  : 数据指针
//*	NumToWrite: 字节数，必须是偶数
//***********************************************************************/
//u8 STMFLASH_BUF[FLASH_PAGE_SIZE];
//void STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)
//{
//	u16 i;    
//	u32 secpos;	   //页号
//	u16 secoff;	   //页内偏移地址（字节计算）
//	u16 secremain; //页内剩余地址（字节计算）	    	
//	u32 offaddr;   //去掉0x08000000后的地址
//	
//	uint32_t PageError = 0;
//	FLASH_EraseInitTypeDef EraseInitStruct = {0};	
//	
//	
//	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))	return;//非法地址
//	
//	HAL_FLASH_Unlock();
//	offaddr	=	WriteAddr-FLASH_BASE;					//实际偏移地址
//	secpos	=	offaddr / FLASH_PAGE_SIZE;				//页号
//	secoff	=	(offaddr % FLASH_PAGE_SIZE);			//在页内的偏移（字节为基本单位）
//	secremain =	FLASH_PAGE_SIZE - secoff;				//页剩余空间大小  
//	if(NumToWrite <= secremain)  secremain = NumToWrite;	//大于该扇区范围
//	
//	while(1) 
//	{	
//		STMFLASH_Read(secpos * FLASH_PAGE_SIZE + FLASH_BASE, STMFLASH_BUF, FLASH_PAGE_SIZE);	//读出整个扇区的内容
//		
//		//擦除本页
//		EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
//		EraseInitStruct.Page        = secpos;
//		EraseInitStruct.NbPages     = 1;
//		if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
//		{
//			printf("擦除内部Flash页%d出错",secpos);
//		}
//		
//		//替换数组内容
//		for(i = 0; i < secremain; i ++)
//		{
//			STMFLASH_BUF [ i + secoff ] = pBuffer [ i ] ;
//		}
//		//写回到Flash中
//		
//		STMFLASH_Write_NoCheck( secpos * FLASH_PAGE_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, FLASH_PAGE_SIZE);	//写入整个页	
//		printf("1234567\r\n");
//		if( secremain == NumToWrite )	break;	//写入结束了
//		
//		else	//写入未结束
//		{
//			secpos ++ ;							//页号增1
//			secoff = 0 ;						//偏移位置为0
//		 	pBuffer += secremain ;  			//指针偏移
//			WriteAddr += secremain ;			//写指定地址偏移
//		 	NumToWrite -= secremain ;			//字节数递减
//			
//			if( NumToWrite > FLASH_PAGE_SIZE)	secremain = FLASH_PAGE_SIZE;	//下一个页还是写不完			
//			else secremain = NumToWrite;										//下一个页可以写完了
//		}	 
//	}	
//	
//	HAL_FLASH_Lock();
//}
//#endif

///**********************************************************************
//*					从指定地址开始读出指定长度的数据
//*	ReadAddr : 起始地址
//*	pBuffer	  : 数据指针
//*	NumToWrite: 字节数
//***********************************************************************/
//void STMFLASH_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)  	
//{
//		memcpy(pBuffer,(uint8_t*)ReadAddr,NumToRead);
//}











/*********************************************************************************
*							STM32内部Flash读写程序
*
*此次用于STM32G070平台。使用HAL库方式实现。
*注意，G070 128K大小，Page size为2K。HAL库方式读写时，一次写入64位数据
*************************************************************************************/
#include "StmFlash.h"
#include <stdio.h>

//重写为读双字
uint64_t STMFLASH_ReadDoubleWord(u32 faddr)
{
	return *(__IO uint64_t*)faddr; 
}

//重写为读双字
void STMFLASH_Read(u32 ReadAddr,uint64_t *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadDoubleWord(ReadAddr);	//读取8个字节
		ReadAddr += 8;									//偏移8个字节
	}
}

#if STM32_FLASH_WREN	//如果使能了写  

//不检查地写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:双字（64位）数 
void STMFLASH_Write_NoCheck(u32 WriteAddr, uint64_t *pBuffer, u16 NumToWrite){
	uint16_t i;
	for(i = 0; i < NumToWrite; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr, pBuffer[i]);
		WriteAddr += 8;
	}
}

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址（此地址必须为2的倍数）
//pBuffer:数据指针
//NumToWrite:双字（64位）数（就是要写入的64位数据的个数）
//通过数据手册查到G070	flash一页2048字节
#define STM_SECTOR_SIZE	2048
		 
uint64_t STMFLASH_BUF[STM_SECTOR_SIZE/8];				//用来暂存读到的扇区数据。最多是整个扇区的2K字节数据

void STMFLASH_Write(u32 WriteAddr, uint64_t *pBuffer, u16 NumToWrite)	
{	
	u16 i;
	u32 secpos;	   //扇区号
	u16 secoff;	   //扇区内偏移地址（以双字计算）
	u16 secremain; //扇区内剩余地址（以双字计算）	    	
	u32 offaddr;   //去掉0x08000000后的地址
	uint32_t PageError = 0;
	
	static FLASH_EraseInitTypeDef EraseInitStruct = {0};
	
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;	//判断到非法地址则退出
	
	HAL_FLASH_Unlock();									//解锁FLASH
	offaddr   = WriteAddr - STM32_FLASH_BASE;			//实际偏移地址
	secpos 	  = offaddr / STM_SECTOR_SIZE;				//扇区地址，属于第几号扇区
	secoff 	  = (offaddr % STM_SECTOR_SIZE) / 8;		//在扇区内的偏移（8个字节为基本单位），也就是属于第几个8字节组
	secremain = STM_SECTOR_SIZE / 8 - secoff;			//扇区剩余空间大小，也就是剩余多少个8字节空间
	if(NumToWrite<=secremain)secremain=NumToWrite;		//如果要写的数据不大于该扇区剩余空间范围
	
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE/8);	//读出整个扇区的内容，暂存到BUF中
		for(i=0;i<secremain;i++)						//校验数据 打算用来写数据的剩余空间如果有非OXFFFF的值，就需要擦掉
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFFFFFF)	break;	// 需要擦除，提前结束循环
		}
		if(i<secremain)//需要擦除	
		{
			EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;		//选择擦除一整页的方式
			EraseInitStruct.Page 	  = secpos;						//从secpos页开始擦除
			EraseInitStruct.NbPages   = 1;							//只擦除这一页
			HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);		//开始擦除这个扇区
//			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区	
			
			for(i=0;i<secremain;i++)					// 复制 要写入的数据 到 暂存着刚读出的全扇区数据的数组 的后半部分中，方便等会儿一起重新写入扇区中
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/8);	 //将存着新旧数据的数组重写入整个扇区 
		}
		else			//不需要擦除的，直接写入扇区剩余区间
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain/8);				   
		if(NumToWrite==secremain) break;//写入结束了
		else//写入未结束
		{
			secpos++;							//扇区地址增1
			secoff=0;							//偏移位置为0
		   	pBuffer	   += secremain;  			//指针偏移
			WriteAddr  += (secremain * 8);		//更新下个写入的地址
		   	NumToWrite -= secremain;			//剩余双字数据组合数递减
			if(NumToWrite>(STM_SECTOR_SIZE/8))secremain=STM_SECTOR_SIZE/8;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	HAL_FLASH_Lock();		//上锁
}
#endif

