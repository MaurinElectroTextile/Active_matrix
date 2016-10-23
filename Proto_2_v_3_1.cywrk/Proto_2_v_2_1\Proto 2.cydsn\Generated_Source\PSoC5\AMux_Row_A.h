/*******************************************************************************
* File Name: AMux_Row_A.h
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

#if !defined(CY_AMUXSEQ_AMux_Row_A_H)
#define CY_AMUXSEQ_AMux_Row_A_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void AMux_Row_A_Start(void);
void AMux_Row_A_Init(void);
void AMux_Row_A_Stop(void);
void AMux_Row_A_Next(void);
void AMux_Row_A_DisconnectAll(void);
int8 AMux_Row_A_GetChannel(void);


/***************************************
*     Initial Parameter Constants
***************************************/
#define AMux_Row_A_CHANNELS  8
#define AMux_Row_A_MUXTYPE   1


/***************************************
*             API Constants
***************************************/

#define AMux_Row_A_MUX_SINGLE   1
#define AMux_Row_A_MUX_DIFF     2

#endif /* CY_AMUXSEQ_AMux_Row_A_H */


/* [] END OF FILE */
