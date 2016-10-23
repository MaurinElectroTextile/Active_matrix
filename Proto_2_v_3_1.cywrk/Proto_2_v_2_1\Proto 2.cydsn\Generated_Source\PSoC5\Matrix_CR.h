/*******************************************************************************
* File Name: Matrix_CR.h  
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

#if !defined(CY_CONTROL_REG_Matrix_CR_H) /* CY_CONTROL_REG_Matrix_CR_H */
#define CY_CONTROL_REG_Matrix_CR_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*         Function Prototypes 
***************************************/    

void    Matrix_CR_Write(uint8 control) ;
uint8   Matrix_CR_Read(void) ;

/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Matrix_CR_Control         (* (reg8 *) Matrix_CR_ctrl_reg__CONTROL_REG )
#endif /* End CY_CONTROL_REG_Matrix_CR_H */

/* [] END OF FILE */
