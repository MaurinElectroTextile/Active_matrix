/*******************************************************************************
* File Name: Pin_Input_Ground.h  
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

#if !defined(CY_PINS_Pin_Input_Ground_H) /* Pins Pin_Input_Ground_H */
#define CY_PINS_Pin_Input_Ground_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_Input_Ground_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_Input_Ground_Write(uint8 value) ;
void    Pin_Input_Ground_SetDriveMode(uint8 mode) ;
uint8   Pin_Input_Ground_ReadDataReg(void) ;
uint8   Pin_Input_Ground_Read(void) ;
uint8   Pin_Input_Ground_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_Input_Ground_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_Input_Ground_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_Input_Ground_DM_RES_UP          PIN_DM_RES_UP
#define Pin_Input_Ground_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_Input_Ground_DM_OD_LO           PIN_DM_OD_LO
#define Pin_Input_Ground_DM_OD_HI           PIN_DM_OD_HI
#define Pin_Input_Ground_DM_STRONG          PIN_DM_STRONG
#define Pin_Input_Ground_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_Input_Ground_MASK               Pin_Input_Ground__MASK
#define Pin_Input_Ground_SHIFT              Pin_Input_Ground__SHIFT
#define Pin_Input_Ground_WIDTH              8u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_Input_Ground_PS                     (* (reg8 *) Pin_Input_Ground__PS)
/* Data Register */
#define Pin_Input_Ground_DR                     (* (reg8 *) Pin_Input_Ground__DR)
/* Port Number */
#define Pin_Input_Ground_PRT_NUM                (* (reg8 *) Pin_Input_Ground__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_Input_Ground_AG                     (* (reg8 *) Pin_Input_Ground__AG)                       
/* Analog MUX bux enable */
#define Pin_Input_Ground_AMUX                   (* (reg8 *) Pin_Input_Ground__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_Input_Ground_BIE                    (* (reg8 *) Pin_Input_Ground__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_Input_Ground_BIT_MASK               (* (reg8 *) Pin_Input_Ground__BIT_MASK)
/* Bypass Enable */
#define Pin_Input_Ground_BYP                    (* (reg8 *) Pin_Input_Ground__BYP)
/* Port wide control signals */                                                   
#define Pin_Input_Ground_CTL                    (* (reg8 *) Pin_Input_Ground__CTL)
/* Drive Modes */
#define Pin_Input_Ground_DM0                    (* (reg8 *) Pin_Input_Ground__DM0) 
#define Pin_Input_Ground_DM1                    (* (reg8 *) Pin_Input_Ground__DM1)
#define Pin_Input_Ground_DM2                    (* (reg8 *) Pin_Input_Ground__DM2) 
/* Input Buffer Disable Override */
#define Pin_Input_Ground_INP_DIS                (* (reg8 *) Pin_Input_Ground__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_Input_Ground_LCD_COM_SEG            (* (reg8 *) Pin_Input_Ground__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_Input_Ground_LCD_EN                 (* (reg8 *) Pin_Input_Ground__LCD_EN)
/* Slew Rate Control */
#define Pin_Input_Ground_SLW                    (* (reg8 *) Pin_Input_Ground__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_Input_Ground_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_Input_Ground__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_Input_Ground_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_Input_Ground__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_Input_Ground_PRTDSI__OE_SEL0        (* (reg8 *) Pin_Input_Ground__PRTDSI__OE_SEL0) 
#define Pin_Input_Ground_PRTDSI__OE_SEL1        (* (reg8 *) Pin_Input_Ground__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_Input_Ground_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_Input_Ground__PRTDSI__OUT_SEL0) 
#define Pin_Input_Ground_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_Input_Ground__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_Input_Ground_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_Input_Ground__PRTDSI__SYNC_OUT) 


#if defined(Pin_Input_Ground__INTSTAT)  /* Interrupt Registers */

    #define Pin_Input_Ground_INTSTAT                (* (reg8 *) Pin_Input_Ground__INTSTAT)
    #define Pin_Input_Ground_SNAP                   (* (reg8 *) Pin_Input_Ground__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_Input_Ground_H */


/* [] END OF FILE */
