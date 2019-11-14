#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

//extern SPI_HandleTypeDef SPI_Handler;

void SPI_Init(void);
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI_ReadWriteByte(u8 TxData);
void SPI_WriteByte(u8 TxData);
u8 SPI_ReadByte(void);
void SPI_Init2(void);
extern SPI_HandleTypeDef  SPI_Handler2;
#endif
