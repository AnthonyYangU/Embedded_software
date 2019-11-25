#ifndef __SIM800C_H__
#define __SIM800C_H__	 
#include "sys.h"
/************************************************
 ALIENTEK 阿波罗STM32F429开发板
 ATK-SIM800C GSM/GPRS模块-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8	//高低字节交换宏定义
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
u8 sim800c_test(void);	         //SIM800C主测试函数
u8 sim800c_tcpudp_test(void);
u8 dataConstruct(DATATYPE datatype,double* data);
//void flush(void);
#endif


