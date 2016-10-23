/*********************************************************************
*
*                  Board Support Packet of Cypress Chips
*
*
*********************************************************************
* FileName:        USBUART_API.C
* Dependencies:
* Processor:
* Complier:
* Company:
*
* Software License Agreement
*
********************************************************************************
* Copyright (2008), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/
#include <device.h>
//#include "FIFO.h"
#include "USBUART_api.h"

#define USBUART_RX_BUFFER_SIZE 255                          /* USB receive buffer size;  max 255*/
uint8 baUSBRxBuf[USBUART_RX_BUFFER_SIZE];                   /* buffer */
FIFO USBRxFIFO;                                             /* FIFO for USB receive buffer */

uint8 bLineCoding[7];

#define USBUART_TX_BUFFER_SIZE 64                           /* USB transmit buffer size, max 64 */

/*******************************************************************************
* FUNCTION NAME: void USBUART_Write( uint8 *pData, uint8 bLength )
*
* Summary:
*  Get the count of received bytes.
*
* Parameters:
*  pData: pointer to the buffer.
*  bLength:  number of bytes to be transmited.
*
* Return:
*  Void.
*
* Theory:
* -None-
*
* Side Effects:
* -None-
*******************************************************************************/
void USBUART_Write(uint8 *pData, uint8 bLength)
{
    if( USBUART_bGetEPState(2) == USBUART_IN_BUFFER_EMPTY )
    {
        if( bLength > USBUART_TX_BUFFER_SIZE ) bLength = USBUART_TX_BUFFER_SIZE;
        USBUART_LoadInEP( 2, pData, bLength );
    }
    return;

}
/*******************************************************************************
* FUNCTION NAME: void USBUART_ReadAll(uint8 *pData)
*
* Summary:
* Read all input data
*
* Parameters:
*  pData: pointer to buffer where data will be placed
*
* Return:
* Void.
*
* Theory:
*
*
* Side Effects:
* -None-
*******************************************************************************/
void USBUART_ReadAll(uint8 *pData)
{
    uint8 bCount;
    bCount = USBUART_bGetRxCount();
    FIFO_Read(&USBRxFIFO,pData,bCount);
	USBUART_EnableOutEP(3);					/* We've read data, allow more to come in */
    return;
}

/*******************************************************************************
* FUNCTION NAME: uint8 USBUART_bGetRxCount(void)
*
* Summary:
*  Get the size of valid data in the receive buffer.
*
* Parameters:
* -None-
*
* Return:
*  The size of valid data in the receive buffer.
*
* Theory:
* -None-
*
* Side Effects:
* -None-
*******************************************************************************/
uint8 USBUART_bGetRxCount(void)
{
    return( FIFO_HoldNum(&USBRxFIFO) );
}

/*******************************************************************************
* FUNCTION NAME: uint8 USBUART_bTxIsReady(void)
*
* Summary:
*  Determines if the transmit buffer is ready
*
* Parameters:
* -None-
*
* Return:
*  0 if not ready, otherwise TX buffer size.
*
* Theory:
* -None-
*
* Side Effects:
* -None-
*******************************************************************************/
uint8 USBUART_bTxIsReady(void)
{
	if(CY_GET_REG8(USBUART_USB__SIE_EP2_CR0)&0x10)
    {
        return USBUART_TX_BUFFER_SIZE;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
* FUNCTION NAME:    uint8 USBUART_Init(void)
*
* Summary:
*  Initialize the component.
*
* Parameters:
* -None-
*
* Return:
* -None-
*
* Theory:
* -None-
*
* Side Effects:
* -None-
*******************************************************************************/
uint8 USBUART_Init(void)
{
    FIFO_Init( &USBRxFIFO, baUSBRxBuf, USBUART_RX_BUFFER_SIZE );
    USBUART_EnableOutEP(3);
    return 1;
}

/*******************************************************************************
* FUNCTION NAME:    uint32 *USBUART_dwGetDTERate(uint32 *dwDTERate)
*
* Summary:
*  Returns the current baud rate set by the terminal
*
* Parameters:
* 
*
* Return:
* 
*
* Theory:
* -None-
*
* Side Effects:
* -None-
*******************************************************************************/
uint32 *USBUART_dwGetDTERate(uint32 * dwDTERate)
{
	((uint8*)&(*dwDTERate))[0] = bLineCoding[3];
	((uint8*)&(*dwDTERate))[1] = bLineCoding[2];
	((uint8*)&(*dwDTERate))[2] = bLineCoding[1];
	((uint8*)&(*dwDTERate))[3] = bLineCoding[0];
	
	return dwDTERate; 
}
