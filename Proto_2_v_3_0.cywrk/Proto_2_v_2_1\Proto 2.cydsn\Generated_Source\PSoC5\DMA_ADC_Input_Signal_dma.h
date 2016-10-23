/******************************************************************************
* File Name: DMA_ADC_Input_Signal_dma.h  
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
#if !defined(__DMA_ADC_Input_Signal_DMA_H__)
#define __DMA_ADC_Input_Signal_DMA_H__



#include <CYDMAC.H>
#include <CYFITTER.H>

#define DMA_ADC_Input_Signal__TD_TERMOUT_EN ((DMA_ADC_Input_Signal__TERMOUT0_EN ? TD_TERMOUT0_EN : 0) | \
    (DMA_ADC_Input_Signal__TERMOUT1_EN ? TD_TERMOUT1_EN : 0))

/* Zero based index of DMA_ADC_Input_Signal dma channel */
extern uint8 DMA_ADC_Input_Signal_DmaHandle;


uint8 DMA_ADC_Input_Signal_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress);
void  DMA_ADC_Input_Signal_DmaRelease(void) ;


/* __DMA_ADC_Input_Signal_DMA_H__ */
#endif
