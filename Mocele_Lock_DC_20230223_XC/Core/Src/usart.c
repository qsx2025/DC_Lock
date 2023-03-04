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
	uint8_t RX2_Buffer [RX2_MAX];      //����2��������
	uint8_t RX3_Buffer [RX3_MAX];      //����3��������
	uint32_t Distance_Value;           //�������복���뷵��ֵ
	uint8_t  Lock_Distance_Flag=3;     //���Ը�3       //���������ʾλ 0�������Ϸ� 1�������뿪��������  2�����뿪��������
  uint8_t  Lock_Distance_Time;       //�����뿪��ʱ
	uint8_t  Lock_Distance_Time_1;       //�����뿪��ʱ
	uint8_t  Lock_Distance_Time_2;       //�����뿪��ʱ
  uint8_t  Lock_Distance_Time_3;       //�����뿪��ʱ
	uint32_t Stalling_Time=0;             //��תʱ���ʱ
	uint32_t Stalling_Time_1=0;           //��תʱ���ʱ
  uint32_t Stalling_Time_2=0;           //��תʱ���ʱ
	uint32_t Stalling_Time_3=0;           //��תʱ���ʱ
  uint8_t  Lock_Statu_Flag;          //����״̬��־λ 0�ر�״̬ 1��������   2������״̬�쳣�������ˣ�	3������
	uint8_t  rx3flag;    //ָ����ձ�־λ
	extern Memory_TypeDef 	 Info;						//�����̼��ṹ��
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
//����3���մ�����
void USRT3_receive_handle(void)
{
	
	uint8_t i,k;      //�ֲ���ʱ����
	
	for(i=0;i<RX3_MAX;i++)                               
	{
//		 
//  //		  printf("Info.JIQIHAO111=%02x\r\n",Info.JIQIHAO);
     if	((rx3buffer[i]==0x55)&&((rx3buffer[i+1]==Info.JIQIHAO) ||(rx3buffer[i+1]==0xff)))	
		 {
			 rx3flag=1;//���ձ�־λ��1	
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
		
//	  RX3_Buffer[i]=rx3buffer[i];                        //����ȡ�����ݸ��Ƶ�RX2_Buffer
//		printf("������մ���RX3_Buffer[%d]=%02x\r\n",i, RX3_Buffer[i]);
	 
	}
	CLR_Buf3();
	

//	printf("��ӡrx3flag=%d\r\n", rx3flag);
//	HAL_UARTEx_ReceiveToIdle_IT(&huart2,rx3buffer,RX3_MAX);
//	HAL_UART_Receive_IT(&huart3,(uint8_t *)rx3buffer,RX3_MAX);      //ÿ����RX3_MAX�����ݾͽ���һ���жϻص������������ݴ浽rx3buffer[]
}

//����2���մ�����
void USRT2_receive_handle(void)
{
	
	uint8_t i;      //�ֲ���ʱ����
	
	for(i=0;i<RX2_MAX;i++)                               
	{	
		
	  RX2_Buffer[i]=rx2buffer[i];                        //����ȡ�����ݸ��Ƶ�RX2_Buffer
//		printf("������մ���RX2_Buffer[%d]=%02x\r\n",i, RX2_Buffer[i]);
		
	}
	
	if(RX2_Buffer[0]||RX2_Buffer[1]||RX2_Buffer[2])      //�����⵽����
	{
		
		Distance_Value=RX2_Buffer[0];
		Distance_Value=Distance_Value<<8|RX2_Buffer[1];
		Distance_Value=Distance_Value<<8|RX2_Buffer[2];
//		printf("����%d mm\r\n",Distance_Value/1000);
		if(Distance_Value/1000>400)           //��������ֵ����λmm  ������������50DM
		{
		  Lock_Distance_Flag=1;                              //���������־λ1�������뿪��������
//			printf("�����뿪\r\n");
		}
		else
	{
//		printf("����%d mm\r\n",Distance_Value/1000);	
	  Lock_Distance_Flag=0;                              //���������־λ0�������Ϸ�
		
	}
	
	}
	
//	HAL_UART_Receive_IT(&huart2,(uint8_t *)rx2buffer,RX2_MAX);      //ÿ����RX2_MAX�����ݾͽ���һ���жϻص������������ݴ浽rx2buffer[]
	
}
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
//��д�ص�����
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	
//  if(huart->Instance == USART2)      //����Ǵ���2���յ��ж�
//	{
//		
//		USRT2_receive_handle();         //���ô���2������
//		
//	}
//	
//	if(huart->Instance == USART3)      //����Ǵ���3���յ��ж�
//	{
//		
//		USRT3_receive_handle();         //���ô���3������
//		
//	}
//	
//	
//}


//�����жϻص�����������SizeΪ����ʵ�ʽ��յ������ֽ���
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart->Instance==USART2)
	{
		//���յ���һ������ͨ�����ڻش�
		USRT2_receive_handle(); 
		//�ٴο��������жϽ��գ���Ȼֻ�����һ������
		HAL_UARTEx_ReceiveToIdle_IT(&huart2,rx2buffer,RX2_MAX);
	}
	if(huart->Instance == USART3)      //����Ǵ���3���յ��ж�
	{
//		printf("����3\r\n");
//		USRT3_receive_handle();         //���ô���3������
//		TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);
//		HAL_UARTEx_ReceiveToIdle_IT(&huart3,rx3buffer,RX3_MAX);
		
	}
}




/* USER CODE END 1 */
