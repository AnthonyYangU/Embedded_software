#include "gu900e.h"

#include "string.h"

#define Short_time 3600
typedef struct
{
    //const int n=100;
    double forcex[40]       ;   //轴力
    double torque[40]       ;   //扭矩
    double battery[40]      ;   //电池电量
    double temperature  ;   //温度
/*    double memsx[n]        ;   //x方向转速
    double memsy[n]        ;   //y方向转速
    double memsz[n]        ;   //z方向转速       */
}testvalue_t;
int32_t comm_get_waittime(uint32_t*  arg);
int32_t gu900e_get_data(uint8_t * buffer,int32_t len);
void gu900e_Data_Packet(uint16_t fuc,testvalue_t * real_data,int32_t len);
int32_t gu900e_trans_response(uint16_t func , uint32_t* arg);
int32_t gu900e_connect(void);
int32_t gu900e_complete_connect(uint16_t fuc,testvalue_t * real_data,int len,uint32_t* Tim_update);
void clear_SendData_buffer(void);
