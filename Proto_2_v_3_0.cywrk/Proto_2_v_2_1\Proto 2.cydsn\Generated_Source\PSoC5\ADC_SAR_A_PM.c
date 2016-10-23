/*******************************************************************************
* File Name: ADC_SAR_A.c
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

#include "ADC_SAR_A.h"


/***************************************
* Local data allocation
***************************************/

static ADC_SAR_A_BACKUP_STRUCT  ADC_SAR_A_backup =
{
    ADC_SAR_A_DISABLED
};
    


/*******************************************************************************
* Function Name: ADC_SAR_A_SaveConfig
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
void ADC_SAR_A_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_A_RestoreConfig
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
void ADC_SAR_A_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_A_Sleep
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
*  ADC_SAR_A_backup - modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_A_Sleep(void)
{
    if((ADC_SAR_A_PWRMGR_SAR_REG  & ADC_SAR_A_ACT_PWR_SAR_EN) != 0u) 
    {
        ADC_SAR_A_backup.enableState = ADC_SAR_A_ENABLED;
        if((ADC_SAR_A_SAR_CSR0_REG & ADC_SAR_A_SAR_SOF_START_CONV) != 0u)
        {
            ADC_SAR_A_backup.enableState |= ADC_SAR_A_STARTED;
        }
        ADC_SAR_A_Stop();
    }
    else
    {
        ADC_SAR_A_backup.enableState = ADC_SAR_A_DISABLED;
    }
    /*ADC_SAR_A_SaveConfig();*/
}


/*******************************************************************************
* Function Name: ADC_SAR_A_Wakeup
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
*  ADC_SAR_A_backup - used. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_A_Wakeup(void)
{
    /*ADC_SAR_A_RestoreConfig();*/
    if(ADC_SAR_A_backup.enableState != ADC_SAR_A_DISABLED)
    {
        ADC_SAR_A_Enable();
        if((ADC_SAR_A_backup.enableState & ADC_SAR_A_STARTED) != 0u)
        {
            ADC_SAR_A_StartConvert();
        }
    } 
}


/* [] END OF FILE */
