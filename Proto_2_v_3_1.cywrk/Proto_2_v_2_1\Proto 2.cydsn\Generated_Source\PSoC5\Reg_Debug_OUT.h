/*******************************************************************************
* File Name: Reg_Debug_OUT.h  
* Version 1.50
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_STATUS_REG_Reg_Debug_OUT_H) /* CY_STATUS_REG_Reg_Debug_OUT_H */
#define CY_STATUS_REG_Reg_Debug_OUT_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 Reg_Debug_OUT_Read(void) ;


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Reg_Debug_OUT_Status    (* (reg8 *) Reg_Debug_OUT_sts_reg__STATUS_REG )

#endif /* End CY_STATUS_REG_Reg_Debug_OUT_H */


/* [] END OF FILE */
