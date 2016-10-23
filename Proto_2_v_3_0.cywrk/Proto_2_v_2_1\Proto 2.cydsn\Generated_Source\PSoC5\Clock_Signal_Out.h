/*******************************************************************************
* File Name: Clock_Signal_Out.h
* Version 1.50
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_CLOCK_Clock_Signal_Out_H)
#define CY_CLOCK_Clock_Signal_Out_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/

void Clock_Signal_Out_Start(void) ;
void Clock_Signal_Out_Stop(void) ;

#if(!(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3 && \
    CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES2) && \
	!(CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5 && \
	CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_5A_ES1))
void Clock_Signal_Out_StopBlock(void) ;
#endif

void Clock_Signal_Out_StandbyPower(uint8 state) ;
void Clock_Signal_Out_SetDividerRegister(uint16 clkDivider, uint8 reset) ;
uint16 Clock_Signal_Out_GetDividerRegister(void) ;
void Clock_Signal_Out_SetModeRegister(uint8 modeBitMask) ;
void Clock_Signal_Out_ClearModeRegister(uint8 modeBitMask) ;
uint8 Clock_Signal_Out_GetModeRegister(void) ;
void Clock_Signal_Out_SetSourceRegister(uint8 clkSource) ;
uint8 Clock_Signal_Out_GetSourceRegister(void) ;
#if defined(Clock_Signal_Out__CFG3)
void Clock_Signal_Out_SetPhaseRegister(uint8 clkPhase) ;
uint8 Clock_Signal_Out_GetPhaseRegister(void) ;
#endif

#define Clock_Signal_Out_Enable()                       Clock_Signal_Out_Start()
#define Clock_Signal_Out_Disable()                      Clock_Signal_Out_Stop()
#define Clock_Signal_Out_SetDivider(clkDivider)         Clock_Signal_Out_SetDividerRegister(clkDivider, 1)
#define Clock_Signal_Out_SetDividerValue(clkDivider)    Clock_Signal_Out_SetDividerRegister((clkDivider) - 1, 1)
#define Clock_Signal_Out_SetMode(clkMode)               Clock_Signal_Out_SetModeRegister(clkMode)
#define Clock_Signal_Out_SetSource(clkSource)           Clock_Signal_Out_SetSourceRegister(clkSource)
#if defined(Clock_Signal_Out__CFG3)
#define Clock_Signal_Out_SetPhase(clkPhase)             Clock_Signal_Out_SetPhaseRegister(clkPhase)
#define Clock_Signal_Out_SetPhaseValue(clkPhase)        Clock_Signal_Out_SetPhaseRegister((clkPhase) + 1)
#endif


/***************************************
*           API Constants
***************************************/

/* Constants SetPhase(), clkPhase parameter. Only valid for PSoC 3 ES2 and earlier. See datasheet for details. */
#if CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3 && \
   (CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES1 || \
    CYDEV_CHIP_REVISION_USED == CYDEV_CHIP_REVISION_3A_ES2)
#define CYCLK_2_5NS             0x01 /* 2.5 ns delay. */
#define CYCLK_3_5NS             0x02 /* 3.5 ns delay. */
#define CYCLK_4_5NS             0x03 /* 4.5 ns delay. */
#define CYCLK_5_5NS             0x04 /* 5.5 ns delay. */
#define CYCLK_6_5NS             0x05 /* 6.5 ns delay. */
#define CYCLK_7_5NS             0x06 /* 7.5 ns delay. */
#define CYCLK_8_5NS             0x07 /* 8.5 ns delay. */
#define CYCLK_9_5NS             0x08 /* 9.5 ns delay. */
#define CYCLK_10_5NS            0x09 /* 10.5 ns delay. */
#define CYCLK_11_5NS            0x0A /* 11.5 ns delay. */
#define CYCLK_12_5NS            0x0B /* 12.5 ns delay. */
#endif


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Clock_Signal_Out_CLKEN              (* (reg8 *) Clock_Signal_Out__PM_ACT_CFG)
#define Clock_Signal_Out_CLKEN_PTR          ((reg8 *) Clock_Signal_Out__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Clock_Signal_Out_CLKSTBY            (* (reg8 *) Clock_Signal_Out__PM_STBY_CFG)
#define Clock_Signal_Out_CLKSTBY_PTR        ((reg8 *) Clock_Signal_Out__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Clock_Signal_Out_DIV_LSB            (* (reg8 *) Clock_Signal_Out__CFG0)
#define Clock_Signal_Out_DIV_LSB_PTR        ((reg8 *) Clock_Signal_Out__CFG0)
#define Clock_Signal_Out_DIV_PTR            ((reg16 *) Clock_Signal_Out__CFG0)

/* Clock MSB divider configuration register. */
#define Clock_Signal_Out_DIV_MSB            (* (reg8 *) Clock_Signal_Out__CFG1)
#define Clock_Signal_Out_DIV_MSB_PTR        ((reg8 *) Clock_Signal_Out__CFG1)

/* Mode and source configuration register */
#define Clock_Signal_Out_MOD_SRC            (* (reg8 *) Clock_Signal_Out__CFG2)
#define Clock_Signal_Out_MOD_SRC_PTR        ((reg8 *) Clock_Signal_Out__CFG2)

#if defined(Clock_Signal_Out__CFG3)
/* Analog clock phase configuration register */
#define Clock_Signal_Out_PHASE              (* (reg8 *) Clock_Signal_Out__CFG3)
#define Clock_Signal_Out_PHASE_PTR          ((reg8 *) Clock_Signal_Out__CFG3)
#endif


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Clock_Signal_Out_CLKEN_MASK         Clock_Signal_Out__PM_ACT_MSK
#define Clock_Signal_Out_CLKSTBY_MASK       Clock_Signal_Out__PM_STBY_MSK

/* CFG2 field masks */
#define Clock_Signal_Out_SRC_SEL_MSK        Clock_Signal_Out__CFG2_SRC_SEL_MASK
#define Clock_Signal_Out_MODE_MASK          (~(Clock_Signal_Out_SRC_SEL_MSK))

#if defined(Clock_Signal_Out__CFG3)
/* CFG3 phase mask */
#define Clock_Signal_Out_PHASE_MASK         Clock_Signal_Out__CFG3_PHASE_DLY_MASK
#endif

#endif /* CY_CLOCK_Clock_Signal_Out_H */


/* [] END OF FILE */
