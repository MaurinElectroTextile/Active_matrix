/*******************************************************************************
* File Name: PGA_HPF.c  
* Version 1.50
*
* Description:
*  This file provides the source code to the API for the inverting PGA 
*  Component.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "PGA_HPF.h"

#if (PGA_HPF_ACLK_ENABLE)
    #include "PGA_HPF_ScBoostClk.h"
#endif /* PGA_HPF_ACLK_ENABLE */

/* Constant array for gain settings */
const uint8 PGA_HPF_GainArray[9] = { 
    (PGA_HPF_PGA_INV_GAIN_01 | PGA_HPF_BIAS_LOW), /* G=-1  */
    (PGA_HPF_PGA_INV_GAIN_03 | PGA_HPF_BIAS_LOW), /* G=-3  */
    (PGA_HPF_PGA_INV_GAIN_07 | PGA_HPF_BIAS_LOW), /* G=-7  */
    (PGA_HPF_PGA_INV_GAIN_15 | PGA_HPF_BIAS_LOW), /* G=-15 */
    (PGA_HPF_PGA_INV_GAIN_22 | PGA_HPF_BIAS_LOW), /* G=-22 */
    (PGA_HPF_PGA_INV_GAIN_24 | PGA_HPF_BIAS_LOW), /* G=-24 */
    (PGA_HPF_PGA_INV_GAIN_31 | PGA_HPF_BIAS_LOW), /* G=-31 */
    (PGA_HPF_PGA_INV_GAIN_47 | PGA_HPF_BIAS_LOW), /* G=-47 */
    (PGA_HPF_PGA_INV_GAIN_49 | PGA_HPF_BIAS_LOW)  /* G=-49 */
};

/* Constant array for gain compenstion settings */
const uint8 PGA_HPF_GainComp[9] = { 
    ( PGA_HPF_COMP_4P35PF  | ( PGA_HPF_REDC_00 >> 2 )), /* G=-1  */
    ( PGA_HPF_COMP_4P35PF  | ( PGA_HPF_REDC_01 >> 2 )), /* G=-3  */
    ( PGA_HPF_COMP_3P0PF   | ( PGA_HPF_REDC_01 >> 2 )), /* G=-7  */
    ( PGA_HPF_COMP_3P0PF   | ( PGA_HPF_REDC_01 >> 2 )), /* G=-15 */
    ( PGA_HPF_COMP_3P6PF   | ( PGA_HPF_REDC_01 >> 2 )), /* G=-22 */
    ( PGA_HPF_COMP_3P0PF   | ( PGA_HPF_REDC_10 >> 2 )), /* G=-24 */
    ( PGA_HPF_COMP_3P6PF   | ( PGA_HPF_REDC_11 >> 2 )), /* G=-31 */
    ( PGA_HPF_COMP_3P6PF   | ( PGA_HPF_REDC_00 >> 2 )), /* G=-47 */
    ( PGA_HPF_COMP_3P6PF   | ( PGA_HPF_REDC_00 >> 2 ))  /* G=-49 */
};

uint8 PGA_HPF_initVar = 0u;


/*******************************************************************************   
* Function Name: PGA_HPF_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PGA_HPF_Start().
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
void PGA_HPF_Init(void) 
{
    /* Set PGA mode */
    PGA_HPF_CR0_REG = PGA_HPF_MODE_PGA;  

    /* Set inverting PGA mode  and reference mode */
    PGA_HPF_CR1_REG &= ~PGA_HPF_PGA_INV;  

    /* Set gain and compensation */
    PGA_HPF_SetGain(PGA_HPF_DEFAULT_GAIN);

    /* Set power */
    PGA_HPF_SetPower(PGA_HPF_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: PGA_HPF_Enable
********************************************************************************
*
* Summary:
*  Enables the inverting PGA block operation
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
void PGA_HPF_Enable(void) 
{
    /* If a boost clock is required  */
    #if (PGA_HPF_ACLK_ENABLE)
        PGA_HPF_ACLK_PM_ACT_CFG_REG |= PGA_HPF_ACLK_ACT_PWR_EN;
        PGA_HPF_ACLK_PM_STBY_CFG_REG |= PGA_HPF_ACLK_STBY_PWR_EN;
        PGA_HPF_ScBoostClk_Enable();  /* turn on boost pump */
        PGA_HPF_BST_REG |= 0x08;
    #endif /* PGA_HPF_ACLK_ENABLE */
    
    /* PSoC3 ES2 or early, PSoC5 ES1 */
    #if (PGA_HPF_PSOC3_ES2 || PGA_HPF_PSOC5_ES1)
        /* Enable Pump only if voltage is below threshold */
        if (PGA_HPF_CYDEV_VDDA_MV < PGA_HPF_VDDA_THRESHOLD_MV)
        {
            PGA_HPF_SC_MISC_REG |= PGA_HPF_PUMP_FORCE;
        }
    /* PSoC3 ES3 or later, PSoC5 ES2 or later */
    #elif (PGA_HPF_PSOC3_ES3 || PGA_HPF_PSOC5_ES2)
        /* Enable charge Pump clock for SC block */
        PGA_HPF_PUMP_CR1_REG |= PGA_HPF_PUMP_CR1_SC_CLKSEL;
        /* Enable Pump only if voltage is below threshold */
        if (PGA_HPF_CYDEV_VDDA_MV < PGA_HPF_VDDA_THRESHOLD_MV)
        {
            PGA_HPF_SC_MISC_REG |= PGA_HPF_PUMP_FORCE;
        }
    #endif /* PGA_HPF_PSOC3_ES2 || PGA_HPF_PSOC5_ES1 */

    /* Enable power to the Amp in Active mode */
    PGA_HPF_PM_ACT_CFG_REG |= PGA_HPF_ACT_PWR_EN;

    /* Enable power to the Amp in Alternative active mode  */
    PGA_HPF_PM_STBY_CFG_REG |= PGA_HPF_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name: PGA_HPF_Start
********************************************************************************
*
* Summary:
*  The start function initializes the inverting PGA with the default values, and sets
*  the power to the given level.  A power level of 0, is the same as executing
*  the stop function.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Theory:
*  This function modifies the global variable PGA_HPF_initVar: which 
*  defines where component's configuration are taken from the customizer 
*  (equals 0) or parameters changed by API will be used instead (equals 1).
*
* Reentrant: 
*  No
*
*******************************************************************************/
void PGA_HPF_Start(void) 
{
    if(PGA_HPF_initVar == 0u)
    {
        PGA_HPF_initVar = 1u;
        PGA_HPF_Init();
    }
    PGA_HPF_Enable();
}


/*******************************************************************************
* Function Name: PGA_HPF_Stop
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
*  No
*
*******************************************************************************/
void PGA_HPF_Stop(void) 
{
    /* Disble power to the Amp in Active mode*/
    PGA_HPF_PM_ACT_CFG_REG &= ~PGA_HPF_ACT_PWR_EN;

    /* Disble power to the Amp in Alternative active mode*/
    PGA_HPF_PM_STBY_CFG_REG &= ~PGA_HPF_STBY_PWR_EN;

    /* If a boost clock is required  */
    /* Disable and power down boost clock */
    #if (PGA_HPF_ACLK_ENABLE)
        PGA_HPF_ACLK_PM_ACT_CFG_REG &= ~PGA_HPF_ACLK_ACT_PWR_EN;
        PGA_HPF_ACLK_PM_STBY_CFG_REG &= ~PGA_HPF_ACLK_STBY_PWR_EN;
        PGA_HPF_ScBoostClk_Disable();  
    #endif /* PGA_HPF_ACLK_ENABLE */
}


/*******************************************************************************
* Function Name: PGA_HPF_SetPower
********************************************************************************
*
* Summary:
*  Set the power of the inverting PGA
*
* Parameters:  
*  power:  Sets power level between (0) and (3) high power
*
* Return: 
*  void 
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void PGA_HPF_SetPower(uint8 power) 
{
    uint8 tmpCR;

    tmpCR = PGA_HPF_CR1_REG & ~PGA_HPF_DRIVE_MASK;
    tmpCR |= (power & PGA_HPF_DRIVE_MASK);
    PGA_HPF_CR1_REG = tmpCR;
}


/*******************************************************************************
* Function Name: PGA_HPF_SetGain
********************************************************************************
*
* Summary:
*  This function sets values of the input and feedback resistors to set a 
*  specific gain of the amplifier.
*
* Parameters:  
*  gain:  Sets gain of amplifier.     
*
* Return: 
*  void
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void PGA_HPF_SetGain(uint8 gain) 
{
    /* Only set new gain if it is a valid gain */
    if( gain <= PGA_HPF_GAIN_MAX)
    {
        /* Clear resistors, redc, and bias */
        PGA_HPF_CR2_REG &= ~(PGA_HPF_RVAL_MASK | PGA_HPF_R20_40B_MASK | 
                                PGA_HPF_REDC_MASK | PGA_HPF_BIAS_MASK );

        /* Set gain value resistors, redc comp, and bias */
        PGA_HPF_CR2_REG |= (PGA_HPF_GainArray[gain] |
                                ( (PGA_HPF_GainComp[gain] << 2 ) & PGA_HPF_REDC_MASK) );

        /* Clear sc_comp  */
        PGA_HPF_CR1_REG &= ~PGA_HPF_COMP_MASK;

        /* Set sc_comp  */
        PGA_HPF_CR1_REG |= ( PGA_HPF_GainComp[gain] | PGA_HPF_COMP_MASK );
    }
}


/* [] END OF FILE */
