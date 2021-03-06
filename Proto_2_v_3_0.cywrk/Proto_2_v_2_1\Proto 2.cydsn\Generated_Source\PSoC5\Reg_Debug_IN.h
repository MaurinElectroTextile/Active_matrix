/*******************************************************************************
* File Name: Reg_Debug_IN.h  
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

#if !defined(CY_CONTROL_REG_Reg_Debug_IN_H) /* CY_CONTROL_REG_Reg_Debug_IN_H */
#define CY_CONTROL_REG_Reg_Debug_IN_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*         Function Prototypes 
***************************************/    

void    Reg_Debug_IN_Write(uint8 control) ;
uint8   Reg_Debug_IN_Read(void) ;

/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Reg_Debug_IN_Control         (* (reg8 *) Reg_Debug_IN_ctrl_reg__CONTROL_REG )
#endif /* End CY_CONTROL_REG_Reg_Debug_IN_H */

/* [] END OF FILE */
