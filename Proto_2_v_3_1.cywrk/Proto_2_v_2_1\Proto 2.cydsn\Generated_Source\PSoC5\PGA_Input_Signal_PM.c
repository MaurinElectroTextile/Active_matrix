/*******************************************************************************
* File Name: PGA_Input_Signal_PM.c  
* Version 1.50
*
* Description:
*  This file provides the power manager source code to the API for the PGA 
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PGA_Input_Signal.h"

static PGA_Input_Signal_BACKUP_STRUCT  PGA_Input_Signal_backup;


/*******************************************************************************  
* Function Name: PGA_Input_Signal_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current user register configuration.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PGA_Input_Signal_SaveConfig(void)
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: PGA_Input_Signal_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations.
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
void PGA_Input_Signal_RestoreConfig(void)
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: PGA_Input_Signal_Sleep
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
*  PGA_Input_Signal_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void PGA_Input_Signal_Sleep(void)
{
    /* Save PGA enable state */
    if(PGA_Input_Signal_ACT_PWR_EN == (PGA_Input_Signal_PM_ACT_CFG_REG & PGA_Input_Signal_ACT_PWR_EN))
    {
        /* Component is enabled */
        PGA_Input_Signal_backup.enableState = 1u;
    }
    else
    {
        /* Component is disabled */
        PGA_Input_Signal_backup.enableState = 0u;
    }

    /* Stop the configuration */
    PGA_Input_Signal_Stop();

    /* Save the configuration */
    PGA_Input_Signal_SaveConfig();
}


/*******************************************************************************  
* Function Name: PGA_Input_Signal_Wakeup
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
*  PGA_Input_Signal_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
* 
*******************************************************************************/
void PGA_Input_Signal_Wakeup(void) 
{
    /* Restore the configurations */
    PGA_Input_Signal_RestoreConfig();

     /* Enable's the component operation */
    if(PGA_Input_Signal_backup.enableState == 1u)
    {
        PGA_Input_Signal_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
