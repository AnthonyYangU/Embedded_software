#include "delay.h"
#include "spi.h"
#include "spi_sdcard.h"
#include "usart.h"
u8 SD_Type;
u8 SD_Init(void)
{
	u16 i;
	u8 r1;
	u16 retry;
	u8 buff[6];
  //初始化sd卡，时钟不能超过400khz
	SPI_Init();
	//printf("SPI_ReadWriteByte(0xff)%d",SPI_ReadWriteByte(0xff));
	//SPI_SetSpeed(SPI_BAUDRATEPRESCALER_256);         
  //设置nss为低电平，选中SD卡
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET); 	
  //延时，等待上电稳定，
	delay_ms(10);
	for(i=0;i<15;i++)
	{
		//至少产生74个脉冲，让SD卡初始化完成
		SPI_ReadWriteByte(0xff);                       
	}
  //printf("\nReturn signal is %d \n",SPI_ReadWriteByte(0xff));
  //循环发送CMD0，直到sd卡返回0x01，进入idle状态
	retry = 0;
	do
	{
		r1 = SD_SendCommand(CMD0,0,0X95);  
		retry++;
	}while((r1!=0x01)&&(retry<200));                 
	if(retry==200)                                    
	{
		return 1;
	}
	r1= SD_SendCommand(CMD8,0x01aa,0x87);            
	if(r1==0x01)                                      
	{
		buff[0] = SPI_ReadWriteByte(0xff);             
		buff[1] = SPI_ReadWriteByte(0xff);
		buff[2] = SPI_ReadWriteByte(0xff);
		buff[3] = SPI_ReadWriteByte(0xff);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
		SPI_ReadWriteByte(0xff);                      
		//printf("\n (1) buff[2] is %d,buff[3] is %d\r\n",buff[2],buff[3]);	
				retry = 0;
		  	do
				{
				r1 = SD_SendCommand(CMD55,0,0);
				
				printf("\n (1) r1 is %d\n",r1);	
				delay_ms(10);
				
				if(r1!=0x01)                                  
					return r1;
				
				r1 = SD_SendCommand(ACMD41,0x40000000,1);
				delay_ms(10);
				
				printf("\n (2) r1 is %d\n",r1);	
				retry++;
				if(retry>200)
					return r1;              
				
				printf("\n (3) r1 is %d\n",r1);	
				delay_ms(10);
				}while(r1!=0); 

		r1 = SD_SendCommand_NoDeassert(CMD58,0,0);
				printf("\n (4) r1 is %d\n",r1);
		if(r1!=0x00)
			return r1;
		buff[0] = SPI_ReadWriteByte(0xff);             
		buff[1] = SPI_ReadWriteByte(0xff);
		buff[2] = SPI_ReadWriteByte(0xff);
		buff[3] = SPI_ReadWriteByte(0xff);

		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);

		SPI_ReadWriteByte(0xff);


		if(buff[0]&0x40)
		{
			SD_Type = SD_TYPE_V2HC;                         
		}
		else
		{
			SD_Type = SD_TYPE_V2;                           
		}
		printf("Type is %d",SD_Type);
		SPI_SetSpeed(SPI_BAUDRATEPRESCALER_2);          
		}
	return r1;
}

u8 SD_SendCommand(u8 cmd,u32 arg,u8 crc)
{
	unsigned char r1=0;
	unsigned int Retry = 0;
	

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);

	SPI_ReadWriteByte(0xff);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);

	SPI_ReadWriteByte(cmd|0x40);                                
	SPI_ReadWriteByte((u8)(arg>>24));                          
	SPI_ReadWriteByte((u8)(arg>>16));
	SPI_ReadWriteByte((u8)(arg>>8));
	SPI_ReadWriteByte((u8)(arg));
	SPI_ReadWriteByte(crc);                                     

	while((r1 = SPI_ReadWriteByte(0xff))==0xff)                 
	{
		Retry++;
		if(Retry>800) break;                                       
	}

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);

	SPI_ReadWriteByte(0xff);                                    

	return r1;
}

u8 SD_SendCommand_NoDeassert(u8 cmd,u32 arg,u8 crc)
{
	unsigned char r1;
	unsigned int Retry = 0;

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);

	SPI_ReadWriteByte(0xff);

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);

	SPI_ReadWriteByte(cmd|0x40);
	SPI_ReadWriteByte((u8)(arg>>24));
	SPI_ReadWriteByte((u8)(arg>>16));
	SPI_ReadWriteByte((u8)(arg>>8));
	SPI_ReadWriteByte((u8)(arg));
	SPI_ReadWriteByte(crc);

	while((r1 = SPI_ReadWriteByte(0xff))==0xff)
	{
		Retry++;
		if(Retry >800) break;
	}
	return r1;
}

u8 SD_ReceiveData(u8 *data,u16 len,u8 release)
{
	u16 retry;
	u8 r1;

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);

	retry = 0;
	do
	{
		r1 = SPI_ReadWriteByte(0xff);
		retry++;
		if(retry>4000)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
			return 1;                                   
		}
	}while(r1!=0xfe);                                  


	while(len--)
	{
		*data = SPI_ReadWriteByte(0xff);
		data++;
	}

	SPI_ReadWriteByte(0xff);          
	SPI_ReadWriteByte(0xff);

	if(release == RELEASE)                       
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
		SPI_ReadWriteByte(0xff);
	}

	return 0;
}

u8 SD_GetCID(u8 *cid_data)
{
	u8 r1;

	r1 = SD_SendCommand(CMD10,0,0xff);                
	if(r1!=0x00)
		return r1;                                     

		SD_ReceiveData(cid_data,16,RELEASE);    
		return 0;
}

u8 SD_GetCSD(u8 *csd_data)
{
	u8 r1;

	r1 = SD_SendCommand(CMD9,0,0xff);                     
	if(r1!=0x00)
		return r1;                             

		SD_ReceiveData(csd_data,16,RELEASE);          
		return 0;
}

u32 SD_GetCapacity(void)
{
    u8 csd[16];
    u32 Capacity;
    u8 r1;
    u16 i;
    u16 temp;

    if(SD_GetCSD(csd)!=0)
    return 0;                                    

    if((csd[0]&0xC0)==0x40)              
    {
       Capacity=((u32)csd[8])<<8;
       Capacity+=(u32)csd[9]+1;
       Capacity = (Capacity)*1024;	      
       Capacity*=512;	                  
    }
    else		                         
    {
       i = csd[6]&0x03;
       i<<=8;
       i += csd[7];
       i<<=2;
       i += ((csd[8]&0xc0)>>6);
       r1 = csd[9]&0x03;
       r1<<=1;
       r1 += ((csd[10]&0x80)>>7);
       r1+=2;
       temp = 1;
       while(r1)
       {
        temp*=2;
         r1--;
       }
       Capacity = ((u32)(i+1))*((u32)temp);
       i = csd[5]&0x0f;
       temp = 1;
   while(i)
   {
      temp*=2;
      i--;
   }

   Capacity *= (u32)temp;                        
   }
   return (u32)Capacity;                 
}

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
  u8 r1;

  SPI_SetSpeed(SPI_BAUDRATEPRESCALER_2 );    

  if(SD_Type!=SD_TYPE_V2HC)	                 
  {
    sector = sector<<9;	                    
  }
   r1 = SD_SendCommand(CMD17,sector, 1);	         
   if(r1 != 0x00)	return r1;                        
   r1 = SD_ReceiveData(buffer, 512, RELEASE);	    
   if(r1 != 0)
     return r1;                           
   else
     return 0; 	                            
}

u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
  u8 r1;

  SPI_SetSpeed(SPI_BAUDRATEPRESCALER_2 );                       

  if(SD_Type != SD_TYPE_V2HC)
  {
    	sector = sector<<9;
  }
  r1 = SD_SendCommand(CMD18, sector, 1);                       
  if(r1 != 0x00)	return r1;
  do	                                                         
  {
    	if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)   
    	{
       		break;                                                
    	}
    	buffer += 512;
  }while(--count);                                              

  SD_SendCommand(CMD12, 0, 1);	                           
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);  	                                      
  SPI_ReadWriteByte(0xFF);
  if(count != 0)
    return count;                                             
  else
    return 0;
}

u8 SD_WaitReady(void)
{
  u8 r1;
  u16 retry=0;
  do
  {
    r1 = SPI_ReadWriteByte(0xFF);              
    retry++;
    if(retry==0xfffe)
    	return 1;
  }while(r1!=0xFF);
    	return 0;
}


u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
  	u8 r1;
  	u16 i;
  	u16 retry;

  	SPI_SetSpeed(SPI_BAUDRATEPRESCALER_2 );                   

    if(SD_Type!=SD_TYPE_V2HC)                           
  	{
     		sector = sector<<9;
  	}

        r1 = SD_SendCommand(CMD24, sector, 0x00);	           
  	if(r1 != 0x00)
  	{
    		return r1;
  	}

        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);                                         

  	SPI_ReadWriteByte(0xff);                                     
  	SPI_ReadWriteByte(0xff);
  	SPI_ReadWriteByte(0xff);

    SPI_ReadWriteByte(0xFE);                              
  	for(i=0;i<512;i++)                                
  	{
     		SPI_ReadWriteByte(*data++);
  	}

  	SPI_ReadWriteByte(0xff);                
  	SPI_ReadWriteByte(0xff);

  	r1 = SPI_ReadWriteByte(0xff);                               

  	if((r1&0x1F)!=0x05)                            
  	{
     		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
     		return r1;                                      
  	}

  	retry = 0;                                              

  	while(!SPI_ReadWriteByte(0xff))                  
  	{
     		retry++;
     		if(retry>65534)                               
     		{
        		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
        		return 1;                                   
     		}
  	}

  	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);                                          
  	SPI_ReadWriteByte(0xff);
        return 0;
}

u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count)
{
  	u8 r1;
  	u16 i;

  	SPI_SetSpeed(SPI_BAUDRATEPRESCALER_2 );                    

  	if(SD_Type != SD_TYPE_V2HC)
        {
    	    sector = sector<<9;
        }
  	/*if(SD_Type != SD_TYPE_MMC)
        {
    	     r1 = SD_SendCommand(ACMD23, count, 0x01);              
        }*/

        r1 = SD_SendCommand(CMD25, sector, 0x01);                 

  	if(r1 != 0x00)	return r1;

  	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);                                           

  	SPI_ReadWriteByte(0xff);                                       
  	SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);

  	do                                                           
  	{

     	    SPI_ReadWriteByte(0xFC);                               

     	    for(i=0;i<512;i++)                                      
     	    {
        	SPI_ReadWriteByte(*data++);
     	    }

	   SPI_ReadWriteByte(0xff);                                    
     SPI_ReadWriteByte(0xff);

     	    r1 = SPI_ReadWriteByte(0xff);                         

     	    if((r1&0x1F)!=0x05)                                    
     	    {
        	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
        	return r1;
     	    }

     	    if(SD_WaitReady()==1)                                  
     	   {
        	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);                                      
        	return 1;
     	   }
   }while(--count);

       SPI_ReadWriteByte(0xFD);                                  

       if(SD_WaitReady())                                         
       {
           HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);                                          
           return 1;
        }

       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);                                          
       SPI_ReadWriteByte(0xff);

       return count;                                           
}
