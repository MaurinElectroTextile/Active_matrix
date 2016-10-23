/***************************************************************************
* File Name: DMA_Rectifier_dma.c  
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
#include <DMA_Rectifier_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* DMA_Rectifier__DRQ_CTL_REG
* 
* 
* DMA_Rectifier__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* DMA_Rectifier__NUMBEROF_TDS
* 
* Priority of this channel.
* DMA_Rectifier__PRIORITY
* 
* True if DMA_Rectifier_TERMIN_SEL is used.
* DMA_Rectifier__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* DMA_Rectifier__TERMIN_SEL
* 
* 
* True if DMA_Rectifier_TERMOUT0_SEL is used.
* DMA_Rectifier__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* DMA_Rectifier__TERMOUT0_SEL
* 
* 
* True if DMA_Rectifier_TERMOUT1_SEL is used.
* DMA_Rectifier__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* DMA_Rectifier__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of DMA_Rectifier dma channel */
uint8 DMA_Rectifier_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 DMA_Rectifier_DmaInitalize
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
uint8 DMA_Rectifier_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress)
{

    /* Allocate a DMA channel. */
    DMA_Rectifier_DmaHandle = DMA_Rectifier__DRQ_NUMBER;

    if(DMA_Rectifier_DmaHandle != DMA_INVALID_CHANNEL)
    {
        /* Configure the channel. */
        CyDmaChSetConfiguration(DMA_Rectifier_DmaHandle,
                                BurstCount,
                                ReqestPerBurst,
                                DMA_Rectifier__TERMOUT0_SEL,
                                DMA_Rectifier__TERMOUT1_SEL,
                                DMA_Rectifier__TERMIN_SEL);

        /* Set the extended address for the transfers */
        CyDmaChSetExtendedAddress(DMA_Rectifier_DmaHandle, UpperSrcAddress, UpperDestAddress);

        /* Set the priority for this channel */
        CyDmaChPriority(DMA_Rectifier_DmaHandle, DMA_Rectifier__PRIORITY);
    }

    return DMA_Rectifier_DmaHandle;
}

/*********************************************************************
* Function Name: void DMA_Rectifier_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with DMA_Rectifier.
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
void DMA_Rectifier_DmaRelease(void) 
{
    /* Disable the channel, even if someone just did! */
    CyDmaChDisable(DMA_Rectifier_DmaHandle);


    /* Free Transfer Descriptors. */


}

