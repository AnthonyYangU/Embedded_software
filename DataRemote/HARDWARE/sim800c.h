#ifndef __SIM800C_H__
#define __SIM800C_H__	 
#include "sys.h"
/************************************************
 ALIENTEK ������STM32F429������
 ATK-SIM800C GSM/GPRSģ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8	//�ߵ��ֽڽ����궨��
#define BATTERY 0
#define TEMPERATURE 1
#define DEEPTH 2
#define FORCE1 3
#define FORCE2 4
#define STRESS1 5
#define STRESS2 6
#define CURRENT 7

typedef uint8_t  DATATYPE;

void sim_send_sms(u8*phonenumber,u8*msg);
void sim_at_response(u8 mode);	
u8* sim800c_check_cmd(u8 *str);
u8 sim800c_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 sim800c_test(void);	         //SIM800C�����Ժ���
u8 sim800c_tcpudp_test(void);
u8 dataConstruct(DATATYPE datatype,double* data);
//void flush(void);
#endif


