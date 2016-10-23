/***************************************************************************
* File Name: DMA_ADC_Input_Signal_dma.c  
* Version 1.50
*
*  Description:
*   Provides an API for the DMAC component. The API includes functions
*   for the DMA controller, DMA channels and Transfer Descriptors.
*
*
*   Note:
*     This module requires the developer to finish or fill in the auto
*     generated funcions and setup the dma channel and TD's.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include <CYLIB.H>
#include <CYDMAC.H>
#include <DMA_ADC_Input_Signal_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* DMA_ADC_Input_Signal__DRQ_CTL_REG
* 
* 
* DMA_ADC_Input_Signal__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* DMA_ADC_Input_Signal__NUMBEROF_TDS
* 
* Priority of this channel.
* DMA_ADC_Input_Signal__PRIORITY
* 
* True if DMA_ADC_Input_Signal_TERMIN_SEL is used.
* DMA_ADC_Input_Signal__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* DMA_ADC_Input_Signal__TERMIN_SEL
* 
* 
* True if DMA_ADC_Input_Signal_TERMOUT0_SEL is used.
* DMA_ADC_Input_Signal__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* DMA_ADC_Input_Signal__TERMOUT0_SEL
* 
* 
* True if DMA_ADC_Input_Signal_TERMOUT1_SEL is used.
* DMA_ADC_Input_Signal__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* DMA_ADC_Input_Signal__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of DMA_ADC_Input_Signal dma channel */
uint8 DMA_ADC_Input_Signal_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 DMA_ADC_Input_Signal_DmaInitalize
**********************************************************************
* Summary:
*   Allocates and initialises a channel of the DMAC to be used by the
*   caller.
*
* Parameters:
*   BurstCount.
*       
*       
*   ReqestPerBurst.
*       
*       
*   UpperSrcAddress.
*       
*       
*   UpperDestAddress.
*       
*
* Return:
*   The channel that can be used by the caller for DMA activity.
*   DMA_INVALID_CHANNEL (0xFF) if there are no channels left. 
*
*
*******************************************************************/
uint8 DMA_ADC_Input_Signal_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress)
{

    /* Allocate a DMA channel. */
    DMA_ADC_Input_Signal_DmaHandle = DMA_ADC_Input_Signal__DRQ_NUMBER;

    if(DMA_ADC_Input_Signal_DmaHandle != DMA_INVALID_CHANNEL)
    {
        /* Configure the channel. */
        CyDmaChSetConfiguration(DMA_ADC_Input_Signal_DmaHandle,
                                BurstCount,
                                ReqestPerBurst,
                                DMA_ADC_Input_Signal__TERMOUT0_SEL,
                                DMA_ADC_Input_Signal__TERMOUT1_SEL,
                                DMA_ADC_Input_Signal__TERMIN_SEL);

        /* Set the extended address for the transfers */
        CyDmaChSetExtendedAddress(DMA_ADC_Input_Signal_DmaHandle, UpperSrcAddress, UpperDestAddress);

        /* Set the priority for this channel */
        CyDmaChPriority(DMA_ADC_Input_Signal_DmaHandle, DMA_ADC_Input_Signal__PRIORITY);
    }

    return DMA_ADC_Input_Signal_DmaHandle;
}

/*********************************************************************
* Function Name: void DMA_ADC_Input_Signal_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with DMA_ADC_Input_Signal.
*
*
* Parameters:
*   void.
*
*
*
* Return:
*   void.
*
*******************************************************************/
void DMA_ADC_Input_Signal_DmaRelease(void) 
{
    /* Disable the channel, even if someone just did! */
    CyDmaChDisable(DMA_ADC_Input_Signal_DmaHandle);


    /* Free Transfer Descriptors. */


}

