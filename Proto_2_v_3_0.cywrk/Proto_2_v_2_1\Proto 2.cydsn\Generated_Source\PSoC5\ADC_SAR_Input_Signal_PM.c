/*******************************************************************************
* File Name: ADC_SAR_Input_Signal.c
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

#include "ADC_SAR_Input_Signal.h"


/***************************************
* Local data allocation
***************************************/

static ADC_SAR_Input_Signal_BACKUP_STRUCT  ADC_SAR_Input_Signal_backup =
{
    ADC_SAR_Input_Signal_DISABLED
};
    


/*******************************************************************************
* Function Name: ADC_SAR_Input_Signal_SaveConfig
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
void ADC_SAR_Input_Signal_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_Input_Signal_RestoreConfig
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
void ADC_SAR_Input_Signal_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_SAR_Input_Signal_Sleep
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
*  ADC_SAR_Input_Signal_backup - modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Input_Signal_Sleep(void)
{
    if((ADC_SAR_Input_Signal_PWRMGR_SAR_REG  & ADC_SAR_Input_Signal_ACT_PWR_SAR_EN) != 0u) 
    {
        ADC_SAR_Input_Signal_backup.enableState = ADC_SAR_Input_Signal_ENABLED;
        if((ADC_SAR_Input_Signal_SAR_CSR0_REG & ADC_SAR_Input_Signal_SAR_SOF_START_CONV) != 0u)
        {
            ADC_SAR_Input_Signal_backup.enableState |= ADC_SAR_Input_Signal_STARTED;
        }
        ADC_SAR_Input_Signal_Stop();
    }
    else
    {
        ADC_SAR_Input_Signal_backup.enableState = ADC_SAR_Input_Signal_DISABLED;
    }
    /*ADC_SAR_Input_Signal_SaveConfig();*/
}


/*******************************************************************************
* Function Name: ADC_SAR_Input_Signal_Wakeup
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
*  ADC_SAR_Input_Signal_backup - used. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_Input_Signal_Wakeup(void)
{
    /*ADC_SAR_Input_Signal_RestoreConfig();*/
    if(ADC_SAR_Input_Signal_backup.enableState != ADC_SAR_Input_Signal_DISABLED)
    {
        ADC_SAR_Input_Signal_Enable();
        if((ADC_SAR_Input_Signal_backup.enableState & ADC_SAR_Input_Signal_STARTED) != 0u)
        {
            ADC_SAR_Input_Signal_StartConvert();
        }
    } 
}


/* [] END OF FILE */
