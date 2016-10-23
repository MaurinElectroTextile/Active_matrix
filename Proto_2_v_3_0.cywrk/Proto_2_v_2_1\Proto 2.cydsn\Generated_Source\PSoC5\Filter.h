/*******************************************************************************
* File Name: Filter.h
* Version 1.50
*
* Description:
*  This header file contains definitions associated with the FILT component.
*
* Note:
* 
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Filter_H) /* FILT Header File */
#define Filter_H

#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* PSoC3 ES2 or early */
#define Filter_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES1 or early */
#define Filter_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define Filter_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define Filter_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


/***************************************
*     Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct _Filter_backupStruct 
{
    uint8 enableState;
    uint8 cr;
    uint8 sr;
    uint8 sema;
    uint8 acu_ram[64];
}   Filter_BACKUP_STRUCT;


/***************************************
* FILT component function prototypes.
****************************************/

void   Filter_Start(void);
void   Filter_Stop(void) ;
uint8 Filter_Read8(uint8 channel) ;
uint16 Filter_Read16(uint8 channel) ;
uint32 Filter_Read24(uint8 channel) ;
void Filter_Write8(uint8 channel, uint8 sample) ;
void Filter_Write16(uint8 channel, uint16 sample) ;
void Filter_Write24(uint8 channel, uint32 sample) ;
void Filter_Sleep(void);
void Filter_Wakeup(void) ;
void Filter_SaveConfig(void);
void Filter_RestoreConfig(void) ;
void Filter_Init(void) ;
void Filter_Enable(void) ;


/*****************************************
* FILT component API Constants.
******************************************/

/* Channel Definitions */
#define Filter_CHANNEL_A             (0u)
#define Filter_CHANNEL_B             (1u)

#define Filter_CHANNEL_A_INTR        (0x08u)
#define Filter_CHANNEL_B_INTR        (0x10u)

#define Filter_ALL_INTR              (0xf8u)

#define Filter_SIGN_BIT              (0x00800000u)
#define Filter_SIGN_BYTE             (0xFF000000u)

/* Component's enable/disable state */
#define Filter_ENABLED               (0x01u)
#define Filter_DISABLED              (0x00u)


/*******************************************************************************
* FILT component macros.
*******************************************************************************/

#define Filter_ClearInterruptSource() \
    do { \
    Filter_SR_REG = Filter_ALL_INTR; \
    } while (0)

#define Filter_IsInterruptChannelA() \
    (Filter_SR_REG & Filter_CHANNEL_A_INTR)

#define Filter_IsInterruptChannelB() \
    (Filter_SR_REG & Filter_CHANNEL_B_INTR)


/*******************************************************************************
* FILT component DFB registers.
*******************************************************************************/

/* DFB Status register */
#define Filter_SR_REG             (* (reg8 *) Filter_DFB__SR)
#define Filter_SR_PTR             (  (reg8 *) Filter_DFB__SR)

/* DFB Control register */
#define Filter_CR_REG             (* (reg8 *) Filter_DFB__CR)
#define Filter_CR_PTR             (  (reg8 *) Filter_DFB__CR)

#endif /* End FILT Header File */


/* [] END OF FILE */
