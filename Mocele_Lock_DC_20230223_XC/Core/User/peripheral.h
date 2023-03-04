#include "stdbool.h"
#include "usart.h"
#include "gpio.h"

#include "tim.h"

#define RX2_MAX 4          //���������С
#define RX3_MAX 20          //���������С
#define RX3_MAX_1 10          //���������С

#define SEND_TO_DJSF1778_S		1            //�л�Ϊ485����ģʽ
#define	RECEIVE_FROM_DJSF1778_S		0        //�л�Ϊ485����ģʽ

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim15;




void LED_Init(void);       //��ʼ��LED
void BEEP_Init(void);      //��ʼ��BEEP
void Peripheral_Init(void);//��ʼ������
void BEEP_MOTOR_Abnormal(bool Alarm );      //MOTOR�쳣BEEP�澯������ڶ�
uint8_t Infrared_Detection(void);           //�����ƽ���
void RCWL_1655_or_9631_UART_Ranging(void);  //�������Ϸ��Ƿ��г���
void MOTOR_SA8321_Control(uint8_t control); //���Ƶ��ת��
void BEEP_Abnormal(bool Alarm );            //BEEP�澯��������ֹͣ�澯
void TransceiverControl_DJSF1778_S(uint8_t TXorRX);//�л�RS485�շ�ģʽ
void WUXIN_RS485_UART_TX(void);
void CLR_Buf3(void);        //�������3����
void Lock_Statu_Send(void); //����״̬�ϱ�
void Fina(void);   //��ʼ�ָ�
void Address_Send(void);
void Read_Add(void);
void Read_Version(void);
void Lock_Com_feedback(uint8_t  Lock_statue,uint8_t com);
unsigned char CRC8_Tab( unsigned char *ptr, unsigned char len,unsigned char precrc );





