#include "sim800c.h"
#include "usart4.h"
#include "delay.h"	
#include "string.h"    
#include "usart1.h"
#include "timer.h"
#include "spi_sdcard.h"
#include "spi.h"

//usmart֧�ֲ��
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART1_RX_STA;
//     1,����USART1_RX_STA;
static testValue_t testValue[5]={0};
void sim_at_response(u8 mode)
{
	if(USART1_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART1_RX_BUF);	//���͵�����
		if(mode)USART1_RX_STA=0;
	} 
}

/////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM800C �������(���Ų��ԡ����Ų��ԡ�GPRS���ԡ���������)���ô���

//SIM800C���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim800c_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//��SIM800C��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim800c_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	
	USART1_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART1->ISR&0X40)==0);//�ȴ���һ�����ݷ������  
		USART1->TDR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//��������
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{ 
			delay_ms(10);
			if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(sim800c_check_cmd(ack))break;//�õ���Ч���� //printf("SUCCEED!!!\r\n");
				USART1_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}



///////////////////////////////////////////////////////////////////////////////////////
//GPRS���Բ��ִ���

const u8 *modetbl[2]={"TCP","UDP"};//����ģʽ
//tcp/udp����
//����������,��ά������
//mode:0:TCP����;1,UDP����)
//ipaddr:ip��ַ
//port:�˿� 

//typedef struct
//{
//		double battery;
//    double temperature;
//		double deepth;
//    double force1[ARR_LEN];
//	  double force2[ARR_LEN];
//		double pressure1[ARR_LEN];
//	  double pressure2[ARR_LEN];
//	  double current[ARR_LEN];
//}testValue_t;

u8 sim800c_tcpudp_test(void)
{ 
	//u8 sendsta=0;
	u16 i,j,ite;
	USART1_RX_STA=0;
	printf("Begin initiation\r\n");
	
	for(i=0;i<5;i++){
		for(j=0;j<ARR_LEN;j++){
			testValue[i].force1[j] = 10.5*i + 3.0*j;
			testValue[i].force2[j] = 6.32 + i*j;
			testValue[i].pressure1[j] = i + 3*j;
			testValue[i].pressure2[j] = 3*i + 5*j;
			testValue[i].current[j] = 4*i + 1.2*j;
		}
		testValue[i].battery = 0.23*i;
		testValue[i].temperature = 23 + 0.5*i;
		testValue[i].deepth = 10.12 + 0.4*i;
	}

	printf("%d",sizeof(testValue[0]));
	if(sim800c_send_cmd("AT+CIPSTART=\"TCP\",\"39.108.179.129\",\"9000\"","OK",500))return 1;		//��������
	while(1)
	{ 
			if(sim800c_send_cmd("AT+CIPSEND",">",500)==0){
				send_multi_data(&testValue[ite],sizeof(testValue[ite]));
				if(sim800c_send_cmd((u8*)0X1A,"SEND OK",1000)==0)
				{
					ite++;
					if(ite>=5){
						sim800c_send_cmd("AT+CIPCLOSE=1","CLOSE OK",500);	//�ر�����
						sim800c_send_cmd("AT+CIPSHUT","SHUT OK",500);
						return 0;
					}
				}
				delay_ms(10);
			}else sim800c_send_cmd((u8*)0X1B,0,0);
				
//			if(sendsta>5){
//				sim800c_send_cmd("AT+CIPCLOSE=1","CLOSE OK",500);	//�ر�����
//				sim800c_send_cmd("AT+CIPSHUT","SHUT OK",500);		//�ر��ƶ����� 
//				if(sim800c_send_cmd("AT+CIPSTART=\"TCP\",\"39.108.179.129\",\"9000\"","OK",500))return ite+2;			//������������				
//				sendsta = 0;
//			}	
	} 
}


//SIM800C GPRS����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 sim800c_gprs_test(void)
{
	u8 state;
	//if(sim800c_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",500))return 6;
	if(sim800c_send_cmd("AT+CGCLASS=\"B\"","OK",1000))return 1;				//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	//if(sim800c_send_cmd("AT+CGMSCLASS=10","OK",1000))return 1;
	//if(sim800c_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",1000))return 2;//����PDP������,��������Э��,��������Ϣ
	if(sim800c_send_cmd("AT+CGATT=1","OK",500))return 3;					//����GPRSҵ��
	//if(sim800c_send_cmd("AT+CIPMODE=1","OK",500))return 3;					//����GPRSҵ��
	if(sim800c_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500))return 4;	 	//����ΪGPRS����ģʽ
	if(sim800c_send_cmd("AT+CLPORT=\"TCP\",\"2000\"","OK",500))return 5;
	//if(sim800c_send_cmd("AT+CIPHEAD=1","OK",500))printf("head fail\r\n");//return 5;	 				//���ý���������ʾIPͷ(�����ж�������Դ); 	
	printf("success\r\n");
	while(1)
	{	
		state = sim800c_tcpudp_test();
		printf("pass");
		USART1_RX_STA=0;
		sim_at_response(1);//���GSMģ�鷢�͹���������,��ʱ�ϴ�������
		if(state==0){
			return 0;
		}
	}
} 

//SIM800C�����Գ���
u8 sim800c_test(void)
{
	u8 state=0; 
	while(sim800c_send_cmd("AT","OK",100))//����Ƿ�Ӧ��ATָ�� 
	{
		printf("NO SIM800C!!!\r\n");
		delay_ms(10);
		printf("TRY TO CONNECT SIM800C...\r\n");
		delay_ms(10);  
	}
	while(1)
	{
		state = sim800c_gprs_test();//GPRS����
		if(state==0){
			return 0;
		}
	} 	
}
