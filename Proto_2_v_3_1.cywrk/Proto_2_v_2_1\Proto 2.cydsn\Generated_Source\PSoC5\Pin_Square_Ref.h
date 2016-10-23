/*******************************************************************************
* File Name: Pin_Square_Ref.h  
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

#if !defined(CY_PINS_Pin_Square_Ref_H) /* Pins Pin_Square_Ref_H */
#define CY_PINS_Pin_Square_Ref_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_Square_Ref_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_Square_Ref_Write(uint8 value) ;
void    Pin_Square_Ref_SetDriveMode(uint8 mode) ;
uint8   Pin_Square_Ref_ReadDataReg(void) ;
uint8   Pin_Square_Ref_Read(void) ;
uint8   Pin_Square_Ref_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_Square_Ref_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_Square_Ref_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_Square_Ref_DM_RES_UP          PIN_DM_RES_UP
#define Pin_Square_Ref_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_Square_Ref_DM_OD_LO           PIN_DM_OD_LO
#define Pin_Square_Ref_DM_OD_HI           PIN_DM_OD_HI
#define Pin_Square_Ref_DM_STRONG          PIN_DM_STRONG
#define Pin_Square_Ref_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_Square_Ref_MASK               Pin_Square_Ref__MASK
#define Pin_Square_Ref_SHIFT              Pin_Square_Ref__SHIFT
#define Pin_Square_Ref_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_Square_Ref_PS                     (* (reg8 *) Pin_Square_Ref__PS)
/* Data Register */
#define Pin_Square_Ref_DR                     (* (reg8 *) Pin_Square_Ref__DR)
/* Port Number */
#define Pin_Square_Ref_PRT_NUM                (* (reg8 *) Pin_Square_Ref__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_Square_Ref_AG                     (* (reg8 *) Pin_Square_Ref__AG)                       
/* Analog MUX bux enable */
#define Pin_Square_Ref_AMUX                   (* (reg8 *) Pin_Square_Ref__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_Square_Ref_BIE                    (* (reg8 *) Pin_Square_Ref__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_Square_Ref_BIT_MASK               (* (reg8 *) Pin_Square_Ref__BIT_MASK)
/* Bypass Enable */
#define Pin_Square_Ref_BYP                    (* (reg8 *) Pin_Square_Ref__BYP)
/* Port wide control signals */                                                   
#define Pin_Square_Ref_CTL                    (* (reg8 *) Pin_Square_Ref__CTL)
/* Drive Modes */
#define Pin_Square_Ref_DM0                    (* (reg8 *) Pin_Square_Ref__DM0) 
#define Pin_Square_Ref_DM1                    (* (reg8 *) Pin_Square_Ref__DM1)
#define Pin_Square_Ref_DM2                    (* (reg8 *) Pin_Square_Ref__DM2) 
/* Input Buffer Disable Override */
#define Pin_Square_Ref_INP_DIS                (* (reg8 *) Pin_Square_Ref__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_Square_Ref_LCD_COM_SEG            (* (reg8 *) Pin_Square_Ref__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_Square_Ref_LCD_EN                 (* (reg8 *) Pin_Square_Ref__LCD_EN)
/* Slew Rate Control */
#define Pin_Square_Ref_SLW                    (* (reg8 *) Pin_Square_Ref__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_Square_Ref_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_Square_Ref__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_Square_Ref_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_Square_Ref__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_Square_Ref_PRTDSI__OE_SEL0        (* (reg8 *) Pin_Square_Ref__PRTDSI__OE_SEL0) 
#define Pin_Square_Ref_PRTDSI__OE_SEL1        (* (reg8 *) Pin_Square_Ref__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_Square_Ref_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_Square_Ref__PRTDSI__OUT_SEL0) 
#define Pin_Square_Ref_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_Square_Ref__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_Square_Ref_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_Square_Ref__PRTDSI__SYNC_OUT) 


#if defined(Pin_Square_Ref__INTSTAT)  /* Interrupt Registers */

    #define Pin_Square_Ref_INTSTAT                (* (reg8 *) Pin_Square_Ref__INTSTAT)
    #define Pin_Square_Ref_SNAP                   (* (reg8 *) Pin_Square_Ref__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_Square_Ref_H */


/* [] END OF FILE */
