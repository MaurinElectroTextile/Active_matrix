/*******************************************************************************
* File Name: Status_Reg_1.h  
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

#if !defined(CY_STATUS_REG_Status_Reg_1_H) /* CY_STATUS_REG_Status_Reg_1_H */
#define CY_STATUS_REG_Status_Reg_1_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 Status_Reg_1_Read(void) ;


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Status_Reg_1_Status    (* (reg8 *) Status_Reg_1_sts_reg__STATUS_REG )

#endif /* End CY_STATUS_REG_Status_Reg_1_H */


/* [] END OF FILE */
