/*
 * SDHC.c
 *
 *  Created on: 13/2/2018
 *      Author: Nicolas
 */

#include "SDHC.h"
static CARD card;


uint8_t SDHC_Init(void)
{

	int i;

	// Enable clocks
	SIM_SCGC3 |= SIM_SCGC3_SDHC_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

	// Enable DMA access via MPU (not currently used)
	//MPU_CESR &= ~MPU_CESR_VLD_MASK;

	// De-init GPIO - to prevent unwanted clocks on bus
	SDHC_ReleaseGPIO();

	/* Reset SDHC */
	SDHC_SYSCTL = SDHC_SYSCTL_RSTA_MASK | SDHC_SYSCTL_SDCLKFS(0x80);
	while (SDHC_SYSCTL & SDHC_SYSCTL_RSTA_MASK) ; // wait

	/* Initial values */ // to do - Check values
	SDHC_VENDOR = 0;
	SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1) | SDHC_BLKATTR_BLKSIZE(SDHC_BLOCK_SIZE);
	SDHC_PROCTL = SDHC_PROCTL_EMODE(SDHC_PROCTL_EMODE_INVARIANT) | SDHC_PROCTL_D3CD_MASK;
	SDHC_WML = SDHC_WML_RDWML(SDHC_FIFO_BUFFER_SIZE) | SDHC_WML_WRWML(SDHC_FIFO_BUFFER_SIZE);

	/* Set the SDHC initial baud rate divider and start */
	//SDHC_SetBaudrate(400);
	SDHC_SetClock(SDHC_SYSCTL_400KHZ);

	/* Poll inhibit bits */
	while (SDHC_PRSSTAT & (SDHC_PRSSTAT_CIHB_MASK | SDHC_PRSSTAT_CDIHB_MASK)) { };

	/* Init GPIO again */
	SDHC_ConfigGPIO();

	/* Enable requests */
	SDHC_IRQSTAT = 0xFFFF;
	SDHC_IRQSTATEN = SDHC_IRQSTATEN_DMAESEN_MASK | SDHC_IRQSTATEN_AC12ESEN_MASK | SDHC_IRQSTATEN_DEBESEN_MASK |SDHC_IRQSTATEN_DCESEN_MASK | SDHC_IRQSTATEN_DTOESEN_MASK | SDHC_IRQSTATEN_CIESEN_MASK |SDHC_IRQSTATEN_CEBESEN_MASK | SDHC_IRQSTATEN_CCESEN_MASK | SDHC_IRQSTATEN_CTOESEN_MASK |SDHC_IRQSTATEN_BRRSEN_MASK | SDHC_IRQSTATEN_BWRSEN_MASK | SDHC_IRQSTATEN_DINTSEN_MASK |SDHC_IRQSTATEN_CRMSEN_MASK | SDHC_IRQSTATEN_TCSEN_MASK | SDHC_IRQSTATEN_CCSEN_MASK;

	// initial clocks... SD spec says only 74 clocks are needed, but if Teensy rebooted
	// while the card was in middle of an operation, thousands of clock cycles can be
	// needed to get the card to complete a prior command and return to a usable state.
	for (i=0; i < 500; i++) {
		SDHC_SYSCTL |= SDHC_SYSCTL_INITA_MASK;
		while (SDHC_SYSCTL & SDHC_SYSCTL_INITA_MASK) { };
	}

	// to do - check if this needed
	SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;

	// Check card
	if (SDHC_PRSSTAT & SDHC_PRSSTAT_CINS_MASK) {
		return 0;
	} else {
		return SDHC_STATUS_NODISK;
	}
}


void SDHC_ConfigGPIO(void)
{
  PORTE_PCR0 = PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK; /* SDHC.D1  */
  PORTE_PCR1 = PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK; /* SDHC.D0  */
  PORTE_PCR2 = PORT_PCR_MUX(4) | PORT_PCR_DSE_MASK;                             /* SDHC.CLK */
  PORTE_PCR3 = PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK; /* SDHC.CMD */
  PORTE_PCR4 = PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK; /* SDHC.D3  */
  PORTE_PCR5 = PORT_PCR_MUX(4) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK; /* SDHC.D2  */
}

// release the SDHC Controller signals
void SDHC_ReleaseGPIO(void)
{
  PORTE_PCR0 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 	/* PULLUP SDHC.D1  */
  PORTE_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 	/* PULLUP SDHC.D0  */
  PORTE_PCR2 = 0;						/* SDHC.CLK */
  PORTE_PCR3 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 	/* PULLUP SDHC.CMD */
  PORTE_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;	/* PULLUP SDHC.D3  */
  PORTE_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 	/* PULLUP SDHC.D2  */
}



void SDHC_SetClock(uint32_t sysctl)
{
	uint32_t n, timeout;

	n = SDHC_SYSCTL;
	// Disable SDHC clocks
	n &= ~SDHC_SYSCTL_SDCLKEN_MASK;
	SDHC_SYSCTL = n;
	// Change dividers
	n &= ~(SDHC_SYSCTL_DTOCV_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DVS_MASK);
	n |= sysctl | SDHC_SYSCTL_DTOCV(0x0E);
	SDHC_SYSCTL = n;
	/* Wait for stable clock */
	timeout = 0xFFFFF;
	while ((!(SDHC_PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK)) && timeout) {
		timeout--;
	}
	/* Enable SDHC clocks */
	SDHC_SYSCTL = n | SDHC_SYSCTL_SDCLKEN_MASK;
	SDHC_IRQSTAT |= SDHC_IRQSTAT_DTOE_MASK;
}


uint8_t SDHC_InitCard(void)
{
  uint8_t resS;
  int resR;
  uint32_t i;

  resS = SDHC_Init();

  card.status = resS;
  card.address = 0;
  card.highCapacity = 0;
  card.version2 = 0;
  card.numBlocks = 0;

  if (resS)
    return resS;

  resR = SDHC_Send_CMD(CMD0,0);
  if (resR) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  resR = SDHC_Send_CMD(CMD8,0x000001AA); // 3.3V and AA check pattern
  if (resR == SDHC_RESULT_OK) {
      if (SDHC_CMDRSP0 != 0x000001AA) {
        card.status = SDHC_STATUS_NOINIT;
        return SDHC_STATUS_NOINIT;
      }
      card.highCapacity = 1;
  } else if (resR == SDHC_RESULT_NO_RESPONSE) {
      // version 1 cards do not respond to CMD8
  } else {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }
  SDHC_Send_CMD(CMD55,0);
  if (SDHC_Send_CMD(ACMD41,0)) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  if (SDHC_CMDRSP0 & 0x300000) {
    uint32_t condition = 0x00300000;
    if (card.highCapacity)
            condition |= 0x40000000;
    i = 0;
    do {
        i++;
        SDHC_Send_CMD(CMD55,0);
        if (SDHC_Send_CMD(ACMD41,condition)) {
          resS = SDHC_STATUS_NOINIT;
          break;
        }
    } while ((!(SDHC_CMDRSP0 & 0x80000000)) && (i < SDHC_INITIALIZATION_MAX_CNT));


    if (resS)
      return resS;

    if ((i >= SDHC_INITIALIZATION_MAX_CNT) || (!(SDHC_CMDRSP0 & 0x40000000)))
        card.highCapacity = 0;
  }

  // Card identify
  if(SDHC_Send_CMD(CMD2,0)) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  // Get card address
  if(SDHC_Send_CMD(CMD3,0)) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  card.address = SDHC_CMDRSP0 & 0xFFFF0000;

  // Get card parameters
  if(SDHC_Send_CMD(CMD9,card.address)) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  if (!(SDHC_CMDRSP3 & 0x00C00000)) {
    uint32_t read_bl_len, c_size, c_size_mult;

    read_bl_len = (SDHC_CMDRSP2 >> 8) & 0x0F;
    c_size = SDHC_CMDRSP2 & 0x03;
    c_size = (c_size << 10) | (SDHC_CMDRSP1 >> 22);
    c_size_mult = (SDHC_CMDRSP1 >> 7) & 0x07;
    card.numBlocks = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << (read_bl_len - 9));
  } else {
    uint32_t c_size;
    card.version2 = 1;
    c_size = (SDHC_CMDRSP1 >> 8) & 0x003FFFFF;
    card.numBlocks = (c_size + 1) << 10;
  }

  // Select card
  if (SDHC_Send_CMD(CMD7,card.address)) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  // Set Block Size to 512
  // Block Size in SDHC Controller is already set to 512 by SDHC_Init();
  // Set 512 Block size in SD card
  if (SDHC_Send_CMD(CMD16,SDHC_BLOCK_SIZE)) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  // Set 4 bit data bus width
  SDHC_Send_CMD(CMD55,card.address);
  if (SDHC_Send_CMD(ACMD6,2)) {
    card.status = SDHC_STATUS_NOINIT;
    return SDHC_STATUS_NOINIT;
  }

  // Set Data bus width also in SDHC controller
  SDHC_PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
  SDHC_PROCTL |= SDHC_PROCTL_DTW(SDHC_PROCTL_DTW_4BIT);

  // De-Init GPIO
  SDHC_ReleaseGPIO();

  // Set the SDHC default baud rate
  SDHC_SetClock(SDHC_SYSCTL_50MHZ);

  // Init GPIO
  SDHC_ConfigGPIO();

  return card.status;
}

// sends the command to SDcard
int SDHC_Send_CMD(uint32_t CMD,uint32_t arg)
{
	SDHC_CMDARG = arg;

    // Card removal check preparation
    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;

    // Wait for cmd line idle // to do timeout PRSSTAT[CDIHB] and the PRSSTAT[CIHB]
    while ((SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB_MASK) || (SDHC_PRSSTAT & SDHC_PRSSTAT_CDIHB_MASK))
        { };

    SDHC_XFERTYP = CMD;

    /* Wait for response */
    if (SDHC_WaitStatus(SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK) {
	SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK |
						SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
        return SDHC_RESULT_ERROR;
    }

    /* Check card removal */
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK) {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return SDHC_RESULT_NOT_READY;
    }

    /* Get response, if available */
    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK) {
        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return SDHC_RESULT_NO_RESPONSE;
    }

    SDHC_IRQSTAT |= SDHC_IRQSTAT_CC_MASK;

    (void)SDHC_CMDRSP0;
    return SDHC_RESULT_OK;

}



//// sends the command to SDcard
//int SDHC_Send_CMD(uint32_t xfertyp)
//{
//    // Card removal check preparation
//    SDHC_IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
//
//    // Wait for cmd line idle // to do timeout PRSSTAT[CDIHB] and the PRSSTAT[CIHB]
//    while ((SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB_MASK) || (SDHC_PRSSTAT & SDHC_PRSSTAT_CDIHB_MASK))
//        { };
//
//    SDHC_XFERTYP = xfertyp;
//
//    /* Wait for response */
//    if (SDHC_WaitStatus(SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK) {
//	SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK |
//						SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
//        return SDHC_RESULT_ERROR;
//    }
//
//    /* Check card removal */
//    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CRM_MASK) {
//        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
//        return SDHC_RESULT_NOT_READY;
//    }
//
//    /* Get response, if available */
//    if (SDHC_IRQSTAT & SDHC_IRQSTAT_CTOE_MASK) {
//        SDHC_IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
//        return SDHC_RESULT_NO_RESPONSE;
//    }
//
//    SDHC_IRQSTAT |= SDHC_IRQSTAT_CC_MASK;
//
//    return SDHC_RESULT_OK;
//
//}






uint32_t SDHC_WaitStatus(uint32_t mask)
{
    uint32_t             result;
    uint32_t             timeout = 1<<24;
    do {
        result = SDHC_IRQSTAT & mask;
        timeout--;
    }
    while(!result && (timeout));

    if (timeout)
      return result;

    return 0;
}




int SDHC_ReadBlock(uint32_t* pData)
{
	int j;
	uint32_t i, irqstat;
	const uint32_t i_max = ((SDHC_BLOCK_SIZE) / (4 * SDHC_FIFO_BUFFER_SIZE));

	for (i = 0; i < i_max; i++) {
		irqstat = SDHC_IRQSTAT;
		SDHC_IRQSTAT = irqstat | SDHC_IRQSTAT_BRR_MASK;
		if (irqstat & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK)) {
			SDHC_IRQSTAT = irqstat | SDHC_IRQSTAT_BRR_MASK |
				SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK;
			SDHC_Send_CMD(CMD12,0);
			return SDHC_RESULT_ERROR;
		}
		while (!(SDHC_PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) { };

		*pData++ = SDHC_DATPORT;
		if(*pData!=0)
		{
			__asm("NOP");
		}
		for(j=0;j<15;j++)
			*pData++ = SDHC_DATPORT;

	}
	return SDHC_RESULT_OK;
}



int SDHC_Read_Sector(uint32_t sector,uint32_t* pData)
{
	int result = 0;
	SDHC_Send_CMD(CMD17,sector);
	result = SDHC_ReadBlock(pData);
	SDHC -> SYSCTL |= SDHC_SYSCTL_RSTC_MASK;
	return result;
}
