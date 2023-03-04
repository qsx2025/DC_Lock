/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_Stalling 140                    //阻转检测
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
volatile int32_t SysSec=0;                 //计时变量，单位s，只读不取
volatile int32_t SysSec_1=0;                 //计时变量，单位s，只读不取
extern	uint8_t  Lock_Distance_Flag ;      //测试赋3       //车锁距离标示位 0代表车在上方 1代表车已离开锁待升起  2车已离开锁已升起 
extern  uint8_t  Lock_Distance_Time;       //车辆离开计时 
extern  uint8_t  Lock_Distance_Time_1;     //车辆离开计时 
extern	uint8_t  Lock_Distance_Time_2;     //车辆离开计时
extern	uint8_t  Lock_Distance_Time_3;     //车辆离开计时
extern  uint32_t Stalling_Time;            //阻转时间
extern  uint32_t Stalling_Time_1;          //阻转时间
extern  uint32_t Stalling_Time_2;          //阻转时间
extern  uint32_t Stalling_Time_3;          //阻转时间
extern  uint16_t Distance_Value;           //车锁距离车距离返回值
extern  uint16_t pwm1;                     //通道1占空比
extern  uint16_t pwm2;                     //通道2占空比
extern  uint8_t  Lock_Statu_Flag;          //车锁状态标志位 0关闭状态 1正在升起  2已升起 3升起中状态异常（升过了）
extern  uint8_t RX3_Buffer [RX3_MAX];      //串口3接收数组
extern  uint8_t rx3flag;                   //串口3指令接收标志位      
uint8_t Instructions;                      //0未收到指令 1收到下降指令，2收到升起指令
uint8_t lock_zhiling_look_flog;            //地锁通讯查询指令 0未收到指令，1收到指令
uint8_t lock_zhiling_look_1_flog;            //地锁通讯查询指令 0未收到指令，1收到指令
uint8_t lock_tongxu_flog=0;                //长时间通讯不上（五分钟左右，降下地锁）（地锁降下后1分钟内通讯不上复位控制板）（地锁关闭状态下，上电一分钟内通讯不上复位控制板）
uint8_t Escalation_Flag;                   //指令执行标志 0未收到指令/执行完毕 1标记收到下降指令 2标记收到上升指令 3下降指令执行遇到阻转，电机上升  4执行上升指令检测到上方有车辆持续两秒，下降地锁  5执行上升指令遇到阻转，电机下降
uint8_t zhiling_Flag; 
uint8_t lcok_up_erro_Flag; 
uint8_t zhuangtaigaibian_Flag;             //初始赋值0 状态改变为1
uint32_t zhiling_Time; 
volatile uint8_t zuzhuan_flog;
uint32_t rester_jishi_time=0;                //通讯故障计时
Memory_TypeDef  Info;						           //地址，版本固件结构体
extern uint8_t 	SecSend;		  //计时 心跳信号	发送
extern	uint8_t Rxbuffer_1[];
extern  uint8_t shangdain_flog;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osThreadId myTask05Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void IWDG_Task(void const * argument);
void Main_Task(void const * argument);
void WuXian_Task(void const * argument);
void Abnormal_Handle_Task(void const * argument);
void BEEP_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, IWDG_Task, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, Main_Task, osPriorityNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, WuXian_Task, osPriorityHigh, 0, 200);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, Abnormal_Handle_Task, osPriorityAboveNormal, 0, 200);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* definition and creation of myTask05 */
  osThreadDef(myTask05, BEEP_Task, osPriorityLow, 0, 128);
  myTask05Handle = osThreadCreate(osThread(myTask05), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	vTaskSuspend(myTask05Handle);          //挂起蜂鸣器报警任务
//	vTaskSuspend(myTask04Handle);                //挂起异常处理任务
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_IWDG_Task */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_IWDG_Task */
void IWDG_Task(void const * argument)
{
  /* USER CODE BEGIN IWDG_Task */
  /* Infinite loop */
  for(;;)
  {
   
    osDelay(1200);                                 //延时1.2s
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);    //翻转指示灯，用于判断程序运行状态
		IWDG->KR = 0xAAAA;	                           //喂狗
		
		
  }
  /* USER CODE END IWDG_Task */
}

/* USER CODE BEGIN Header_Main_Task */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Main_Task */
void Main_Task(void const * argument)
{
  /* USER CODE BEGIN Main_Task */

  /* Infinite loop */
  for(;;)
  {
		if(Infrared_Detection()==0)          //如果车锁处于已关闭状态
		{
			if(shangdain_flog==1)
			{
				shangdain_flog=0;
				SysSec_1=0;
				zhiling_Time=0;      //上电清零时间？
				zhiling_Flag=0;       //刚上电1分钟内没车，通讯正常升起电机？	
				lock_tongxu_flog=0;
			  lcok_up_erro_Flag=0x00;             //已经降锁
				printf("初次上电标记已降锁\r\n");
			
			}
//		  printf("zuzhuan_flog=%d\r\n",zuzhuan_flog);
//			if(ADC1_GetVaule()<ADC_Stalling)  
      if(zuzhuan_flog==0x01)			
			{
				RCWL_1655_or_9631_UART_Ranging();  //检测车辆状态
				osDelay(10);                       //等待10ms	
				if(Instructions==1)                
				{
					Instructions=0;
					Lock_Com_feedback(1,0x25);       //降锁指令完成上报
				}				
        if(Instructions==2)
				{
					BEEP_Init();                     //准备升起，告警提示
					Instructions=0;                  //上升指令已执行
					Escalation_Flag=2;               //开始上升
					if(Lock_Distance_Flag==1)
					{
						printf("关闭状态下收到升起指令没有车辆，控制电机升起\r\n");				
						MOTOR_SA8321_Control(1);       //没有车辆，控制电机升起	
            osDelay(500);						
					
					}
					else
					{
						
						MOTOR_SA8321_Control(0);       //有车辆停止         
						printf("关闭状态下收到升起指令检测到车辆，车辆暂停\r\n");	
					}					
				}else
        {
					
					if(Escalation_Flag==2)
					{
						
						MOTOR_SA8321_Control(1);      //没有车辆，控制电机升起 
            osDelay(500);							
					
					}else if(Escalation_Flag==1||Escalation_Flag==3)
					{
						MOTOR_SA8321_Control(2);       //执行下降指令	
            osDelay(500);		
				
					}else
          {
						
						if(zhiling_Flag==1)
						{
//							printf("zhiling_Time=%d\r\n",zhiling_Time);
							if(zhiling_Time++>1000)        //五分钟计时
							{
								zhiling_Time=0;
								zhiling_Flag=0;
								printf("5分钟计时时间到\r\n");
							}	
						}
						if(zhiling_Flag==0)
						{
							RCWL_1655_or_9631_UART_Ranging();  //检测车辆状态
							osDelay(10);                       //等待10ms	
							if(Lock_Distance_Flag==1)
							{					
								if(lock_tongxu_flog==1)
								{
								Lock_Distance_Time++;          //10秒计时，如果持续10秒内无车辆升起车锁								
								
								if(Lock_Distance_Time>150)
								{
									Lock_Distance_Time=0;
									printf("在关闭状态下检测到车辆离开1分钟时间到升起地锁\r\n");
									MOTOR_SA8321_Control(1);     //10秒内若仍没有车辆，控制电机升起					
									Escalation_Flag=2;           //升起地锁
									osDelay(500);	
								}									
								}

							}
							else
							{		
								Lock_Distance_Time=0;	         //计时清零
	//							printf("10s时间到检测到车辆计时清零\r\n");
							}
						}	
						if(SysSec_1<120)
						{
//							printf("SysSec_1=%d\r\n",SysSec_1);
							if(lock_zhiling_look_1_flog==1)    //如果通讯故障，2分钟复位
							{
								lock_zhiling_look_1_flog=0;
								SysSec_1=0;
							}
						}
            else
						{
						  printf("2分钟通讯异常复位\r\n");
				      __disable_irq();      //关闭所有中断
              NVIC_SystemReset();   //软件复位单片机
						}	
						
						if(lcok_up_erro_Flag==0x01)
						{
							SysSec_1=0;
					  	vTaskResume(myTask05Handle);     //蜂鸣器告警
						}						
					}					
				}
//      printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
			}else
			{
//      printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
				if(Escalation_Flag==2)             //收到指令上升遇到阻转
				{
					printf("执行上升命令遇到阻转\r\n");
					MOTOR_SA8321_Control(2);         //下降地锁  
          Escalation_Flag=3;				
				  osDelay(1000); 
				}
				if(Escalation_Flag==1||Escalation_Flag==3)
				{
					printf("执行下降命令遇到阻转或者上升遇到阻转，默认地锁归位\r\n");
					BEEP_Init();                     //BEEP初始化
					MOTOR_SA8321_Control(0);           //停止电机
          Escalation_Flag=0;                 //标记下降指令完成	
          SysSec_1=0;					
          Lock_Statu_Flag=0x01;                 //标记车锁状态	
          lcok_up_erro_Flag=0x00;             //已经降锁					
					Lock_Com_feedback(1,0x01);       //降锁指令完成上报
					zhiling_Flag=1;
					zhiling_Time=0;
				}

			}
		}
		
		if((Infrared_Detection()==1))  //如果车锁状态是已开启状态
		{
			SysSec_1=0;
			if(shangdain_flog==1)
			{
				shangdain_flog=0;
			  lcok_up_erro_Flag=0x01;             //已经升锁	
        lock_tongxu_flog=1;				
			}
//      printf("zuzhuan_flog=%d\r\n",zuzhuan_flog);
//			if(ADC1_GetVaule()<ADC_Stalling)
			if(zuzhuan_flog==0x01)
			{
				if(Instructions==2)
				{
					Instructions=0;
					Lock_Com_feedback(1,0x26); //升锁指令完成
				}
//				if(Instructions==1) 				
			if(Instructions==1||(lock_tongxu_flog!=1))          //如果收到下降指令  //通讯故障下降地锁
				{
					
					BEEP_Init();               //准备下降，告警提示
					Instructions=0;            //下降指令已执行
					Escalation_Flag=1;         //标记收到指令下降 
					MOTOR_SA8321_Control(2);   //下降电机
					osDelay(500);		
					printf("开启状态收到下降指令下降电机\r\n");		
				}else
				{
					if(Escalation_Flag==1||Escalation_Flag==3)
					{
					  MOTOR_SA8321_Control(2);   //下降电机
				  	osDelay(500);	
					}else if(Escalation_Flag==2)
					{
						MOTOR_SA8321_Control(1);      //没有车辆，控制电机升起 
            osDelay(500);			
					}else
					{
						if(lcok_up_erro_Flag==0x01)
						{
					  		vTaskSuspend(myTask05Handle);          //挂起蜂鸣器报警任务
								BEEP_Abnormal(0);                  //关闭蜂鸣器

						}
					}
	
				}	
//       printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());				
			}else
			{
//				printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
				if(Escalation_Flag==1)
				{
					printf("执行下降指令遇到阻转\r\n");
					MOTOR_SA8321_Control(2);   //下降电机
					osDelay(500);						
				}
				if(Escalation_Flag==2)
				{
					printf("执行上升指令遇到阻转默认电机开启归位\r\n");
					BEEP_Abnormal(0);            //关闭蜂鸣器
					MOTOR_SA8321_Control(0);     //停止电机
					Lock_Statu_Flag=0x00;        //锁已升起	
					Escalation_Flag=0;           //标记上升完成
					lcok_up_erro_Flag=0x01;      //已经升锁
					BEEP_Init();                 //BEEP初始化
					Lock_Com_feedback(1,0x26); //升锁指令完成
					printf("开启状态升锁锁指令完成上报\r\n");
				}
			}
			
		}	
		
    osDelay(100);
  }
  /* USER CODE END Main_Task */
}

/* USER CODE BEGIN Header_WuXian_Task */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WuXian_Task */
void WuXian_Task(void const * argument)
{
  /* USER CODE BEGIN WuXian_Task */
//	uint8_t i;
  /* Infinite loop */
  for(;;)
  {

		if(Re485_Scan())
		{
//			for(i=0;i<RX3_MAX_1;i++)
//			{
//			printf("Rxbuffer_1[%d]=%02x\r\n",i,Rxbuffer_1[i]);
//			
//			}
		
			switch(Rxbuffer_1[3]) //判断指令
			{
				
				case 0x25:          //降锁指令
				  Instructions=1;
					printf("降锁指令\r\n");
					break;
				case 0x26:          //升锁指令
				  Instructions=2;
					printf("升锁指令\r\n");
					break;
				case 0x03:          //休眠指令
					Lock_Com_feedback(1,0x03);
					printf("休眠指令\r\n");
					break;
				case 0x04:          //唤醒指令
					Lock_Com_feedback(1,0x04);
					printf("唤醒指令\r\n");
					break;
				case 0x21:          //查询地锁状态指令
					lock_zhiling_look_flog=1;
				  lock_zhiling_look_1_flog=1;
					Lock_Statu_Send();  //向服务器发送地锁状态
					printf("查询地锁状态指令\r\n");
					break;
				case 0x06:          //写地址命令	
          Address_Send();					
				  Info.JIQIHAO=RX3_Buffer[1];
					STMFLASH_Write(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
				  printf("写地址指令\r\n");
					break;
				case 0x07:          //读地址命令
				  STMFLASH_Read(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
				  Read_Add();
					 printf("读地址指令\r\n");
					break;
				case 0x08:          //波特率设置命令
					 printf("波特率设置\r\n");
					break;
				case 0x09:          //报警命令
					printf("报警指令\r\n");
					break;
				case 0x0A:          //地锁版本读取命令
					STMFLASH_Read(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
				  Read_Version();
				  printf("地锁版本读取\r\n");
					break;
				case 0x0B:          //复位命令
					printf("复位指令\r\n");
					__disable_irq();      //关闭所有中断
          NVIC_SystemReset();   //软件复位单片机
					break;
				
			}

		}
	
		if(SecSend>10)   //如果在开启或者关闭状态每10分钟上报状态
		{
			SecSend=0;
			if(Lock_Statu_Flag==0x01)
			{
			Lock_Com_feedback(1,0x25);
			
			}
			if(Lock_Statu_Flag==0x00)
			{
			Lock_Com_feedback(1,0x26); //升锁指令完成
			
			}
		
		
		}
		if(SysSec<300)  //五分钟计时
		{
//			printf("SysSec=%d\r\n",SysSec);
			if(lock_zhiling_look_flog==1)
			{
				lock_zhiling_look_flog=0;
				lock_tongxu_flog=1;
//			  printf("SysSec=%d\r\n",SysSec);
				SysSec=0;
			}			
		
		}else
		{
			SysSec=0;
			lock_tongxu_flog=0;
		}
		
    osDelay(200);
  }
  /* USER CODE END WuXian_Task */
}

/* USER CODE BEGIN Header_Abnormal_Handle_Task */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Abnormal_Handle_Task */
void Abnormal_Handle_Task(void const * argument)
{
  /* USER CODE BEGIN Abnormal_Handle_Task */
  /* Infinite loop */
  for(;;)
  {

		if(ADC1_GetVaule()>ADC_Stalling)
		{
			zuzhuan_flog=0x00;
//		  printf("异常值ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
			ADC1_GetVaule();
      osDelay(500);
		}else
		{
			printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());        //不要注释掉
			zuzhuan_flog=0x01;
//			ADC1_GetVaule();
		}

		
		
		
		
//		if(ADC1_GetVaule()>ADC_Stalling)
//		{
//			zuzhuan_flog=0x00;
//		  printf("异常值ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
//		
//		}else
//		{
//			if(++i>5)       //阻转后暂时不刷新
//			{
//				i=0;
//			  zuzhuan_flog=0x01;
//				printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
//			}
//		}
		
		
//		printf("异常告警任务\r\n");
    osDelay(50);
  }
  /* USER CODE END Abnormal_Handle_Task */
}

/* USER CODE BEGIN Header_BEEP_Task */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BEEP_Task */
void BEEP_Task(void const * argument)
{
  /* USER CODE BEGIN BEEP_Task */
  /* Infinite loop */
  for(;;)
  {
			MX_TIM15_Init();
			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
			osDelay(100);
			MX_TIM15_Init_1(2000);
			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP响
//		  printf("蜂鸣器任务\r\n");
			osDelay(200);
  }
  /* USER CODE END BEEP_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

