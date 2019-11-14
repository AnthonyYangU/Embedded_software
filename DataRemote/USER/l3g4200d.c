#include "l3g4200d.h"
#include "delay.h"
	
AngularRate  ang_rate;
void L3g4200d_Init()
{
		SPI_Init2();
		spi_CsHigh;
	  //printf("SPI_Init2 success\n");
    l3g4200d_WriteByte(CTRL_REG1, 0x0F);  //normal mode
   
    l3g4200d_WriteByte(CTRL_REG2, 0x00);
    
    l3g4200d_WriteByte(CTRL_REG3, 0x10); 
    
    l3g4200d_WriteByte(CTRL_REG4, 0x10); 
                                   // 0x00  FS = 250 dps; 
                                   // 0x10 FS = 500 dps;     
                                   // 0x20 FS = 2000 dps; 
                                   //MEASUREMENT_RANGE ?mems_l3g4200d.h???
    l3g4200d_WriteByte(CTRL_REG5, 0x00); 
   
}

void Read_AngularRate(void)
{
   
    uint32_t res_timeout = 0x2000;
		//delay_ms(10);
    
    int8_t status = l3g4200d_ReadByte(STATUS_REG);
     
		//printf("The status is %d",status);
	
    while((status & 0x80) == 0 && res_timeout--)
    {
       status = l3g4200d_ReadByte(STATUS_REG);
    }   
		//printf("Rest time is %d\n",res_timeout);
		//printf("Status is %d\n",status);
	
		//printf("Over storage, status is %d",status);
    l3g4200d_ReadBuf(OUT_X_L ,(uint8_t*)&ang_rate.Out_X ,2);
        

    l3g4200d_ReadBuf(OUT_Y_L ,(uint8_t*)&ang_rate.Out_Y ,2);
    
    
    l3g4200d_ReadBuf(OUT_Z_L,(uint8_t*)&ang_rate.Out_Z ,2);
		
		//ang_rate.Out_X = 12;
}


void L3g4200d_Disable()
{
    l3g4200d_WriteByte(CTRL_REG1, 0x08);  //power down mode,
}


uint8_t Read_Temp(void)
{
    return l3g4200d_ReadByte(OUT_TEMP);
}

void l3g4200d_WriteByte(u8 address,u8 data)
{
	address = address & 0x3F;
	spi_CsLow;
	
	SPI_WriteByte(address);
	SPI_WriteByte(data);
	
	spi_CsHigh;
}

u8 l3g4200d_ReadByte( u8 address)
{
	u8 ret;
	address = (address|0x80)&0xBF;
	spi_CsLow;
	
	SPI_WriteByte(address);
	ret = SPI_ReadByte();
	spi_CsHigh;
	return ret;
}

void l3g4200d_WriteBuf( uint8_t address , uint8_t* data_addr , uint8_t len)
{
    u8 i=0;  
    address = (address & 0x3F ) | 0x40;
    
    spi_CsLow;
      
    SPI_WriteByte(address);
    
    for(i = 0; i<len ;i++)
    {
        SPI_WriteByte(data_addr[i]);
    }
    
    spi_CsHigh;

}


void l3g4200d_ReadBuf( uint8_t address ,uint8_t* data_addr , uint8_t len )
{
		//u8 read[2]={0};
    address = address | 0x80;
    
    spi_CsLow;

    SPI_WriteByte(address);
    HAL_SPI_Receive(&SPI_Handler2,data_addr,2,1000);
	/*
    for(i = 0; i<len ;i++)
    {
        data_addr[i] = SPI_ReadByte();
    }
   */
    spi_CsHigh;
}
