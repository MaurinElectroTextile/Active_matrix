/*******************************************************************************
* File Name: Accu_OUTM.h  
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

#if !defined(CY_STATUS_REG_Accu_OUTM_H) /* CY_STATUS_REG_Accu_OUTM_H */
#define CY_STATUS_REG_Accu_OUTM_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 Accu_OUTM_Read(void) ;


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Accu_OUTM_Status    (* (reg8 *) Accu_OUTM_sts_reg__STATUS_REG )

#endif /* End CY_STATUS_REG_Accu_OUTM_H */


/* [] END OF FILE */
