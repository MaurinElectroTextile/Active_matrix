/*******************************************************************************
* File Name: VDAC8_Debug.c  
* Version 1.60
*
*  Description:
*    This file provides the source code to the API for the 8-bit Voltage DAC 
*    (VDAC8) User Module.
*
*   Note:
*     Any unusual or non-standard behavior should be noted here. Other-
*     wise, this section should remain blank.
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "VDAC8_Debug.h"

uint8 VDAC8_Debug_initVar = 0;


/*******************************************************************************
* Function Name: VDAC8_Debug_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void:  
*
* Return: 
*  (void)
*
* Theory: 
*
* Side Effects: 
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_Debug_Init(void) 
{
    VDAC8_Debug_CR0 = (VDAC8_Debug_MODE_V )  ;    

    /* Set default data source */
    if(VDAC8_Debug_DEFAULT_DATA_SRC != 0 )    
    {
        VDAC8_Debug_CR1 = (VDAC8_Debug_DEFAULT_CNTL | VDAC8_Debug_DACBUS_ENABLE) ;   
    }
    else
    {
        VDAC8_Debug_CR1 = (VDAC8_Debug_DEFAULT_CNTL | VDAC8_Debug_DACBUS_DISABLE) ;  
    } 
    
    /* Set default strobe mode */
    if(VDAC8_Debug_DEFAULT_STRB != 0)
    {
        VDAC8_Debug_Strobe |= VDAC8_Debug_STRB_EN ;
    }
    
    /* Set default range */
    VDAC8_Debug_SetRange(VDAC8_Debug_DEFAULT_RANGE); 
    
    /* Set default speed */
    VDAC8_Debug_SetSpeed(VDAC8_Debug_DEFAULT_SPEED);
}


/*******************************************************************************
* Function Name: VDAC8_Debug_Enable
********************************************************************************
* Summary:
*     Enable the VDAC8
* 
* Parameters:  
*  void:  
*
* Return: 
*  (void)
*
* Theory: 
*
* Side Effects: 
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_Debug_Enable(void) 
{
    VDAC8_Debug_PWRMGR |= VDAC8_Debug_ACT_PWR_EN;
    VDAC8_Debug_STBY_PWRMGR |= VDAC8_Debug_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name: VDAC8_Debug_Start
********************************************************************************
*
* Summary:
*  The start function initializes the voltage DAC with the default values, 
*  and sets the power to the given level.  A power level of 0, is the same as executing
*  the stop function.
*
* Parameters:  
*  Power:   Sets power level between off (0) and (3) high power
*
* Return: 
*  (void) 
*
* Global variables:
*  VDAC8_Debug_initVar: Is modified when this function is called for the first 
*   time. Is used to ensure that initialization happens only once.
*
* Reenrancy: 
*  No
*
*******************************************************************************/
void VDAC8_Debug_Start(void) 
{  
    /* Hardware initiazation only needs to occure the first time */
    if ( VDAC8_Debug_initVar == 0)  
    { 
        VDAC8_Debug_Init();
       
        VDAC8_Debug_initVar = 1;
    }  
   
    /* Enable power to DAC */
    VDAC8_Debug_Enable();
    
    /* Set default value */
    VDAC8_Debug_SetValue(VDAC8_Debug_DEFAULT_DATA); 
}


/*******************************************************************************
* Function Name: VDAC8_Debug_Stop
********************************************************************************
*
* Summary:
*  Powers down DAC to lowest power state.
*
* Parameters:  
*  (void)  
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_Debug_Stop(void) 
{
   /* Disble power to DAC */
   VDAC8_Debug_PWRMGR &= ~VDAC8_Debug_ACT_PWR_EN;
   VDAC8_Debug_STBY_PWRMGR &= ~VDAC8_Debug_STBY_PWR_EN;
}


/*******************************************************************************
* Function Name: VDAC8_Debug_SetSpeed
********************************************************************************
*
* Summary:
*  Set DAC speed
*
* Parameters:  
*  power:   Sets speed value 
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_Debug_SetSpeed(uint8 speed) 
{
   /* Clear power mask then write in new value */
   VDAC8_Debug_CR0 &= ~VDAC8_Debug_HS_MASK ;    
   VDAC8_Debug_CR0 |=  ( speed & VDAC8_Debug_HS_MASK) ;    
}


/*******************************************************************************
* Function Name: VDAC8_Debug_SetRange
********************************************************************************
*
* Summary:
*  Set one of three current ranges.
*
* Parameters:  
*  Range:  Sets one of Three valid ranges.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_Debug_SetRange(uint8 range) 
{
   VDAC8_Debug_CR0 &= ~VDAC8_Debug_RANGE_MASK ;      /* Clear existing mode */
   VDAC8_Debug_CR0 |= ( range & VDAC8_Debug_RANGE_MASK ) ; /*  Set Range  */
   VDAC8_Debug_DacTrim();
}


/*******************************************************************************
* Function Name: VDAC8_Debug_SetValue
********************************************************************************
*
* Summary:
*  Set 8-bit DAC value
*
* Parameters:  
*  value:  Sets DAC value between 0 and 255.
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*******************************************************************************/
void VDAC8_Debug_SetValue(uint8 value) 
{
   VDAC8_Debug_Data = value;                             /*  Set Value  */
   
   /* PSOC3 silicons earlier to ES3 require a double write */
   #if (VDAC8_Debug_PSOC3_ES2 || VDAC8_Debug_PSOC5_ES1)
       VDAC8_Debug_Data = value;
   #endif
}


/*******************************************************************************
* Function Name: VDAC8_Debug_DacTrim
********************************************************************************
*
* Summary:
*  Set the trim value for the given range.
*
* Parameters:  
*   range:  1V or 4V range.  See constants.  
*
* Return: 
*  (void) 
*
* Theory: 
*
* Side Effects:
*
* Reenrancy: 
*  Yes
*
*******************************************************************************/
void VDAC8_Debug_DacTrim(void) 
{
    uint8 mode;
    
    mode = ((VDAC8_Debug_CR0 & VDAC8_Debug_RANGE_MASK) >> 2) + VDAC8_Debug_TRIM_M7_1V_RNG_OFFSET;
    
    VDAC8_Debug_TR = CY_GET_XTND_REG8((uint8 *)(VDAC8_Debug_DAC_TRIM_BASE + mode));
}


/* [] END OF FILE */
