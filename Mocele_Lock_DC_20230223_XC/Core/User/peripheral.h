#include "stdbool.h"
#include "usart.h"
#include "gpio.h"

#include "tim.h"

#define RX2_MAX 4          //接收数组大小
#define RX3_MAX 20          //接收数组大小
#define RX3_MAX_1 10          //接收数组大小

#define SEND_TO_DJSF1778_S		1            //切换为485发送模式
#define	RECEIVE_FROM_DJSF1778_S		0        //切换为485接收模式

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim15;




void LED_Init(void);       //初始化LED
void BEEP_Init(void);      //初始化BEEP
void Peripheral_Init(void);//初始化外设
void BEEP_MOTOR_Abnormal(bool Alarm );      //MOTOR异常BEEP告警，电机摆动
uint8_t Infrared_Detection(void);           //红外电平检测
void RCWL_1655_or_9631_UART_Ranging(void);  //检测地锁上方是否有车辆
void MOTOR_SA8321_Control(uint8_t control); //控制电机转动
void BEEP_Abnormal(bool Alarm );            //BEEP告警，正常后停止告警
void TransceiverControl_DJSF1778_S(uint8_t TXorRX);//切换RS485收发模式
void WUXIN_RS485_UART_TX(void);
void CLR_Buf3(void);        //清除串口3数据
void Lock_Statu_Send(void); //地锁状态上报
void Fina(void);   //初始恢复
void Address_Send(void);
void Read_Add(void);
void Read_Version(void);
void Lock_Com_feedback(uint8_t  Lock_statue,uint8_t com);
unsigned char CRC8_Tab( unsigned char *ptr, unsigned char len,unsigned char precrc );





