/*******************************************************************************
* File Name: Filter_PM.c
* Version 1.50
*
* Description:
*  This file provides the power managaer API source code for the FILT component.
*
* Note:
*  
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Filter_PVT.h"

static Filter_BACKUP_STRUCT  Filter_backup = 
{
    Filter_DISABLED,
    Filter_RUN_MASK,
    
};


/*******************************************************************************
* Function Name: Filter_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Filter_backup:  This global structure variable is used to store 
*  configuration registers which are non retention whenever user wants to go 
*  sleep mode by calling Sleep() API.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_SaveConfig(void)
{
    Filter_backup.cr = Filter_CR_REG;
    Filter_backup.sr = Filter_SR_REG;
    Filter_backup.sema = Filter_SEMA_REG;
    cymemcpy(Filter_backup.acu_ram, Filter_ACU_RAM, Filter_ACU_RAM_SIZE); 
}


/*******************************************************************************
* Function Name: Filter_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Filter_backup:  This global structure variable is used to restore 
*  configuration registers which are non retention whenever user wants to switch 
*  to active power mode by calling Wakeup() API.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_RestoreConfig(void) 
{
    Filter_CR_REG = Filter_backup.cr;
    Filter_SR_REG = Filter_backup.sr;
    Filter_SEMA_REG = Filter_backup.sema;
    cymemcpy(Filter_ACU_RAM, Filter_backup.acu_ram, Filter_ACU_RAM_SIZE); 
}


/*******************************************************************************
* Function Name: Filter_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Filter_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_Sleep(void)
{
    /* Save Filter enable state */
    if(Filter_PM_ACT_MSK == (Filter_PM_ACT_CFG_REG & Filter_PM_ACT_MSK))
    {
        /* Component is enabled */
        Filter_backup.enableState = Filter_ENABLED;
    }
    else
    {
        /* Component is disabled */
        Filter_backup.enableState = Filter_DISABLED;
    }

    /* Stop the configuration */
    Filter_Stop();

    /* Save the configuration */
    Filter_SaveConfig();
}


/*******************************************************************************
* Function Name: Filter_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Filter_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_Wakeup(void) 
{
    /* Restore the configuration */
    Filter_RestoreConfig();
    
    /* Enable's the component operation */
    if(Filter_backup.enableState == Filter_ENABLED)
    {
        Filter_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
