/*******************************************************************************
* File Name: Opamp_Half_Vdd.c
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

#include "Opamp_Half_Vdd.h"

uint8 Opamp_Half_Vdd_initVar = 0u;


/*******************************************************************************   
* Function Name: Opamp_Half_Vdd_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  Opamp_Half_Vdd_Start().
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
void Opamp_Half_Vdd_Init(void) 
{
    Opamp_Half_Vdd_SetPower(Opamp_Half_Vdd_DEFAULT_POWER);

    /* Set follower mode if selected */
    Opamp_Half_Vdd_SW_REG |= Opamp_Half_Vdd_DEFAULT_MODE;
}


/*******************************************************************************   
* Function Name: Opamp_Half_Vdd_Enable
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
void Opamp_Half_Vdd_Enable(void) 
{
    /* Enable negative charge pumps in ANIF */
    Opamp_Half_Vdd_PUMP_CR1_REG  |= (Opamp_Half_Vdd_PUMP_CR1_CLKSEL | Opamp_Half_Vdd_PUMP_CR1_FORCE);
    
    /* Enable power to buffer in active mode */
    Opamp_Half_Vdd_PM_ACT_CFG_REG |= Opamp_Half_Vdd_ACT_PWR_EN;
    
    /* Enable power to buffer in alternative active mode */
    Opamp_Half_Vdd_PM_STBY_CFG_REG |= Opamp_Half_Vdd_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name:   Opamp_Half_Vdd_Start
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
*  Opamp_Half_Vdd_initVar:  Used to check the initial configuration, modified 
*  when this function is called for the first time.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Half_Vdd_Start(void) 
{
    if(Opamp_Half_Vdd_initVar == 0u)
    {
        Opamp_Half_Vdd_initVar = 1u;
        Opamp_Half_Vdd_Init();
    }
    
    Opamp_Half_Vdd_Enable();

}


/*******************************************************************************
* Function Name: Opamp_Half_Vdd_Stop
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
void Opamp_Half_Vdd_Stop(void) 
{
    /* Disable negative charge pumps for ANIF only if the one ABuf is turned ON */
    if(Opamp_Half_Vdd_PM_ACT_CFG_REG == Opamp_Half_Vdd_ACT_PWR_EN)
    {
        Opamp_Half_Vdd_PUMP_CR1_REG &= ~(Opamp_Half_Vdd_PUMP_CR1_CLKSEL | Opamp_Half_Vdd_PUMP_CR1_FORCE);
    }
    
    /* Disable power to buffer in active mode template */
    Opamp_Half_Vdd_PM_ACT_CFG_REG &= ~Opamp_Half_Vdd_ACT_PWR_EN;

    /* Disable power to buffer in alternative active mode template */
    Opamp_Half_Vdd_PM_STBY_CFG_REG &= ~Opamp_Half_Vdd_STBY_PWR_EN;
}


/*******************************************************************************
* Funciton Name:   Opamp_Half_Vdd_SetPower
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
void Opamp_Half_Vdd_SetPower(uint8 power) 
{
    Opamp_Half_Vdd_CR_REG = ((Opamp_Half_Vdd_CR_REG & ~Opamp_Half_Vdd_PWR_MASK) | 
                               ( power & Opamp_Half_Vdd_PWR_MASK));   /* Set device power */
}


/* [] END OF FILE */
