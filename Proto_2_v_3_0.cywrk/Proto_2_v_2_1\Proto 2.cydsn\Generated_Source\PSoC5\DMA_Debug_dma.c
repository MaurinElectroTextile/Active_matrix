/***************************************************************************
* File Name: DMA_Debug_dma.c  
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
#include <DMA_Debug_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* DMA_Debug__DRQ_CTL_REG
* 
* 
* DMA_Debug__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* DMA_Debug__NUMBEROF_TDS
* 
* Priority of this channel.
* DMA_Debug__PRIORITY
* 
* True if DMA_Debug_TERMIN_SEL is used.
* DMA_Debug__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* DMA_Debug__TERMIN_SEL
* 
* 
* True if DMA_Debug_TERMOUT0_SEL is used.
* DMA_Debug__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* DMA_Debug__TERMOUT0_SEL
* 
* 
* True if DMA_Debug_TERMOUT1_SEL is used.
* DMA_Debug__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* DMA_Debug__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of DMA_Debug dma channel */
uint8 DMA_Debug_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 DMA_Debug_DmaInitalize
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
uint8 DMA_Debug_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress)
{

    /* Allocate a DMA channel. */
    DMA_Debug_DmaHandle = DMA_Debug__DRQ_NUMBER;

    if(DMA_Debug_DmaHandle != DMA_INVALID_CHANNEL)
    {
        /* Configure the channel. */
        CyDmaChSetConfiguration(DMA_Debug_DmaHandle,
                                BurstCount,
                                ReqestPerBurst,
                                DMA_Debug__TERMOUT0_SEL,
                                DMA_Debug__TERMOUT1_SEL,
                                DMA_Debug__TERMIN_SEL);

        /* Set the extended address for the transfers */
        CyDmaChSetExtendedAddress(DMA_Debug_DmaHandle, UpperSrcAddress, UpperDestAddress);

        /* Set the priority for this channel */
        CyDmaChPriority(DMA_Debug_DmaHandle, DMA_Debug__PRIORITY);
    }

    return DMA_Debug_DmaHandle;
}

/*********************************************************************
* Function Name: void DMA_Debug_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with DMA_Debug.
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
void DMA_Debug_DmaRelease(void) 
{
    /* Disable the channel, even if someone just did! */
    CyDmaChDisable(DMA_Debug_DmaHandle);


    /* Free Transfer Descriptors. */


}

