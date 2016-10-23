/*******************************************************************************
* File Name: AMux_Row_B.h
* Version 1.50
*
*  Description:
*    This file contains the constants and function prototypes for the AMuxSeq.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUXSEQ_AMux_Row_B_H)
#define CY_AMUXSEQ_AMux_Row_B_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void AMux_Row_B_Start(void);
void AMux_Row_B_Init(void);
void AMux_Row_B_Stop(void);
void AMux_Row_B_Next(void);
void AMux_Row_B_DisconnectAll(void);
int8 AMux_Row_B_GetChannel(void);


/***************************************
*     Initial Parameter Constants
***************************************/
#define AMux_Row_B_CHANNELS  8
#define AMux_Row_B_MUXTYPE   1


/***************************************
*             API Constants
***************************************/

#define AMux_Row_B_MUX_SINGLE   1
#define AMux_Row_B_MUX_DIFF     2

#endif /* CY_AMUXSEQ_AMux_Row_B_H */


/* [] END OF FILE */
