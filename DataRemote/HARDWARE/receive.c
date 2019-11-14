
#include "receive.h"
//#include "main.h"

extern int32_t GU900E_RECV_BUF_HEAD;
extern int32_t GU900E_RECV_BUF_REAR;
extern uint8_t GU900E_RECV_BUF[GU900E_RECV_BUF_LENGTH];
static uint32_t receive_timeout = 0;
//数据头8个字节
static const uint8_t comm_HeadID[] = {'\x5a', '\x5a'} ;
//static uint16_t comm_DeviceID=0x0006;
static uint16_t comm_DeviceID=0x0001;
static uint16_t comm_swid=0x0000;
static uint16_t comm_Opid=0x0101;

uint8_t comm_recv_buffer[64];
uint8_t gu900e_send_data_buffer[976];

static uint8_t SendIP[] = {"\r\nAT+CIPSTART=\"TCP\",\"39.105.181.70\",\"5999\"\r\n"};

/*
*******************************************************************************
*
* 从接收缓冲区中接收len个uint8数据至指定地址
* 返回接受的数量
*
*******************************************************************************
*/
int32_t
gu900e_get_data(uint8_t * buffer,int32_t len)
{
    int32_t offset = 0;

     receive_timeout = 0xffffff;
    do
    {
      if(GU900E_RECV_BUF_HEAD != GU900E_RECV_BUF_REAR)
      {
        buffer[offset] = GU900E_RECV_BUF[GU900E_RECV_BUF_REAR];
        GU900E_RECV_BUF_REAR = GU900E_RECV_BUF_REAR + 1;

      }
        offset ++;
    }while(receive_timeout--&&(offset<len));
     return offset;
}

void clear_recv_buffer(void)
{
  int32_t offset = 0;
  while(offset<64)
  {
    comm_recv_buffer[offset]=NULL;
    offset++;
  }

}
void clear_SendData_buffer(void)
{
  int32_t offset = 0;
  while(offset<976)
  {
    gu900e_send_data_buffer[offset]=NULL;
    offset++;
  }

}
/*****************************************
*
*检测回复的前8个字节是否正确，并且把
*后四个字节放到arg中，用于设置定时器
*后二十字节放到ip中，用于改变链接地址
* 3 更改时间，-3不更改时间
*****************************************/
int32_t
comm_get_waittime(uint32_t*  arg)
{

    int32_t ret = 0;
    int32_t time_out=0xffffff;

    int32_t receive=0;

    while (time_out--)
    {
      if(GU900E_RECV_BUF[47]=='\r'&&GU900E_RECV_BUF[48]=='\n')
      {
        ret = 0;
        break;
      }

    }
      while(GU900E_RECV_BUF[receive]!=0x5A)
      {
          receive++;

      }
    	memcpy(comm_recv_buffer,&GU900E_RECV_BUF[receive],32);
    	if(0x5A5A != *((uint16_t *)(&comm_recv_buffer[0])))
    		ret = 2;
    	else if(( 0x0001 != *((uint16_t *)(&comm_recv_buffer[2]))))
    		ret = 2;
    	else if( 0x0000 != *((uint16_t *)(&comm_recv_buffer[4])))
    		ret = 2;
    	else if( 0x01 != *((uint8_t *)(&comm_recv_buffer[6])))
    		ret = 2;
        else if( 0x00 != *((uint8_t *)(&comm_recv_buffer[7])))
                ret=2;
    	else
    	{
            *arg = *(uint32_t *)(&comm_recv_buffer[8]) ;
            ret=3;
        //    if(comm_recv_buffer[])
         }
   // }
    return ret;
}


/*
*******************************************
* 发送的数据包为 gu900e_send_data_buffer[64]
*
*******************************************
*/
void
gu900e_Data_Packet(uint16_t fuc,testvalue_t * real_data,int len)
{
  //起始码(0x5a5a)/设备号(uint16)/窗(uint8)/功能码(uint16)/数据部分(len*uint8)
  memcpy(&gu900e_send_data_buffer[0],&comm_HeadID,2 );
  memcpy(&gu900e_send_data_buffer[2],&comm_DeviceID,2);
  memcpy(&gu900e_send_data_buffer[4],&comm_swid,2);
  memcpy(&gu900e_send_data_buffer[6],&fuc,2);//操作码
  memcpy(&gu900e_send_data_buffer[8],real_data,(len));
}
/***************************************
* 返回 -8 TCP链接失败
* 返回 -9 链接成功，发送失败
* 返回  6 发送数据成功
*
***************************************/
//建立tcpip连接
int32_t
gu900e_connect()
{
  int32_t ret = 0;
  int32_t status=0;
  ret = gu900e_tcpip_connect(&SendIP[0]);
  while((ret != 4)&&(status<2))//断开 重新 连接，以确保能够发送数据
  {
   gu900e_tcpip_disconnect("\r\nAT+CIPCLOSE\r\n");
   gu900e_clear_buff();
   ret = gu900e_tcpip_connect(&SendIP[0]);
   status ++;
  }
//  App_delay(1000);
  status = 0;
  if(4 == ret)
  {
    //App_delay(200);
    ret = gu900e_SendData("\r\nAT+CIPSEND=976\r\n",gu900e_send_data_buffer);
    if(6== ret)
    {

      return ret;   //发送成功

    }
      else ret=-9; //链接成功，发送失败
  }
   else ret = -8; //TCP链接失败
    return ret;
}

//在tcpip连接\发送一组数据后接收上位机响应返回的数据
int32_t
gu900e_trans_response(uint16_t func , uint32_t* arg)
{
    int32_t ret = 0;
    if(func == comm_Opid)
        ret = comm_get_waittime(arg);//返回数据正确3，错误2
    else
        ret = 0;

    return ret;
}

//整套建立TCP、发送数据流程
int32_t
gu900e_complete_connect(uint16_t fuc,testvalue_t * real_data,int len,uint32_t* Tim_update)
{
  int32_t state = 0;
  gu900e_clear_buff();                      //清除接受缓冲区
  gu900e_Data_Packet( fuc,real_data, len); //打包好要发送的数据

  state = gu900e_connect();      //建立连接并发送数据
  if(state==6)                  //发送正常
  {
  //  gu900e_wait(500);
    state=gu900e_trans_response(fuc,Tim_update);//返回数据正确3，错误2
    clear_recv_buffer();
  }
  else
  {
    //*Tim_update = Short_time;     //如果发送失败，将定时器的值设置成Tim_update 1小时
     state =5;
  }

  return state;
}
