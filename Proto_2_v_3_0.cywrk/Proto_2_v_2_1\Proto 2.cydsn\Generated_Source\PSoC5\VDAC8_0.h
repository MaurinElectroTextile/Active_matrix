/*******************************************************************************
* File Name: VDAC8_0.h  
* Version 1.60
*
*  Description:
*    This file contains the function prototypes and constants used in
*    the 8-bit Voltage DAC (vDAC8) User Module.
*
*   Note:
*     
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_VDAC8_VDAC8_0_H) 
#define CY_VDAC8_VDAC8_0_H

#include "cytypes.h"
#include "cyfitter.h"


/* PSoC3 ES2 or early */
#define VDAC8_0_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))                    
/* PSoC5 ES1 or early */
#define VDAC8_0_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define VDAC8_0_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define VDAC8_0_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct VDAC8_0_backupStruct
{
    uint8 enableState; 
	uint8 data_value;
}VDAC8_0_backupStruct;


/***************************************
*        Function Prototypes 
***************************************/

void VDAC8_0_Start(void);
void VDAC8_0_Stop(void)            ;
void VDAC8_0_SetSpeed(uint8 speed) ;
void VDAC8_0_SetRange(uint8 range) ;
void VDAC8_0_SetValue(uint8 value) ;
void VDAC8_0_DacTrim(void)         ;
void VDAC8_0_Init(void)            ;
void VDAC8_0_Enable(void)          ;
void VDAC8_0_SaveConfig(void);
void VDAC8_0_RestoreConfig(void);
void VDAC8_0_Sleep(void);
void VDAC8_0_Wakeup(void)          ;
  
  
/***************************************
*            API Constants
***************************************/

/* SetRange constants */

#define VDAC8_0_RANGE_1V       0x00u
#define VDAC8_0_RANGE_4V       0x04u


/* Power setting for Start API  */
#define VDAC8_0_LOWSPEED       0x00u
#define VDAC8_0_HIGHSPEED      0x02u


/***************************************
*  Initialization Parameter Constants
***************************************/

#define VDAC8_0_DEFAULT_RANGE    0     /* Default DAC range */
#define VDAC8_0_DEFAULT_SPEED    0     /* Default DAC speed */
#define VDAC8_0_DEFAULT_CNTL     0x00u             /* Default Control */
#define VDAC8_0_DEFAULT_STRB     0    /* Default Strobe mode */
#define VDAC8_0_DEFAULT_DATA     100  /* Initial DAC value */
#define VDAC8_0_DEFAULT_DATA_SRC 0    /* Default Data Source */


/***************************************
*              Registers        
***************************************/

#define VDAC8_0_CR0         (* (reg8 *) VDAC8_0_viDAC8__CR0 )
#define VDAC8_0_CR1         (* (reg8 *) VDAC8_0_viDAC8__CR1 )
#define VDAC8_0_Data        (* (reg8 *) VDAC8_0_viDAC8__D )
#define VDAC8_0_Data_PTR    (  (reg8 *) VDAC8_0_viDAC8__D )
#define VDAC8_0_Strobe      (* (reg8 *) VDAC8_0_viDAC8__STROBE )
#define VDAC8_0_SW0         (* (reg8 *) VDAC8_0_viDAC8__SW0 )
#define VDAC8_0_SW2         (* (reg8 *) VDAC8_0_viDAC8__SW2 )
#define VDAC8_0_SW3         (* (reg8 *) VDAC8_0_viDAC8__SW3 )
#define VDAC8_0_SW4         (* (reg8 *) VDAC8_0_viDAC8__SW4 )
#define VDAC8_0_TR          (* (reg8 *) VDAC8_0_viDAC8__TR )
#define VDAC8_0_PWRMGR      (* (reg8 *) VDAC8_0_viDAC8__PM_ACT_CFG )  /* Power manager */
#define VDAC8_0_STBY_PWRMGR (* (reg8 *) VDAC8_0_viDAC8__PM_STBY_CFG )  /* Standby Power manager */


/***************************************
*         Register Constants       
***************************************/

/* CR0 vDac Control Register 0 definitions */

/* Bit Field  DAC_HS_MODE                  */
#define VDAC8_0_HS_MASK        0x02u
#define VDAC8_0_HS_LOWPOWER    0x00u
#define VDAC8_0_HS_HIGHSPEED   0x02u

/* Bit Field  DAC_MODE                  */
#define VDAC8_0_MODE_MASK      0x10u
#define VDAC8_0_MODE_V         0x00u
#define VDAC8_0_MODE_I         0x10u

/* Bit Field  DAC_RANGE                  */
#define VDAC8_0_RANGE_MASK     0x0Cu
#define VDAC8_0_RANGE_0        0x00u
#define VDAC8_0_RANGE_1        0x04u

/* CR1 iDac Control Register 1 definitions */

/* Bit Field  DAC_MX_DATA                  */
#define VDAC8_0_SRC_MASK       0x20u
#define VDAC8_0_SRC_REG        0x00u
#define VDAC8_0_SRC_UDB        0x20u

/* This bit enable reset from UDB array      */
#define VDAC8_0_RESET_MASK     0x10u
#define VDAC8_0_RESET_ENABLE   0x10u
#define VDAC8_0_RESET_DISABLE  0x00u

/* This bit enables data from DAC bus      */
#define VDAC8_0_DACBUS_MASK     0x20u
#define VDAC8_0_DACBUS_ENABLE   0x20u
#define VDAC8_0_DACBUS_DISABLE  0x00u

/* DAC STROBE Strobe Control Register definitions */

/* Bit Field  DAC_MX_STROBE                  */
#define VDAC8_0_STRB_MASK     0x08u
#define VDAC8_0_STRB_EN       0x08u
#define VDAC8_0_STRB_DIS      0x00u

/* PM_ACT_CFG (Active Power Mode CFG Register)     */ 
#define VDAC8_0_ACT_PWR_EN   VDAC8_0_viDAC8__PM_ACT_MSK /* Power enable mask */ 
#define VDAC8_0_STBY_PWR_EN  VDAC8_0_viDAC8__PM_STBY_MSK  /* Standby Power enable mask */


/***********************************************************************************
*              Trim    
* Note - VDAC trim values are stored in the "Customer Table" area in 
* Row 1 of the Hidden Flash.  There are 8 bytes of trim data for each VDAC block.
* The values are:
*       I Gain offset, min range, Sourcing
*       I Gain offset, min range, Sinking
*       I Gain offset, med range, Sourcing
*       I Gain offset, med range, Sinking
*       I Gain offset, max range, Sourcing
*       I Gain offset, max range, Sinking
*       V Gain offset, 1V range
*       V Gain offset, 4V range
*
* The data set for the 4 VDACs are arranged using a left side/right side approach:
*   Left 0, Left 1, Right 0, Right 1.
* When mapped to the VDAC0 thru VDAC3 as:
*   VDAC 0, VDAC 2, VDAC 1, VDAC 3
************************************************************************************/
#define VDAC8_0_TRIM_M7_1V_RNG_OFFSET  0x06u
#define VDAC8_0_TRIM_M8_4V_RNG_OFFSET  0x07u

#define VDAC8_0_DAC_TRIM_BASE     (VDAC8_0_viDAC8__TRIM__M1)

#endif /* CY_VDAC8_VDAC8_0_H  */


/* [] END OF FILE */


