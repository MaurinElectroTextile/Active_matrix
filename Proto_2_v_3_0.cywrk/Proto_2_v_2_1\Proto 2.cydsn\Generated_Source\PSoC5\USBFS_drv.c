/*******************************************************************************
* File Name: USBFS_drv.c
* Version 1.50
*
* Description:
*  Endpoint 0 Driver for the USBFS Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "USBFS.h"
#include "CyLib.h"


/***************************************
* Forward references for the EP0 ISR
***************************************/

void  USBFS_HandleSetup(void);
void  USBFS_HandleIN(void);
void  USBFS_HandleOUT(void);
uint8 USBFS_InitControlRead(void);
void  USBFS_ControlReadDataStage(void);
void  USBFS_ControlReadStatusStage(void);
void  USBFS_ControlReadPrematureStatus(void);
uint8 USBFS_InitControlWrite(void);
uint8 USBFS_InitZeroLengthControlTransfer(void);
void  USBFS_ControlWriteDataStage(void);
void  USBFS_ControlWriteStatusStage(void);
void  USBFS_ControlWritePrematureStatus(void);
uint8 USBFS_InitNoDataControlTransfer(void);
void  USBFS_NoDataControlStatusStage(void);
void  USBFS_InitializeStatusBlock(void);
void  USBFS_UpdateStatusBlock(uint8 completionCode);


/***************************************
* Request Handlers
***************************************/

uint8 USBFS_HandleStandardRqst(void);
uint8 USBFS_DispatchClassRqst(void);
uint8 USBFS_HandleVendorRqst(void);


/***************************************
* External data references
***************************************/


/***************************************
* Global data allocation
***************************************/

volatile T_USBFS_EP_CTL_BLOCK USBFS_EP[USBFS_MAX_EP];
volatile uint8 USBFS_epHalt;          /* Not used, should be deleted in ver 2.0 */
volatile uint8 USBFS_configuration;
volatile uint8 USBFS_interfaceSetting[USBFS_MAX_INTERFACES_NUMBER];
volatile uint8 USBFS_deviceAddress;
volatile uint8 USBFS_deviceStatus;
volatile uint8 USBFS_device;


/***************************************
* Local data allocation
***************************************/

volatile uint8 USBFS_ep0Toggle;
volatile uint8 USBFS_lastPacketSize;
volatile uint8 USBFS_transferState;
volatile T_USBFS_TD currentTD;
volatile uint8 USBFS_ep0Mode;
volatile uint8 USBFS_ep0Count;
volatile uint16 USBFS_transferByteCount;


/*******************************************************************************
* Function Name: USBFS_ep_0_Interrupt
********************************************************************************
*
* Summary:
*  This Interrupt Service Routine handles Endpoint 0 (Control Pipe) traffic.
*  It dispactches setup requests and handles the data and status stages.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USBFS_EP_0_ISR)
{
    uint8 bRegTemp;
    uint8 enableInterrupts;

    bRegTemp = CY_GET_REG8(USBFS_EP0_CR_PTR);
    if (bRegTemp & USBFS_MODE_ACKD)
    {
        if (bRegTemp & USBFS_MODE_SETUP_RCVD)
        {
            USBFS_HandleSetup();
        }
        else if (bRegTemp & USBFS_MODE_IN_RCVD)
        {
            USBFS_HandleIN();
        }
        else if (bRegTemp & USBFS_MODE_OUT_RCVD)
        {
            USBFS_HandleOUT();
        }
        else
        {
            USBFS_ep0Mode = USBFS_MODE_STALL_IN_OUT;
        }
        enableInterrupts = CyEnterCriticalSection();
        CY_SET_REG8(USBFS_EP0_CNT_PTR, USBFS_ep0Toggle | USBFS_ep0Count);
        #if(USBFS_PSOC3_UPTO_ES3) /* Workaround */

            /* Disable SIE*/
            bRegTemp = CY_GET_REG8(USBFS_CR0_PTR ) & ~USBFS_CR0_ENABLE;
            CY_SET_REG8(USBFS_CR0_PTR, bRegTemp);
            /* disable require some time(~1us) for SIE to be disabled */ 
            CyDelayUs(1);
            /* Set the Mode Register  */
            CY_SET_REG8(USBFS_EP0_CR_PTR, USBFS_ep0Mode);
            /* Write/Read/Verify/overwrite mode register till success verification */
            while(CY_GET_REG8(USBFS_EP0_CR_PTR) != USBFS_ep0Mode)
            {
                CY_SET_REG8(USBFS_EP0_CR_PTR, USBFS_ep0Mode);
            }
            /* Enable SIE */
            CY_SET_REG8(USBFS_CR0_PTR , bRegTemp | USBFS_CR0_ENABLE);
        #else
            /* Set the Mode Register  */
            CY_SET_REG8(USBFS_EP0_CR_PTR, USBFS_ep0Mode);
        #endif /* End CYDEV_CHIP_DIE_EXPECT */
        CyExitCriticalSection(enableInterrupts);           
    }
    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(USBFS_PSOC3_ES2 && (USBFS_ep_0__ES2_PATCH))
        USBFS_ISR_PATCH();
    #endif /* End USBFS_PSOC3_ES2*/
}


/*******************************************************************************
* Function Name: USBFS_HandleSetup
********************************************************************************
*
* Summary:
*  This Routine dispatches requests for the four USB request types
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_HandleSetup(void)
{
    uint8 requestHandled = USBFS_FALSE;
    
    /* In case the previous transfer did not complete, close it out */
    USBFS_UpdateStatusBlock(USBFS_XFER_PREMATURE);

    switch (CY_GET_REG8(USBFS_bmRequestType) & USBFS_RQST_TYPE_MASK)
    {
        case USBFS_RQST_TYPE_STD:
            requestHandled = USBFS_HandleStandardRqst();
            break;
        case USBFS_RQST_TYPE_CLS:
            requestHandled = USBFS_DispatchClassRqst();
            break;
        case USBFS_RQST_TYPE_VND:
            requestHandled = USBFS_HandleVendorRqst();
            break;
        default:    /* requestHandled is initialezed as FALSE by default */
            break;
    }
    if (requestHandled == USBFS_FALSE)
    {
        USBFS_ep0Mode = USBFS_MODE_STALL_IN_OUT;
    }
}


/*******************************************************************************
* Function Name: USBFS_HandleIN
********************************************************************************
*
* Summary:
*  This routine handles EP0 IN transfers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_HandleIN(void)
{
    switch (USBFS_transferState)
    {
        case USBFS_TRANS_STATE_IDLE:
            break;
        case USBFS_TRANS_STATE_CONTROL_READ:
            USBFS_ControlReadDataStage();
            break;
        case USBFS_TRANS_STATE_CONTROL_WRITE:
            USBFS_ControlWriteStatusStage();
            break;
        case USBFS_TRANS_STATE_NO_DATA_CONTROL:
            USBFS_NoDataControlStatusStage();
            break;
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBFS_HandleOUT
********************************************************************************
*
* Summary:
*  This routine handles EP0 OUT transfers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_HandleOUT(void)
{
    switch (USBFS_transferState)
    {
        case USBFS_TRANS_STATE_IDLE:
            break;
        case USBFS_TRANS_STATE_CONTROL_READ:
            USBFS_ControlReadStatusStage();
            break;
        case USBFS_TRANS_STATE_CONTROL_WRITE:
            USBFS_ControlWriteDataStage();
            break;
        case USBFS_TRANS_STATE_NO_DATA_CONTROL:
            /* Update the completion block */
            USBFS_UpdateStatusBlock(USBFS_XFER_ERROR);
            /* We expect no more data, so stall INs and OUTs */
            USBFS_ep0Mode = USBFS_MODE_STALL_IN_OUT;
            break;
        default:    /* there are no more states */
            break;
    }
}


/*******************************************************************************
* Function Name: USBFS_LoadEP0
********************************************************************************
*
* Summary:
*  This routine loads the EP0 data registers for OUT transfers.  It uses the
*  currentTD (previously initialized by the _InitControlWrite function and
*  updated for each OUT transfer, and the bLastPacketSize) to determine how
*  many uint8s to transfer on the current OUT.
*
*  If the number of uint8s remaining is zero and the last transfer was full,
*  we need to send a zero length packet.  Otherwise we send the minimum
*  of the control endpoint size (8) or remaining number of uint8s for the
*  transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_LoadEP0(void)
{
    /* Update the transfer byte count from the last transaction */
    USBFS_transferByteCount += USBFS_lastPacketSize;
    /* Now load the next transaction */
    USBFS_ep0Count = 0u;
    while ((currentTD.count > 0u) && (USBFS_ep0Count < 8u))
    {
        USBFS_EP0_DR0_PTR[USBFS_ep0Count] = *currentTD.pData++;
        USBFS_ep0Count++;
        currentTD.count--;
    }
    /* Support zero-length packet*/
    if( (USBFS_ep0Count > 0u) || (USBFS_lastPacketSize == 8u) )
    {
        /* Update the data toggle */
        USBFS_ep0Toggle ^= USBFS_EP0_CNT_DATA_TOGGLE;
        /* Set the Mode Register  */
        USBFS_ep0Mode = USBFS_MODE_ACK_IN_STATUS_OUT;
        /* Update the state (or stay the same) */
        USBFS_transferState = USBFS_TRANS_STATE_CONTROL_READ;
    }
    else
    {
        /* Expect Status Stage Out */
        USBFS_ep0Mode = USBFS_MODE_STATUS_OUT_ONLY;
        /* Update the state (or stay the same) */
        USBFS_transferState = USBFS_TRANS_STATE_CONTROL_READ;
    }

    /* Save the packet size for next time */
    USBFS_lastPacketSize = USBFS_ep0Count;
}


/*******************************************************************************
* Function Name: USBFS_InitControlRead
********************************************************************************
*
* Summary:
*  Initialize a control read transaction, usable to send data to the host.
*  The following global variables shold be initialized before this function 
*  called. To send zero length packet use InitZeroLengthControlTransfer 
*  function.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Global variables:
*  currentTD.count - counts of data to be sent.
*  currentTD.pData - data pointer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_InitControlRead(void)
{
    uint16 xferCount;
    if(currentTD.count == 0)
    {
        USBFS_InitZeroLengthControlTransfer();
    }
    else
    {
        /* Set up the state machine */
        USBFS_transferState = USBFS_TRANS_STATE_CONTROL_READ;
        /* Set the toggle, it gets updated in LoadEP */
        USBFS_ep0Toggle = 0u;
        /* Initialize the Status Block */
        USBFS_InitializeStatusBlock();
        xferCount = (((uint16)CY_GET_REG8(USBFS_lengthHi) << 8u) | (CY_GET_REG8(USBFS_lengthLo)));
    
        if (currentTD.count > xferCount)
        {
            currentTD.count = xferCount;
        }
        USBFS_LoadEP0();
    }

    return(USBFS_TRUE);
}


/*******************************************************************************
* Function Name: USBFS_InitZeroLengthControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a zero length data IN transfer.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_InitZeroLengthControlTransfer(void)
{
    /* Update the state */
    USBFS_transferState = USBFS_TRANS_STATE_CONTROL_READ;
    /* Set the data toggle */
    USBFS_ep0Toggle = USBFS_EP0_CNT_DATA_TOGGLE;
    /* Set the Mode Register  */
    USBFS_ep0Mode = USBFS_MODE_ACK_IN_STATUS_OUT;
    /* Save the packet size for next time */
    USBFS_lastPacketSize = 0u;
    USBFS_ep0Count = 0u;
    
    return(USBFS_TRUE);
}
/*******************************************************************************
* Function Name: USBFS_ControlReadDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control read transfer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_ControlReadDataStage(void)
{
    USBFS_LoadEP0();
}


/*******************************************************************************
* Function Name: USBFS_ControlReadStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control read transfer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_ControlReadStatusStage(void)
{
    /* Update the transfer byte count */
    USBFS_transferByteCount += USBFS_lastPacketSize;
    /* Go Idle */
    USBFS_transferState = USBFS_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBFS_UpdateStatusBlock(USBFS_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USBFS_ep0Mode =  USBFS_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBFS_InitControlWrite
********************************************************************************
*
* Summary:
*  Initialize a control write transaction
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_InitControlWrite(void)
{
    uint16 xferCount;

    /* Set up the state machine */
    USBFS_transferState = USBFS_TRANS_STATE_CONTROL_WRITE;
    /* This migh not be necessary */
    USBFS_ep0Toggle = USBFS_EP0_CNT_DATA_TOGGLE;
    /* Initialize the Status Block */
    USBFS_InitializeStatusBlock();

    xferCount = (((uint16)CY_GET_REG8(USBFS_lengthHi) << 8u) | (CY_GET_REG8(USBFS_lengthLo)));

    if (currentTD.count > xferCount)
    {
        currentTD.count = xferCount;
    }

    /* Expect Data or Status Stage */
    USBFS_ep0Mode = USBFS_MODE_ACK_OUT_STATUS_IN;

    return(USBFS_TRUE);
}


/*******************************************************************************
* Function Name: USBFS_ControlWriteDataStage
********************************************************************************
*
* Summary:
*  Handle the Data Stage of a control write transfer
*       1. Get the data (We assume the destination was validated previously)
*       2. Update the count and data toggle
*       3. Update the mode register for the next transaction
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_ControlWriteDataStage(void)
{
    uint8 *pReg = (uint8 *)USBFS_EP0_DR0_PTR;
    
    USBFS_ep0Count = (CY_GET_REG8(USBFS_EP0_CNT_PTR ) & 0x0Fu) - 2u;

    USBFS_transferByteCount += USBFS_ep0Count;

    while ((currentTD.count > 0u) && (USBFS_ep0Count > 0u))
    {
        *currentTD.pData++ = CY_GET_REG8(pReg++);
        USBFS_ep0Count--;
        currentTD.count--;
    }
    /* Update the data toggle */
    USBFS_ep0Toggle ^= USBFS_EP0_CNT_DATA_TOGGLE;
    /* Expect Data or Status Stage */
    USBFS_ep0Mode = USBFS_MODE_ACK_OUT_STATUS_IN;
}


/*******************************************************************************
* Function Name: USBFS_ControlWriteStatusStage
********************************************************************************
*
* Summary:
*  Handle the Status Stage of a control write transfer
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_ControlWriteStatusStage(void)
{
    /* Go Idle */
    USBFS_transferState = USBFS_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBFS_UpdateStatusBlock(USBFS_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USBFS_ep0Mode = USBFS_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBFS_InitNoDataControlTransfer
********************************************************************************
*
* Summary:
*  Initialize a no data control transfer
*
* Parameters:
*  None.
*
* Return:
*  requestHandled state.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_InitNoDataControlTransfer(void)
{
    USBFS_transferState = USBFS_TRANS_STATE_NO_DATA_CONTROL;
    USBFS_ep0Mode = USBFS_MODE_STATUS_IN_ONLY;
    USBFS_ep0Toggle = USBFS_EP0_CNT_DATA_TOGGLE;
    USBFS_ep0Count = 0u;
    
    return(USBFS_TRUE);
}


/*******************************************************************************
* Function Name: USBFS_NoDataControlStatusStage
********************************************************************************
* Summary:
*  Handle the Status Stage of a no data control transfer.
*
*  SET_ADDRESS is special, since we need to receive the status stage with
*  the old address.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_NoDataControlStatusStage(void)
{
    /* Change the USB address register if we got a SET_ADDRESS. */
    if (USBFS_deviceAddress != 0u)
    {
        CY_SET_REG8(USBFS_CR0_PTR, USBFS_deviceAddress | USBFS_CR0_ENABLE);
        USBFS_deviceAddress = 0u;
    }
    /* Go Idle */
    USBFS_transferState = USBFS_TRANS_STATE_IDLE;
    /* Update the completion block */
    USBFS_UpdateStatusBlock(USBFS_XFER_STATUS_ACK);
     /* We expect no more data, so stall INs and OUTs */
    USBFS_ep0Mode = USBFS_MODE_STALL_IN_OUT;
}


/*******************************************************************************
* Function Name: USBFS_UpdateStatusBlock
********************************************************************************
*
* Summary:
*  Update the Completion Status Block for a Request.  The block is updated
*  with the completion code the USBFS_transferByteCount.  The
*  StatusBlock Pointer is set to NULL.
*
* Parameters:
*  completionCode - status. 
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_UpdateStatusBlock(uint8 completionCode)
{
    if (currentTD.pStatusBlock != USBFS_NULL)
    {
        currentTD.pStatusBlock->status = completionCode;
        currentTD.pStatusBlock->length = USBFS_transferByteCount;
        currentTD.pStatusBlock = USBFS_NULL;
    }
}


/*******************************************************************************
* Function Name: USBFS_InitializeStatusBlock
********************************************************************************
*
* Summary:
*  Initialize the Completion Status Block for a Request.  The completion
*  code is set to USB_XFER_IDLE.
*
*  Also, initializes USBFS_transferByteCount.  Save some space,
*  this is the only consumer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_InitializeStatusBlock(void)
{
    USBFS_transferByteCount = 0u;
    if (currentTD.pStatusBlock != USBFS_NULL)
    {
        currentTD.pStatusBlock->status = USBFS_XFER_IDLE;
        currentTD.pStatusBlock->length = 0u;
    }
}


/* [] END OF FILE */
