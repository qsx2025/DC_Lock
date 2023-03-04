#include "peripheral.h"
#include "stdio.h"

#define pwm 50

extern Memory_TypeDef 	 Info;						//升级固件结构体
extern uint8_t RX3_Buffer [RX3_MAX];      //串口3接收数组

//RX2_MAX
uint8_t rx2buffer[RX2_MAX];         //暂存串口接收到的RX2_MAX字节数据
uint8_t rx3buffer[RX3_MAX];         //暂存串口接收到的RX2_MAX字节数据
uint16_t pwm1=0;                    //通道1占空比
uint16_t pwm2=0;                    //通道2占空比
extern uint8_t  Lock_Distance_Flag;
extern uint8_t  Lock_Statu_Flag;          //车锁状态标志位 0关闭状态 1正在升起   2升起中状态异常（升过了）	3已升起
//LED初始化
void LED_Init(void)
{
	
 HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);      //初始化LED亮
	
}

//BEEP初始化
void BEEP_Init(void)
{
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,0);             //电机向下
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,90);            //电机向上
	__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
  HAL_Delay(100);                                              //延时500ms  	
  __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,0);              //BEEP关
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,0);             //电机停止
//	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,0);             //电机停止

}

//初始化外设
void Peripheral_Init(void)
{
	
	LED_Init();      //LED初始化
	BEEP_Init();     //BEEP初始化

}

/*

函数功能：BEEP告警，正常后停止告警。
函数参数：Alarm [1告警 0关闭告警]

函数返回值：无

*/
void BEEP_Abnormal(bool Alarm )
{
//	uint32_t  i;
	
	if(Alarm)
	{
		
//		while(Alarm)
//		{
//			printf("进入beep\r\n");

			MX_TIM15_Init();
			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
			HAL_Delay(100);
			MX_TIM15_Init_1(2000);
			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
			HAL_Delay(200);
//			if(i>99)
//			{
//			
//			MX_TIM15_Init_1(800);
//			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响

//			
//			}
//			HAL_Delay(2000);

//			if(i<200)
//			{

//			
//			MX_TIM15_Init();
//			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
//			HAL_Delay(100);
//			MX_TIM15_Init_1(2000+i*3);
//			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
//			HAL_Delay(200);
//			
//			}



//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
//			HAL_Delay(1000);                                             //延时500ms 
//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,90); 
//			HAL_Delay(500);  		
//			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,0);              //BEEP关
		
//		}
		

	
	}else
	{
		
		__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,0);              //BEEP关
	
	
	}


}


/*

函数功能：MOTOR异常BEEP告警，电机摆动，正常后停止告警和摆动。[主要是实现电机受外力作用的情况下，处于状态不正常状态下告警]

函数参数：Alarm [1告警 0关闭告警]

函数返回值：无

*/
void BEEP_MOTOR_Abnormal(bool Alarm )
{
	
	while(Alarm)
	{
		
    __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,10);

    //电机摆动
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,90);             //电机向下
		HAL_Delay(500);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,0);              //电机向下	
   	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,90);             //电机向上
		HAL_Delay(500);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,0);              //电机向上			
    		
	
	}
	
	  __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,0);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,0); 
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,0); 	
	
}

/*

函数功能：红外电平检测

函数参数：无

函数返回值：

*/
uint8_t Infrared_Detection(void)
{
	
	uint8_t state;    //局部变量，暂存返回值  
	
	 	if(HAL_GPIO_ReadPin(Infrared_IN_CL6_GPIO_Port,Infrared_IN_CL6_Pin)==GPIO_PIN_SET)
		{
			
//      printf("CL6为高\r\n"); 
			state=1;    
		
		}
	 	if(HAL_GPIO_ReadPin(Infrared_IN_CL6_GPIO_Port,Infrared_IN_CL6_Pin)==GPIO_PIN_RESET)
		{
			
//			printf("CL6为低\r\n");      
			state=0;      //	
		
		}
		
	return state;  //返回值

}

/* 

函数功能：检测地锁上方是否有车辆

函数适用：适用于RCWL1655/9631测距芯片UART模式

函数参数：无

函数返回值：无

*/
void RCWL_1655_or_9631_UART_Ranging(void)
{
	
	uint8_t  buf[1]={0xA0};
//		uint8_t  buf[1]={0xF1};
	
	HAL_UART_Transmit(&huart2, buf, 1, 0xffff);      //阻塞式发送查询命令0xA0
//	HAL_UART_Receive_IT(&huart2,(uint8_t *)rx2buffer,RX2_MAX);
//	printf("查询地锁上方是否有车辆\r\n");
	HAL_Delay(200);     //等待200ms
  
}

/* 

函数功能：向充电桩上报状态

函数参数：无

函数返回值：无

*/
void WUXIN_RS485_UART_TX(void)
{
	uint8_t  buf[1]={0x17};
	TransceiverControl_DJSF1778_S(SEND_TO_DJSF1778_S);
	HAL_UART_Transmit(&huart3, buf, 1, 0xffff);      
	HAL_Delay(100);     //等待100ms
	TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);
  
}

/*

函数功能：控制电机转动

函数参数：control 0控制停止 1控制反转 2控制正转 3异常摆动

函数返回值：无

*/
void MOTOR_SA8321_Control(uint8_t control)
{
	
	switch (control)
	{
		
		case 0:
			
		  pwm1=	pwm2=pwm;      //pwm调整占空比，调节转速 [pwm值相同时停止转动]
		
			break;
		
		case 1:
			
		  pwm1=0,pwm2=pwm;     //pwm1为0，pwm2有值，电机正转  下降			
			
			break;
		
		case 2:
			
		  pwm1=pwm,pwm2=0;     //pwm1有值，pwm2为0，电机反转   上升 
			
			break;
		
		case 3:
			
		  pwm1=0,pwm2=pwm;
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm1);    //通过修改比较值来改变占空比
		  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, pwm2);    //通过修改比较值来改变占空比
		  HAL_Delay(200);
			pwm1=pwm,pwm2=0;
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm1);    //通过修改比较值来改变占空比
		  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, pwm2);    //通过修改比较值来改变占空比
		  HAL_Delay(200);
		  pwm1=pwm2=0;
			break;
		
		default :
			
			break;
		
	}
	
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm1);    //通过修改比较值来改变占空比
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, pwm2);    //通过修改比较值来改变占空比

}

/*
	电路使用了SN65LBC184差分收发器控制收发方向。
	提供 SEND_TO_DJSF1778_S 和 RECEIVE_FROM_DJSF1778_S 两种宏可选。
*/
void TransceiverControl_DJSF1778_S(uint8_t TXorRX)
{
	
	HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, (GPIO_PinState)TXorRX);      //切换收发模式

}


/*******************************************************************************
* 函数名 : CLR_Buf3
* 描述   : 清除串口3缓存数据
*******************************************************************************/
void CLR_Buf3(void)
{
	uint16_t k;
	for(k=0;k<RX3_MAX;k++)      //将缓存内容清零
	{
		rx3buffer[k] = 0x00;
	} 
}

void Fina(void)
{
	uint8_t i ;
	uint8_t j=0;
//	STMFLASH_Read(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
//  printf("Info.JIQIHAO=%02x\r\n",Info.JIQIHAO);
//	if(Info.JIQIHAO==0 ||Info.JIQIHAO==0xffffffff)
//	{
//	Info.JIQIHAO=MachineNumber;
//	STMFLASH_Write(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
//	}
//	if(Info.version==0 ||Info.version==0xff)
//	{
//	Info.version=VERSION;
//	STMFLASH_Write(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
//	}

	for(i=0;i<10;i++)
	{
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET)
		{
			HAL_Delay(10);
			j++;
		}
		
	}
	if(j>9)
	{
			Info.JIQIHAO=0x02;
	printf("Info.JIQIHAO=%02x\r\n",Info.JIQIHAO);
	}else
	{
		Info.JIQIHAO=0x01;
	printf("Info.JIQIHAO=%02x\r\n",Info.JIQIHAO);
	
	}

	
	


}
	

//地锁状态上报
void Lock_Statu_Send(void)
{
	
	uint8_t Lockstatu[10];
	
	uint8_t i;

	Lockstatu[0]=0x5a;             //帧头
	Lockstatu[1]=Info.JIQIHAO;    
	Lockstatu[2]=0x05;
	Lockstatu[3]=0x21;
	Lockstatu[4]=Lock_Statu_Flag;  //地锁状态查询
	Lockstatu[5]=0x00;   //Lock_Distance_Flag;    //实际上传参数意义需要标注  测试默认无车
	Lockstatu[6]=0x03;                  // 统一不校验
	Lockstatu[7]=0x01;    
	Lockstatu[8]=CRC8_Tab(Lockstatu+1,7,0);
	Lockstatu[9]=0xAA;              //帧尾
	
	for(i=0;i<10;i++)
	{
//	printf("地锁状态回复Lockstatu[%d]=%02x\r\n",i,Lockstatu[i]);
	
	}
	TransceiverControl_DJSF1778_S(SEND_TO_DJSF1778_S);
	HAL_UART_Transmit(&huart3, Lockstatu, sizeof(Lockstatu), 300); 
  TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);
	
	
}
//写地址成功上报
void Address_Send(void)
{
	uint8_t Address_buf[7]; 
	
	Address_buf[0]=0x5a;
	Address_buf[1]=Info.JIQIHAO;
	Address_buf[2]=0x02;
	Address_buf[3]=0x06;
	Address_buf[4]=RX3_Buffer[1];
	Address_buf[5]=0x00;
	Address_buf[6]=0xAA;
	TransceiverControl_DJSF1778_S(SEND_TO_DJSF1778_S);
	HAL_UART_Transmit(&huart3, Address_buf, sizeof(Address_buf), 300); 
	TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);


}

//向上位机返回读地址信息
void Read_Add(void)
{
	uint8_t Read_buff[7];
	Read_buff[0]=0x5a;
	Read_buff[1]=Info.JIQIHAO;
	Read_buff[2]=0x01;
	Read_buff[3]=0x07;
	Read_buff[4]=Info.JIQIHAO;
	Read_buff[5]=0x00;
	Read_buff[6]=0xAA;
	TransceiverControl_DJSF1778_S(SEND_TO_DJSF1778_S);
	HAL_UART_Transmit(&huart3, Read_buff, sizeof(Read_buff), 300); 
	TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);

	


}
//向上位机返回返回版本信息
void Read_Version(void)
{
	uint8_t Read_version_buff[7];
	Read_version_buff[0]=0x5a;
	Read_version_buff[1]=Info.JIQIHAO;
	Read_version_buff[2]=0x01;
	Read_version_buff[3]=0x0A;
	Read_version_buff[4]=Info.version;
	Read_version_buff[5]=0x00;
	Read_version_buff[6]=0xAA;
	TransceiverControl_DJSF1778_S(SEND_TO_DJSF1778_S);
	HAL_UART_Transmit(&huart3, Read_version_buff, sizeof(Read_version_buff), 300); 
	TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);


}

//开锁指令反馈
void Lock_Com_feedback(uint8_t  Lock_statue,uint8_t com)
{
	uint8_t buf[7];
	if(Lock_statue==1)
	{

		buf[0]=0x5a;
		buf[1]=Info.JIQIHAO;    //后期要改
		buf[2]=0x02;
		buf[3]=com;
		if(Info.JIQIHAO==0x01)
		{
			if(com==0x25)
			{
			buf[4]=0x42;
			}else
			{
				buf[4]=0x17;			
			}
		
		}
		if(Info.JIQIHAO==0x02)
		{
			if(com==0x26)
			{
			buf[4]=0xCA;
			}else
			{
			buf[4]=0x9F;
			}
		
		}
		buf[5]=0x00;
		buf[6]=0xAA;
	}
	else if(Lock_statue==0)
	{
		buf[0]=0x5B;
		buf[1]=Info.JIQIHAO;    //后期要改
		buf[2]=0x01;
		buf[3]=com;
		buf[4]=0x00;
		buf[5]=0xAA;
	
	}

TransceiverControl_DJSF1778_S(SEND_TO_DJSF1778_S);
HAL_UART_Transmit(&huart3, buf, sizeof(buf), 300); 
TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);

}


 unsigned char  CRC8_TAB[256] = 
{
	0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
	0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
	0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
	0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
	0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
	0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
	0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
	0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
	0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
	0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
	0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
	0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
	0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
	0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
	0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
	0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
	0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
	0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
	0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
	0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
	0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
	0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
	0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
	0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
	0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
	0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
	0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
	0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
	0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
	0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
	0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
	0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35
};
//precrc为CRC-8初始因子，如无特殊规定，则默认为0x00
unsigned char CRC8_Tab( unsigned char *ptr, unsigned char len,unsigned char precrc )
{
	unsigned char index;
	unsigned char crc8=precrc;
	while( len-- )
	{
		index = crc8 ^( *(ptr++) );
//		_nop_();
//		_nop_();
		crc8 =CRC8_TAB[ index ];
//		_nop_();
//		_nop_();
	}
//	_nop_();
//	_nop_();
	return( crc8 );
}


