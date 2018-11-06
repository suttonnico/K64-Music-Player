/*
 * SDHC.h
 *
 *  Created on: 13/2/2018
 *      Author: Nicolas
 */

#ifndef SOURCES_SDHC_H_
#define SOURCES_SDHC_H_

#include "Template/LDM.h"

#include "CMSIS/MK64F12.h"

#define SDHC_STATUS_NOINIT              0x01    /* Drive not initialized */
#define SDHC_STATUS_NODISK              0x02    /* No medium in the drive */
#define SDHC_STATUS_PROTECT             0x04    /* Write protected */


#define IO_SDHC_ATTRIBS (IO_DEV_ATTR_READ | IO_DEV_ATTR_REMOVE | IO_DEV_ATTR_SEEK | IO_DEV_ATTR_WRITE | IO_DEV_ATTR_BLOCK_MODE)

#define SDHC_XFERTYP_RSPTYP_NO              (0x00)
#define SDHC_XFERTYP_RSPTYP_136             (0x01)
#define SDHC_XFERTYP_RSPTYP_48              (0x02)
#define SDHC_XFERTYP_RSPTYP_48BUSY          (0x03)

#define SDHC_XFERTYP_CMDTYP_ABORT           (0x03)

#define SDHC_PROCTL_EMODE_INVARIANT         (0x02)

#define SDHC_PROCTL_DTW_1BIT                (0x00)
#define SDHC_PROCTL_DTW_4BIT                (0x01)
#define SDHC_PROCTL_DTW_8BIT                (0x10)

#define SDHC_INITIALIZATION_MAX_CNT 100000

/* SDHC commands */
#define SDHC_CMD0                           (0)
#define SDHC_CMD1                           (1)
#define SDHC_CMD2                           (2)
#define SDHC_CMD3                           (3)
#define SDHC_CMD4                           (4)
#define SDHC_CMD5                           (5)
#define SDHC_CMD6                           (6)
#define SDHC_CMD7                           (7)
#define SDHC_CMD8                           (8)
#define SDHC_CMD9                           (9)
#define SDHC_CMD10                          (10)
#define SDHC_CMD11                          (11)
#define SDHC_CMD12                          (12)
#define SDHC_CMD13                          (13)
#define SDHC_CMD15                          (15)
#define SDHC_CMD16                          (16)
#define SDHC_CMD17                          (17)
#define SDHC_CMD18                          (18)
#define SDHC_CMD20                          (20)
#define SDHC_CMD24                          (24)
#define SDHC_CMD25                          (25)
#define SDHC_CMD26                          (26)
#define SDHC_CMD27                          (27)
#define SDHC_CMD28                          (28)
#define SDHC_CMD29                          (29)
#define SDHC_CMD30                          (30)
#define SDHC_CMD32                          (32)
#define SDHC_CMD33                          (33)
#define SDHC_CMD34                          (34)
#define SDHC_CMD35                          (35)
#define SDHC_CMD36                          (36)
#define SDHC_CMD37                          (37)
#define SDHC_CMD38                          (38)
#define SDHC_CMD39                          (39)
#define SDHC_CMD40                          (40)
#define SDHC_CMD42                          (42)
#define SDHC_CMD52                          (52)
#define SDHC_CMD53                          (53)
#define SDHC_CMD55                          (55)
#define SDHC_CMD56                          (56)
#define SDHC_CMD60                          (60)
#define SDHC_CMD61                          (61)
#define SDHC_ACMD6                          (0x40 + 6)
#define SDHC_ACMD13                         (0x40 + 13)
#define SDHC_ACMD22                         (0x40 + 22)
#define SDHC_ACMD23                         (0x40 + 23)
#define SDHC_ACMD41                         (0x40 + 41)
#define SDHC_ACMD42                         (0x40 + 42)
#define SDHC_ACMD51                         (0x40 + 51)

#define SDHC_FIFO_BUFFER_SIZE               16
#define SDHC_BLOCK_SIZE                     512

#define SDHC_SYSCTL_DIVISOR(prescale, divisor) \
	(SDHC_SYSCTL_SDCLKFS((prescale)>>1)|SDHC_SYSCTL_DVS((divisor)-1))



#define SDHC_SYSCTL_400KHZ  SDHC_SYSCTL_DIVISOR(8, 15)  // 400 kHz
#define SDHC_SYSCTL_25MHZ   SDHC_SYSCTL_DIVISOR(2, 1)   // 24 MHz
#define SDHC_SYSCTL_50MHZ   SDHC_SYSCTL_DIVISOR(2, 1)   // 24 MHz

typedef struct {
	uint8_t  status;
	uint8_t  highCapacity;
	uint8_t  version2;
	uint8_t  tranSpeed;
	uint32_t address;
	uint32_t numBlocks;
	uint32_t lastCardStatus;
} SD_CARD_DESCRIPTOR;


enum {
        SDHC_RESULT_OK = 0,             /* 0: Successful */
        SDHC_RESULT_ERROR,              /* 1: R/W Error */
        SDHC_RESULT_WRPRT,              /* 2: Write Protected */
        SDHC_RESULT_NOT_READY,          /* 3: Not Ready */
        SDHC_RESULT_PARERR,             /* 4: Invalid Parameter */
        SDHC_RESULT_NO_RESPONSE         /* 5: No Response */ // from old diskio.h
};

void SDHC_InitGPIO(void);
uint8_t SDHC_Init(void);
 void SDHC_ReleaseGPIO(void);
void SDHC_SetClock(uint32_t sysctl);
uint8_t SDHC_InitCard(void);
int SDHC_Send_CMD(uint32_t xfertyp);
int SDHC_Send_CMD0(void);
int SDHC_Send_CMD2(void);
int SDHC_Send_CMD3(void);
int SDHC_Send_CMD7(uint32_t address);
int SDHC_Send_CMD8(uint32_t cond);
int SDHC_Send_CMD9(uint32_t address);
uint32_t SDHC_WaitStatus(uint32_t mask);
int SDHC_Send_CMD16(uint32_t block_size);
int SDHC_Send_ACMD6(uint32_t address, uint32_t width);
int SDHC_ReadBlock(uint32_t* pData);
int SDHC_Send_ACMD41(uint32_t cond);
int SDHC_Send_CMD12(void);
int SDHC_Send_CMD17(uint32_t sector);
int SDHC_Read_Sector(uint32_t sector,uint32_t* pData);

#endif /* SOURCES_SDHC_H_ */
