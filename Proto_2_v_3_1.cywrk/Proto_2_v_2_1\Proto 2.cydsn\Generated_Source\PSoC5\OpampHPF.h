/*******************************************************************************
* File Name: OpampHPF.h  
* Version 1.50
*
* Description:
*  This file contains the function prototypes and constants used in
*  the PGA User Module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PGA_OpampHPF_H) 
#define CY_PGA_OpampHPF_H 

#include "cytypes.h"
#include "cyfitter.h"


/***************************************  
* Conditional Compilation Parameters
***************************************/

/* PSoC3 ES2 or early */
#define OpampHPF_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES1 or early */
#define OpampHPF_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define OpampHPF_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define OpampHPF_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct _OpampHPF_backupStruct
{
    uint8   enableState;
}   OpampHPF_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void OpampHPF_Start(void);
void OpampHPF_Stop(void) ; 
void OpampHPF_SetPower(uint8 power) ;
void OpampHPF_SetGain(uint8 gain) ;
void OpampHPF_Sleep(void);
void OpampHPF_Wakeup(void);
void OpampHPF_SaveConfig(void);
void OpampHPF_RestoreConfig(void);
void OpampHPF_Init(void) ;
void OpampHPF_Enable(void) ;


/***************************************
*            API Constants        
***************************************/

/* Power constants for SetPower function */
#define OpampHPF_MINPOWER            (0x00u)
#define OpampHPF_LOWPOWER            (0x01u)
#define OpampHPF_MEDPOWER            (0x02u)
#define OpampHPF_HIGHPOWER           (0x03u)

/* Constants for SetGain function */
#define OpampHPF_GAIN_01             (0x00u)
#define OpampHPF_GAIN_02             (0x01u)
#define OpampHPF_GAIN_04             (0x02u)
#define OpampHPF_GAIN_08             (0x03u)
#define OpampHPF_GAIN_16             (0x04u)
#define OpampHPF_GAIN_24             (0x05u)
#define OpampHPF_GAIN_32             (0x07u)
#define OpampHPF_GAIN_48             (0x08u)
#define OpampHPF_GAIN_50             (0x09u)
#define OpampHPF_GAIN_MAX            (0x09u)

/* Constant for VDDA Threshold */
#define OpampHPF_CYDEV_VDDA_MV       (CYDEV_VDDA_MV)
#define OpampHPF_VDDA_THRESHOLD_MV   (3500u)


/***************************************
*       Initialization Values
***************************************/

#define OpampHPF_DEFAULT_GAIN        (0u)
#define OpampHPF_VREF_MODE           (0 ? 0x00u : OpampHPF_GNDVREF_E)
#define OpampHPF_DEFAULT_POWER       (3)
#define OpampHPF_ACLK_ENABLE         (0)


/***************************************
*              Registers        
***************************************/

#define OpampHPF_CR0_REG                  (* (reg8 *) OpampHPF_SC__CR0 )
#define OpampHPF_CR0_PTR                  (  (reg8 *) OpampHPF_SC__CR0 )
#define OpampHPF_CR1_REG                  (* (reg8 *) OpampHPF_SC__CR1 )
#define OpampHPF_CR1_PTR                  (  (reg8 *) OpampHPF_SC__CR1 )
#define OpampHPF_CR2_REG                  (* (reg8 *) OpampHPF_SC__CR2 )
#define OpampHPF_CR2_PTR                  (  (reg8 *) OpampHPF_SC__CR2 )
#define OpampHPF_PM_ACT_CFG_REG           (* (reg8 *) OpampHPF_SC__PM_ACT_CFG )  /* Power manager */
#define OpampHPF_PM_ACT_CFG_PTR           (  (reg8 *) OpampHPF_SC__PM_ACT_CFG )  
#define OpampHPF_PM_STBY_CFG_REG          (* (reg8 *) OpampHPF_SC__PM_STBY_CFG )  /* Power manager */
#define OpampHPF_PM_STBY_CFG_PTR          (  (reg8 *) OpampHPF_SC__PM_STBY_CFG )  
#define OpampHPF_BST_REG                  (* (reg8 *) OpampHPF_SC__BST )
#define OpampHPF_BST_PTR                  (  (reg8 *) OpampHPF_SC__BST )
#define OpampHPF_PUMP_CR1_REG             (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1) /* Pump clock selectin register */
#define OpampHPF_PUMP_CR1_PTR             (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)

/* Pump Register for SC block */
#define OpampHPF_SC_MISC_REG              (* (reg8 *) CYDEV_ANAIF_RT_SC_MISC)
#define OpampHPF_SC_MISC_PTR              (  (reg8 *) CYDEV_ANAIF_RT_SC_MISC)

#if (OpampHPF_ACLK_ENABLE)
    /* Active mode power enable register */
    #define OpampHPF_ACLK_PM_ACT_CFG_REG  (* (reg8 *) OpampHPF_ScBoostClk__PM_ACT_CFG ) /* Power manager */
    #define OpampHPF_ACLK_PM_ACT_CFG_PTR  (  (reg8 *) OpampHPF_ScBoostClk__PM_ACT_CFG ) 

    /* Alternative Active mode power enable register */
    #define OpampHPF_ACLK_PM_STBY_CFG_REG  (* (reg8 *) OpampHPF_ScBoostClk__PM_STBY_CFG ) /* Power manager */
    #define OpampHPF_ACLK_PM_STBY_CFG_PTR  (  (reg8 *) OpampHPF_ScBoostClk__PM_STBY_CFG ) 

    /* Power enable mask */
    #define OpampHPF_ACLK_ACT_PWR_EN    OpampHPF_ScBoostClk__PM_ACT_MSK  /* Power enable mask */
    #define OpampHPF_ACLK_STBY_PWR_EN   OpampHPF_ScBoostClk__PM_STBY_MSK  /* Power enable mask */
#endif /* OpampHPF_ACLK_ENABLE */

/* PM_ACT_CFG (Active Power Mode CFG Register)mask */ 
#define OpampHPF_ACT_PWR_EN          OpampHPF_SC__PM_ACT_MSK /* Power enable mask */

/* PM_STBY_CFG (Alternate Active Power Mode CFG Register)mask */ 
#define OpampHPF_STBY_PWR_EN          OpampHPF_SC__PM_STBY_MSK /* Power enable mask */


/***************************************
*            Register Constants        
***************************************/

/* SC_MISC constants */
#define OpampHPF_PUMP_FORCE          (0x20u)
#define OpampHPF_PUMP_AUTO           (0x10u)
#define OpampHPF_DIFF_PGA_1_3        (0x02u)
#define OpampHPF_DIFF_PGA_0_2        (0x01u)

/* ANIF.PUMP.CR1 Constants */
#define OpampHPF_PUMP_CR1_SC_CLKSEL  (0x80u)

/* CR0 SC/CT Control Register 0 definitions */
#define OpampHPF_MODE_PGA            (0x0Cu)

/* CR1 SC/CT Control Register 1 definitions */

/* Bit Field  SC_COMP_ENUM */
#define OpampHPF_COMP_MASK           (0x0Cu)
#define OpampHPF_COMP_3P0PF          (0x00u)
#define OpampHPF_COMP_3P6PF          (0x04u)
#define OpampHPF_COMP_4P35PF         (0x08u)
#define OpampHPF_COMP_5P1PF          (0x0Cu)

/* Bit Field  SC_DIV2_ENUM */
#define OpampHPF_DIV2_MASK           (0x10u)
#define OpampHPF_DIV2_DISABLE        (0x00u)
#define OpampHPF_DIV2_ENABLE         (0x10u)

/* Bit Field  SC_DRIVE_ENUM */
#define OpampHPF_DRIVE_MASK          (0x03u)
#define OpampHPF_DRIVE_280UA         (0x00u)
#define OpampHPF_DRIVE_420UA         (0x01u)
#define OpampHPF_DRIVE_530UA         (0x02u)
#define OpampHPF_DRIVE_650UA         (0x03u)

/* Bit Field  SC_PGA_MODE_ENUM */
#define OpampHPF_PGA_MODE_MASK       (0x20u)
#define OpampHPF_PGA_INV             (0x00u)
#define OpampHPF_PGA_NINV            (0x20u)

/* CR2 SC/CT Control Register 2 definitions */

/* Bit Field  SC_BIAS_CONTROL_ENUM */
#define OpampHPF_BIAS_MASK           (0x01u)
#define OpampHPF_BIAS_NORMAL         (0x00u)
#define OpampHPF_BIAS_LOW            (0x01u)

/* Bit Field  SC_PGA_GNDVREF_ENUM  */
#define OpampHPF_GNDVREF_MASK        (0x80u)
#define OpampHPF_GNDVREF_DI          (0x00u)
#define OpampHPF_GNDVREF_E           (0x80u)

/* Bit Field  SC_R20_40B_ENUM */
#define OpampHPF_R20_40B_MASK        (0x02u)
#define OpampHPF_R20_40B_40K         (0x00u)
#define OpampHPF_R20_40B_20K         (0x02u)

/* Bit Field  SC_REDC_ENUM */
#define OpampHPF_REDC_MASK           (0x0Cu)
#define OpampHPF_REDC_00             (0x00u)
#define OpampHPF_REDC_01             (0x04u)
#define OpampHPF_REDC_10             (0x08u)
#define OpampHPF_REDC_11             (0x0Cu)

/* Bit Field  SC_RVAL_ENUM */
#define OpampHPF_RVAL_MASK           (0x70u)
#define OpampHPF_RVAL_20K            (0x00u)
#define OpampHPF_RVAL_30K            (0x10u)
#define OpampHPF_RVAL_40K            (0x20u)
#define OpampHPF_RVAL_60K            (0x30u)
#define OpampHPF_RVAL_120K           (0x40u)
#define OpampHPF_RVAL_250K           (0x50u)
#define OpampHPF_RVAL_500K           (0x60u)
#define OpampHPF_RVAL_1000K          (0x70u)

/* Bit Field  PGA_GAIN_ENUM */
#define OpampHPF_PGA_GAIN_MASK       (0x72u)
#define OpampHPF_PGA_GAIN_01         (0x00u)
#define OpampHPF_PGA_GAIN_02         (0x10u)
#define OpampHPF_PGA_GAIN_04         (0x20u)
#define OpampHPF_PGA_GAIN_08         (0x30u)
#define OpampHPF_PGA_GAIN_16         (0x40u)
#define OpampHPF_PGA_GAIN_24         (0x50u)
#define OpampHPF_PGA_GAIN_25         (0x70u)
#define OpampHPF_PGA_GAIN_32         (0x52u)
#define OpampHPF_PGA_GAIN_48         (0x62u)
#define OpampHPF_PGA_GAIN_50         (0x72u)

#endif /* CY_PGA_OpampHPF_H */


/* [] END OF FILE */
