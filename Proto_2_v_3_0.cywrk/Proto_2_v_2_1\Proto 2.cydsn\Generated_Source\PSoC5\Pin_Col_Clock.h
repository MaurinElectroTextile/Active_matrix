/*******************************************************************************
* File Name: Pin_Col_Clock.h  
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

#if !defined(CY_PINS_Pin_Col_Clock_H) /* Pins Pin_Col_Clock_H */
#define CY_PINS_Pin_Col_Clock_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pin_Col_Clock_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    Pin_Col_Clock_Write(uint8 value) ;
void    Pin_Col_Clock_SetDriveMode(uint8 mode) ;
uint8   Pin_Col_Clock_ReadDataReg(void) ;
uint8   Pin_Col_Clock_Read(void) ;
uint8   Pin_Col_Clock_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pin_Col_Clock_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pin_Col_Clock_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pin_Col_Clock_DM_RES_UP          PIN_DM_RES_UP
#define Pin_Col_Clock_DM_RES_DWN         PIN_DM_RES_DWN
#define Pin_Col_Clock_DM_OD_LO           PIN_DM_OD_LO
#define Pin_Col_Clock_DM_OD_HI           PIN_DM_OD_HI
#define Pin_Col_Clock_DM_STRONG          PIN_DM_STRONG
#define Pin_Col_Clock_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pin_Col_Clock_MASK               Pin_Col_Clock__MASK
#define Pin_Col_Clock_SHIFT              Pin_Col_Clock__SHIFT
#define Pin_Col_Clock_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pin_Col_Clock_PS                     (* (reg8 *) Pin_Col_Clock__PS)
/* Data Register */
#define Pin_Col_Clock_DR                     (* (reg8 *) Pin_Col_Clock__DR)
/* Port Number */
#define Pin_Col_Clock_PRT_NUM                (* (reg8 *) Pin_Col_Clock__PRT) 
/* Connect to Analog Globals */                                                  
#define Pin_Col_Clock_AG                     (* (reg8 *) Pin_Col_Clock__AG)                       
/* Analog MUX bux enable */
#define Pin_Col_Clock_AMUX                   (* (reg8 *) Pin_Col_Clock__AMUX) 
/* Bidirectional Enable */                                                        
#define Pin_Col_Clock_BIE                    (* (reg8 *) Pin_Col_Clock__BIE)
/* Bit-mask for Aliased Register Access */
#define Pin_Col_Clock_BIT_MASK               (* (reg8 *) Pin_Col_Clock__BIT_MASK)
/* Bypass Enable */
#define Pin_Col_Clock_BYP                    (* (reg8 *) Pin_Col_Clock__BYP)
/* Port wide control signals */                                                   
#define Pin_Col_Clock_CTL                    (* (reg8 *) Pin_Col_Clock__CTL)
/* Drive Modes */
#define Pin_Col_Clock_DM0                    (* (reg8 *) Pin_Col_Clock__DM0) 
#define Pin_Col_Clock_DM1                    (* (reg8 *) Pin_Col_Clock__DM1)
#define Pin_Col_Clock_DM2                    (* (reg8 *) Pin_Col_Clock__DM2) 
/* Input Buffer Disable Override */
#define Pin_Col_Clock_INP_DIS                (* (reg8 *) Pin_Col_Clock__INP_DIS)
/* LCD Common or Segment Drive */
#define Pin_Col_Clock_LCD_COM_SEG            (* (reg8 *) Pin_Col_Clock__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pin_Col_Clock_LCD_EN                 (* (reg8 *) Pin_Col_Clock__LCD_EN)
/* Slew Rate Control */
#define Pin_Col_Clock_SLW                    (* (reg8 *) Pin_Col_Clock__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pin_Col_Clock_PRTDSI__CAPS_SEL       (* (reg8 *) Pin_Col_Clock__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pin_Col_Clock_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pin_Col_Clock__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pin_Col_Clock_PRTDSI__OE_SEL0        (* (reg8 *) Pin_Col_Clock__PRTDSI__OE_SEL0) 
#define Pin_Col_Clock_PRTDSI__OE_SEL1        (* (reg8 *) Pin_Col_Clock__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pin_Col_Clock_PRTDSI__OUT_SEL0       (* (reg8 *) Pin_Col_Clock__PRTDSI__OUT_SEL0) 
#define Pin_Col_Clock_PRTDSI__OUT_SEL1       (* (reg8 *) Pin_Col_Clock__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pin_Col_Clock_PRTDSI__SYNC_OUT       (* (reg8 *) Pin_Col_Clock__PRTDSI__SYNC_OUT) 


#if defined(Pin_Col_Clock__INTSTAT)  /* Interrupt Registers */

    #define Pin_Col_Clock_INTSTAT                (* (reg8 *) Pin_Col_Clock__INTSTAT)
    #define Pin_Col_Clock_SNAP                   (* (reg8 *) Pin_Col_Clock__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins Pin_Col_Clock_H */


/* [] END OF FILE */
