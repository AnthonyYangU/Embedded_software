#ifndef __L3G4200D_H 
#define __L3G4200D_H 
#include "spi.h"

#define WHO_AM_I  0x0F 
#define CTRL_REG1 0x20  
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define OUT_TEMP  0x26
#define OUT_X_L   0x28 //X-axis angular rate data
#define OUT_X_H   0x29
#define OUT_Y_L   0x2A
#define OUT_Y_H   0x2B
#define OUT_Z_L   0x2C
#define OUT_Z_H   0x2D
#define STATUS_REG 0x27
#define spi_CsLow  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
#define spi_CsHigh  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
#define MEASUREMENT_RANGE_500 


typedef struct 
{
    u16 Out_X;
    u16 Out_Y;
    u16 Out_Z;  
}AngularRate;

extern AngularRate  ang_rate;

#ifdef MEASUREMENT_RANGE_250
#define SENSITIVITY 8.75/1000
#endif

#ifdef MEASUREMENT_RANGE_500
#define SENSITIVITY 17.50/1000
#endif

#ifdef MEASUREMENT_RANGE_2000
#define SENSITIVITY 70/1000
#endif

void l3g4200d_WriteByte(u8 address,u8 data);
u8 l3g4200d_ReadByte(u8 address);
void l3g4200d_WriteBuf(u8 address,u8* data_addr,u8 len);
void l3g4200d_ReadBuf(u8 address,u8* data_addr,u8 len);

void L3g4200d_Init(void);
u8 Read_Temp(void);
void Read_AngularRate(void);
void L3g4200d_Disable(void);
#endif
