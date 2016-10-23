/*******************************************************************************
* File Name: Opamp_Sine_In.c
* Version 1.50
*
* Description:
*  This file provides the source code to the API for the OpAmp (Analog Buffer) 
*  Component.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Opamp_Sine_In.h"

uint8 Opamp_Sine_In_initVar = 0u;


/*******************************************************************************   
* Function Name: Opamp_Sine_In_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  Opamp_Sine_In_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  Yes
* 
*******************************************************************************/
void Opamp_Sine_In_Init(void) 
{
    Opamp_Sine_In_SetPower(Opamp_Sine_In_DEFAULT_POWER);

    /* Set follower mode if selected */
    Opamp_Sine_In_SW_REG |= Opamp_Sine_In_DEFAULT_MODE;
}


/*******************************************************************************   
* Function Name: Opamp_Sine_In_Enable
********************************************************************************
*
* Summary:
*  Enables the OpAmp block operation
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  Yes
* 
*******************************************************************************/
void Opamp_Sine_In_Enable(void) 
{
    /* Enable negative charge pumps in ANIF */
    Opamp_Sine_In_PUMP_CR1_REG  |= (Opamp_Sine_In_PUMP_CR1_CLKSEL | Opamp_Sine_In_PUMP_CR1_FORCE);
    
    /* Enable power to buffer in active mode */
    Opamp_Sine_In_PM_ACT_CFG_REG |= Opamp_Sine_In_ACT_PWR_EN;
    
    /* Enable power to buffer in alternative active mode */
    Opamp_Sine_In_PM_STBY_CFG_REG |= Opamp_Sine_In_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name:   Opamp_Sine_In_Start
********************************************************************************
*
* Summary:
*  The start function initializes the Analog Buffer with the default values, and 
*  sets the power to the given level.  A power level of 0, is the same as 
*  executing the stop function.
*
* Parameters:
*  void
*
* Return:  
*  void
*
* Global variables:
*  Opamp_Sine_In_initVar:  Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Sine_In_Start(void) 
{
    if(Opamp_Sine_In_initVar == 0u)
    {
        Opamp_Sine_In_initVar = 1u;
        Opamp_Sine_In_Init();
    }
    
    Opamp_Sine_In_Enable();

}


/*******************************************************************************
* Function Name: Opamp_Sine_In_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:  
*  void
*
* Return:  
*  void
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void Opamp_Sine_In_Stop(void) 
{
    /* Disable negative charge pumps for ANIF only if the one ABuf is turned ON */
    if(Opamp_Sine_In_PM_ACT_CFG_REG == Opamp_Sine_In_ACT_PWR_EN)
    {
        Opamp_Sine_In_PUMP_CR1_REG &= ~(Opamp_Sine_In_PUMP_CR1_CLKSEL | Opamp_Sine_In_PUMP_CR1_FORCE);
    }
    
    /* Disable power to buffer in active mode template */
    Opamp_Sine_In_PM_ACT_CFG_REG &= ~Opamp_Sine_In_ACT_PWR_EN;

    /* Disable power to buffer in alternative active mode template */
    Opamp_Sine_In_PM_STBY_CFG_REG &= ~Opamp_Sine_In_STBY_PWR_EN;
}


/*******************************************************************************
* Funciton Name:   Opamp_Sine_In_SetPower
********************************************************************************
*
* Summary:
*  Sets power level of Analog buffer.
*
* Parameters: 
*  power:  Sets power level between low (1) and high power (3).
*
* Return:  
*  void
*
* Reentrant:
*  Yes
*
**********************************************************************************/
void Opamp_Sine_In_SetPower(uint8 power) 
{
    Opamp_Sine_In_CR_REG = ((Opamp_Sine_In_CR_REG & ~Opamp_Sine_In_PWR_MASK) | 
                               ( power & Opamp_Sine_In_PWR_MASK));   /* Set device power */
}


/* [] END OF FILE */
