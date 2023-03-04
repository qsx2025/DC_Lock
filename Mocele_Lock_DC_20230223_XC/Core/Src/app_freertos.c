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
#define ADC_Stalling 140                    //��ת���
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
volatile int32_t SysSec=0;                 //��ʱ��������λs��ֻ����ȡ
volatile int32_t SysSec_1=0;                 //��ʱ��������λs��ֻ����ȡ
extern	uint8_t  Lock_Distance_Flag ;      //���Ը�3       //���������ʾλ 0�������Ϸ� 1�������뿪��������  2�����뿪�������� 
extern  uint8_t  Lock_Distance_Time;       //�����뿪��ʱ 
extern  uint8_t  Lock_Distance_Time_1;     //�����뿪��ʱ 
extern	uint8_t  Lock_Distance_Time_2;     //�����뿪��ʱ
extern	uint8_t  Lock_Distance_Time_3;     //�����뿪��ʱ
extern  uint32_t Stalling_Time;            //��תʱ��
extern  uint32_t Stalling_Time_1;          //��תʱ��
extern  uint32_t Stalling_Time_2;          //��תʱ��
extern  uint32_t Stalling_Time_3;          //��תʱ��
extern  uint16_t Distance_Value;           //�������복���뷵��ֵ
extern  uint16_t pwm1;                     //ͨ��1ռ�ձ�
extern  uint16_t pwm2;                     //ͨ��2ռ�ձ�
extern  uint8_t  Lock_Statu_Flag;          //����״̬��־λ 0�ر�״̬ 1��������  2������ 3������״̬�쳣�������ˣ�
extern  uint8_t RX3_Buffer [RX3_MAX];      //����3��������
extern  uint8_t rx3flag;                   //����3ָ����ձ�־λ      
uint8_t Instructions;                      //0δ�յ�ָ�� 1�յ��½�ָ�2�յ�����ָ��
uint8_t lock_zhiling_look_flog;            //����ͨѶ��ѯָ�� 0δ�յ�ָ�1�յ�ָ��
uint8_t lock_zhiling_look_1_flog;            //����ͨѶ��ѯָ�� 0δ�յ�ָ�1�յ�ָ��
uint8_t lock_tongxu_flog=0;                //��ʱ��ͨѶ���ϣ���������ң����µ��������������º�1������ͨѶ���ϸ�λ���ư壩�������ر�״̬�£��ϵ�һ������ͨѶ���ϸ�λ���ư壩
uint8_t Escalation_Flag;                   //ָ��ִ�б�־ 0δ�յ�ָ��/ִ����� 1����յ��½�ָ�� 2����յ�����ָ�� 3�½�ָ��ִ��������ת���������  4ִ������ָ���⵽�Ϸ��г����������룬�½�����  5ִ������ָ��������ת������½�
uint8_t zhiling_Flag; 
uint8_t lcok_up_erro_Flag; 
uint8_t zhuangtaigaibian_Flag;             //��ʼ��ֵ0 ״̬�ı�Ϊ1
uint32_t zhiling_Time; 
volatile uint8_t zuzhuan_flog;
uint32_t rester_jishi_time=0;                //ͨѶ���ϼ�ʱ
Memory_TypeDef  Info;						           //��ַ���汾�̼��ṹ��
extern uint8_t 	SecSend;		  //��ʱ �����ź�	����
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
	vTaskSuspend(myTask05Handle);          //�����������������
//	vTaskSuspend(myTask04Handle);                //�����쳣��������
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
   
    osDelay(1200);                                 //��ʱ1.2s
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);    //��תָʾ�ƣ������жϳ�������״̬
		IWDG->KR = 0xAAAA;	                           //ι��
		
		
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
		if(Infrared_Detection()==0)          //������������ѹر�״̬
		{
			if(shangdain_flog==1)
			{
				shangdain_flog=0;
				SysSec_1=0;
				zhiling_Time=0;      //�ϵ�����ʱ�䣿
				zhiling_Flag=0;       //���ϵ�1������û����ͨѶ������������	
				lock_tongxu_flog=0;
			  lcok_up_erro_Flag=0x00;             //�Ѿ�����
				printf("�����ϵ����ѽ���\r\n");
			
			}
//		  printf("zuzhuan_flog=%d\r\n",zuzhuan_flog);
//			if(ADC1_GetVaule()<ADC_Stalling)  
      if(zuzhuan_flog==0x01)			
			{
				RCWL_1655_or_9631_UART_Ranging();  //��⳵��״̬
				osDelay(10);                       //�ȴ�10ms	
				if(Instructions==1)                
				{
					Instructions=0;
					Lock_Com_feedback(1,0x25);       //����ָ������ϱ�
				}				
        if(Instructions==2)
				{
					BEEP_Init();                     //׼�����𣬸澯��ʾ
					Instructions=0;                  //����ָ����ִ��
					Escalation_Flag=2;               //��ʼ����
					if(Lock_Distance_Flag==1)
					{
						printf("�ر�״̬���յ�����ָ��û�г��������Ƶ������\r\n");				
						MOTOR_SA8321_Control(1);       //û�г��������Ƶ������	
            osDelay(500);						
					
					}
					else
					{
						
						MOTOR_SA8321_Control(0);       //�г���ֹͣ         
						printf("�ر�״̬���յ�����ָ���⵽������������ͣ\r\n");	
					}					
				}else
        {
					
					if(Escalation_Flag==2)
					{
						
						MOTOR_SA8321_Control(1);      //û�г��������Ƶ������ 
            osDelay(500);							
					
					}else if(Escalation_Flag==1||Escalation_Flag==3)
					{
						MOTOR_SA8321_Control(2);       //ִ���½�ָ��	
            osDelay(500);		
				
					}else
          {
						
						if(zhiling_Flag==1)
						{
//							printf("zhiling_Time=%d\r\n",zhiling_Time);
							if(zhiling_Time++>1000)        //����Ӽ�ʱ
							{
								zhiling_Time=0;
								zhiling_Flag=0;
								printf("5���Ӽ�ʱʱ�䵽\r\n");
							}	
						}
						if(zhiling_Flag==0)
						{
							RCWL_1655_or_9631_UART_Ranging();  //��⳵��״̬
							osDelay(10);                       //�ȴ�10ms	
							if(Lock_Distance_Flag==1)
							{					
								if(lock_tongxu_flog==1)
								{
								Lock_Distance_Time++;          //10���ʱ���������10�����޳���������								
								
								if(Lock_Distance_Time>150)
								{
									Lock_Distance_Time=0;
									printf("�ڹر�״̬�¼�⵽�����뿪1����ʱ�䵽�������\r\n");
									MOTOR_SA8321_Control(1);     //10��������û�г��������Ƶ������					
									Escalation_Flag=2;           //�������
									osDelay(500);	
								}									
								}

							}
							else
							{		
								Lock_Distance_Time=0;	         //��ʱ����
	//							printf("10sʱ�䵽��⵽������ʱ����\r\n");
							}
						}	
						if(SysSec_1<120)
						{
//							printf("SysSec_1=%d\r\n",SysSec_1);
							if(lock_zhiling_look_1_flog==1)    //���ͨѶ���ϣ�2���Ӹ�λ
							{
								lock_zhiling_look_1_flog=0;
								SysSec_1=0;
							}
						}
            else
						{
						  printf("2����ͨѶ�쳣��λ\r\n");
				      __disable_irq();      //�ر������ж�
              NVIC_SystemReset();   //�����λ��Ƭ��
						}	
						
						if(lcok_up_erro_Flag==0x01)
						{
							SysSec_1=0;
					  	vTaskResume(myTask05Handle);     //�������澯
						}						
					}					
				}
//      printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
			}else
			{
//      printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
				if(Escalation_Flag==2)             //�յ�ָ������������ת
				{
					printf("ִ����������������ת\r\n");
					MOTOR_SA8321_Control(2);         //�½�����  
          Escalation_Flag=3;				
				  osDelay(1000); 
				}
				if(Escalation_Flag==1||Escalation_Flag==3)
				{
					printf("ִ���½�����������ת��������������ת��Ĭ�ϵ�����λ\r\n");
					BEEP_Init();                     //BEEP��ʼ��
					MOTOR_SA8321_Control(0);           //ֹͣ���
          Escalation_Flag=0;                 //����½�ָ�����	
          SysSec_1=0;					
          Lock_Statu_Flag=0x01;                 //��ǳ���״̬	
          lcok_up_erro_Flag=0x00;             //�Ѿ�����					
					Lock_Com_feedback(1,0x01);       //����ָ������ϱ�
					zhiling_Flag=1;
					zhiling_Time=0;
				}

			}
		}
		
		if((Infrared_Detection()==1))  //�������״̬���ѿ���״̬
		{
			SysSec_1=0;
			if(shangdain_flog==1)
			{
				shangdain_flog=0;
			  lcok_up_erro_Flag=0x01;             //�Ѿ�����	
        lock_tongxu_flog=1;				
			}
//      printf("zuzhuan_flog=%d\r\n",zuzhuan_flog);
//			if(ADC1_GetVaule()<ADC_Stalling)
			if(zuzhuan_flog==0x01)
			{
				if(Instructions==2)
				{
					Instructions=0;
					Lock_Com_feedback(1,0x26); //����ָ�����
				}
//				if(Instructions==1) 				
			if(Instructions==1||(lock_tongxu_flog!=1))          //����յ��½�ָ��  //ͨѶ�����½�����
				{
					
					BEEP_Init();               //׼���½����澯��ʾ
					Instructions=0;            //�½�ָ����ִ��
					Escalation_Flag=1;         //����յ�ָ���½� 
					MOTOR_SA8321_Control(2);   //�½����
					osDelay(500);		
					printf("����״̬�յ��½�ָ���½����\r\n");		
				}else
				{
					if(Escalation_Flag==1||Escalation_Flag==3)
					{
					  MOTOR_SA8321_Control(2);   //�½����
				  	osDelay(500);	
					}else if(Escalation_Flag==2)
					{
						MOTOR_SA8321_Control(1);      //û�г��������Ƶ������ 
            osDelay(500);			
					}else
					{
						if(lcok_up_erro_Flag==0x01)
						{
					  		vTaskSuspend(myTask05Handle);          //�����������������
								BEEP_Abnormal(0);                  //�رշ�����

						}
					}
	
				}	
//       printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());				
			}else
			{
//				printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
				if(Escalation_Flag==1)
				{
					printf("ִ���½�ָ��������ת\r\n");
					MOTOR_SA8321_Control(2);   //�½����
					osDelay(500);						
				}
				if(Escalation_Flag==2)
				{
					printf("ִ������ָ��������תĬ�ϵ��������λ\r\n");
					BEEP_Abnormal(0);            //�رշ�����
					MOTOR_SA8321_Control(0);     //ֹͣ���
					Lock_Statu_Flag=0x00;        //��������	
					Escalation_Flag=0;           //����������
					lcok_up_erro_Flag=0x01;      //�Ѿ�����
					BEEP_Init();                 //BEEP��ʼ��
					Lock_Com_feedback(1,0x26); //����ָ�����
					printf("����״̬������ָ������ϱ�\r\n");
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
		
			switch(Rxbuffer_1[3]) //�ж�ָ��
			{
				
				case 0x25:          //����ָ��
				  Instructions=1;
					printf("����ָ��\r\n");
					break;
				case 0x26:          //����ָ��
				  Instructions=2;
					printf("����ָ��\r\n");
					break;
				case 0x03:          //����ָ��
					Lock_Com_feedback(1,0x03);
					printf("����ָ��\r\n");
					break;
				case 0x04:          //����ָ��
					Lock_Com_feedback(1,0x04);
					printf("����ָ��\r\n");
					break;
				case 0x21:          //��ѯ����״ָ̬��
					lock_zhiling_look_flog=1;
				  lock_zhiling_look_1_flog=1;
					Lock_Statu_Send();  //����������͵���״̬
					printf("��ѯ����״ָ̬��\r\n");
					break;
				case 0x06:          //д��ַ����	
          Address_Send();					
				  Info.JIQIHAO=RX3_Buffer[1];
					STMFLASH_Write(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
				  printf("д��ַָ��\r\n");
					break;
				case 0x07:          //����ַ����
				  STMFLASH_Read(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
				  Read_Add();
					 printf("����ַָ��\r\n");
					break;
				case 0x08:          //��������������
					 printf("����������\r\n");
					break;
				case 0x09:          //��������
					printf("����ָ��\r\n");
					break;
				case 0x0A:          //�����汾��ȡ����
					STMFLASH_Read(InfoAddr,(uint64_t *)&Info,sizeof(Info)/8);
				  Read_Version();
				  printf("�����汾��ȡ\r\n");
					break;
				case 0x0B:          //��λ����
					printf("��λָ��\r\n");
					__disable_irq();      //�ر������ж�
          NVIC_SystemReset();   //�����λ��Ƭ��
					break;
				
			}

		}
	
		if(SecSend>10)   //����ڿ������߹ر�״̬ÿ10�����ϱ�״̬
		{
			SecSend=0;
			if(Lock_Statu_Flag==0x01)
			{
			Lock_Com_feedback(1,0x25);
			
			}
			if(Lock_Statu_Flag==0x00)
			{
			Lock_Com_feedback(1,0x26); //����ָ�����
			
			}
		
		
		}
		if(SysSec<300)  //����Ӽ�ʱ
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
//		  printf("�쳣ֵADC1_GetVaule=%d\r\n",ADC1_GetVaule());
			ADC1_GetVaule();
      osDelay(500);
		}else
		{
			printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());        //��Ҫע�͵�
			zuzhuan_flog=0x01;
//			ADC1_GetVaule();
		}

		
		
		
		
//		if(ADC1_GetVaule()>ADC_Stalling)
//		{
//			zuzhuan_flog=0x00;
//		  printf("�쳣ֵADC1_GetVaule=%d\r\n",ADC1_GetVaule());
//		
//		}else
//		{
//			if(++i>5)       //��ת����ʱ��ˢ��
//			{
//				i=0;
//			  zuzhuan_flog=0x01;
//				printf("ADC1_GetVaule=%d\r\n",ADC1_GetVaule());
//			}
//		}
		
		
//		printf("�쳣�澯����\r\n");
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
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP��
			osDelay(100);
			MX_TIM15_Init_1(2000);
			HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);
			__HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1,900);            //BEEP��
//		  printf("����������\r\n");
			osDelay(200);
  }
  /* USER CODE END BEEP_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

