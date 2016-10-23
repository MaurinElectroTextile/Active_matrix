/*******************************************************************************
* File Name: ADC_SAR_B.c
* Version 1.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "ADC_SAR_B.h"


/***************************************
* Local data allocation
***************************************/

static ADC_SAR_B_BACKUP_STRUCT  ADC_SAR_B_backup =
{
    ADC_SAR_B_DISABLED
};
    


/*******************************************************************************
* Function Name: ADC_SAR_B_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_B_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_B_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  ADC_SAR_B_backup - modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_Sleep(void)
{
    if((ADC_SAR_B_PWRMGR_SAR_REG  & ADC_SAR_B_ACT_PWR_SAR_EN) != 0u) 
    {
        ADC_SAR_B_backup.enableState = ADC_SAR_B_ENABLED;
        if((ADC_SAR_B_SAR_CSR0_REG & ADC_SAR_B_SAR_SOF_START_CONV) != 0u)
        {
            ADC_SAR_B_backup.enableState |= ADC_SAR_B_STARTED;
        }
        ADC_SAR_B_Stop();
    }
    else
    {
        ADC_SAR_B_backup.enableState = ADC_SAR_B_DISABLED;
    }
    /*ADC_SAR_B_SaveConfig();*/
}


/*******************************************************************************
* Function Name: ADC_SAR_B_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  ADC_SAR_B_backup - used. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_Wakeup(void)
{
    /*ADC_SAR_B_RestoreConfig();*/
    if(ADC_SAR_B_backup.enableState != ADC_SAR_B_DISABLED)
    {
        ADC_SAR_B_Enable();
        if((ADC_SAR_B_backup.enableState & ADC_SAR_B_STARTED) != 0u)
        {
            ADC_SAR_B_StartConvert();
        }
    } 
}


/* [] END OF FILE */
