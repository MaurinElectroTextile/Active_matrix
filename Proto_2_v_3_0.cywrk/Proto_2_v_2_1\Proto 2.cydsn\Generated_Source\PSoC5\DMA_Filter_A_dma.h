/******************************************************************************
* File Name: DMA_Filter_A_dma.h  
* Version 1.50
*
*  Description:
*   Provides the function definitions for the DMA Controller.
*
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#if !defined(__DMA_Filter_A_DMA_H__)
#define __DMA_Filter_A_DMA_H__



#include <CYDMAC.H>
#include <CYFITTER.H>

#define DMA_Filter_A__TD_TERMOUT_EN ((DMA_Filter_A__TERMOUT0_EN ? TD_TERMOUT0_EN : 0) | \
    (DMA_Filter_A__TERMOUT1_EN ? TD_TERMOUT1_EN : 0))

/* Zero based index of DMA_Filter_A dma channel */
extern uint8 DMA_Filter_A_DmaHandle;


uint8 DMA_Filter_A_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress);
void  DMA_Filter_A_DmaRelease(void) ;


/* __DMA_Filter_A_DMA_H__ */
#endif
