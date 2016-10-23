/*******************************************************************************
* File Name: Rectifier_INH.h  
* Version 1.50
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_CONTROL_REG_Rectifier_INH_H) /* CY_CONTROL_REG_Rectifier_INH_H */
#define CY_CONTROL_REG_Rectifier_INH_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*         Function Prototypes 
***************************************/    

void    Rectifier_INH_Write(uint8 control) ;
uint8   Rectifier_INH_Read(void) ;

/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Rectifier_INH_Control         (* (reg8 *) Rectifier_INH_ctrl_reg__CONTROL_REG )
#endif /* End CY_CONTROL_REG_Rectifier_INH_H */

/* [] END OF FILE */
