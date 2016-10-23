/*******************************************************************************
* File Name: PGA_HPF.h  
* Version 1.50
*
* Description:
*  This file contains the function prototypes and constants used in
*  the PGA_Inv User Module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PGA_Inv_PGA_HPF_H) 
#define CY_PGA_Inv_PGA_HPF_H 

#include "cytypes.h"
#include "cyfitter.h"


/***************************************  
* Conditional Compilation Parameters
****************************************/

/* PSoC3 ES2 or early */
#define PGA_HPF_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES1 or early */
#define PGA_HPF_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define PGA_HPF_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define PGA_HPF_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


/***************************************
*   Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct _PGA_HPF_backupStruct
{
    uint8   enableState;
}   PGA_HPF_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void PGA_HPF_Start(void);
void PGA_HPF_Stop(void) ;
void PGA_HPF_SetPower(uint8 power) ;
void PGA_HPF_SetGain(uint8 gain) ;
void PGA_HPF_Sleep(void);
void PGA_HPF_Wakeup(void) ;
void PGA_HPF_SaveConfig(void);
void PGA_HPF_RestoreConfig(void);
void PGA_HPF_Init(void) ;
void PGA_HPF_Enable(void) ;


/***************************************
*           API Constants        
***************************************/

/* Power constants for SetPower function */
#define PGA_HPF_MINPOWER             (0x00u)
#define PGA_HPF_LOWPOWER             (0x01u)
#define PGA_HPF_MEDPOWER             (0x02u)
#define PGA_HPF_HIGHPOWER            (0x03u)

/* Constants for SetGain function */
#define PGA_HPF_GAIN_01              (0x00u)
#define PGA_HPF_GAIN_03              (0x01u)
#define PGA_HPF_GAIN_07              (0x02u)
#define PGA_HPF_GAIN_15              (0x03u)
#define PGA_HPF_GAIN_22              (0x04u)
#define PGA_HPF_GAIN_24              (0x06u)
#define PGA_HPF_GAIN_31              (0x07u)
#define PGA_HPF_GAIN_47              (0x08u)
#define PGA_HPF_GAIN_49              (0x09u)
#define PGA_HPF_GAIN_MAX             (0x09u)

/* Constant for VDDA Threshold */
#define PGA_HPF_CYDEV_VDDA_MV        (CYDEV_VDDA_MV)
#define PGA_HPF_VDDA_THRESHOLD_MV    (3500u)


/***************************************
*       Initialization Values
***************************************/

#define PGA_HPF_DEFAULT_GAIN         (2u)
#define PGA_HPF_DEFAULT_POWER        (3u)
#define PGA_HPF_ACLK_ENABLE          (0u)


/***************************************
*              Registers
***************************************/

#define PGA_HPF_CR0_REG                  (* (reg8 *) PGA_HPF_SC__CR0 )
#define PGA_HPF_CR0_PTR                  (  (reg8 *) PGA_HPF_SC__CR0 )
#define PGA_HPF_CR1_REG                  (* (reg8 *) PGA_HPF_SC__CR1 )
#define PGA_HPF_CR1_PTR                  (  (reg8 *) PGA_HPF_SC__CR1 )
#define PGA_HPF_CR2_REG                  (* (reg8 *) PGA_HPF_SC__CR2 )
#define PGA_HPF_CR2_PTR                  (  (reg8 *) PGA_HPF_SC__CR2 )
#define PGA_HPF_PM_ACT_CFG_REG           (* (reg8 *) PGA_HPF_SC__PM_ACT_CFG )  /* Power manager */
#define PGA_HPF_PM_ACT_CFG_PTR           (  (reg8 *) PGA_HPF_SC__PM_ACT_CFG )
#define PGA_HPF_PM_STBY_CFG_REG          (* (reg8 *) PGA_HPF_SC__PM_STBY_CFG )  /* Power manager */
#define PGA_HPF_PM_STBY_CFG_PTR          (  (reg8 *) PGA_HPF_SC__PM_STBY_CFG )
#define PGA_HPF_SW0_REG                  (* (reg8 *) PGA_HPF_SC__SW0 )
#define PGA_HPF_SW0_PTR                  (  (reg8 *) PGA_HPF_SC__SW0 )
#define PGA_HPF_SW2_REG                  (* (reg8 *) PGA_HPF_SC__SW2 )
#define PGA_HPF_SW2_PTR                  (  (reg8 *) PGA_HPF_SC__SW2 )
#define PGA_HPF_SW3_REG                  (* (reg8 *) PGA_HPF_SC__SW3 )
#define PGA_HPF_SW3_PTR                  (  (reg8 *) PGA_HPF_SC__SW3 )
#define PGA_HPF_SW4_REG                  (* (reg8 *) PGA_HPF_SC__SW4 )
#define PGA_HPF_SW4_PTR                  (  (reg8 *) PGA_HPF_SC__SW4 )
#define PGA_HPF_SW6_REG                  (* (reg8 *) PGA_HPF_SC__SW6 )
#define PGA_HPF_SW6_PTR                  (  (reg8 *) PGA_HPF_SC__SW6 )
#define PGA_HPF_SW7_REG                  (* (reg8 *) PGA_HPF_SC__SW7 )
#define PGA_HPF_SW7_PTR                  (  (reg8 *) PGA_HPF_SC__SW7 )
#define PGA_HPF_SW8_REG                  (* (reg8 *) PGA_HPF_SC__SW8 )
#define PGA_HPF_SW8_PTR                  (  (reg8 *) PGA_HPF_SC__SW8 )
#define PGA_HPF_SW10_REG                 (* (reg8 *) PGA_HPF_SC__SW10 )
#define PGA_HPF_SW10_PTR                 (  (reg8 *) PGA_HPF_SC__SW10 )
#define PGA_HPF_CLK_REG                  (* (reg8 *) PGA_HPF_SC__CLK )
#define PGA_HPF_CLK_PTR                  (  (reg8 *) PGA_HPF_SC__CLK )
#define PGA_HPF_BST_REG                  (* (reg8 *) PGA_HPF_SC__BST )
#define PGA_HPF_BST_PTR                  (  (reg8 *) PGA_HPF_SC__BST )

 /* Pump clock selectin register */
#define PGA_HPF_PUMP_CR1_REG             (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)
#define PGA_HPF_PUMP_CR1_PTR             (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)

/* Pump Register for SC block */
#define PGA_HPF_SC_MISC_REG              (* (reg8 *) CYDEV_ANAIF_RT_SC_MISC)
#define PGA_HPF_SC_MISC_PTR              (  (reg8 *) CYDEV_ANAIF_RT_SC_MISC)

#if (PGA_HPF_ACLK_ENABLE)
    /* Active power mode register */
    #define PGA_HPF_ACLK_PM_ACT_CFG_REG       (* (reg8 *) PGA_HPF_ScBoostClk__PM_ACT_CFG )  /* Power manager */
    #define PGA_HPF_ACLK_PM_ACT_CFG_PTR       (* (reg8 *) PGA_HPF_ScBoostClk__PM_ACT_CFG )

    /* Alternative Active power mode register */
    #define PGA_HPF_ACLK_PM_STBY_CFG_REG      (* (reg8 *) PGA_HPF_ScBoostClk__PM_STBY_CFG )  /* Power manager */
    #define PGA_HPF_ACLK_PM_STBY_CFG_PTR      (* (reg8 *) PGA_HPF_ScBoostClk__PM_STBY_CFG )

    /* Power enable mask */
    #define PGA_HPF_ACLK_ACT_PWR_EN   PGA_HPF_ScBoostClk__PM_ACT_MSK /* Power enable mask */
    #define PGA_HPF_ACLK_STBY_PWR_EN  PGA_HPF_ScBoostClk__PM_STBY_MSK /* Power enable mask */
#endif


/* PM_ACT_CFG (Active Power Mode CFG Register) mask */ 
#define PGA_HPF_ACT_PWR_EN            PGA_HPF_SC__PM_ACT_MSK /* Power enable mask */
#define PGA_HPF_STBY_PWR_EN           PGA_HPF_SC__PM_STBY_MSK /* Power enable mask */


/***************************************
*           Register Constants
***************************************/

/* CR0 SC/CT Control Register 0 definitions */
#define PGA_HPF_MODE_PGA             (0x0Cu)

/* CR1 SC/CT Control Register 1 definitions */

/* Bit Field  SC_COMP_ENUM */
#define PGA_HPF_COMP_MASK            (0x0Cu)
#define PGA_HPF_COMP_3P0PF           (0x00u)
#define PGA_HPF_COMP_3P6PF           (0x04u)
#define PGA_HPF_COMP_4P35PF          (0x08u)
#define PGA_HPF_COMP_5P1PF           (0x0Cu)

/* Bit Field  SC_DIV2_ENUM */
#define PGA_HPF_DIV2_MASK            (0x10u)
#define PGA_HPF_DIV2_DISABLE         (0x00u)
#define PGA_HPF_DIV2_ENABLE          (0x10u)

/* Bit Field  SC_DRIVE_ENUM */
#define PGA_HPF_DRIVE_MASK           (0x03u)
#define PGA_HPF_DRIVE_280UA          (0x00u)
#define PGA_HPF_DRIVE_420UA          (0x01u)
#define PGA_HPF_DRIVE_530UA          (0x02u)
#define PGA_HPF_DRIVE_650UA          (0x03u)

/* Bit Field  SC_PGA_MODE_ENUM */
#define PGA_HPF_PGA_MODE_MASK        (0x20u)
#define PGA_HPF_PGA_INV              (0x00u)
#define PGA_HPF_PGA_NINV             (0x20u)

/* CR2 SC/CT Control Register 2 definitions */

/* Bit Field  SC_BIAS_CONTROL_ENUM */
#define PGA_HPF_BIAS_MASK            (0x01u)
#define PGA_HPF_BIAS_NORMAL          (0x00u)
#define PGA_HPF_BIAS_LOW             (0x01u)

/* Bit Field  SC_PGA_GNDVREF_ENUM */
#define PGA_HPF_GNDVREF_MASK         (0x80u)
#define PGA_HPF_GNDVREF_DI           (0x00u)
#define PGA_HPF_GNDVREF_E            (0x80u)

/* Bit Field  SC_R20_40B_ENUM */
#define PGA_HPF_R20_40B_MASK         (0x02u)
#define PGA_HPF_R20_40B_40K          (0x00u)
#define PGA_HPF_R20_40B_20K          (0x02u)

/* Bit Field  SC_REDC_ENUM */
#define PGA_HPF_REDC_MASK            (0x0Cu)
#define PGA_HPF_REDC_00              (0x00u)
#define PGA_HPF_REDC_01              (0x04u)
#define PGA_HPF_REDC_10              (0x08u)
#define PGA_HPF_REDC_11              (0x0Cu)

/* Bit Field  SC_RVAL_ENUM */
#define PGA_HPF_RVAL_MASK            (0x70u)
#define PGA_HPF_RVAL_20K             (0x00u)
#define PGA_HPF_RVAL_30K             (0x10u)
#define PGA_HPF_RVAL_40K             (0x20u)
#define PGA_HPF_RVAL_60K             (0x30u)
#define PGA_HPF_RVAL_120K            (0x40u)
#define PGA_HPF_RVAL_250K            (0x50u)
#define PGA_HPF_RVAL_500K            (0x60u)
#define PGA_HPF_RVAL_1000K           (0x70u)

/* SC_MISC constants */
#define PGA_HPF_PUMP_FORCE           (0x20u)
#define PGA_HPF_PUMP_AUTO            (0x10u)
#define PGA_HPF_DIFF_PGA_1_3         (0x02u)
#define PGA_HPF_DIFF_PGA_0_2         (0x01u)

/* Bit Field  PGA_INVERTING_GAIN_ENUM */
#define PGA_HPF_PGA_INV_GAIN_MASK    (0x72u)
#define PGA_HPF_PGA_INV_GAIN_00      (0x00u)
#define PGA_HPF_PGA_INV_GAIN_01      (0x10u)
#define PGA_HPF_PGA_INV_GAIN_03      (0x20u)
#define PGA_HPF_PGA_INV_GAIN_07      (0x30u)
#define PGA_HPF_PGA_INV_GAIN_15      (0x40u)
#define PGA_HPF_PGA_INV_GAIN_22      (0x50u)
#define PGA_HPF_PGA_INV_GAIN_24      (0x70u)
#define PGA_HPF_PGA_INV_GAIN_31      (0x52u)
#define PGA_HPF_PGA_INV_GAIN_47      (0x62u)
#define PGA_HPF_PGA_INV_GAIN_49      (0x72u)

/* ANIF.PUMP.CR1 Constants */
#define PGA_HPF_PUMP_CR1_SC_CLKSEL   (0x80u)

#endif /* CY_PGA_Inv_PGA_HPF_H */


/* [] END OF FILE */
