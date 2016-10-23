/*******************************************************************************
* File Name: AMux_Row_B.c
* Version 1.50
*
*  Description:
*    This file contains functions for the AMuxSeq.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "AMux_Row_B.h"

uint8 AMux_Row_B_initVar = 0u;


/*******************************************************************************
* Function Name: AMux_Row_B_Start
********************************************************************************
* Summary:
*  Disconnect all channels. The next time Next is called, channel 0 will be
*  connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Row_B_Start(void)
{
    AMux_Row_B_DisconnectAll();
    AMux_Row_B_initVar = 1u;
}


/*******************************************************************************
* Function Name: AMux_Row_B_Init
********************************************************************************
* Summary:
*  Disconnect all channels. The next time Next is called, channel 0 will be
*  connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Row_B_Init(void)
{
    AMux_Row_B_DisconnectAll();
}


/*******************************************************************************
* Function Name: AMux_Row_B_Stop
********************************************************************************
* Summary:
*  Disconnect all channels. The next time Next is called, channel 0 will be
*  connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Row_B_Stop(void)
{
    AMux_Row_B_DisconnectAll();
}

#if(AMux_Row_B_MUXTYPE == AMux_Row_B_MUX_DIFF)

extern int8 AMux_Row_B_CYAMUXSIDE_A_curChannel;


/*******************************************************************************
* Function Name: AMux_Row_B_Next
********************************************************************************
* Summary:
*  Disconnects the previous channel and connects the next one in the sequence.
*  When Next is called for the first time after Init, Start, Enable, Stop, or
*  DisconnectAll, it connects channel 0.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Row_B_Next(void)
{
    AMux_Row_B_CYAMUXSIDE_A_Next();
    AMux_Row_B_CYAMUXSIDE_B_Next();
}


/*******************************************************************************
* Function Name: AMux_Row_B_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels. The next time Next is called, channel
*  0 will be connected.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void AMux_Row_B_DisconnectAll(void)
{
    AMux_Row_B_CYAMUXSIDE_A_DisconnectAll();
    AMux_Row_B_CYAMUXSIDE_B_DisconnectAll();
}


/*******************************************************************************
* Function Name: AMux_Row_B_GetChannel
********************************************************************************
* Summary:
*  The currently connected channel is retuned. If no channel is connected
*  returns -1.
*
* Parameters:
*  void
*
* Return:
*  The current channel or -1.
*
*******************************************************************************/
int8 AMux_Row_B_GetChannel(void)
{
    return AMux_Row_B_CYAMUXSIDE_A_curChannel;
}

#else

extern int8 AMux_Row_B_curChannel;


/*******************************************************************************
* Function Name: AMux_Row_B_GetChannel
********************************************************************************
* Summary:
*  The currently connected channel is retuned. If no channel is connected
*  returns -1.
*
* Parameters:
*  void
*
* Return:
*  The current channel or -1.
*
*******************************************************************************/
int8 AMux_Row_B_GetChannel(void)
{
    return AMux_Row_B_curChannel;
}

#endif


/* [] END OF FILE */
