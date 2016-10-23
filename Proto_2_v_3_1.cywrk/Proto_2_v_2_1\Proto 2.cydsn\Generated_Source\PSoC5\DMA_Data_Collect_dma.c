/***************************************************************************
* File Name: DMA_Data_Collect_dma.c  
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
#include <DMA_Data_Collect_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* DMA_Data_Collect__DRQ_CTL_REG
* 
* 
* DMA_Data_Collect__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* DMA_Data_Collect__NUMBEROF_TDS
* 
* Priority of this channel.
* DMA_Data_Collect__PRIORITY
* 
* True if DMA_Data_Collect_TERMIN_SEL is used.
* DMA_Data_Collect__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* DMA_Data_Collect__TERMIN_SEL
* 
* 
* True if DMA_Data_Collect_TERMOUT0_SEL is used.
* DMA_Data_Collect__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* DMA_Data_Collect__TERMOUT0_SEL
* 
* 
* True if DMA_Data_Collect_TERMOUT1_SEL is used.
* DMA_Data_Collect__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* DMA_Data_Collect__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of DMA_Data_Collect dma channel */
uint8 DMA_Data_Collect_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 DMA_Data_Collect_DmaInitalize
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
uint8 DMA_Data_Collect_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress)
{

    /* Allocate a DMA channel. */
    DMA_Data_Collect_DmaHandle = DMA_Data_Collect__DRQ_NUMBER;

    if(DMA_Data_Collect_DmaHandle != DMA_INVALID_CHANNEL)
    {
        /* Configure the channel. */
        CyDmaChSetConfiguration(DMA_Data_Collect_DmaHandle,
                                BurstCount,
                                ReqestPerBurst,
                                DMA_Data_Collect__TERMOUT0_SEL,
                                DMA_Data_Collect__TERMOUT1_SEL,
                                DMA_Data_Collect__TERMIN_SEL);

        /* Set the extended address for the transfers */
        CyDmaChSetExtendedAddress(DMA_Data_Collect_DmaHandle, UpperSrcAddress, UpperDestAddress);

        /* Set the priority for this channel */
        CyDmaChPriority(DMA_Data_Collect_DmaHandle, DMA_Data_Collect__PRIORITY);
    }

    return DMA_Data_Collect_DmaHandle;
}

/*********************************************************************
* Function Name: void DMA_Data_Collect_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with DMA_Data_Collect.
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
void DMA_Data_Collect_DmaRelease(void) 
{
    /* Disable the channel, even if someone just did! */
    CyDmaChDisable(DMA_Data_Collect_DmaHandle);


    /* Free Transfer Descriptors. */


}

