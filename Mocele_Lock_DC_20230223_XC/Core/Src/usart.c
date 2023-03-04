/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
	uint8_t RX2_Buffer [RX2_MAX];      //串口2接收数组
	uint8_t RX3_Buffer [RX3_MAX];      //串口3接收数组
	uint32_t Distance_Value;           //车锁距离车距离返回值
	uint8_t  Lock_Distance_Flag=3;     //测试赋3       //车锁距离标示位 0代表车在上方 1代表车已离开锁待升起  2车已离开锁已升起
  uint8_t  Lock_Distance_Time;       //车辆离开计时
	uint8_t  Lock_Distance_Time_1;       //车辆离开计时
	uint8_t  Lock_Distance_Time_2;       //车辆离开计时
  uint8_t  Lock_Distance_Time_3;       //车辆离开计时
	uint32_t Stalling_Time=0;             //阻转时间计时
	uint32_t Stalling_Time_1=0;           //阻转时间计时
  uint32_t Stalling_Time_2=0;           //阻转时间计时
	uint32_t Stalling_Time_3=0;           //阻转时间计时
  uint8_t  Lock_Statu_Flag;          //车锁状态标志位 0关闭状态 1正在升起   2升起中状态异常（升过了）	3已升起
	uint8_t  rx3flag;    //指令接收标志位
	extern Memory_TypeDef 	 Info;						//升级固件结构体
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 2400;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */

    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PA5     ------> USART3_TX
    PB0     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_4_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART3_4_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PA5     ------> USART3_TX
    PB0     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_4_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//串口3接收处理函数
void USRT3_receive_handle(void)
{
	
	uint8_t i,k;      //局部临时变量
	
	for(i=0;i<RX3_MAX;i++)                               
	{
//		 
//  //		  printf("Info.JIQIHAO111=%02x\r\n",Info.JIQIHAO);
     if	((rx3buffer[i]==0x55)&&((rx3buffer[i+1]==Info.JIQIHAO) ||(rx3buffer[i+1]==0xff)))	
		 {
			 rx3flag=1;//接收标志位置1	
			 for(k=0;k<rx3buffer[i+2];k++)
			 {
				 RX3_Buffer[k]=rx3buffer[i+3+k];
//  			  printf("RX3_Buffer[%d]=%02x\r\n",k, RX3_Buffer[k]);
				 
			 
			 }
			 

		 }
//		 else
//		 {
//			 rx3flag=0;
//		 
//		 
//		 }
		
//	  RX3_Buffer[i]=rx3buffer[i];                        //将读取的数据复制到RX2_Buffer
//		printf("进入接收处理RX3_Buffer[%d]=%02x\r\n",i, RX3_Buffer[i]);
	 
	}
	CLR_Buf3();
	

//	printf("打印rx3flag=%d\r\n", rx3flag);
//	HAL_UARTEx_ReceiveToIdle_IT(&huart2,rx3buffer,RX3_MAX);
//	HAL_UART_Receive_IT(&huart3,(uint8_t *)rx3buffer,RX3_MAX);      //每接收RX3_MAX个数据就进入一次中断回调函数，数据暂存到rx3buffer[]
}

//串口2接收处理函数
void USRT2_receive_handle(void)
{
	
	uint8_t i;      //局部临时变量
	
	for(i=0;i<RX2_MAX;i++)                               
	{	
		
	  RX2_Buffer[i]=rx2buffer[i];                        //将读取的数据复制到RX2_Buffer
//		printf("进入接收处理RX2_Buffer[%d]=%02x\r\n",i, RX2_Buffer[i]);
		
	}
	
	if(RX2_Buffer[0]||RX2_Buffer[1]||RX2_Buffer[2])      //如果检测到车辆
	{
		
		Distance_Value=RX2_Buffer[0];
		Distance_Value=Distance_Value<<8|RX2_Buffer[1];
		Distance_Value=Distance_Value<<8|RX2_Buffer[2];
//		printf("距离%d mm\r\n",Distance_Value/1000);
		if(Distance_Value/1000>400)           //测量距离值，单位mm  如果测距距离大于50DM
		{
		  Lock_Distance_Flag=1;                              //车锁距离标志位1代表车已离开待锁升起
//			printf("车已离开\r\n");
		}
		else
	{
//		printf("距离%d mm\r\n",Distance_Value/1000);	
	  Lock_Distance_Flag=0;                              //车锁距离标志位0代表车在上方
		
	}
	
	}
	
//	HAL_UART_Receive_IT(&huart2,(uint8_t *)rx2buffer,RX2_MAX);      //每接收RX2_MAX个数据就进入一次中断回调函数，数据暂存到rx2buffer[]
	
}
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
//重写回调函数
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	
//  if(huart->Instance == USART2)      //如果是串口2接收到中断
//	{
//		
//		USRT2_receive_handle();         //调用串口2处理函数
//		
//	}
//	
//	if(huart->Instance == USART3)      //如果是串口3接收到中断
//	{
//		
//		USRT3_receive_handle();         //调用串口3处理函数
//		
//	}
//	
//	
//}


//空闲中断回调函数，参数Size为串口实际接收到数据字节数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance==USART2)
	{
		//把收到的一包数据通过串口回传
		USRT2_receive_handle(); 
		//再次开启空闲中断接收，不然只会接收一次数据
		HAL_UARTEx_ReceiveToIdle_IT(&huart2,rx2buffer,RX2_MAX);
	}
	if(huart->Instance == USART3)      //如果是串口3接收到中断
	{
//		printf("串口3\r\n");
//		USRT3_receive_handle();         //调用串口3处理函数
//		TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);
//		HAL_UARTEx_ReceiveToIdle_IT(&huart3,rx3buffer,RX3_MAX);
		
	}
}




/* USER CODE END 1 */
