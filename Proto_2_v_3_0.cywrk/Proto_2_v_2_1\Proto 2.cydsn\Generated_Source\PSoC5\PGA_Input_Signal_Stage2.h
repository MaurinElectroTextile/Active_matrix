/*******************************************************************************
* File Name: PGA_Input_Signal_Stage2.h  
* Version 1.60
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

#if !defined(CY_PGA_PGA_Input_Signal_Stage2_H) 
#define CY_PGA_PGA_Input_Signal_Stage2_H 

#include "cytypes.h"
#include "cyfitter.h"


/***************************************  
* Conditional Compilation Parameters
***************************************/

/* PSoC3 ES2 or early */
#define PGA_Input_Signal_Stage2_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES1 or early */
#define PGA_Input_Signal_Stage2_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define PGA_Input_Signal_Stage2_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define PGA_Input_Signal_Stage2_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct _PGA_Input_Signal_Stage2_backupStruct
{
    uint8   enableState;
}   PGA_Input_Signal_Stage2_BACKUP_STRUCT;


/***************************************
*        Function Prototypes 
***************************************/

void PGA_Input_Signal_Stage2_Start(void);
void PGA_Input_Signal_Stage2_Stop(void) ; 
void PGA_Input_Signal_Stage2_SetPower(uint8 power) ;
void PGA_Input_Signal_Stage2_SetGain(uint8 gain) ;
void PGA_Input_Signal_Stage2_Sleep(void);
void PGA_Input_Signal_Stage2_Wakeup(void);
void PGA_Input_Signal_Stage2_SaveConfig(void);
void PGA_Input_Signal_Stage2_RestoreConfig(void);
void PGA_Input_Signal_Stage2_Init(void) ;
void PGA_Input_Signal_Stage2_Enable(void) ;


/***************************************
*            API Constants        
***************************************/

/* Power constants for SetPower function */
#define PGA_Input_Signal_Stage2_MINPOWER            (0x00u)
#define PGA_Input_Signal_Stage2_LOWPOWER            (0x01u)
#define PGA_Input_Signal_Stage2_MEDPOWER            (0x02u)
#define PGA_Input_Signal_Stage2_HIGHPOWER           (0x03u)

/* Constants for SetGain function */
#define PGA_Input_Signal_Stage2_GAIN_01             (0x00u)
#define PGA_Input_Signal_Stage2_GAIN_02             (0x01u)
#define PGA_Input_Signal_Stage2_GAIN_04             (0x02u)
#define PGA_Input_Signal_Stage2_GAIN_08             (0x03u)
#define PGA_Input_Signal_Stage2_GAIN_16             (0x04u)
#define PGA_Input_Signal_Stage2_GAIN_24             (0x05u)
#define PGA_Input_Signal_Stage2_GAIN_32             (0x06u)
#define PGA_Input_Signal_Stage2_GAIN_48             (0x07u)
#define PGA_Input_Signal_Stage2_GAIN_50             (0x08u)
#define PGA_Input_Signal_Stage2_GAIN_MAX            (0x08u)

/* Constant for VDDA Threshold */
#define PGA_Input_Signal_Stage2_CYDEV_VDDA_MV       (CYDEV_VDDA_MV)
#define PGA_Input_Signal_Stage2_VDDA_THRESHOLD_MV   (3500u)

/* Constant for Minimum VDDA Threshold */
#define PGA_Input_Signal_Stage2_MINIMUM_VDDA_THRESHOLD_MV   (2700u)

/***************************************
*       Initialization Values
***************************************/

#define PGA_Input_Signal_Stage2_DEFAULT_GAIN        (0u)
#define PGA_Input_Signal_Stage2_VREF_MODE           (1 ? 0x00u : PGA_Input_Signal_Stage2_GNDVREF_E)
#define PGA_Input_Signal_Stage2_DEFAULT_POWER       (2)


/***************************************
*              Registers        
***************************************/

#define PGA_Input_Signal_Stage2_CR0_REG                  (* (reg8 *) PGA_Input_Signal_Stage2_SC__CR0 )
#define PGA_Input_Signal_Stage2_CR0_PTR                  (  (reg8 *) PGA_Input_Signal_Stage2_SC__CR0 )
#define PGA_Input_Signal_Stage2_CR1_REG                  (* (reg8 *) PGA_Input_Signal_Stage2_SC__CR1 )
#define PGA_Input_Signal_Stage2_CR1_PTR                  (  (reg8 *) PGA_Input_Signal_Stage2_SC__CR1 )
#define PGA_Input_Signal_Stage2_CR2_REG                  (* (reg8 *) PGA_Input_Signal_Stage2_SC__CR2 )
#define PGA_Input_Signal_Stage2_CR2_PTR                  (  (reg8 *) PGA_Input_Signal_Stage2_SC__CR2 )
#define PGA_Input_Signal_Stage2_PM_ACT_CFG_REG           (* (reg8 *) PGA_Input_Signal_Stage2_SC__PM_ACT_CFG )  /* Power manager */
#define PGA_Input_Signal_Stage2_PM_ACT_CFG_PTR           (  (reg8 *) PGA_Input_Signal_Stage2_SC__PM_ACT_CFG )  
#define PGA_Input_Signal_Stage2_PM_STBY_CFG_REG          (* (reg8 *) PGA_Input_Signal_Stage2_SC__PM_STBY_CFG )  /* Power manager */
#define PGA_Input_Signal_Stage2_PM_STBY_CFG_PTR          (  (reg8 *) PGA_Input_Signal_Stage2_SC__PM_STBY_CFG )  
#define PGA_Input_Signal_Stage2_BST_REG                  (* (reg8 *) PGA_Input_Signal_Stage2_SC__BST )
#define PGA_Input_Signal_Stage2_BST_PTR                  (  (reg8 *) PGA_Input_Signal_Stage2_SC__BST )
#define PGA_Input_Signal_Stage2_PUMP_CR1_REG             (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1) /* Pump clock selectin register */
#define PGA_Input_Signal_Stage2_PUMP_CR1_PTR             (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)

/* Pump Register for SC block */
#define PGA_Input_Signal_Stage2_SC_MISC_REG              (* (reg8 *) CYDEV_ANAIF_RT_SC_MISC)
#define PGA_Input_Signal_Stage2_SC_MISC_PTR              (  (reg8 *) CYDEV_ANAIF_RT_SC_MISC)

#if(PGA_Input_Signal_Stage2_CYDEV_VDDA_MV < PGA_Input_Signal_Stage2_MINIMUM_VDDA_THRESHOLD_MV)
    /* Active mode power enable register */
    #define PGA_Input_Signal_Stage2_ACLK_PM_ACT_CFG_REG  (* (reg8 *) PGA_Input_Signal_Stage2_ScBoostClk__PM_ACT_CFG ) /* Power manager */
    #define PGA_Input_Signal_Stage2_ACLK_PM_ACT_CFG_PTR  (  (reg8 *) PGA_Input_Signal_Stage2_ScBoostClk__PM_ACT_CFG ) 

    /* Alternative Active mode power enable register */
    #define PGA_Input_Signal_Stage2_ACLK_PM_STBY_CFG_REG  (* (reg8 *) PGA_Input_Signal_Stage2_ScBoostClk__PM_STBY_CFG ) /* Power manager */
    #define PGA_Input_Signal_Stage2_ACLK_PM_STBY_CFG_PTR  (  (reg8 *) PGA_Input_Signal_Stage2_ScBoostClk__PM_STBY_CFG ) 

    /* Power enable mask */
    #define PGA_Input_Signal_Stage2_ACLK_ACT_PWR_EN    PGA_Input_Signal_Stage2_ScBoostClk__PM_ACT_MSK  /* Power enable mask */
    #define PGA_Input_Signal_Stage2_ACLK_STBY_PWR_EN   PGA_Input_Signal_Stage2_ScBoostClk__PM_STBY_MSK  /* Power enable mask */
#endif /* PGA_Input_Signal_Stage2_ACLK_ENABLE */

/* PM_ACT_CFG (Active Power Mode CFG Register)mask */ 
#define PGA_Input_Signal_Stage2_ACT_PWR_EN          PGA_Input_Signal_Stage2_SC__PM_ACT_MSK /* Power enable mask */

/* PM_STBY_CFG (Alternate Active Power Mode CFG Register)mask */ 
#define PGA_Input_Signal_Stage2_STBY_PWR_EN          PGA_Input_Signal_Stage2_SC__PM_STBY_MSK /* Power enable mask */


/***************************************
*            Register Constants        
***************************************/

/* SC_MISC constants */
#define PGA_Input_Signal_Stage2_PUMP_FORCE          (0x20u)
#define PGA_Input_Signal_Stage2_PUMP_AUTO           (0x10u)
#define PGA_Input_Signal_Stage2_DIFF_PGA_1_3        (0x02u)
#define PGA_Input_Signal_Stage2_DIFF_PGA_0_2        (0x01u)

/* ANIF.PUMP.CR1 Constants */
#define PGA_Input_Signal_Stage2_PUMP_CR1_SC_CLKSEL  (0x80u)

/* CR0 SC/CT Control Register 0 definitions */
#define PGA_Input_Signal_Stage2_MODE_PGA            (0x0Cu)

/* CR1 SC/CT Control Register 1 definitions */

/* Bit Field  SC_COMP_ENUM */
#define PGA_Input_Signal_Stage2_COMP_MASK           (0x0Cu)
#define PGA_Input_Signal_Stage2_COMP_3P0PF          (0x00u)
#define PGA_Input_Signal_Stage2_COMP_3P6PF          (0x04u)
#define PGA_Input_Signal_Stage2_COMP_4P35PF         (0x08u)
#define PGA_Input_Signal_Stage2_COMP_5P1PF          (0x0Cu)

/* Bit Field  SC_DIV2_ENUM */
#define PGA_Input_Signal_Stage2_DIV2_MASK           (0x10u)
#define PGA_Input_Signal_Stage2_DIV2_DISABLE        (0x00u)
#define PGA_Input_Signal_Stage2_DIV2_ENABLE         (0x10u)

/* Bit Field  SC_DRIVE_ENUM */
#define PGA_Input_Signal_Stage2_DRIVE_MASK          (0x03u)
#define PGA_Input_Signal_Stage2_DRIVE_280UA         (0x00u)
#define PGA_Input_Signal_Stage2_DRIVE_420UA         (0x01u)
#define PGA_Input_Signal_Stage2_DRIVE_530UA         (0x02u)
#define PGA_Input_Signal_Stage2_DRIVE_650UA         (0x03u)

/* Bit Field  SC_PGA_MODE_ENUM */
#define PGA_Input_Signal_Stage2_PGA_MODE_MASK       (0x20u)
#define PGA_Input_Signal_Stage2_PGA_INV             (0x00u)
#define PGA_Input_Signal_Stage2_PGA_NINV            (0x20u)

/* CR2 SC/CT Control Register 2 definitions */

/* Bit Field  SC_BIAS_CONTROL_ENUM */
#define PGA_Input_Signal_Stage2_BIAS_MASK           (0x01u)
#define PGA_Input_Signal_Stage2_BIAS_NORMAL         (0x00u)
#define PGA_Input_Signal_Stage2_BIAS_LOW            (0x01u)

/* Bit Field  SC_PGA_GNDVREF_ENUM  */
#define PGA_Input_Signal_Stage2_GNDVREF_MASK        (0x80u)
#define PGA_Input_Signal_Stage2_GNDVREF_DI          (0x00u)
#define PGA_Input_Signal_Stage2_GNDVREF_E           (0x80u)

/* Bit Field  SC_R20_40B_ENUM */
#define PGA_Input_Signal_Stage2_R20_40B_MASK        (0x02u)
#define PGA_Input_Signal_Stage2_R20_40B_40K         (0x00u)
#define PGA_Input_Signal_Stage2_R20_40B_20K         (0x02u)

/* Bit Field  SC_REDC_ENUM */
#define PGA_Input_Signal_Stage2_REDC_MASK           (0x0Cu)
#define PGA_Input_Signal_Stage2_REDC_00             (0x00u)
#define PGA_Input_Signal_Stage2_REDC_01             (0x04u)
#define PGA_Input_Signal_Stage2_REDC_10             (0x08u)
#define PGA_Input_Signal_Stage2_REDC_11             (0x0Cu)

/* Bit Field  SC_RVAL_ENUM */
#define PGA_Input_Signal_Stage2_RVAL_MASK           (0x70u)
#define PGA_Input_Signal_Stage2_RVAL_20K            (0x00u)
#define PGA_Input_Signal_Stage2_RVAL_30K            (0x10u)
#define PGA_Input_Signal_Stage2_RVAL_40K            (0x20u)
#define PGA_Input_Signal_Stage2_RVAL_60K            (0x30u)
#define PGA_Input_Signal_Stage2_RVAL_120K           (0x40u)
#define PGA_Input_Signal_Stage2_RVAL_250K           (0x50u)
#define PGA_Input_Signal_Stage2_RVAL_500K           (0x60u)
#define PGA_Input_Signal_Stage2_RVAL_1000K          (0x70u)

/* Bit Field  PGA_GAIN_ENUM */
#define PGA_Input_Signal_Stage2_PGA_GAIN_MASK       (0x72u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_01         (0x00u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_02         (0x10u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_04         (0x20u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_08         (0x30u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_16         (0x40u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_24         (0x50u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_25         (0x70u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_32         (0x52u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_48         (0x62u)
#define PGA_Input_Signal_Stage2_PGA_GAIN_50         (0x72u)

#endif /* CY_PGA_PGA_Input_Signal_Stage2_H */


/* [] END OF FILE */
