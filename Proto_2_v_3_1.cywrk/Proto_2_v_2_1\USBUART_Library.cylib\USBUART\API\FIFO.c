/*********************************************************************
*
*                  Board Support Packet of Cypress Chips
*                  FIFO Machine
*
*********************************************************************
* FileName:        FIFO.C
* Dependencies:
*                  FIFO.H
* Processor:
* Complier:
* Company:
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

/*********************************************************************
 * Function:        STATUS FIFO_Init(
 *                                  P_FIFO pFIFO,
 *                                  uint8* pFIFOBuffer,
 *                                  uint8 bBufferSize
 *                                  )
 *
 * PreCondition:    None
 *
 * Input:           pFIFO           pointer to FIFO structure
 *                  pFIFOBuffer     pointer to FIFO buffer
 *                  wBufferSize     buffer size
 *
 * Output:
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void FIFO_Init(P_FIFO pFIFO, uint8* pFIFOBuffer, uint8 bBufferSize)
{
    if( pFIFOBuffer == NULL )
        return;                     /* if not define buffer return with error */
    pFIFO->bSize = bBufferSize;     /* set FIFO buffer size */
    pFIFO->pStart = pFIFO->pWrite = pFIFO->pRead = pFIFOBuffer;
                                    /* set start & write & read pointer */
    return;
}

/*********************************************************************
 * Function:        uint8    FIFO_Write(
 *                                  P_FIFO pFIFO,
 *                                  uint8* pBuffer,
 *                                  uint8 bCount
 *                                  )
 *
 * PreCondition:    FIFO_Init() is already called
 *
 * Input:           pFIFO           pointer to FIFO structure
 *                  pBuffer         pointer to buffer which contains the data
 *                                  to be written
 *                  bCount          number of data
 *
 * Output:
 *                  uint8          number of data write to the FIFO
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
uint8 FIFO_Write( P_FIFO pFIFO, uint8* pBuffer,uint8 bCount )
{
    uint8 wBlankCount;

    /* calculate the blank cell */
    wBlankCount = FIFO_BlankNum( pFIFO );
    if( wBlankCount  < bCount )
        bCount = wBlankCount;
    else
        wBlankCount = bCount;

    while( bCount-- > 0 )
    {
        /* write to the FIFO, and pointe to the next */
        *pFIFO->pWrite++ = *pBuffer++;
        /* check if it is the bottom of FIFO */
        if( pFIFO->pWrite >= pFIFO->pStart + pFIFO->bSize)
            pFIFO->pWrite = pFIFO->pStart;
    }
    return wBlankCount;
}

/*********************************************************************
 * Function:        uint8    FIFO_Read(
 *                                  P_FIFO pFIFO,
 *                                  uint8* pBuffer,
 *                                  uint8 bCount
 *                                  )
 *
 * PreCondition:    FIFO_Init() is already called
 *
 * Input:           pFIFO           pointer to FIFO structure
 *                  pBuffer         pointer to buffer which contains the data
 *                                  to be written
 *                  bCount          number of data
 *
 * Output:          uint8           number read from the FIFO
 *
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
uint8 FIFO_Read( P_FIFO pFIFO,uint8* pBuffer,uint8 bCount )
{
    uint8 wHoldCount;

    /* calculate the number of valid data in FIFO */
    wHoldCount = FIFO_HoldNum( pFIFO );

    if( wHoldCount < bCount)
        bCount = wHoldCount;
    else
        wHoldCount = bCount;

    while( bCount-- > 0 )
    {
        /* read from the FIFO, and pointe to the next */
        *pBuffer++ = *pFIFO->pRead++;
        /* check if it is the bottom of FIFO */
        if( pFIFO->pRead >= pFIFO->pStart + pFIFO->bSize)
            pFIFO->pRead = pFIFO->pStart;
    }
    return wHoldCount;
}

/*********************************************************************
 * Function:        void    FIFO_PutChar(
 *                                  P_FIFO pFIFO,
 *                                  CHAR c
 *                                  )
 *
 * PreCondition:    FIFO_Init() is already called
 *
 * Input:           pFIFO           pointer to FIFO structure
 *                  c               character
 *
 * Output:          void            no error
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void FIFO_PutChar( P_FIFO pFIFO, uint8 c )
{
    if( !FIFO_BlankNum( pFIFO ) )   /* check the blank cell number */
          return;                     /* if zero return error */
    else
    {
        /* push the char and point to next cell */
        *pFIFO->pWrite++ = c;
        /* check if it is point to the bottom of the buffer */
        if( pFIFO->pWrite >= pFIFO->pStart + pFIFO->bSize)
            pFIFO->pWrite = pFIFO->pStart;
        return;
    }
}

/*********************************************************************
 * Function:        void    FIFO_GetChar(
 *                                  P_FIFO pFIFO,
 *                                  uint* pCha
 *                                  )
 *
 * PreCondition:    FIFO_Init() is already called
 *
 * Input:           pFIFO           pointer to FIFO structure
 *                  pCha            address of a character
 *
 * Output:
 *
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void FIFO_GetChar( P_FIFO pFIFO, uint8* pCha )
{
    if( !FIFO_HoldNum( pFIFO ) )    /* check if there is any data in FIFO */
        return;                     /* if zeor return with error */
    else
    {
        /* read the data and move the read pointer to next cell */
        *pCha = *pFIFO->pRead++;
        /* check if it is point to the bottom of the buffer */
        if( pFIFO->pRead >= pFIFO->pStart + pFIFO->bSize)
            pFIFO->pRead = pFIFO->pStart;
        return;
    }
}

/*********************************************************************
 * Function:        uint8 FIFO_BlankNum(
 *                                  P_FIFO pFIFO
 *                                  )
 *
 * PreCondition:    FIFO_Init() is already called
 *
 * Input:           pFIFO           pointer to FIFO structure
 *
 * Output:          number of blank cell
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
uint8 FIFO_BlankNum( P_FIFO pFIFO )
{
    uint8 bBlankNum;

    if( pFIFO->pWrite == pFIFO->pRead )
        bBlankNum = pFIFO->bSize - 1;
    else if( pFIFO->pWrite > pFIFO->pRead )
        bBlankNum = pFIFO->bSize + pFIFO->pRead - pFIFO->pWrite - 1;
    else
        bBlankNum = pFIFO->pRead - pFIFO->pWrite - 1;
    return bBlankNum;
}

/*********************************************************************
 * Function:        uint8 FIFO_HoldNum(
 *                                  P_FIFO pFIFO
 *                                  )
 *
 * PreCondition:    FIFO_Init() is already called
 *
 * Input:           pFIFO           pointer to FIFO structure
 *
 * Output:          number of valid data in FIFO
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
uint8 FIFO_HoldNum( P_FIFO pFIFO )
{
    uint8 bHoldNum;
    if( pFIFO->pWrite == pFIFO->pRead)
        bHoldNum = 0;
    else if( pFIFO->pWrite > pFIFO->pRead)
        bHoldNum = pFIFO->pWrite - pFIFO->pRead;
    else
        bHoldNum = pFIFO->bSize + pFIFO->pWrite - pFIFO->pRead;
    return bHoldNum;
}
