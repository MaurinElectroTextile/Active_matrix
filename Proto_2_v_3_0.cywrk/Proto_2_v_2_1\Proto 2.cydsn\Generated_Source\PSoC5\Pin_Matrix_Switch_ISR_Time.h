/*******************************************************************************
* File Name: Pin_Matrix_Switch_ISR_Time.h  
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

#if !defined(CY_PINS_Pin_Matrix_Switch_ISR_Time_H) /* Pins Pin_Matrix_Switch_ISR_Time_H */
#define CY_PINS_Pin_Matrix_Switch_ISR_Time_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_Matrix_Switch_ISR_Time_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_Matrix_Switch_ISR_Time_Write(uint8 value) ;
void    Pin_Matrix_Switch_ISR_Time_SetDriveMode(uint8 mode) ;
uint8   Pin_Matrix_Switch_ISR_Time_ReadDataReg(void) ;
uint8   Pin_Matrix_Switch_ISR_Time_Read(void) ;
uint8   Pin_Matrix_Switch_ISR_Time_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_Matrix_Switch_ISR_Time_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_Matrix_Switch_ISR_Time_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_Matrix_Switch_ISR_Time_DM_RES_UP          PIN_DM_RES_UP
#define Pin_Matrix_Switch_ISR_Time_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_Matrix_Switch_ISR_Time_DM_OD_LO           PIN_DM_OD_LO
#define Pin_Matrix_Switch_ISR_Time_DM_OD_HI           PIN_DM_OD_HI
#define Pin_Matrix_Switch_ISR_Time_DM_STRONG          PIN_DM_STRONG
#define Pin_Matrix_Switch_ISR_Time_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_Matrix_Switch_ISR_Time_MASK               Pin_Matrix_Switch_ISR_Time__MASK
#define Pin_Matrix_Switch_ISR_Time_SHIFT              Pin_Matrix_Switch_ISR_Time__SHIFT
#define Pin_Matrix_Switch_ISR_Time_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_Matrix_Switch_ISR_Time_PS                     (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PS)
/* Data Register */
#define Pin_Matrix_Switch_ISR_Time_DR                     (* (reg8 *) Pin_Matrix_Switch_ISR_Time__DR)
/* Port Number */
#define Pin_Matrix_Switch_ISR_Time_PRT_NUM                (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_Matrix_Switch_ISR_Time_AG                     (* (reg8 *) Pin_Matrix_Switch_ISR_Time__AG)                       
/* Analog MUX bux enable */
#define Pin_Matrix_Switch_ISR_Time_AMUX                   (* (reg8 *) Pin_Matrix_Switch_ISR_Time__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_Matrix_Switch_ISR_Time_BIE                    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_Matrix_Switch_ISR_Time_BIT_MASK               (* (reg8 *) Pin_Matrix_Switch_ISR_Time__BIT_MASK)
/* Bypass Enable */
#define Pin_Matrix_Switch_ISR_Time_BYP                    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__BYP)
/* Port wide control signals */                                                   
#define Pin_Matrix_Switch_ISR_Time_CTL                    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__CTL)
/* Drive Modes */
#define Pin_Matrix_Switch_ISR_Time_DM0                    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__DM0) 
#define Pin_Matrix_Switch_ISR_Time_DM1                    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__DM1)
#define Pin_Matrix_Switch_ISR_Time_DM2                    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__DM2) 
/* Input Buffer Disable Override */
#define Pin_Matrix_Switch_ISR_Time_INP_DIS                (* (reg8 *) Pin_Matrix_Switch_ISR_Time__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_Matrix_Switch_ISR_Time_LCD_COM_SEG            (* (reg8 *) Pin_Matrix_Switch_ISR_Time__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_Matrix_Switch_ISR_Time_LCD_EN                 (* (reg8 *) Pin_Matrix_Switch_ISR_Time__LCD_EN)
/* Slew Rate Control */
#define Pin_Matrix_Switch_ISR_Time_SLW                    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_Matrix_Switch_ISR_Time_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_Matrix_Switch_ISR_Time_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_Matrix_Switch_ISR_Time_PRTDSI__OE_SEL0        (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRTDSI__OE_SEL0) 
#define Pin_Matrix_Switch_ISR_Time_PRTDSI__OE_SEL1        (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_Matrix_Switch_ISR_Time_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRTDSI__OUT_SEL0) 
#define Pin_Matrix_Switch_ISR_Time_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_Matrix_Switch_ISR_Time_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_Matrix_Switch_ISR_Time__PRTDSI__SYNC_OUT) 


#if defined(Pin_Matrix_Switch_ISR_Time__INTSTAT)  /* Interrupt Registers */

    #define Pin_Matrix_Switch_ISR_Time_INTSTAT                (* (reg8 *) Pin_Matrix_Switch_ISR_Time__INTSTAT)
    #define Pin_Matrix_Switch_ISR_Time_SNAP                   (* (reg8 *) Pin_Matrix_Switch_ISR_Time__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_Matrix_Switch_ISR_Time_H */


/* [] END OF FILE */
