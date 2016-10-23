/*******************************************************************************
* File Name: PGA_Stage1_B_PM.c  
* Version 1.60
*
* Description:
*  This file provides the power manager source code to the API for the 
*  inverting PGA component.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "PGA_Stage1_B.h"

static PGA_Stage1_B_BACKUP_STRUCT  PGA_Stage1_B_backup;


/*******************************************************************************  
* Function Name: PGA_Stage1_B_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Reentrant:
*  No
*
*******************************************************************************/
void PGA_Stage1_B_SaveConfig(void)
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: PGA_Stage1_B_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations.
* 
* Parameters:
*  void.
* 
* Return:
*  void
*
* Reentrant:
*  No
*
*******************************************************************************/
void PGA_Stage1_B_RestoreConfig(void)
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: PGA_Stage1_B_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PGA_Stage1_B_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void PGA_Stage1_B_Sleep(void)
{
    /* Save PGA_Inv enable state */
    if(PGA_Stage1_B_ACT_PWR_EN == (PGA_Stage1_B_PM_ACT_CFG_REG & PGA_Stage1_B_ACT_PWR_EN))
    {
        /* Component is enabled */
        PGA_Stage1_B_backup.enableState = 1u;
    }
    else
    {
        /* Component is disabled */
        PGA_Stage1_B_backup.enableState = 0u;
    }
    /* Stop the configuration */
    PGA_Stage1_B_Stop();

    /* Save the configurations */
    PGA_Stage1_B_SaveConfig();
}


/*******************************************************************************  
* Function Name: PGA_Stage1_B_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  void
*
* Global variables:
*  PGA_Stage1_B_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void PGA_Stage1_B_Wakeup(void) 
{
    /* Restore the register configurations */
    PGA_Stage1_B_RestoreConfig();
    
    /* Enable's the component operation */
    if(PGA_Stage1_B_backup.enableState == 1u)
    {
        PGA_Stage1_B_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
