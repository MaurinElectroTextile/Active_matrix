/*******************************************************************************
* File Name: OpampHPF.c  
* Version 1.50
*
* Description:
*  This file provides the source code to the API for the PGA 
*  User Module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "OpampHPF.h"

#if (OpampHPF_ACLK_ENABLE)
    #include "OpampHPF_ScBoostClk.h"
#endif /* OpampHPF_ACLK_ENABLE */

/* Constant array for gain settings */
const uint8 OpampHPF_GainArray[9] = { 
   (OpampHPF_RVAL_20K   | OpampHPF_R20_40B_40K | OpampHPF_BIAS_LOW), /* G=1  */
   (OpampHPF_RVAL_30K   | OpampHPF_R20_40B_40K | OpampHPF_BIAS_LOW), /* G=2  */
   (OpampHPF_RVAL_40K   | OpampHPF_R20_40B_40K | OpampHPF_BIAS_LOW), /* G=4  */
   (OpampHPF_RVAL_60K   | OpampHPF_R20_40B_40K | OpampHPF_BIAS_LOW), /* G=8  */
   (OpampHPF_RVAL_120K  | OpampHPF_R20_40B_40K | OpampHPF_BIAS_LOW), /* G=16 */
   (OpampHPF_RVAL_250K  | OpampHPF_R20_40B_40K | OpampHPF_BIAS_LOW), /* G=24 */
   (OpampHPF_RVAL_250K  | OpampHPF_R20_40B_20K | OpampHPF_BIAS_LOW), /* G=32 */
   (OpampHPF_RVAL_500K  | OpampHPF_R20_40B_20K | OpampHPF_BIAS_LOW), /* G=48 */
   (OpampHPF_RVAL_1000K | OpampHPF_R20_40B_20K | OpampHPF_BIAS_LOW)  /* G=50 */
};

/* Constant array for gain compenstion settings */
const uint8 OpampHPF_GainComp[9] = { 
   ( OpampHPF_COMP_4P35PF  | ( OpampHPF_REDC_00 >> 2 )), /* G=1  */
   ( OpampHPF_COMP_4P35PF  | ( OpampHPF_REDC_01 >> 2 )), /* G=2  */
   ( OpampHPF_COMP_3P0PF   | ( OpampHPF_REDC_01 >> 2 )), /* G=4  */
   ( OpampHPF_COMP_3P0PF   | ( OpampHPF_REDC_01 >> 2 )), /* G=8  */
   ( OpampHPF_COMP_3P6PF   | ( OpampHPF_REDC_01 >> 2 )), /* G=16 */
   ( OpampHPF_COMP_3P6PF   | ( OpampHPF_REDC_11 >> 2 )), /* G=24 */
   ( OpampHPF_COMP_3P6PF   | ( OpampHPF_REDC_11 >> 2 )), /* G=32 */
   ( OpampHPF_COMP_3P6PF   | ( OpampHPF_REDC_00 >> 2 )), /* G=48 */
   ( OpampHPF_COMP_3P6PF   | ( OpampHPF_REDC_00 >> 2 ))  /* G=50 */
};

uint8 OpampHPF_initVar = 0u;


/*******************************************************************************   
* Function Name: OpampHPF_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  OpampHPF_Start().
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
void OpampHPF_Init(void) 
{
    /* Set PGA mode */
    OpampHPF_CR0_REG = OpampHPF_MODE_PGA;  
    
    /* Set non-inverting PGA mode  and reference mode */
    OpampHPF_CR1_REG |= OpampHPF_PGA_NINV;  

    /* Set default gain and ref mode */
    OpampHPF_CR2_REG = OpampHPF_VREF_MODE;

    /* Set gain and compensation */
    OpampHPF_SetGain(OpampHPF_DEFAULT_GAIN);

    /* Set power */
    OpampHPF_SetPower(OpampHPF_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: OpampHPF_Enable
********************************************************************************
*
* Summary:
*  Enables the PGA block operation
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
void OpampHPF_Enable(void) 
{
    /* If a boost clock is required  */
    #if (OpampHPF_ACLK_ENABLE)
        OpampHPF_ACLK_PM_ACT_CFG_REG |= OpampHPF_ACLK_ACT_PWR_EN;
        OpampHPF_ACLK_PM_STBY_CFG_REG |= OpampHPF_ACLK_STBY_PWR_EN;
        OpampHPF_ScBoostClk_Enable();  /* turn on boost pump */
        OpampHPF_BST_REG |= 0x08;
    #endif /* OpampHPF_ACLK_ENABLE */
   
    /* PSoC3 ES2 or early, PSoC5 ES1 */
    #if (OpampHPF_PSOC3_ES2 || OpampHPF_PSOC5_ES1)
        /* Enable Pump only if voltage is below threshold */
        if (OpampHPF_CYDEV_VDDA_MV < OpampHPF_VDDA_THRESHOLD_MV)
        {
            OpampHPF_SC_MISC_REG |= OpampHPF_PUMP_FORCE;
        }
        
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (OpampHPF_PSOC3_ES3 || OpampHPF_PSOC5_ES2)
        /* Enable charge Pump clock for SC block */
        OpampHPF_PUMP_CR1_REG |= OpampHPF_PUMP_CR1_SC_CLKSEL;
        /* Enable Pump only if voltage is below threshold */
        if (OpampHPF_CYDEV_VDDA_MV < OpampHPF_VDDA_THRESHOLD_MV)
        {
            OpampHPF_SC_MISC_REG |= OpampHPF_PUMP_FORCE;
        }
    #endif /* OpampHPF_PSOC3_ES2 || OpampHPF_PSOC5_ES1 */
     
    /* Enable power to the Amp in Active mode*/
    OpampHPF_PM_ACT_CFG_REG |= OpampHPF_ACT_PWR_EN;

    /* Enable power to the Amp in Alternative Active mode*/
    OpampHPF_PM_STBY_CFG_REG |= OpampHPF_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name: OpampHPF_Start
********************************************************************************
*
* Summary:
*  The start function initializes the PGA with the default values, and sets
*  the power to the given level.  A power level of 0, is the same as executing
*  the stop function.
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
void OpampHPF_Start(void) 
{
    if(OpampHPF_initVar == 0u)
    {
        OpampHPF_Init();                    
        OpampHPF_initVar = 1u;  
    }
    
    OpampHPF_Enable();              
}


/*******************************************************************************
* Function Name: OpampHPF_Stop
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
void OpampHPF_Stop(void) 
{
    /* Disable pumps only if only one SC block is in use */
    if (OpampHPF_PM_ACT_CFG_REG == OpampHPF_ACT_PWR_EN)
    {
       OpampHPF_SC_MISC_REG &= ~OpampHPF_PUMP_FORCE;
    }   
    /* Disble power to the Amp in Active mode template */
    OpampHPF_PM_ACT_CFG_REG &= ~OpampHPF_ACT_PWR_EN;

    /* Disble power to the Amp in Alternative Active mode template */
    OpampHPF_PM_STBY_CFG_REG &= ~OpampHPF_STBY_PWR_EN;

    /* If a boost clock is required  */
    /* Disable and power down boost clock */
    #if (OpampHPF_ACLK_ENABLE)
        OpampHPF_ACLK_PM_ACT_CFG_REG &= ~OpampHPF_ACLK_ACT_PWR_EN;
        OpampHPF_ACLK_PM_STBY_CFG_REG &= ~OpampHPF_ACLK_STBY_PWR_EN;
        OpampHPF_ScBoostClk_Disable();  
    #endif /* OpampHPF_ACLK_ENABLE */

}


/*******************************************************************************
* Function Name: OpampHPF_SetPower
********************************************************************************
*
* Summary:
*  Set the power of the PGA
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
void OpampHPF_SetPower(uint8 power) 
{
    uint8 tmpCR;

    tmpCR = OpampHPF_CR1_REG & ~OpampHPF_DRIVE_MASK;
    tmpCR |= (power & OpampHPF_DRIVE_MASK);
    OpampHPF_CR1_REG = tmpCR;  
}


/*******************************************************************************
* Function Name: OpampHPF_SetGain
********************************************************************************
*
* Summary:
*  This function sets values of the input and feedback resistors to set a 
*  specific gain of the amplifier.
*
* Parameters:  
*  gain:  Gain value of PGA (See header file for gain values.)
*
* Return: 
*  void 
*
* Reentrant: 
*  Yes
*
*******************************************************************************/
void OpampHPF_SetGain(uint8 gain) 
{
    /* Only set new gain if it is a valid gain */
    if( gain <= OpampHPF_GAIN_MAX)
    {
        /* Clear resistors, redc, and bias */
        OpampHPF_CR2_REG &= ~(OpampHPF_RVAL_MASK | OpampHPF_R20_40B_MASK | 
                                OpampHPF_REDC_MASK | OpampHPF_BIAS_MASK );

        /* Set gain value resistors, redc comp, and bias */
        OpampHPF_CR2_REG |= (OpampHPF_GainArray[gain] |
                                ( (OpampHPF_GainComp[gain] << 2 ) & OpampHPF_REDC_MASK));
             
        /* Clear sc_comp  */
        OpampHPF_CR1_REG &= ~OpampHPF_COMP_MASK;
        /* Set sc_comp  */
        OpampHPF_CR1_REG |= ( OpampHPF_GainComp[gain] | OpampHPF_COMP_MASK );
    }
}


/* [] END OF FILE */
