/*******************************************************************************
* File Name: Matrix_SR.h  
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

#if !defined(CY_STATUS_REG_Matrix_SR_H) /* CY_STATUS_REG_Matrix_SR_H */
#define CY_STATUS_REG_Matrix_SR_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 Matrix_SR_Read(void) ;


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Matrix_SR_Status    (* (reg8 *) Matrix_SR_sts_reg__STATUS_REG )

#endif /* End CY_STATUS_REG_Matrix_SR_H */


/* [] END OF FILE */
