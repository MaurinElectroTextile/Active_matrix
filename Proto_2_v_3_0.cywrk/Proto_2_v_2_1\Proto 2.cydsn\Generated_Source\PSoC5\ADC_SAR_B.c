/*******************************************************************************
* File Name: ADC_SAR_B.c
* Version 1.50
*
* Description:
*  This file provides the source code to the API for the Successive
*  approximation ADC Component.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CyLib.h"
#include "ADC_SAR_B.h"

#if(ADC_SAR_B_DEFAULT_INTERNAL_CLK)
    #include "ADC_SAR_B_theACLK.h"
#endif /* End ADC_SAR_B_DEFAULT_INTERNAL_CLK */


/***************************************
* Forward function references
***************************************/
void ADC_SAR_B_SetRef(int8 refMode);
void ADC_SAR_B_CalcGain(uint8 resolution);


/***************************************
* Global data allocation
***************************************/
uint8 ADC_SAR_B_initVar = 0u;
int16 ADC_SAR_B_offset;
int16 ADC_SAR_B_countsPerVolt;   /* Gain compensation */
int16 ADC_SAR_B_shift;


/*******************************************************************************
* Function Name: ADC_SAR_B_Start
********************************************************************************
*
* Summary:
*  Performs all required initialization for this component and enables the
*  power.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The ADC_SAR_B_initVar variable is used to indicate when/if initial 
*  configuration of this component has happened. The variable is initialized to 
*  zero and set to 1 the first time ADC_Start() is called. This allows for 
*  component Re-Start without re-initialization in all subsequent calls to the 
*  ADC_SAR_B_Start() routine.
*  If re-initialization of the component is required the variable should be set 
*  to zero before call of ADC_SAR_B_Start() routine, or the user may call 
*  ADC_SAR_B_Init() and ADC_SAR_B_Enable() as done in the 
*  ADC_SAR_B_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_Start(void)
{

    /* If not Initialized then initialize all required hardware and software */
    if(ADC_SAR_B_initVar == 0u)
    {
        ADC_SAR_B_Init();
        ADC_SAR_B_initVar = 1u;
    }
    ADC_SAR_B_Enable();
}


/*******************************************************************************
* Function Name: ADC_SAR_B_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  ADC_SAR_B_Start().
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global variables:
*  The ADC_SAR_B_offset variable is initialized to 0.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_Init(void)
{
    uint8 tmpReg;

    /* This is only valid if there is an internal clock */
    #if(ADC_SAR_B_DEFAULT_INTERNAL_CLK)
        ADC_SAR_B_theACLK_SetMode(CYCLK_DUTY);
    #endif /* End ADC_SAR_B_DEFAULT_INTERNAL_CLK */

    /* Start and set interrupt vector */
    CyIntSetPriority(ADC_SAR_B_INTC_NUMBER, ADC_SAR_B_INTC_PRIOR_NUMBER);
    CyIntSetVector(ADC_SAR_B_INTC_NUMBER, ADC_SAR_B_ISR );

    /* default CSR0=0 */
    tmpReg = ADC_SAR_B_SAR_CSR0_REG;

    /* Set Power parameter  */
    tmpReg |= (ADC_SAR_B_DEFAULT_POWER << ADC_SAR_B_SAR_POWER_SHIFT);

    /* Clear DAC value at beginning of sampling */
    tmpReg |= ADC_SAR_B_SAR_HIZ_CLEAR;

    /*Set Convertion mode */
    if(ADC_SAR_B_DEFAULT_CONV_MODE)                /* If triggered mode */
    {
        tmpReg |= ADC_SAR_B_SAR_MX_SOF_UDB;            /* source: UDB */
        tmpReg |= ADC_SAR_B_SAR_SOF_MODE_EDGE;        /* Set edge-sensitive sof source */
     }
    ADC_SAR_B_SAR_CSR0_REG = tmpReg;

    /* Enable IRQ mode*/
    ADC_SAR_B_SAR_CSR1_REG |= ADC_SAR_B_SAR_IRQ_MASK_EN | ADC_SAR_B_SAR_IRQ_MODE_EDGE;
    
    /*Set SAR ADC resolution ADC */
    ADC_SAR_B_SetResolution(ADC_SAR_B_DEFAULT_RESOLUTION);
    ADC_SAR_B_offset = 0;

    /*Sets input range of the ADC*/
    switch (ADC_SAR_B_DEFAULT_RANGE)
    {
        case ADC_SAR_B__VSS_TO_VREF:
            ADC_SAR_B_SetRef(ADC_SAR_B_DEFAULT_RANGE);
            break;

        case ADC_SAR_B__VSSA_TO_VDDA:
            /* Set bit for VDD/2 mode */
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_RESVDA_EN;
            ADC_SAR_B_SetRef(ADC_SAR_B__VSSA_TO_VDDA);
            break;
        case ADC_SAR_B__VSSA_TO_VDAC:
            ADC_SAR_B_SetRef(ADC_SAR_B__VSSA_TO_VDDA);
            break;

        case ADC_SAR_B__VNEG_VREF_DIFF:
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_CP_EN;         /* Enable charge pump*/
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_BUF_VCM_EN;    /* Enable VCM reference buffer*/
            ADC_SAR_B_SetRef(ADC_SAR_B_DEFAULT_RANGE);
            break;

        case ADC_SAR_B__VNEG_VDDA_DIFF:
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_CP_EN;         /* Enable charge pump*/
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_BUF_VCM_EN;    /* Enable VCM reference buffer*/
            ADC_SAR_B_SetRef(ADC_SAR_B__VSSA_TO_VDDA);
            break;

        case ADC_SAR_B__VNEG_VDDA_2_DIFF:
             /* Set bit for VDD/2 mode*/
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_RESVDA_EN;
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_CP_EN;         /* Enable charge pump*/
            ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_BUF_VCM_EN;    /* Enable VCM reference buffer*/
            ADC_SAR_B_SetRef(ADC_SAR_B__VSSA_TO_VDDA);
            break;
            
        case ADC_SAR_B__VNEG_VDAC_DIFF:
            ADC_SAR_B_SetRef(ADC_SAR_B__VSSA_TO_VDDA);
            break;

        default:
            /* default case is not used*/
            break;
     }
}


/*******************************************************************************
* Function Name: ADC_SAR_B_Enable
********************************************************************************
*  
* Summary: 
*  Enables the clock and power for SAR ADC.
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
void ADC_SAR_B_Enable(void)
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /*ADC_SAR_B_SAR_TR0_REG = ADC_SAR_B_SAR_CAP_TRIM_4;*/ /*Set attenuation capacitor*/

    /* This is only valid if there is an internal clock */
    #if(ADC_SAR_B_DEFAULT_INTERNAL_CLK)
        ADC_SAR_B_PWRMGR_CLK_REG |= ADC_SAR_B_ACT_PWR_CLK_EN;
        ADC_SAR_B_theACLK_Enable();
    #endif /* End ADC_SAR_B_DEFAULT_INTERNAL_CLK */

    /* Enable clock for SAR ADC*/
    ADC_SAR_B_SAR_CLK_REG |= ADC_SAR_B_SAR_MX_CLK_EN;

     /* Enable power for ADC */
    ADC_SAR_B_PWRMGR_SAR_REG |= ADC_SAR_B_ACT_PWR_SAR_EN;
    
    /* Clear a pending interrupt */
    CyIntClearPending(ADC_SAR_B_INTC_NUMBER);
    
    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: ADC_SAR_B_SetRef
********************************************************************************
*
* Summary:
*   Sets reference for ADC
*
* Parameters:
*  refMode: Reference configuration.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_SetRef(int8 refMode)
{
    uint8 tmpReg;
    if(refMode == ADC_SAR_B__VSSA_TO_VDDA)
    {
        /* Use internal reference*/
        tmpReg = ADC_SAR_B_SAR_REF_S3_LSB_EN | ADC_SAR_B_SAR_REF_S4_LSB_EN;
        /*tmpReg = ADC_SAR_B_SAR_REF_S4_LSB_EN | ADC_SAR_B_SAR_REF_S7_LSB_EN;*/   /*IROS need update*/
        ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_BUF_VREF_EN;    /* Enable Int Ref Amp*/
    }
    else
    {
        switch (ADC_SAR_B_DEFAULT_REFERENCE)
        {
            case ADC_SAR_B__INT_REF_NOT_BYPASSED:
                tmpReg = ADC_SAR_B_SAR_REF_S3_LSB_EN | ADC_SAR_B_SAR_REF_S4_LSB_EN;
                ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_BUF_VREF_EN; /* Enable Int Ref Amp*/
                break;
            case ADC_SAR_B__INT_REF_BYPASS:
                tmpReg = ADC_SAR_B_SAR_REF_S2_LSB_EN | 
                         ADC_SAR_B_SAR_REF_S3_LSB_EN | 
                         ADC_SAR_B_SAR_REF_S4_LSB_EN;
                ADC_SAR_B_SAR_CSR3_REG |= ADC_SAR_B_SAR_EN_BUF_VREF_EN; /* Enable Int Ref Amp*/
                /*ADC_SAR_B_SAR_CSR3 |= ADC_SAR_B_SAR_EN_BUF_VCM_EN;  */ /* Enable VCM Ref Amp*/
                break;
            case ADC_SAR_B__EXT_REF:
                tmpReg = ADC_SAR_B_SAR_REF_S2_LSB_EN;
                ADC_SAR_B_SAR_CSR3_REG &= ~ADC_SAR_B_SAR_EN_BUF_VREF_EN; /* Disable Int Ref Amp*/
                break;
            default:
                /* default case is not used*/
                break;
        }
    }
    ADC_SAR_B_SAR_CSR6_REG = tmpReg;
    ADC_SAR_B_SAR_CSR7_REG = ADC_SAR_B_SAR_REF_S_MSB_DIS;
}


/*******************************************************************************
* Function Name: ADC_SAR_B_Stop
********************************************************************************
*
* Summary:
*  Stops and powers the SAR ADC component off.
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
void ADC_SAR_B_Stop(void)
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Stop all conversions */
    ADC_SAR_B_SAR_CSR0_REG &= ~ADC_SAR_B_SAR_SOF_START_CONV;

    /* Disable power for ADC */
    ADC_SAR_B_PWRMGR_SAR_REG &= ~ADC_SAR_B_ACT_PWR_SAR_EN;

    /* This is only valid if there is an internal clock */
    #if(ADC_SAR_B_DEFAULT_INTERNAL_CLK)
        ADC_SAR_B_PWRMGR_CLK_REG &= ~ADC_SAR_B_ACT_PWR_CLK_EN;
        ADC_SAR_B_theACLK_Disable();
    #endif /* End ADC_SAR_B_DEFAULT_INTERNAL_CLK */

    CyExitCriticalSection(enableInterrupts);
    
}


/*******************************************************************************
* Function Name: ADC_SAR_B_IRQ_Enable
********************************************************************************
* Summary:
*  Enables the interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void ADC_SAR_B_IRQ_Enable(void)
{
    /* Enable the general interrupt. */
    CyIntEnable(ADC_SAR_B_INTC_NUMBER);
}


/*******************************************************************************
* Function Name: ADC_SAR_B_IRQ_Disable
********************************************************************************
*
* Summary:
*  Disables the Interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void ADC_SAR_B_IRQ_Disable(void)
{
    /* Disable the general interrupt. */
    CyIntDisable(ADC_SAR_B_INTC_NUMBER);
}


/*******************************************************************************
* Function Name: ADC_SAR_B_SetPower
********************************************************************************
*
* Summary:
*  Sets the Power mode.
*
* Parameters:
*  power:  Power setting for ADC
*  0 ->    Normal
*  1 ->    Half power
*  2 ->    1/3rd power
*  3 ->    Quarter power.
*
* Return:
*  None.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void ADC_SAR_B_SetPower(uint8 power)
{
    uint8 tmpReg;

    /* mask off invalid power settings */
    power &= ~ADC_SAR_B_SAR_API_POWER_MASK;

    /* Set Power parameter  */
    tmpReg = ADC_SAR_B_SAR_CSR0_REG & ~ADC_SAR_B_SAR_POWER_MASK;
    tmpReg |= (power << ADC_SAR_B_SAR_POWER_SHIFT);
    ADC_SAR_B_SAR_CSR0_REG = tmpReg;
}


/*******************************************************************************
* Function Name: ADC_SAR_B_SetResolution
********************************************************************************
*
* Summary:
*  Sets the Relution of the SAR.
*
* Parameters:
*  resolution:
*  12 ->    RES12
*  10 ->    RES10
*  8  ->    RES8
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Theory:
*
* Side Effects:
*  This function calls CalcGain procedure to calculate new gain based on 
*  resolution 
*
*******************************************************************************/
void ADC_SAR_B_SetResolution(uint8 resolution)
{
    uint8 tmpReg;

    /*Set SAR ADC resolution ADC */
    switch (resolution)
    {
        case ADC_SAR_B__BITS_12:
            tmpReg = ADC_SAR_B_SAR_RESOLUTION_12BIT;
            break;
        case ADC_SAR_B__BITS_10:
            tmpReg = ADC_SAR_B_SAR_RESOLUTION_10BIT;
            break;
        case ADC_SAR_B__BITS_8:
            tmpReg = ADC_SAR_B_SAR_RESOLUTION_8BIT;
            break;
        default:
            tmpReg = ADC_SAR_B_SAR_RESOLUTION_12BIT;
            break;
    }
    
     tmpReg |= ADC_SAR_B_SAR_SAMPLE_WIDTH;   /* 17 conversion cycles @ 12bits + 1 gap*/
    ADC_SAR_B_SAR_CSR2_REG = tmpReg;
    
     /* Calculate gain for convert counts to volts */
    ADC_SAR_B_CalcGain(resolution);
}


/*******************************************************************************
* Function Name: ADC_SAR_B_StartConvert
********************************************************************************
*
* Summary:
*  Starts ADC conversion using the given mode.
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
* Theory:
*  Forces the ADC to initiate a conversion. In Free Running mode, the ADC will 
*  run continuously. In a triggered mode the function also acts as a software
*  version of the SOC. Here every conversion has to be triggered by the routine. 
*  This writes into the SOC bit in SAR_CTRL reg.
*
* Side Effects:
*  In a triggered mode the function switches source for SOF from the external 
*  pin to the internal SOF generation. Application should not call StartConvert
*  if external source used for SOF.
*******************************************************************************/
void ADC_SAR_B_StartConvert(void)
{
    if(ADC_SAR_B_DEFAULT_CONV_MODE)                /* If triggered mode */
    {
        ADC_SAR_B_SAR_CSR0_REG &= ~ADC_SAR_B_SAR_MX_SOF_UDB;   /* source: SOF bit */
    }
    
    /* Start the conversion */
    ADC_SAR_B_SAR_CSR0_REG |= ADC_SAR_B_SAR_SOF_START_CONV;
}


/*******************************************************************************
* Function Name: ADC_SAR_B_StopConvert
********************************************************************************
*
* Summary:
*  Stops ADC conversion using the given mode.
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
* Theory:
*  Stops ADC conversion in Free Running mode. 
*  This writes into the SOC bit in SAR_CTRL reg.
*
* Side Effects:
*  In a triggered mode the function set a software version of the SOC to low level
*  and switch SOF source to hardware SOF input.
*  
*******************************************************************************/
void ADC_SAR_B_StopConvert(void)
{
    /* Stop all conversions */
    ADC_SAR_B_SAR_CSR0_REG &= ~ADC_SAR_B_SAR_SOF_START_CONV;

    if(ADC_SAR_B_DEFAULT_CONV_MODE)                /* If triggered mode */
    {
        /* Return source to UDB for hardware SOF signal */
        ADC_SAR_B_SAR_CSR0_REG |= ADC_SAR_B_SAR_MX_SOF_UDB;    /* source: UDB */
    }
}


/*******************************************************************************
* Function Name: ADC_SAR_B_IsEndConversion
********************************************************************************
*
* Summary:
*  Queries the ADC to see if conversion is complete
*
* Parameters:
*  retMode:  Wait mode,
*   0 if return with answer imediately.
*   1 if wait until conversion is complete, then return.
*
* Return:
*  (uint8)  0 =>  Conversion not complete.
*           1 =>  Conversion complete.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 ADC_SAR_B_IsEndConversion(uint8 retMode)
{
    uint8 status;

    do
    {
        status = ADC_SAR_B_SAR_CSR1_REG & ADC_SAR_B_SAR_EOF_1;
    } while ((status != ADC_SAR_B_SAR_EOF_1) && (retMode == ADC_SAR_B_WAIT_FOR_RESULT));

    return(status);
}


/*******************************************************************************
* Function Name: ADC_SAR_B_GetResult8
********************************************************************************
*
* Summary:
*  Returns an 8-bit result or the LSB of the last conversion. 
*  ADC_SAR_B_IsEndConversion() should be called to verify that the data 
*   sample is ready 
*
* Parameters:
*  None.
*
* Return:
*  ADC result.
*
* Global Variables:
*  ADC_SAR_B_shift - used to convert the ADC counts to the 2's 
*  compliment form.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
int8 ADC_SAR_B_GetResult8( void )
{
    return( ADC_SAR_B_SAR_WRK0_REG - (int8)ADC_SAR_B_shift);
}


/*******************************************************************************
* Function Name: ADC_SAR_B_GetResult16
********************************************************************************
*
* Summary:
*  Gets the data available in the SAR DATA registers.
*  ADC_SAR_B_IsEndConversion() should be called to verify that the data 
*   sample is ready 
*
* Parameters:
*  None.
*
* Return:
*  ADC result. WORD value which has the converted 12bits. In the differential 
*  input mode the SAR ADC outputs digitally converted data in binary offset 
*  scheme, this function converts it to ited, so this function converts the 
*  data into 2's compliment form. 
*
* Global Variables:
*  ADC_SAR_B_shift - used to convert the ADC counts to the 2's 
*  compliment form.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
int16 ADC_SAR_B_GetResult16( void )
{
    return( ((ADC_SAR_B_SAR_WRK1_REG << 8u ) | ADC_SAR_B_SAR_WRK0_REG ) - ADC_SAR_B_shift);
}


/*******************************************************************************
* Function Name: ADC_SAR_B_SetOffset
********************************************************************************
*
* Summary:
*  This function sets the offset for voltage readings.
*
* Parameters:
*  int16: Offset in counts
*
* Return:
*  None.
*
* Global Variables:
*  The ADC_SAR_B_offset variable modified. This variable is used for 
*  offset calibration purpose. 
*  Affects the ADC_SAR_B_CountsTo_Volts, 
*  ADC_SAR_B_CountsTo_mVolts, ADC_SAR_B_CountsTo_uVolts functions 
*  by subtracting the given offset. 
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void ADC_SAR_B_SetOffset(int16 offset)
{
    ADC_SAR_B_offset = offset;
}


/*******************************************************************************
* Function Name: ADC_SAR_B_CalcGain
********************************************************************************
*
* Summary:
*  This function calculates the ADC gain in counts per volt.
*
* Parameters:
*  uint8: resolution
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_B_shift variable initialized. This variable is used to 
*  convert the ADC counts to the 2's compliment form. 
*  ADC_SAR_B_countsPerVolt variable initialized. This variable is used 
*  for gain calibration purpose. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_CalcGain( uint8 resolution )
{
    uint32 counts = ADC_SAR_B_SAR_WRK_MAX;       /*default 12 bits*/
    uint16 diff_zero = ADC_SAR_B_SAR_DIFF_SHIFT;
    
    if(resolution == ADC_SAR_B__BITS_10)
    {
        counts >>= 2u;
        diff_zero >>= 2u;
    }
    if(resolution == ADC_SAR_B__BITS_8)
    {
        counts >>= 4u;
        diff_zero >>= 4u;
    }
    counts *= 1000u; /* To avoid float point arithmetic*/

        switch (ADC_SAR_B_DEFAULT_RANGE)
        {   /*TODO: use float point when it will be available*/
            case ADC_SAR_B__VSS_TO_VREF:
                ADC_SAR_B_countsPerVolt = counts / ADC_SAR_B_DEFAULT_REF_VOLTAGE_MV / 2;
                ADC_SAR_B_shift = 0;
                break;

            case ADC_SAR_B__VSSA_TO_VDDA:
                ADC_SAR_B_countsPerVolt = counts / ADC_SAR_B_DEFAULT_REF_VOLTAGE_MV;
                ADC_SAR_B_shift = 0;
                break;

            case ADC_SAR_B__VSSA_TO_VDAC:
                ADC_SAR_B_countsPerVolt = counts / ADC_SAR_B_DEFAULT_REF_VOLTAGE_MV / 2;
                ADC_SAR_B_shift = 0;
                break;

            case ADC_SAR_B__VNEG_VREF_DIFF:
                ADC_SAR_B_countsPerVolt = counts / ADC_SAR_B_DEFAULT_REF_VOLTAGE_MV / 2;
                ADC_SAR_B_shift = diff_zero;
                break;
                
            case ADC_SAR_B__VNEG_VDDA_DIFF:
                ADC_SAR_B_countsPerVolt = counts / ADC_SAR_B_DEFAULT_REF_VOLTAGE_MV / 2;
                ADC_SAR_B_shift = diff_zero;
                break;

            case ADC_SAR_B__VNEG_VDDA_2_DIFF:
                ADC_SAR_B_countsPerVolt = counts / ADC_SAR_B_DEFAULT_REF_VOLTAGE_MV;
                ADC_SAR_B_shift = diff_zero;
                break;

            case ADC_SAR_B__VNEG_VDAC_DIFF:
                ADC_SAR_B_countsPerVolt = counts / ADC_SAR_B_DEFAULT_REF_VOLTAGE_MV / 2;
                ADC_SAR_B_shift = diff_zero;
                break;

            default:
                /* if default case is hit, invalid argument was passed.*/
                break;
    
         }
}


/*******************************************************************************
* Function Name: ADC_SAR_B_SetGain
********************************************************************************
*
* Summary:
*  This function sets the ADC gain in counts per volt.
*
* Parameters:
*  int16  adcGain  counts per volt
*
* Return:
*  None.
*
* Global Variables:
*  ADC_SAR_B_countsPerVolt variable modified. This variable is used 
*  for gain calibration purpose. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_SAR_B_SetGain(int16 adcGain)
{
    ADC_SAR_B_countsPerVolt = adcGain;
}


/*******************************************************************************
* Function Name: ADC_SAR_B_CountsTo_mVolts
********************************************************************************
*
* Summary:
*  This function converts ADC counts to mVolts
*
* Parameters:
*  int16  adcCounts   Reading from ADC.
*
* Return:
*  int16  Result in mVolts
*
* Global Variables:
*  ADC_SAR_B_offset variable used.
*  ADC_SAR_B_countsPerVolt variable used.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
int16 ADC_SAR_B_CountsTo_mVolts(int16 adcCounts)
{

    int16 mVolts;

    /* Subtract ADC offset */
    adcCounts -= ADC_SAR_B_offset;

    mVolts = ( (int32)adcCounts * 1000 ) / ADC_SAR_B_countsPerVolt ;

    return( mVolts );
}


/*******************************************************************************
* Function Name: ADC_SAR_B_CountsTo_uVolts
********************************************************************************
*
* Summary:
*  This function converts ADC counts to micro Volts
*
* Parameters:
*  int16  adcCounts   Reading from ADC.
*
* Return:
*  int32  Result in micro Volts
*
* Global Variables:
*  ADC_SAR_B_offset variable used.
*  ADC_SAR_B_countsPerVolt variable used.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
int32 ADC_SAR_B_CountsTo_uVolts(int16 adcCounts)
{

    int32 uVolts;

    /* Subtract ADC offset */
    adcCounts -= ADC_SAR_B_offset;

    uVolts = ( (int32)adcCounts * 500000 ) / (ADC_SAR_B_countsPerVolt / 2) ;

    return( uVolts );
}


/*******************************************************************************
* Function Name: ADC_SAR_B_CountsTo_Volts
********************************************************************************
*
* Summary:
*  This function converts ADC counts to Volts
*
* Parameters:
*  int16  adcCounts   Reading from ADC.
*
* Return:
*  float  Result in mVolts
*
* Global Variables:
*  ADC_SAR_B_offset variable used.
*  ADC_SAR_B_countsPerVolt variable used.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
float ADC_SAR_B_CountsTo_Volts(int16 adcCounts)
{
    float volts;

    /* Subtract ADC offset */
    adcCounts -= ADC_SAR_B_offset;

    volts = (float)adcCounts / (float)ADC_SAR_B_countsPerVolt;   

    return( volts );
}


/* [] END OF FILE */
