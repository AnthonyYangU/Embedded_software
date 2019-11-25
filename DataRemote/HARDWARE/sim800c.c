#include "sim800c.h"
#include "usart4.h"
#include "delay.h"	
#include "string.h"    
#include "usart1.h"
#include "timer.h"
#include "spi_sdcard.h"
#include "spi.h"
//usmartÖ§³Ö²¿·
//½«ÊÕµ½µÄATÖ¸ÁîÓ¦´ğÊı¾İ·µ»Ø¸øµçÄÔ´®¿Ú
//mode:0,²»ÇåÁãUSART1_RX_STA;
//     1,ÇåÁãUSART1_RX_STA;
static testValue_t testValue[5]={0};
//void flush(void){
//	memset(testValue,0,5320);
//}
void sim_at_response(u8 mode)
{
	if(USART1_RX_STA&0X8000)		//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//Ìí¼Ó½áÊø·û
		printf("%s",USART1_RX_BUF);	//·¢ËÍµ½´®¿Ú
		if(mode)USART1_RX_STA=0;
	} 
}

/////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM800C ¸÷Ïî²âÊÔ(²¦ºÅ²âÊÔ¡¢¶ÌĞÅ²âÊÔ¡¢GPRS²âÊÔ¡¢À¶ÑÀ²âÊÔ)¹²ÓÃ´úÂë

//SIM800C·¢ËÍÃüÁîºó,¼ì²â½ÓÊÕµ½µÄÓ¦´ğ
//str:ÆÚ´ıµÄÓ¦´ğ½á¹û
//·µ»ØÖµ:0,Ã»ÓĞµÃµ½ÆÚ´ıµÄÓ¦´ğ½á¹û
//    ÆäËû,ÆÚ´ıÓ¦´ğ½á¹ûµÄÎ»ÖÃ(strµÄÎ»ÖÃ)
u8* sim800c_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)		//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//Ìí¼Ó½áÊø·û
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//ÏòSIM800C·¢ËÍÃüÁî
//cmd:·¢ËÍµÄÃüÁî×Ö·û´®(²»ĞèÒªÌí¼Ó»Ø³µÁË),µ±cmd<0XFFµÄÊ±ºò,·¢ËÍÊı×Ö(±ÈÈç·¢ËÍ0X1A),´óÓÚµÄÊ±ºò·¢ËÍ×Ö·û´®.
//ack:ÆÚ´ıµÄÓ¦´ğ½á¹û,Èç¹ûÎª¿Õ,Ôò±íÊ¾²»ĞèÒªµÈ´ıÓ¦´ğ
//waittime:µÈ´ıÊ±¼ä(µ¥Î»:10ms)
//·µ»ØÖµ:0,·¢ËÍ³É¹¦(µÃµ½ÁËÆÚ´ıµÄÓ¦´ğ½á¹û)
//       1,·¢ËÍÊ§°Ü
u8 sim800c_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	
	USART1_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART1->ISR&0X40)==0);//µÈ´ıÉÏÒ»´ÎÊı¾İ·¢ËÍÍê³É  
		USART1->TDR=(u32)cmd;
	}else u2_printf("%s\r\n",cmd);//·¢ËÍÃüÁî
	
	if(ack&&waittime)		//ĞèÒªµÈ´ıÓ¦´ğ
	{
		while(--waittime)	//µÈ´ıµ¹¼ÆÊ±
		{ 
			delay_ms(10);
			if(USART1_RX_STA&0X8000)//½ÓÊÕµ½ÆÚ´ıµÄÓ¦´ğ½á¹û
			{
				if(sim800c_check_cmd(ack))break;//µÃµ½ÓĞĞ§Êı¾İ //printf("SUCCEED!!!\r\n");
				USART1_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////
//GPRS²âÊÔ²¿·Ö´úÂë

const u8 *modetbl[2]={"TCP","UDP"};//Á¬½ÓÄ£Ê½
//tcp/udp²âÊÔ
//´øĞÄÌø¹¦ÄÜ,ÒÔÎ¬³ÖÁ¬½Ó
//mode:0:TCP²âÊÔ;1,UDP²âÊÔ)
//ipaddr:ipµØÖ·
//port:¶Ë¿Ú 

u8 sim800c_tcpudp_test(void)
{ 
	//u8 sendsta=0;
	u16 i,j,ite=0,errorTime;
	static u8 repeatTime;
	USART1_RX_STA=0;
	printf("Begin initiation\r\n");

	printf("The size of testValue is %d \r\n",sizeof(testValue));
	if(sim800c_send_cmd("AT+CIPSTART=\"TCP\",\"39.108.179.129\",\"9000\"","OK",500))return 1;		//·¢ÆğÁ¬½Ó
	while(1)
	{ 
			if(sim800c_send_cmd("AT+CIPSEND",">",1000)==0){
				send_multi_data(&testValue[ite],sizeof(testValue[ite]));
				if(sim800c_send_cmd((u8*)0X1A,"SEND OK",1000)==0)
				{
					ite=ite+1;
					if(ite>=5){
						sim800c_send_cmd("AT+CIPCLOSE=1","CLOSE OK",500);	//¹Ø±ÕÁ¬½Ó						
						sim800c_send_cmd("AT+CIPSHUT","SHUT OK",500);//¹Ø±Õ³¡¾°
						printf("ite number: %d \r\n",ite);
						return 0;
					}
					printf("Ite is %d\r\n",ite);
				}else{
					sim800c_send_cmd((u8*)0X1B,0,0);
					errorTime++;
					printf("error\r\n");
				}
				delay_ms(10);
			}else{
				sim800c_send_cmd((u8*)0X1B,0,0);
				errorTime++;
				printf("Send error\r\n");
			}
			if(errorTime>3){
				sim800c_send_cmd("AT+CIPCLOSE=1","CLOSE OK",500);	//¹Ø±ÕÁ¬½Ó
				sim800c_send_cmd("AT+CIPSHUT","SHUT OK",500);				
			  printf("Fatal error\r\n");
				return ++repeatTime;
			}
	} 
}


//SIM800C GPRS²âÊÔ
//ÓÃÓÚ²âÊÔTCP/UDPÁ¬½Ó
//·µ»ØÖµ:0,Õı³£
//    ÆäËû,´íÎó´úÂë
u8 sim800c_gprs_test(void)
{
	u8 state;
	//if(sim800c_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",500))return 6;
	if(sim800c_send_cmd("AT+CGCLASS=\"B\"","OK",1000))return 1;				//ÉèÖÃGPRSÒÆ¶¯Ì¨Àà±ğÎªB,Ö§³Ö°ü½»»»ºÍÊı¾İ½»»» 
	//if(sim800c_send_cmd("AT+CGMSCLASS=10","OK",1000))return 1;
	if(sim800c_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",1000))return 2;//ÉèÖÃPDPÉÏÏÂÎÄ,»¥ÁªÍø½ÓĞ­Òé,½ÓÈëµãµÈĞÅÏ¢
	if(sim800c_send_cmd("AT+CGATT=1","OK",500))return 3;					//¸½×ÅGPRSÒµÎñ
	//if(sim800c_send_cmd("AT+CIPMODE=1","OK",500))return 3;					//¸½×ÅGPRSÒµÎñ
	if(sim800c_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500));//return 4;	 	//ÉèÖÃÎªGPRSÁ¬½ÓÄ£Ê½
	if(sim800c_send_cmd("AT+CLPORT=\"TCP\",\"2000\"","OK",500))return 5;
	//if(sim800c_send_cmd("AT+CIPHEAD=1","OK",500))printf("head fail\r\n");//return 5;	 				//ÉèÖÃ½ÓÊÕÊı¾İÏÔÊ¾IPÍ·(·½±ãÅĞ¶ÏÊı¾İÀ´Ô´); 	
	printf("success\r\n");
	while(1)
	{	
		state = sim800c_tcpudp_test();
		printf("The state is %d",state);
		USART1_RX_STA=0;
		//sim_at_response(1);//¼ì²éGSMÄ£¿é·¢ËÍ¹ıÀ´µÄÊı¾İ,¼°Ê±ÉÏ´«¸øµçÄÔ
		if(state==0){
			return 0;
		}
		if(state>=2){
			return 2;
		}
	}
} 

//SIM800CÖ÷²âÊÔ³ÌĞò
u8 sim800c_test(void)
{
	u8 state=0; 
	while(sim800c_send_cmd("AT","OK",100))//¼ì²âÊÇ·ñÓ¦´ğATÖ¸Áî 
	{
		printf("NO SIM800C!!!\r\n");
		delay_ms(10);
		printf("TRY TO CONNECT SIM800C...\r\n");
		delay_ms(10);  
	}
	while(1)
	{
		state = sim800c_gprs_test();//GPRS²âÊÔ
		if(state==0)return 0;
	} 	
}
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
u8 dataConstruct(DATATYPE datatype,double* data)
{
	u8 i,j;
	if(datatype==BATTERY){
		for(i=0;i<5;i++){
			testValue[i].battery=data[i];
		}
		return 0;
	}
	if(datatype==TEMPERATURE){
		for(i=0;i<5;i++){
			testValue[i].temperature=data[i];
		}
		return 0;
	}
	if(datatype==DEEPTH){
		for(i=0;i<5;i++){
			testValue[i].deepth=data[i];
		}
		return 0;		
	}
	if(datatype==FORCE1){
		for(i=0;i<5;i++){
			for(j=0;j<ARR_LEN;j++){
				testValue[i].force1[j] = data[26*i+j];
			}
		}
		return 0;		
	}
	if(datatype==FORCE2){
		for(i=0;i<5;i++){
			for(j=0;j<ARR_LEN;j++){
				testValue[i].force2[j] = data[26*i+j];
			}
		}	
		return 0;
	}
	if(datatype==STRESS1){
		for(i=0;i<5;i++){
			for(j=0;j<ARR_LEN;j++){
				testValue[i].stress1[j] = data[26*i+j];
			}
		}
		return 0;
	}
	if(datatype==STRESS2){
		for(i=0;i<5;i++){
			for(j=0;j<ARR_LEN;j++){
				testValue[i].stress2[j] = data[26*i+j];
			}
		}	
		return 0;
	}	
	if(datatype==CURRENT){
		for(i=0;i<5;i++){
			for(j=0;j<ARR_LEN;j++){
				testValue[i].current[j] = data[26*i+j];
			}
		}
		return 0;		
	}		
	return 1;
}

