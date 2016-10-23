/*******************************************************************************
* File Name: PGA_Signal_Stage2_A.c  
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
#include "PGA_Signal_Stage2_A.h"

#if(PGA_Signal_Stage2_A_CYDEV_VDDA_MV < PGA_Signal_Stage2_A_MINIMUM_VDDA_THRESHOLD_MV)
    #include "PGA_Signal_Stage2_A_ScBoostClk.h"
#endif /* PGA_Signal_Stage2_A_ACLK_ENABLE */


/* Constant array for gain settings */
const uint8 PGA_Signal_Stage2_A_GainArray[9] = { 
   (PGA_Signal_Stage2_A_RVAL_20K   | PGA_Signal_Stage2_A_R20_40B_40K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=1  */
   (PGA_Signal_Stage2_A_RVAL_30K   | PGA_Signal_Stage2_A_R20_40B_40K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=2  */
   (PGA_Signal_Stage2_A_RVAL_40K   | PGA_Signal_Stage2_A_R20_40B_40K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=4  */
   (PGA_Signal_Stage2_A_RVAL_60K   | PGA_Signal_Stage2_A_R20_40B_40K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=8  */
   (PGA_Signal_Stage2_A_RVAL_120K  | PGA_Signal_Stage2_A_R20_40B_40K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=16 */
   (PGA_Signal_Stage2_A_RVAL_250K  | PGA_Signal_Stage2_A_R20_40B_40K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=24 */
   (PGA_Signal_Stage2_A_RVAL_250K  | PGA_Signal_Stage2_A_R20_40B_20K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=32 */
   (PGA_Signal_Stage2_A_RVAL_500K  | PGA_Signal_Stage2_A_R20_40B_20K | PGA_Signal_Stage2_A_BIAS_LOW), /* G=48 */
   (PGA_Signal_Stage2_A_RVAL_1000K | PGA_Signal_Stage2_A_R20_40B_20K | PGA_Signal_Stage2_A_BIAS_LOW)  /* G=50 */
};

/* Constant array for gain compenstion settings */
const uint8 PGA_Signal_Stage2_A_GainComp[9] = { 
   ( PGA_Signal_Stage2_A_COMP_4P35PF  | ( PGA_Signal_Stage2_A_REDC_00 >> 2 )), /* G=1  */
   ( PGA_Signal_Stage2_A_COMP_4P35PF  | ( PGA_Signal_Stage2_A_REDC_01 >> 2 )), /* G=2  */
   ( PGA_Signal_Stage2_A_COMP_3P0PF   | ( PGA_Signal_Stage2_A_REDC_01 >> 2 )), /* G=4  */
   ( PGA_Signal_Stage2_A_COMP_3P0PF   | ( PGA_Signal_Stage2_A_REDC_01 >> 2 )), /* G=8  */
   ( PGA_Signal_Stage2_A_COMP_3P6PF   | ( PGA_Signal_Stage2_A_REDC_01 >> 2 )), /* G=16 */
   ( PGA_Signal_Stage2_A_COMP_3P6PF   | ( PGA_Signal_Stage2_A_REDC_11 >> 2 )), /* G=24 */
   ( PGA_Signal_Stage2_A_COMP_3P6PF   | ( PGA_Signal_Stage2_A_REDC_11 >> 2 )), /* G=32 */
   ( PGA_Signal_Stage2_A_COMP_3P6PF   | ( PGA_Signal_Stage2_A_REDC_00 >> 2 )), /* G=48 */
   ( PGA_Signal_Stage2_A_COMP_3P6PF   | ( PGA_Signal_Stage2_A_REDC_00 >> 2 ))  /* G=50 */
};

uint8 PGA_Signal_Stage2_A_initVar = 0u;


/*******************************************************************************   
* Function Name: PGA_Signal_Stage2_A_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PGA_Signal_Stage2_A_Start().
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
void PGA_Signal_Stage2_A_Init(void) 
{
    /* Set PGA mode */
    PGA_Signal_Stage2_A_CR0_REG = PGA_Signal_Stage2_A_MODE_PGA;  
    
    /* Set non-inverting PGA mode  and reference mode */
    PGA_Signal_Stage2_A_CR1_REG |= PGA_Signal_Stage2_A_PGA_NINV;  

    /* Set default gain and ref mode */
    PGA_Signal_Stage2_A_CR2_REG = PGA_Signal_Stage2_A_VREF_MODE;

    /* Set gain and compensation */
    PGA_Signal_Stage2_A_SetGain(PGA_Signal_Stage2_A_DEFAULT_GAIN);

    /* Set power */
    PGA_Signal_Stage2_A_SetPower(PGA_Signal_Stage2_A_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: PGA_Signal_Stage2_A_Enable
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
void PGA_Signal_Stage2_A_Enable(void) 
{
    /* If a boost clock is required  */
    #if(PGA_Signal_Stage2_A_CYDEV_VDDA_MV < PGA_Signal_Stage2_A_MINIMUM_VDDA_THRESHOLD_MV)
        PGA_Signal_Stage2_A_ACLK_PM_ACT_CFG_REG |= PGA_Signal_Stage2_A_ACLK_ACT_PWR_EN;
        PGA_Signal_Stage2_A_ACLK_PM_STBY_CFG_REG |= PGA_Signal_Stage2_A_ACLK_STBY_PWR_EN;
		PGA_Signal_Stage2_A_ScBoostClk_Enable();  /* turn on boost pump */
        PGA_Signal_Stage2_A_BST_REG |= 0x08;
	 #endif /* PGA_Signal_Stage2_A_ACLK_ENABLE */
   
    /* PSoC3 ES2 or early, PSoC5 ES1 */
    #if (PGA_Signal_Stage2_A_PSOC3_ES2 || PGA_Signal_Stage2_A_PSOC5_ES1)
        /* Enable Pump only if voltage is below threshold */
        if (PGA_Signal_Stage2_A_CYDEV_VDDA_MV < PGA_Signal_Stage2_A_VDDA_THRESHOLD_MV)
        {
            PGA_Signal_Stage2_A_SC_MISC_REG |= PGA_Signal_Stage2_A_PUMP_FORCE;
        }
        
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (PGA_Signal_Stage2_A_PSOC3_ES3 || PGA_Signal_Stage2_A_PSOC5_ES2)
        /* Enable charge Pump clock for SC block */
        PGA_Signal_Stage2_A_PUMP_CR1_REG |= PGA_Signal_Stage2_A_PUMP_CR1_SC_CLKSEL;
        /* Enable Pump only if voltage is below threshold */
        if (PGA_Signal_Stage2_A_CYDEV_VDDA_MV < PGA_Signal_Stage2_A_VDDA_THRESHOLD_MV)
        {
            PGA_Signal_Stage2_A_SC_MISC_REG |= PGA_Signal_Stage2_A_PUMP_FORCE;
        }
    #endif /* PGA_Signal_Stage2_A_PSOC3_ES2 || PGA_Signal_Stage2_A_PSOC5_ES1 */
     
    /* Enable power to the Amp in Active mode*/
    PGA_Signal_Stage2_A_PM_ACT_CFG_REG |= PGA_Signal_Stage2_A_ACT_PWR_EN;

    /* Enable power to the Amp in Alternative Active mode*/
    PGA_Signal_Stage2_A_PM_STBY_CFG_REG |= PGA_Signal_Stage2_A_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name: PGA_Signal_Stage2_A_Start
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
void PGA_Signal_Stage2_A_Start(void) 
{
    if(PGA_Signal_Stage2_A_initVar == 0u)
    {
        PGA_Signal_Stage2_A_Init();                    
        PGA_Signal_Stage2_A_initVar = 1u;  
    }
    
    PGA_Signal_Stage2_A_Enable();              
}


/*******************************************************************************
* Function Name: PGA_Signal_Stage2_A_Stop
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
void PGA_Signal_Stage2_A_Stop(void) 
{
    /* Disable pumps only if only one SC block is in use */
    if (PGA_Signal_Stage2_A_PM_ACT_CFG_REG == PGA_Signal_Stage2_A_ACT_PWR_EN)
    {
       PGA_Signal_Stage2_A_SC_MISC_REG &= ~PGA_Signal_Stage2_A_PUMP_FORCE;
    }   
    /* Disble power to the Amp in Active mode template */
    PGA_Signal_Stage2_A_PM_ACT_CFG_REG &= ~PGA_Signal_Stage2_A_ACT_PWR_EN;

    /* Disble power to the Amp in Alternative Active mode template */
    PGA_Signal_Stage2_A_PM_STBY_CFG_REG &= ~PGA_Signal_Stage2_A_STBY_PWR_EN;

    /* If a boost clock is required  */
    /* Disable SC Block boost clk control, if used (MinVdda < 2.7V) */
    #if(PGA_Signal_Stage2_A_CYDEV_VDDA_MV < PGA_Signal_Stage2_A_MINIMUM_VDDA_THRESHOLD_MV)
        PGA_Signal_Stage2_A_ACLK_PM_ACT_CFG_REG &= ~PGA_Signal_Stage2_A_ACLK_ACT_PWR_EN;
        PGA_Signal_Stage2_A_ACLK_PM_STBY_CFG_REG &= ~PGA_Signal_Stage2_A_ACLK_STBY_PWR_EN;
        PGA_Signal_Stage2_A_ScBoostClk_Disable();      		
    #endif /* PGA_Signal_Stage2_A_ACLK_ENABLE */

}


/*******************************************************************************
* Function Name: PGA_Signal_Stage2_A_SetPower
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
void PGA_Signal_Stage2_A_SetPower(uint8 power) 
{
    uint8 tmpCR;

    tmpCR = PGA_Signal_Stage2_A_CR1_REG & ~PGA_Signal_Stage2_A_DRIVE_MASK;
    tmpCR |= (power & PGA_Signal_Stage2_A_DRIVE_MASK);
    PGA_Signal_Stage2_A_CR1_REG = tmpCR;  
}


/*******************************************************************************
* Function Name: PGA_Signal_Stage2_A_SetGain
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
void PGA_Signal_Stage2_A_SetGain(uint8 gain) 
{
    /* Only set new gain if it is a valid gain */
    if( gain <= PGA_Signal_Stage2_A_GAIN_MAX)
    {
        /* Clear resistors, redc, and bias */
        PGA_Signal_Stage2_A_CR2_REG &= ~(PGA_Signal_Stage2_A_RVAL_MASK | PGA_Signal_Stage2_A_R20_40B_MASK | 
                                PGA_Signal_Stage2_A_REDC_MASK | PGA_Signal_Stage2_A_BIAS_MASK );

        /* Set gain value resistors, redc comp, and bias */
        PGA_Signal_Stage2_A_CR2_REG |= (PGA_Signal_Stage2_A_GainArray[gain] |
                                ( (PGA_Signal_Stage2_A_GainComp[gain] << 2 ) & PGA_Signal_Stage2_A_REDC_MASK));
             
        /* Clear sc_comp  */
        PGA_Signal_Stage2_A_CR1_REG &= ~PGA_Signal_Stage2_A_COMP_MASK;
        /* Set sc_comp  */
        PGA_Signal_Stage2_A_CR1_REG |= ( PGA_Signal_Stage2_A_GainComp[gain] | PGA_Signal_Stage2_A_COMP_MASK );
    }
}


/* [] END OF FILE */
