/*******************************************************************************
* File Name: Opamp_Half_Vdd.h  
* Version 1.50
*
* Description:
*  This file contains the function prototypes and constants used in
*  the OpAmp (Analog Buffer) Component.
*
* Note:
*     
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_OPAMP_Opamp_Half_Vdd_H) 
#define CY_OPAMP_Opamp_Half_Vdd_H 

#include "cytypes.h"
#include "cyfitter.h"


/***************************************  
* Conditional Compilation Parameters
***************************************/

/* PSoC3 ES2 or early */
#define Opamp_Half_Vdd_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES1 or early */
#define Opamp_Half_Vdd_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define Opamp_Half_Vdd_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define Opamp_Half_Vdd_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct _Opamp_Half_Vdd_backupStruct
{
    uint8   enableState;
}   Opamp_Half_Vdd_BACKUP_STRUCT;


/**************************************
*        Function Prototypes 
**************************************/

void Opamp_Half_Vdd_Start(void);
void Opamp_Half_Vdd_Stop(void) ;
void Opamp_Half_Vdd_SetPower(uint8 power) ;
void Opamp_Half_Vdd_Sleep(void);
void Opamp_Half_Vdd_Wakeup(void) ;
void Opamp_Half_Vdd_SaveConfig(void);
void Opamp_Half_Vdd_RestoreConfig(void);
void Opamp_Half_Vdd_Init(void) ;
void Opamp_Half_Vdd_Enable(void) ;


/**************************************
*           API Constants        
**************************************/

/* Power constants for SetPower() function */
#if (Opamp_Half_Vdd_PSOC3_ES3 || Opamp_Half_Vdd_PSOC5_ES2)
    #define Opamp_Half_Vdd_LPOCPOWER (0x00u)
    #define Opamp_Half_Vdd_LOWPOWER  (0x01u)
    #define Opamp_Half_Vdd_MEDPOWER  (0x02u)
    #define Opamp_Half_Vdd_HIGHPOWER (0x03u)
#else
    #define Opamp_Half_Vdd_LOWPOWER  (0x01u)
    #define Opamp_Half_Vdd_MEDPOWER  (0x02u)
    #define Opamp_Half_Vdd_HIGHPOWER (0x00u)
#endif

/**************************************
*           Parameter Defaults        
**************************************/
#if (Opamp_Half_Vdd_PSOC3_ES3 || Opamp_Half_Vdd_PSOC5_ES2)
    #define Opamp_Half_Vdd_DEFAULT_POWER    (3u)
#endif

#if (Opamp_Half_Vdd_PSOC3_ES2 || Opamp_Half_Vdd_PSOC5_ES1)
    #if (3u == 0)
        #error "Low Power Over Compensated Mode is not supported on ES2 silicon. Please select another default power setting"
    #elif (3u == 3)
        #define Opamp_Half_Vdd_DEFAULT_POWER (0u)
    #else
        #define Opamp_Half_Vdd_DEFAULT_POWER (3u)
    #endif
#endif

#define Opamp_Half_Vdd_DEFAULT_MODE     (1u)


/**************************************
*             Registers        
**************************************/

#define Opamp_Half_Vdd_CR_REG        (* (reg8 *) Opamp_Half_Vdd_ABuf__CR)
#define Opamp_Half_Vdd_CR_PTR        (  (reg8 *) Opamp_Half_Vdd_ABuf__CR)

#define Opamp_Half_Vdd_MX_REG        (* (reg8 *) Opamp_Half_Vdd_ABuf__MX)
#define Opamp_Half_Vdd_MX_PTR        (  (reg8 *) Opamp_Half_Vdd_ABuf__MX)

#define Opamp_Half_Vdd_SW_REG        (* (reg8 *) Opamp_Half_Vdd_ABuf__SW)
#define Opamp_Half_Vdd_SW_PTR        (  (reg8 *) Opamp_Half_Vdd_ABuf__SW)

/* Active mode power manager register */
#define Opamp_Half_Vdd_PM_ACT_CFG_REG    (* (reg8 *) Opamp_Half_Vdd_ABuf__PM_ACT_CFG)
#define Opamp_Half_Vdd_PM_ACT_CFG_PTR    (  (reg8 *) Opamp_Half_Vdd_ABuf__PM_ACT_CFG)

/* Alternative mode power manager register */
#define Opamp_Half_Vdd_PM_STBY_CFG_REG    (* (reg8 *) Opamp_Half_Vdd_ABuf__PM_STBY_CFG)
#define Opamp_Half_Vdd_PM_STBY_CFG_PTR    (  (reg8 *) Opamp_Half_Vdd_ABuf__PM_STBY_CFG)

/* ANIF.PUMP.CR1 Pump Configuration Register 1 */
#define Opamp_Half_Vdd_PUMP_CR1_REG  (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)
#define Opamp_Half_Vdd_PUMP_CR1_PTR  (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)

/* Trim register defines */
#define Opamp_Half_Vdd_TRO_REG       (* (reg8 *) Opamp_Half_Vdd_ABuf__TR0)
#define Opamp_Half_Vdd_TRO_PTR       (  (reg8 *) Opamp_Half_Vdd_ABuf__TR0)

#define Opamp_Half_Vdd_TR1_REG       (* (reg8 *) Opamp_Half_Vdd_ABuf__TR1)
#define Opamp_Half_Vdd_TR1_PTR       (  (reg8 *) Opamp_Half_Vdd_ABuf__TR1)

/* PM_ACT_CFG (Active Power Mode CFG Register) mask */ 
#define Opamp_Half_Vdd_ACT_PWR_EN   Opamp_Half_Vdd_ABuf__PM_ACT_MSK 

/* PM_STBY_CFG (Alternative Active Power Mode CFG Register) mask */ 
#define Opamp_Half_Vdd_STBY_PWR_EN   Opamp_Half_Vdd_ABuf__PM_STBY_MSK 


/**************************************
*       Register Constants        
**************************************/

/* CX Analog Buffer Input Selection Register */

/* Power mode defines */
#define Opamp_Half_Vdd_PWR_MASK           (0x03u)
#if (Opamp_Half_Vdd_PSOC3_ES3 || Opamp_Half_Vdd_PSOC5_ES2)
    #define Opamp_Half_Vdd_PWR_LPOC        (0x00u)
    #define Opamp_Half_Vdd_PWR_LOW         (0x01u)
    #define Opamp_Half_Vdd_PWR_MEDIUM      (0x02u)
    #define Opamp_Half_Vdd_PWR_HIGH        (0x03u)
#else
    #define Opamp_Half_Vdd_PWR_LOW         (0x01u)
    #define Opamp_Half_Vdd_PWR_MEDIUM      (0x02u)
    #define Opamp_Half_Vdd_PWR_HIGH        (0x00u)
#endif

/* MX Analog Buffer Input Selection Register */

/* Bit Field  MX_VN */
#define Opamp_Half_Vdd_MX_VN_MASK       (0x30u)
#define Opamp_Half_Vdd_MX_VN_NC         (0x00u)
#define Opamp_Half_Vdd_MX_VN_AG4        (0x20u)
#define Opamp_Half_Vdd_MX_VN_AG6        (0x30u)

/* Bit Field  MX_VP */
#define Opamp_Half_Vdd_MX_VP_MASK       (0x0Fu)
#define Opamp_Half_Vdd_MX_VP_NC         (0x00u)
#define Opamp_Half_Vdd_MX_VP_VREF       (0x07u)
#define Opamp_Half_Vdd_MX_VP_AG4        (0x08u)
#define Opamp_Half_Vdd_MX_VP_AG5        (0x09u)
#define Opamp_Half_Vdd_MX_VP_AG6        (0x0Au)
#define Opamp_Half_Vdd_MX_VP_AG7        (0x08u)
#define Opamp_Half_Vdd_MX_VP_ABUS0      (0x0Cu)
#define Opamp_Half_Vdd_MX_VP_ABUS1      (0x0Du)
#define Opamp_Half_Vdd_MX_VP_ABUS2      (0x0Eu)
#define Opamp_Half_Vdd_MX_VP_ABUS3      (0x0Fu)

/* SW Analog Buffer Routing Switch Reg */

/* Bit Field  SW */
#define Opamp_Half_Vdd_SW_MASK          (0x07u) 
#define Opamp_Half_Vdd_SW_SWINP         (0x04u)  /* Enable positive input */
#define Opamp_Half_Vdd_SW_SWINN         (0x02u)  /* Enable negative input */
#define Opamp_Half_Vdd_SW_SWFOL         (0x01u)  /* Enable follower mode  */

/* Pump configuration register masks */
#define Opamp_Half_Vdd_PUMP_CR1_CLKSEL  (0x40u)
#define Opamp_Half_Vdd_PUMP_CR1_FORCE   (0x20u)
#define Opamp_Half_Vdd_PUMP_CR1_AUTO    (0x10u)

#endif /* CY_OPAMP_Opamp_Half_Vdd_H */



/* [] END OF FILE */
