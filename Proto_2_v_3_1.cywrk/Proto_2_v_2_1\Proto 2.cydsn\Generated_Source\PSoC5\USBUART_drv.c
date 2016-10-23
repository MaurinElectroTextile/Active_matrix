/*******************************************************************************
* File Name: USBUART_drv.c
* Version 0.0
*
*  Description:
*    Endpoint 0 Driver for the USBFS Component.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/



#include "cydevice.h"
#include "cyfitter.h"
#include "USBUART.h"

/*******************************************************************************
* Forward references for the EP0 ISR
********************************************************************************/
void  USBUART_HandleSetup(void);
void  USBUART_HandleIN(void);
void  USBUART_HandleOUT(void);
uint8 USBUART_InitControlRead(void);
void  USBUART_ControlReadDataStage(void);
void  USBUART_ControlReadStatusStage(void);
void  USBUART_ControlReadPrematureStatus(void);
uint8 USBUART_InitControlWrite(void);
void  USBUART_ControlWriteDataStage(void);
void  USBUART_ControlWriteStatusStage(void);
void  USBUART_ControlWritePrematureStatus(void);
uint8 USBUART_InitNoDataControlTransfer(void);
void  USBUART_NoDataControlStatusStage(void);
void USBUART_InitializeStatusBlock(void);
void USBUART_UpdateStatusBlock(uint8 bCompletionCode);

/*******************************************************************************
* Request Handlers
********************************************************************************/
uint8 USBUART_HandleStandardRqst(void);
uint8 USBUART_DispatchClassRqst(void);
uint8 USBUART_HandleVendorRqst(void);

/*******************************************************************************
* External data references
********************************************************************************/

/*******************************************************************************
* Global data allocation
********************************************************************************/
T_USBUART_EP_CTL_BLOCK USBUART_EP[9];
uint8 USBUART_bEPHalt;
uint8 USBUART_bConfiguration;
uint8 USBUART_bInterfaceSetting[9];
uint8 USBUART_bDeviceAddress;
uint8 USBUART_bDeviceStatus;
uint8 USBUART_bDevice;
/*******************************************************************************
* Local data allocation
********************************************************************************/
uint8 USBUART_bEP0Toggle;
uint8 USBUART_bLastPacketSize;
uint8 USBUART_bTransferState;
T_USBUART_TD CurrentTD;
uint8 USBUART_bEP0Mode;
uint8 USBUART_bEP0Count;
uint16 USBUART_TransferByteCount;

/*******************************************************************************
* Function Name: USBUART_ep_0_Interrupt
********************************************************************************
* Summary:
*   This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.  It
*   dispactches setup requests and handles the data and status stages.
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_0_ISR)
{
	uint8 bRegTemp = *USBUART_EP0_CR;
		
	if (!bRegTemp & USBUART_MODE_ACKD) return;             
	if (bRegTemp & USBUART_MODE_SETUP_RCVD) {
		USBUART_HandleSetup();
	} 
	else if (bRegTemp & USBUART_MODE_IN_RCVD) {
		USBUART_HandleIN();	
	}
	else if (bRegTemp & USBUART_MODE_OUT_RCVD) {
		USBUART_HandleOUT();
	}
	else {
//		ASSERT(0);
	}
	CY_SET_REG8(USBUART_EP0_CNT, USBUART_bEP0Toggle | USBUART_bEP0Count);
	/* Set the Mode Register  */
	CY_SET_REG8(USBUART_EP0_CR, USBUART_bEP0Mode);
	
   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_0__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	
	
}
/*******************************************************************************
* Function Name: USBUART_HandleSetup
********************************************************************************
* Summary:
*   This Routine dispatches requests for the four USB request types
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_HandleSetup(void)
{
	uint8 bRequestHandled;
    /* In case the previous transfer did not complete, close it out */
    USBUART_UpdateStatusBlock(USBUART_XFER_PREMATURE);
    
    switch (CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_TYPE_MASK)
	{
	case USBUART_RQST_TYPE_STD:
		bRequestHandled = USBUART_HandleStandardRqst();
		break;
	case USBUART_RQST_TYPE_CLS:
		bRequestHandled = USBUART_DispatchClassRqst();
		break;
	case USBUART_RQST_TYPE_VND:
		bRequestHandled = USBUART_HandleVendorRqst();
		break;
	default:
		bRequestHandled = USBUART_FALSE;
		break;
	}
	if (bRequestHandled == USBUART_FALSE){
		USBUART_bEP0Mode = USBUART_MODE_STALL_IN_OUT;
	}
}
/*******************************************************************************
* Function Name: USBUART_HandleIN
********************************************************************************
* Summary:
*   This routine handles EP0 IN transfers.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_HandleIN(void)
{
	switch (USBUART_bTransferState)
	{
	case USBUART_TRANS_STATE_IDLE:
		break;
	case USBUART_TRANS_STATE_CONTROL_READ:
		USBUART_ControlReadDataStage();
		break;
	case USBUART_TRANS_STATE_CONTROL_WRITE:
		USBUART_ControlReadStatusStage();
		break;
	case USBUART_TRANS_STATE_NO_DATA_CONTROL:
		USBUART_NoDataControlStatusStage();
		break;
	default:
		break;	
	}
}
/*******************************************************************************
* Function Name: USBUART_HandleOUT
********************************************************************************
* Summary:
*   This routine handles EP0 OUT transfers.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_HandleOUT(void)
{
	switch (USBUART_bTransferState)
	{
	case USBUART_TRANS_STATE_IDLE:
		break;
	case USBUART_TRANS_STATE_CONTROL_READ:
		USBUART_ControlReadStatusStage();
		break;
	case USBUART_TRANS_STATE_CONTROL_WRITE:
		USBUART_ControlWriteDataStage();
		break;
	case USBUART_TRANS_STATE_NO_DATA_CONTROL:
        /* Update the completion block */
        USBUART_UpdateStatusBlock(USBUART_XFER_ERROR);
        /* We expect no more data, so stall INs and OUTs */
        USBUART_bEP0Mode = USBUART_MODE_STALL_IN_OUT;
		break;
	default:
		break;	
	}
}
/*******************************************************************************
* Function Name: USBUART_LoadEP0
********************************************************************************
* Summary:
*   This routine loads the EP0 data registers for OUT transfers.  It uses the
*   CurrentTD (previously initialized by the _InitControlWrite function and
*   updated for each OUT transfer, and the bLastPacketSize) to determine how
*   many uint8s to transfer on the current OUT.
*
*   If the number of uint8s remaining is zero and the last transfer was full, 
*   we need to send a zero length packet.  Otherwise we send the minimum
*   of the control endpoint size (8) or remaining number of uint8s for the
*   transaction.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_LoadEP0(void)
{
	/* Update the transfer byte count from the last transaction */
    USBUART_TransferByteCount += USBUART_bLastPacketSize;
    /* Now load the next transaction */
    USBUART_bEP0Count = 0;
	while ((CurrentTD.wCount > 0) && (USBUART_bEP0Count < 8))
	{
        USBUART_EP0_DR0[USBUART_bEP0Count] = *CurrentTD.pData++;
		USBUART_bEP0Count++;
		CurrentTD.wCount--;
	}

	if ((USBUART_bEP0Count > 0) || (USBUART_bLastPacketSize == 8))
	{
		/* Update the data toggle */
		USBUART_bEP0Toggle ^= USBUART_EP0_CNT_DATA_TOGGLE;
		/* Set the Mode Register  */
		USBUART_bEP0Mode = USBUART_MODE_ACK_IN_STATUS_OUT;
		/* Update the state (or stay the same) */
		USBUART_bTransferState = USBUART_TRANS_STATE_CONTROL_READ;
	}
	else
	{
		/* Expect Status Stage Out */
		USBUART_bEP0Mode = USBUART_MODE_STATUS_OUT_ONLY;
		/* Update the state (or stay the same) */
		USBUART_bTransferState = USBUART_TRANS_STATE_CONTROL_READ;
	}

	/* Save the packet size for next time */
	USBUART_bLastPacketSize = USBUART_bEP0Count;
}
/*******************************************************************************
* Function Name: USBUART_InitControlRead
********************************************************************************
* Summary:
*   Initialize a control read transaction
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_InitControlRead(void)
{
    uint16 wXferCount;

    /* Set up the state machine */
    USBUART_bTransferState = USBUART_TRANS_STATE_CONTROL_READ;
    /* Set the toggle, it gets updated in LoadEP */
    USBUART_bEP0Toggle = 0;
    /* Initialize the Status Block */
    USBUART_InitializeStatusBlock();
    wXferCount = (((uint16)*USBUART_wLengthHi << 8) | (*USBUART_wLengthLo));

    if (CurrentTD.wCount > wXferCount)
    {
        CurrentTD.wCount = wXferCount;
    }
    USBUART_LoadEP0();

    return USBUART_TRUE;
}
/*******************************************************************************
* Function Name: USBUART_ControlReadDataStage
********************************************************************************
* Summary:
*   Handle the Data Stage of a control read transfer
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_ControlReadDataStage(void)
{
	USBUART_LoadEP0();
}
/*******************************************************************************
* Function Name: USBUART_ControlReadStatusStage
********************************************************************************
* Summary:
*   Handle the Status Stage of a control read transfer
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_ControlReadStatusStage(void)
{
	/* Update the transfer byte count */
    USBUART_TransferByteCount += USBUART_bLastPacketSize;
    /* Go Idle */
    USBUART_bTransferState = USBUART_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_UpdateStatusBlock(USBUART_XFER_STATUS_ACK);
 	/* We expect no more data, so stall INs and OUTs */
	USBUART_bEP0Mode = USBUART_MODE_STALL_IN_OUT;
}
/*******************************************************************************
* Function Name: USBUART_InitControlWrite
********************************************************************************
* Summary:
*   Initialize a control write transaction
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_InitControlWrite(void)
{
    uint16 wXferCount;
    
    /* Set up the state machine */
    USBUART_bTransferState = USBUART_TRANS_STATE_CONTROL_WRITE;
    /* This migh not be necessary */
    USBUART_bEP0Toggle = USBUART_EP0_CNT_DATA_TOGGLE;;
    /* Initialize the Status Block */
    USBUART_InitializeStatusBlock();

    wXferCount = (((uint16)CY_GET_REG8(USBUART_wLengthHi) << 8) | (CY_GET_REG8(USBUART_wLengthLo)));

    if (CurrentTD.wCount > wXferCount)
    {
        CurrentTD.wCount = wXferCount;
    }
    
	/* Expect Data or Status Stage */
	USBUART_bEP0Mode = USBUART_MODE_ACK_OUT_STATUS_IN;
    
    return USBUART_TRUE;
}
/*******************************************************************************
* Function Name: USBUART_ControlWriteDataStage
********************************************************************************
* Summary:
*   Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       3. Update the count and data toggle
*       2. Update the mode register for the next transaction
* Parameters:  
*   None
*******************************************************************************/
void USBUART_ControlWriteDataStage(void)
{
	uint8 *pReg = (uint8 *)USBUART_EP0_DR0; 
	USBUART_bEP0Count = (CY_GET_REG8(USBUART_EP0_CNT ) & 0x0F) - 2;
    
    USBUART_TransferByteCount += USBUART_bEP0Count;
    
    while ((CurrentTD.wCount > 0) && (USBUART_bEP0Count > 0))
	{
        *CurrentTD.pData++ = CY_GET_REG8(pReg++);
        USBUART_bEP0Count--;
		CurrentTD.wCount--;
	}
	/* Update the data toggle */
	USBUART_bEP0Toggle ^= USBUART_EP0_CNT_DATA_TOGGLE;
	/* Expect Data or Status Stage */
	USBUART_bEP0Mode = USBUART_MODE_ACK_OUT_STATUS_IN;
}
/*******************************************************************************
* Function Name: USBUART_ControlWriteStatusStage
********************************************************************************
* Summary:
*   Handle the Status Stage of a control write transfer
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_ControlWriteStatusStage(void)
{
	/* Go Idle */
    USBUART_bTransferState = USBUART_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_UpdateStatusBlock(USBUART_XFER_STATUS_ACK);
 	/* We expect no more data, so stall INs and OUTs */
	USBUART_bEP0Mode = USBUART_MODE_STALL_IN_OUT;
}
/*******************************************************************************
* Function Name: USBUART_InitNoDataControlTransfer
********************************************************************************
* Summary:
*   Initialize a no data control transfer
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_InitNoDataControlTransfer(void)
{
	USBUART_bTransferState = USBUART_TRANS_STATE_NO_DATA_CONTROL;
	USBUART_bEP0Mode = USBUART_MODE_STATUS_IN_ONLY;
	return USBUART_TRUE;
}
/*******************************************************************************
* Function Name: USBUART_NoDataControlStatusStage
********************************************************************************
* Summary:
*   Handle the Status Stage of a no data control transfer.
*
*   SET_ADDRESS is special, since we need to receive the status stage with
*   the old address.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_NoDataControlStatusStage(void)
{
	/* Change the USB address register if we got a SET_ADDRESS. */
    if (USBUART_bDeviceAddress != 0)
    {
        CY_SET_REG8(USBUART_CR0, USBUART_bDeviceAddress | USBUART_CR0_ENABLE);
        USBUART_bDeviceAddress = 0;
    }
    	/* Go Idle */
    USBUART_bTransferState = USBUART_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBUART_UpdateStatusBlock(USBUART_XFER_STATUS_ACK);
 	/* We expect no more data, so stall INs and OUTs */
	USBUART_bEP0Mode = USBUART_MODE_STALL_IN_OUT;
}
/*******************************************************************************
* Function Name: USBUART_UpdateStatusBlock
********************************************************************************
* Summary:
*   Update the Completion Status Block for a Request.  The block is updated
*   with the completion code the USBUART_TransferByteCount.  The
*   StatusBlock Pointer  is set to NULL.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_UpdateStatusBlock(uint8 bCompletionCode)
{
    if (CurrentTD.pStatusBlock != USBUART_NULL)
    {
        CurrentTD.pStatusBlock->bStatus = bCompletionCode;
        CurrentTD.pStatusBlock->wLength = USBUART_TransferByteCount;
        CurrentTD.pStatusBlock = USBUART_NULL;
    }
}
/*******************************************************************************
* Function Name: USBUART_InitializeStatusBlock
********************************************************************************
* Summary:
*   Initialize the Completion Status Block for a Request.  The completion
*   code is set to USB_XFER_IDLE.
*
*   Also, initializes USBUART_TransferByteCount.  Save some space,
*   this is the only consumer.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_InitializeStatusBlock(void)
{
    USBUART_TransferByteCount = 0;
    if (CurrentTD.pStatusBlock != USBUART_NULL)
    {
        CurrentTD.pStatusBlock->bStatus = USBUART_XFER_IDLE;
        CurrentTD.pStatusBlock->wLength = 0;
    }
}
