/******************************************************************************
* File Name: DMA_Rectifier_dma.h  
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
#if !defined(__DMA_Rectifier_DMA_H__)
#define __DMA_Rectifier_DMA_H__



#include <CYDMAC.H>
#include <CYFITTER.H>

#define DMA_Rectifier__TD_TERMOUT_EN ((DMA_Rectifier__TERMOUT0_EN ? TD_TERMOUT0_EN : 0) | \
    (DMA_Rectifier__TERMOUT1_EN ? TD_TERMOUT1_EN : 0))

/* Zero based index of DMA_Rectifier dma channel */
extern uint8 DMA_Rectifier_DmaHandle;


uint8 DMA_Rectifier_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress);
void  DMA_Rectifier_DmaRelease(void) ;


/* __DMA_Rectifier_DMA_H__ */
#endif
