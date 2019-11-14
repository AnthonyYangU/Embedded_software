#ifndef __SPI_SDCARD_H
#define __SPI_SDCARD_H
#include "sys.h"

u8 SD_Init(void);
u8 SD_SendCommand(u8 cmd,u32 arg,u8 crc);
u8 SD_SendCommand_NoDeassert(u8 cmd,u32 arg,u8 crc);
u8 SD_ReceiveData(u8 *data,u16 len,u8 release);
u8 SD_GetCID(u8 *cid_data);
u8 SD_GetCSD(u8 *csd_data);
u32 SD_GetCapacity(void);
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count);
u8 SD_WaitReady(void);
u8 SD_WriteSingleBlock(u32 sector, const u8 *data);
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);

#define SD_TYPE_V2HC             0
#define SD_TYPE_V2               1
#define RELEASE                  0
#define NO_RELEASE               1

#define CMD0                     0                       /* Reset */
#define CMD1                     1                         /* Send Operator Condition - SEND_OP_COND */
#define CMD8                     8                     /* Send Interface Condition - SEND_IF_COND	*/
#define CMD9                     9                       /* Read CSD */
#define CMD10                    10                  /* Read CID */
#define CMD12                    12                   /* Stop data transmit */
#define CMD16                    16                  /* Set block size, should return 0x00 */
#define CMD17                    17                  /* Read single block */
#define CMD18                    18                 /* Read multi block */
#define ACMD23                   23           /* Prepare erase N-blokcs before multi block write */
#define CMD24                    24              /* Write single block */
#define CMD25                    25              /* Write multi block */
#define ACMD41                   41             /* should return 0x00 */
#define CMD55                    55               /* should return 0x01 */
#define CMD58                    58              /* Read OCR */
#define CMD59                    59               /* CRC disable/enbale, should return 0x00 */
#endif
