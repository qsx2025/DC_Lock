/* USER CODE BEGIN Header */
/*
�������������������
Ӳ����Դ��1.����оƬ��G070VBT6�����������оƬ��SA8321�������оƬ��RCWL-1655��RCWL-9631��AJ-SR04�������λ��״̬��⣨���⣩��ͨ��[����ͨ�ţ�433M_A��������ͨ�ţ�RS485��]
����оƬ��Դ���䣺�ٵ��ԣ�USART1��PA9,PA10����ӡ������Ϣ��ָʾ��ָʾ��[LED]��PA15��[�ߵ�ƽ��Ч]��������[BEEP]��PB15��[�ߵ�ƽ��Ч]
									�ۼ�⣺
									1)��������λ��״̬�������������ߵ�ƽ[CL5��PB10��,CL6��PB9��]��������ŵ�ƽ[CL4��PB1��]
									2)���ģ���⳵�Ƿ��뿪��USART2��PA2��PA3��[������9600]
									�ܿ��ƣ����Ƶ����Ŀ������رգ�ֹͣ[INA��PB13����INB(PB14)]��PWM��
									��ͨ�ţ�
									1)����ͨ��[SPI][433M_A][SCLK1��PA0����RST(PA1)��MISO1(PB2)��MOSI1��PB11����NSS1��PB12��][SW_CTL1��PB3����SW_CTL2��PB4����BUSY1��PB5����DIO1��PB6����DIO2��PB7��]
									2)����ͨ��USART3[RX485][RX��PB0����TX��PA5����DIR��PB8��]
									��������ͨ�ö�ʱ������ʱ�����ж�
���������ƣ�
(1)����Ԥ��ʵ��Ч����
��ͨ��ĳ��ͨ�ŷ�ʽ�����߻������ߣ����ճ��׮�·���ָ����ݳ��׮�·�ָ��ִ����Ӧ�Ķ���������ִ�н���ϱ������׮��
�ڵ�����������Դ����ʻ��󣨻��߳����򿪺�ʱ��û�г������룩�����رճ���
�۵��������ϼ�ʱ�����澯��Ϣ�����ϱ������׮
(2)����ʵ��˼·��
�ٳ����ϵ��ʼ����
�ڳ�ʼ�����֮��ʹ��FREERTOS�����ĸ����񣺷ֱ�Ϊ���Ź������������ȼ�Ϊ��ߣ�ִ��ι������[ָʾ�ƿɷ��������񣬱�ʶ�����Ƿ��ܷ�]
                                                 �쳣���������������ȼ������ڿ��Ź���ʱ�̼���ж��Ϸ��Ƿ��г������ǣ����޳������ǵȴ�3S����Ƶ��ʹ�������ڴ�״̬����ǳ���״̬Ϊ�򿪣���������ʾ��λ��ͨ���ϱ�����״̬��������Ƶ��ʹ�������ڹر�״̬����ǳ���״̬Ϊ�رգ���������ʾ��λ��ͨ��������ʱ���ϱ�����״̬������������״̬�ǹرգ�����û�н��յ�����ָ���⵽����״̬�仯������������������ʾ��������Ƹ�λ����λ��ɱ��������
																								 ͨ������ͨ�����߻������ߵķ�ʽ�����յ����ݴ�ŵ�һ�����顣��[�������ȼ����]
																								 ����������ʱ��ȡ����״̬������ִ�г��׮����ָ�ִ�У����ֺͳ��׮�����Ӻ�ͨ�ţ�[�������ȼ�����ͨ����������]
���������Ϣ��
            1.�汾V1.0
						2.�༭�ˣ�sibo
						3.������ڣ�2022.07
									        
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
  uint8_t pDat; 
	uint8_t Rxbuffer[RX3_MAX];
	uint8_t Rxbuffer_1[RX3_MAX_1];
	uint16_t Uart3RxCount=0;
	uint8_t shangdain_flog;
extern	Memory_TypeDef  Info;						           //��ַ���汾�̼��ṹ��
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern volatile int32_t SysSec;
extern volatile int32_t SysSec_1;
uint8_t 	SecSend=0;		  //��ʱ �����ź�	����
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_IWDG_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_TIM15_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);					    //ʹ��Timer6�ж�
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1); //����PWMͨ��1���������
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2); //����PWMͨ��2���������
	HAL_TIMEx_PWMN_Start(&htim15,TIM_CHANNEL_1);//����PWMͨ��3��BEEP
	Peripheral_Init();                          //��ʼ������
	Fina();
  HAL_UARTEx_ReceiveToIdle_IT(&huart2,rx2buffer,RX2_MAX);
  TransceiverControl_DJSF1778_S(RECEIVE_FROM_DJSF1778_S);
//  HAL_UARTEx_ReceiveToIdle_IT(&huart3,rx3buffer,RX3_MAX);
  HAL_UART_Receive_IT(&huart3,&pDat,1);
	shangdain_flog=1;
	printf("�ϵ�\r\n");
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void RxScan(void)
{

	Rxbuffer[Uart3RxCount]=pDat;
	if(++Uart3RxCount>=RX3_MAX)  Uart3RxCount=0;


while(HAL_UART_Receive_IT(&huart3,&pDat,1)!=HAL_OK);
}// �ض��򴮿ڴ�ӡ����

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance ==USART3)
	{
//	  printf("�����жϺ���\r\n");
		RxScan();
//		HAL_StatusTypeDef return_state;
//		return_state = HAL_UART_Receive_IT(&huart3,&pDat,1);	
//		if(return_state == HAL_BUSY)
//		{
//			
//			__HAL_UART_CLEAR_OREFLAG(&huart3);
//			huart3.RxState = HAL_UART_STATE_READY;
//			huart3.Lock=HAL_UNLOCKED;
//		  return_state=HAL_UART_Receive_IT(&huart3,&pDat,1);
//		
//		
//		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    // UNUSED(huart);
    /* NOTE: This function should not be modified, when the callback is needed,
the HAL_UART_ErrorCallback could be implemented in the user file
*/
    if( huart->ErrorCode & HAL_UART_ERROR_ORE )//Overflow error
   {
       uint32_t temp = huart->Instance->ISR;
       temp = huart->Instance->RDR;
   }
}


uint8_t  Re485_Scan(void)
{
	uint8_t k=0;
	uint8_t FRAMEHEAD_address=0;
	uint8_t ReciveFirst=1;
	uint8_t i;
	uint8_t findflag=0;
	for(k=0;k<20;k++)
	{
		if((Rxbuffer[k]==0x55)&&(Rxbuffer[k+1]==Info.JIQIHAO))
		{
			if(ReciveFirst){
				ReciveFirst=0;
				FRAMEHEAD_address=k;
			}
			if(Uart3RxCount>=FRAMEHEAD_address+6)
			{
				findflag=1;	
				Uart3RxCount=0;				
				for(i=0;i<6;i++)
				{
					Rxbuffer_1[i]=Rxbuffer[i+FRAMEHEAD_address];
//					printf("Rxbuffer[%d]=%02x\r\n",i,Rxbuffer[i]);
				}
				for(i=0;i<20;i++)
				{
					Rxbuffer[i]=0;
				}
			}
			k=20;
				
		}
		
	}
	return 	findflag;
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM6){
	  static uint16_t count0=0;  				//�����ڲ�����10ms�Զ���1����	
		count0++;	
		
		if(count0%10==0){ 					//ÿ10ms
		
//			printf("Infrared_Detection()=%02x\r\n",Infrared_Detection());
		
		}		
		if(count0%100==0){ 					//ÿs
		
			SysSec++;
			SysSec_1++;
		
		}
		if(count0>=6000)          //ÿmin
      {  				
				count0=0;
				if(SecSend!=0xff)  
				{
				SecSend++;
				}					
			}
	}
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
