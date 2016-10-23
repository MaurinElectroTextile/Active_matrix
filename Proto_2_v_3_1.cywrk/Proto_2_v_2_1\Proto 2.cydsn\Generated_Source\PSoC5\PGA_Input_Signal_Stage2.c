/*******************************************************************************
* File Name: PGA_Input_Signal_Stage2.c  
* Version 1.60
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
#include "PGA_Input_Signal_Stage2.h"

#if(PGA_Input_Signal_Stage2_CYDEV_VDDA_MV < PGA_Input_Signal_Stage2_MINIMUM_VDDA_THRESHOLD_MV)
    #include "PGA_Input_Signal_Stage2_ScBoostClk.h"
#endif /* PGA_Input_Signal_Stage2_ACLK_ENABLE */


/* Constant array for gain settings */
const uint8 PGA_Input_Signal_Stage2_GainArray[9] = { 
   (PGA_Input_Signal_Stage2_RVAL_20K   | PGA_Input_Signal_Stage2_R20_40B_40K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=1  */
   (PGA_Input_Signal_Stage2_RVAL_30K   | PGA_Input_Signal_Stage2_R20_40B_40K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=2  */
   (PGA_Input_Signal_Stage2_RVAL_40K   | PGA_Input_Signal_Stage2_R20_40B_40K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=4  */
   (PGA_Input_Signal_Stage2_RVAL_60K   | PGA_Input_Signal_Stage2_R20_40B_40K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=8  */
   (PGA_Input_Signal_Stage2_RVAL_120K  | PGA_Input_Signal_Stage2_R20_40B_40K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=16 */
   (PGA_Input_Signal_Stage2_RVAL_250K  | PGA_Input_Signal_Stage2_R20_40B_40K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=24 */
   (PGA_Input_Signal_Stage2_RVAL_250K  | PGA_Input_Signal_Stage2_R20_40B_20K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=32 */
   (PGA_Input_Signal_Stage2_RVAL_500K  | PGA_Input_Signal_Stage2_R20_40B_20K | PGA_Input_Signal_Stage2_BIAS_LOW), /* G=48 */
   (PGA_Input_Signal_Stage2_RVAL_1000K | PGA_Input_Signal_Stage2_R20_40B_20K | PGA_Input_Signal_Stage2_BIAS_LOW)  /* G=50 */
};

/* Constant array for gain compenstion settings */
const uint8 PGA_Input_Signal_Stage2_GainComp[9] = { 
   ( PGA_Input_Signal_Stage2_COMP_4P35PF  | ( PGA_Input_Signal_Stage2_REDC_00 >> 2 )), /* G=1  */
   ( PGA_Input_Signal_Stage2_COMP_4P35PF  | ( PGA_Input_Signal_Stage2_REDC_01 >> 2 )), /* G=2  */
   ( PGA_Input_Signal_Stage2_COMP_3P0PF   | ( PGA_Input_Signal_Stage2_REDC_01 >> 2 )), /* G=4  */
   ( PGA_Input_Signal_Stage2_COMP_3P0PF   | ( PGA_Input_Signal_Stage2_REDC_01 >> 2 )), /* G=8  */
   ( PGA_Input_Signal_Stage2_COMP_3P6PF   | ( PGA_Input_Signal_Stage2_REDC_01 >> 2 )), /* G=16 */
   ( PGA_Input_Signal_Stage2_COMP_3P6PF   | ( PGA_Input_Signal_Stage2_REDC_11 >> 2 )), /* G=24 */
   ( PGA_Input_Signal_Stage2_COMP_3P6PF   | ( PGA_Input_Signal_Stage2_REDC_11 >> 2 )), /* G=32 */
   ( PGA_Input_Signal_Stage2_COMP_3P6PF   | ( PGA_Input_Signal_Stage2_REDC_00 >> 2 )), /* G=48 */
   ( PGA_Input_Signal_Stage2_COMP_3P6PF   | ( PGA_Input_Signal_Stage2_REDC_00 >> 2 ))  /* G=50 */
};

uint8 PGA_Input_Signal_Stage2_initVar = 0u;


/*******************************************************************************   
* Function Name: PGA_Input_Signal_Stage2_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PGA_Input_Signal_Stage2_Start().
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
void PGA_Input_Signal_Stage2_Init(void) 
{
    /* Set PGA mode */
    PGA_Input_Signal_Stage2_CR0_REG = PGA_Input_Signal_Stage2_MODE_PGA;  
    
    /* Set non-inverting PGA mode  and reference mode */
    PGA_Input_Signal_Stage2_CR1_REG |= PGA_Input_Signal_Stage2_PGA_NINV;  

    /* Set default gain and ref mode */
    PGA_Input_Signal_Stage2_CR2_REG = PGA_Input_Signal_Stage2_VREF_MODE;

    /* Set gain and compensation */
    PGA_Input_Signal_Stage2_SetGain(PGA_Input_Signal_Stage2_DEFAULT_GAIN);

    /* Set power */
    PGA_Input_Signal_Stage2_SetPower(PGA_Input_Signal_Stage2_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: PGA_Input_Signal_Stage2_Enable
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
void PGA_Input_Signal_Stage2_Enable(void) 
{
    /* If a boost clock is required  */
    #if(PGA_Input_Signal_Stage2_CYDEV_VDDA_MV < PGA_Input_Signal_Stage2_MINIMUM_VDDA_THRESHOLD_MV)
        PGA_Input_Signal_Stage2_ACLK_PM_ACT_CFG_REG |= PGA_Input_Signal_Stage2_ACLK_ACT_PWR_EN;
        PGA_Input_Signal_Stage2_ACLK_PM_STBY_CFG_REG |= PGA_Input_Signal_Stage2_ACLK_STBY_PWR_EN;
		PGA_Input_Signal_Stage2_ScBoostClk_Enable();  /* turn on boost pump */
        PGA_Input_Signal_Stage2_BST_REG |= 0x08;
	 #endif /* PGA_Input_Signal_Stage2_ACLK_ENABLE */
   
    /* PSoC3 ES2 or early, PSoC5 ES1 */
    #if (PGA_Input_Signal_Stage2_PSOC3_ES2 || PGA_Input_Signal_Stage2_PSOC5_ES1)
        /* Enable Pump only if voltage is below threshold */
        if (PGA_Input_Signal_Stage2_CYDEV_VDDA_MV < PGA_Input_Signal_Stage2_VDDA_THRESHOLD_MV)
        {
            PGA_Input_Signal_Stage2_SC_MISC_REG |= PGA_Input_Signal_Stage2_PUMP_FORCE;
        }
        
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (PGA_Input_Signal_Stage2_PSOC3_ES3 || PGA_Input_Signal_Stage2_PSOC5_ES2)
        /* Enable charge Pump clock for SC block */
        PGA_Input_Signal_Stage2_PUMP_CR1_REG |= PGA_Input_Signal_Stage2_PUMP_CR1_SC_CLKSEL;
        /* Enable Pump only if voltage is below threshold */
        if (PGA_Input_Signal_Stage2_CYDEV_VDDA_MV < PGA_Input_Signal_Stage2_VDDA_THRESHOLD_MV)
        {
            PGA_Input_Signal_Stage2_SC_MISC_REG |= PGA_Input_Signal_Stage2_PUMP_FORCE;
        }
    #endif /* PGA_Input_Signal_Stage2_PSOC3_ES2 || PGA_Input_Signal_Stage2_PSOC5_ES1 */
     
    /* Enable power to the Amp in Active mode*/
    PGA_Input_Signal_Stage2_PM_ACT_CFG_REG |= PGA_Input_Signal_Stage2_ACT_PWR_EN;

    /* Enable power to the Amp in Alternative Active mode*/
    PGA_Input_Signal_Stage2_PM_STBY_CFG_REG |= PGA_Input_Signal_Stage2_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name: PGA_Input_Signal_Stage2_Start
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
void PGA_Input_Signal_Stage2_Start(void) 
{
    if(PGA_Input_Signal_Stage2_initVar == 0u)
    {
        PGA_Input_Signal_Stage2_Init();                    
        PGA_Input_Signal_Stage2_initVar = 1u;  
    }
    
    PGA_Input_Signal_Stage2_Enable();              
}


/*******************************************************************************
* Function Name: PGA_Input_Signal_Stage2_Stop
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
void PGA_Input_Signal_Stage2_Stop(void) 
{
    /* Disable pumps only if only one SC block is in use */
    if (PGA_Input_Signal_Stage2_PM_ACT_CFG_REG == PGA_Input_Signal_Stage2_ACT_PWR_EN)
    {
       PGA_Input_Signal_Stage2_SC_MISC_REG &= ~PGA_Input_Signal_Stage2_PUMP_FORCE;
    }   
    /* Disble power to the Amp in Active mode template */
    PGA_Input_Signal_Stage2_PM_ACT_CFG_REG &= ~PGA_Input_Signal_Stage2_ACT_PWR_EN;

    /* Disble power to the Amp in Alternative Active mode template */
    PGA_Input_Signal_Stage2_PM_STBY_CFG_REG &= ~PGA_Input_Signal_Stage2_STBY_PWR_EN;

    /* If a boost clock is required  */
    /* Disable SC Block boost clk control, if used (MinVdda < 2.7V) */
    #if(PGA_Input_Signal_Stage2_CYDEV_VDDA_MV < PGA_Input_Signal_Stage2_MINIMUM_VDDA_THRESHOLD_MV)
        PGA_Input_Signal_Stage2_ACLK_PM_ACT_CFG_REG &= ~PGA_Input_Signal_Stage2_ACLK_ACT_PWR_EN;
        PGA_Input_Signal_Stage2_ACLK_PM_STBY_CFG_REG &= ~PGA_Input_Signal_Stage2_ACLK_STBY_PWR_EN;
        PGA_Input_Signal_Stage2_ScBoostClk_Disable();      		
    #endif /* PGA_Input_Signal_Stage2_ACLK_ENABLE */

}


/*******************************************************************************
* Function Name: PGA_Input_Signal_Stage2_SetPower
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
void PGA_Input_Signal_Stage2_SetPower(uint8 power) 
{
    uint8 tmpCR;

    tmpCR = PGA_Input_Signal_Stage2_CR1_REG & ~PGA_Input_Signal_Stage2_DRIVE_MASK;
    tmpCR |= (power & PGA_Input_Signal_Stage2_DRIVE_MASK);
    PGA_Input_Signal_Stage2_CR1_REG = tmpCR;  
}


/*******************************************************************************
* Function Name: PGA_Input_Signal_Stage2_SetGain
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
void PGA_Input_Signal_Stage2_SetGain(uint8 gain) 
{
    /* Only set new gain if it is a valid gain */
    if( gain <= PGA_Input_Signal_Stage2_GAIN_MAX)
    {
        /* Clear resistors, redc, and bias */
        PGA_Input_Signal_Stage2_CR2_REG &= ~(PGA_Input_Signal_Stage2_RVAL_MASK | PGA_Input_Signal_Stage2_R20_40B_MASK | 
                                PGA_Input_Signal_Stage2_REDC_MASK | PGA_Input_Signal_Stage2_BIAS_MASK );

        /* Set gain value resistors, redc comp, and bias */
        PGA_Input_Signal_Stage2_CR2_REG |= (PGA_Input_Signal_Stage2_GainArray[gain] |
                                ( (PGA_Input_Signal_Stage2_GainComp[gain] << 2 ) & PGA_Input_Signal_Stage2_REDC_MASK));
             
        /* Clear sc_comp  */
        PGA_Input_Signal_Stage2_CR1_REG &= ~PGA_Input_Signal_Stage2_COMP_MASK;
        /* Set sc_comp  */
        PGA_Input_Signal_Stage2_CR1_REG |= ( PGA_Input_Signal_Stage2_GainComp[gain] | PGA_Input_Signal_Stage2_COMP_MASK );
    }
}


/* [] END OF FILE */
