/*******************************************************************************
* File Name: Pin_SW_Reset_Baseline.h  
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

#if !defined(CY_PINS_Pin_SW_Reset_Baseline_H) /* Pins Pin_SW_Reset_Baseline_H */
#define CY_PINS_Pin_SW_Reset_Baseline_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_SW_Reset_Baseline_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_SW_Reset_Baseline_Write(uint8 value) ;
void    Pin_SW_Reset_Baseline_SetDriveMode(uint8 mode) ;
uint8   Pin_SW_Reset_Baseline_ReadDataReg(void) ;
uint8   Pin_SW_Reset_Baseline_Read(void) ;
uint8   Pin_SW_Reset_Baseline_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_SW_Reset_Baseline_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_SW_Reset_Baseline_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_SW_Reset_Baseline_DM_RES_UP          PIN_DM_RES_UP
#define Pin_SW_Reset_Baseline_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_SW_Reset_Baseline_DM_OD_LO           PIN_DM_OD_LO
#define Pin_SW_Reset_Baseline_DM_OD_HI           PIN_DM_OD_HI
#define Pin_SW_Reset_Baseline_DM_STRONG          PIN_DM_STRONG
#define Pin_SW_Reset_Baseline_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_SW_Reset_Baseline_MASK               Pin_SW_Reset_Baseline__MASK
#define Pin_SW_Reset_Baseline_SHIFT              Pin_SW_Reset_Baseline__SHIFT
#define Pin_SW_Reset_Baseline_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_SW_Reset_Baseline_PS                     (* (reg8 *) Pin_SW_Reset_Baseline__PS)
/* Data Register */
#define Pin_SW_Reset_Baseline_DR                     (* (reg8 *) Pin_SW_Reset_Baseline__DR)
/* Port Number */
#define Pin_SW_Reset_Baseline_PRT_NUM                (* (reg8 *) Pin_SW_Reset_Baseline__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_SW_Reset_Baseline_AG                     (* (reg8 *) Pin_SW_Reset_Baseline__AG)                       
/* Analog MUX bux enable */
#define Pin_SW_Reset_Baseline_AMUX                   (* (reg8 *) Pin_SW_Reset_Baseline__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_SW_Reset_Baseline_BIE                    (* (reg8 *) Pin_SW_Reset_Baseline__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_SW_Reset_Baseline_BIT_MASK               (* (reg8 *) Pin_SW_Reset_Baseline__BIT_MASK)
/* Bypass Enable */
#define Pin_SW_Reset_Baseline_BYP                    (* (reg8 *) Pin_SW_Reset_Baseline__BYP)
/* Port wide control signals */                                                   
#define Pin_SW_Reset_Baseline_CTL                    (* (reg8 *) Pin_SW_Reset_Baseline__CTL)
/* Drive Modes */
#define Pin_SW_Reset_Baseline_DM0                    (* (reg8 *) Pin_SW_Reset_Baseline__DM0) 
#define Pin_SW_Reset_Baseline_DM1                    (* (reg8 *) Pin_SW_Reset_Baseline__DM1)
#define Pin_SW_Reset_Baseline_DM2                    (* (reg8 *) Pin_SW_Reset_Baseline__DM2) 
/* Input Buffer Disable Override */
#define Pin_SW_Reset_Baseline_INP_DIS                (* (reg8 *) Pin_SW_Reset_Baseline__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_SW_Reset_Baseline_LCD_COM_SEG            (* (reg8 *) Pin_SW_Reset_Baseline__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_SW_Reset_Baseline_LCD_EN                 (* (reg8 *) Pin_SW_Reset_Baseline__LCD_EN)
/* Slew Rate Control */
#define Pin_SW_Reset_Baseline_SLW                    (* (reg8 *) Pin_SW_Reset_Baseline__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_SW_Reset_Baseline_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_SW_Reset_Baseline__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_SW_Reset_Baseline_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_SW_Reset_Baseline__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_SW_Reset_Baseline_PRTDSI__OE_SEL0        (* (reg8 *) Pin_SW_Reset_Baseline__PRTDSI__OE_SEL0) 
#define Pin_SW_Reset_Baseline_PRTDSI__OE_SEL1        (* (reg8 *) Pin_SW_Reset_Baseline__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_SW_Reset_Baseline_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_SW_Reset_Baseline__PRTDSI__OUT_SEL0) 
#define Pin_SW_Reset_Baseline_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_SW_Reset_Baseline__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_SW_Reset_Baseline_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_SW_Reset_Baseline__PRTDSI__SYNC_OUT) 


#if defined(Pin_SW_Reset_Baseline__INTSTAT)  /* Interrupt Registers */

    #define Pin_SW_Reset_Baseline_INTSTAT                (* (reg8 *) Pin_SW_Reset_Baseline__INTSTAT)
    #define Pin_SW_Reset_Baseline_SNAP                   (* (reg8 *) Pin_SW_Reset_Baseline__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_SW_Reset_Baseline_H */


/* [] END OF FILE */
