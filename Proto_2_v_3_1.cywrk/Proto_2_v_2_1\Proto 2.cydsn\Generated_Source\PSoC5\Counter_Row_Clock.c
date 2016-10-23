/*******************************************************************************
* File Name: Counter_Row_Clock.c  
* Version 1.50
*
*  Description:
*     The Counter User Module consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Counter_Row_Clock.h"

uint8 Counter_Row_Clock_initVar = 0u;


/*******************************************************************************
* Function Name: Counter_Row_Clock_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_Init(void) 
{
        #if (!Counter_Row_Clock_UsingFixedFunction && !Counter_Row_Clock_ControlRegRemoved)
            uint8 ctrl;
        #endif
        
        #if (Counter_Row_Clock_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Counter_Row_Clock_CONTROL &= Counter_Row_Clock_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (Counter_Row_Clock_PSOC3_ES2 || Counter_Row_Clock_PSOC5_ES1)
                Counter_Row_Clock_CONTROL2 &= ~Counter_Row_Clock_CTRL_MODE_MASK;
            #endif
            #if (Counter_Row_Clock_PSOC3_ES3 || Counter_Row_Clock_PSOC5_ES2)
                Counter_Row_Clock_CONTROL3 &= ~Counter_Row_Clock_CTRL_MODE_MASK;                
            #endif
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Counter_Row_Clock_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Counter_Row_Clock_CONTROL |= 0x04u;
            #endif
            
            /* Set the IRQ to use the status register interrupts */
            Counter_Row_Clock_CONTROL2 |= Counter_Row_Clock_CTRL2_IRQ_SEL;
        #else
            #if(!Counter_Row_Clock_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Counter_Row_Clock_CONTROL & ~Counter_Row_Clock_CTRL_CMPMODE_MASK;
            Counter_Row_Clock_CONTROL = ctrl | Counter_Row_Clock_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Counter_Row_Clock_CONTROL & ~Counter_Row_Clock_CTRL_CAPMODE_MASK;
            Counter_Row_Clock_CONTROL = ctrl | Counter_Row_Clock_DEFAULT_CAPTURE_MODE;
            #endif
        #endif 
        
        /* Clear all data in the FIFO's */
        #if (!Counter_Row_Clock_UsingFixedFunction)
            Counter_Row_Clock_ClearFIFO();
        #endif
        
        /* Set Initial values from Configuration */
        Counter_Row_Clock_WritePeriod(Counter_Row_Clock_INIT_PERIOD_VALUE);
        Counter_Row_Clock_WriteCounter(Counter_Row_Clock_INIT_COUNTER_VALUE);
        Counter_Row_Clock_SetInterruptMode(Counter_Row_Clock_INIT_INTERRUPTS_MASK);
        
        #if (!Counter_Row_Clock_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            Counter_Row_Clock_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Counter_Row_Clock_WriteCompare(Counter_Row_Clock_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            Counter_Row_Clock_STATUS_AUX_CTRL |= Counter_Row_Clock_STATUS_ACTL_INT_EN_MASK;
        #endif
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Counter_Row_Clock_UsingFixedFunction)
        Counter_Row_Clock_GLOBAL_ENABLE |= Counter_Row_Clock_BLOCK_EN_MASK;
        Counter_Row_Clock_GLOBAL_STBY_ENABLE |= Counter_Row_Clock_BLOCK_STBY_EN_MASK;
    #endif   
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Counter_Row_Clock_ControlRegRemoved || Counter_Row_Clock_UsingFixedFunction)
        Counter_Row_Clock_CONTROL |= Counter_Row_Clock_CTRL_ENABLE;                
    #endif
    
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  Counter_Row_Clock_initVar: Is modified when this function is called for the first 
*   time. Is used to ensure that initialization happens only once.
*
* Reentrant
*  No
*
*******************************************************************************/
void Counter_Row_Clock_Start(void) 
{
    if(Counter_Row_Clock_initVar == 0u)
    {
        Counter_Row_Clock_Init();
        
        Counter_Row_Clock_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Counter_Row_Clock_Enable();        
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_Stop(void) 
{
    /* Disable Counter */
    #if(!Counter_Row_Clock_ControlRegRemoved || Counter_Row_Clock_UsingFixedFunction)
        Counter_Row_Clock_CONTROL &= ~Counter_Row_Clock_CTRL_ENABLE;        
    #endif
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Counter_Row_Clock_UsingFixedFunction)
        Counter_Row_Clock_GLOBAL_ENABLE &= ~Counter_Row_Clock_BLOCK_EN_MASK;
        Counter_Row_Clock_GLOBAL_STBY_ENABLE &= ~Counter_Row_Clock_BLOCK_STBY_EN_MASK;
    #endif
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_SetInterruptMode(uint8 interruptsMask) 
{
    Counter_Row_Clock_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_GetInterruptSource
********************************************************************************
* Summary:
* Returns the status register with data about the interrupt source.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8): Status Register Bit-Field of interrupt source(s)
*
* Side Effects:  The Status register may be clear on read and all interrupt
*                sources must be handled.
*
* Reentrant
*  Yes
*
*******************************************************************************/
uint8 Counter_Row_Clock_GetInterruptSource(void) 
{
    return Counter_Row_Clock_STATUS;
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
* Reentrant
*  Yes
*
*******************************************************************************/
uint8   Counter_Row_Clock_ReadStatusRegister(void) 
{
    return Counter_Row_Clock_STATUS;
}


#if(!Counter_Row_Clock_ControlRegRemoved)
/*******************************************************************************
* Function Name: Counter_Row_Clock_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
* Reentrant
*  Yes
*
*******************************************************************************/
uint8   Counter_Row_Clock_ReadControlRegister(void) 
{
    return Counter_Row_Clock_CONTROL;
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*   
* Reentrant
*  Yes
*
*******************************************************************************/
void    Counter_Row_Clock_WriteControlRegister(uint8 control) 
{
    Counter_Row_Clock_CONTROL = control;
}

#endif  /* (!Counter_Row_Clock_ControlRegRemoved) */


/*******************************************************************************
* Function Name: Counter_Row_Clock_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_WriteCounter(uint8 counter) 
{
    #if(Counter_Row_Clock_UsingFixedFunction)
        CY_SET_REG16(Counter_Row_Clock_COUNTER_LSB_PTR, (uint16)counter);
    #else
        CY_SET_REG8(Counter_Row_Clock_COUNTER_LSB_PTR, counter);
    #endif
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
* Reentrant
*  Yes
*
*******************************************************************************/
uint8 Counter_Row_Clock_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    CY_GET_REG8(Counter_Row_Clock_COUNTER_LSB_PTR);
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    return (CY_GET_REG8(Counter_Row_Clock_STATICCOUNT_LSB_PTR));
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
* Reentrant
*  Yes
*
*******************************************************************************/
uint8 Counter_Row_Clock_ReadCapture(void) 
{
   return ( CY_GET_REG8(Counter_Row_Clock_STATICCOUNT_LSB_PTR) );  
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_WritePeriod(uint8 period) 
{
    #if(Counter_Row_Clock_UsingFixedFunction)
        CY_SET_REG16(Counter_Row_Clock_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(Counter_Row_Clock_PERIOD_LSB_PTR,period);
    #endif
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
* Reentrant
*  Yes
*
*******************************************************************************/
uint8 Counter_Row_Clock_ReadPeriod(void) 
{
   return ( CY_GET_REG8(Counter_Row_Clock_PERIOD_LSB_PTR));
}


#if (!Counter_Row_Clock_UsingFixedFunction)
/*******************************************************************************
* Function Name: Counter_Row_Clock_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_WriteCompare(uint8 compare) 
{
    #if(Counter_Row_Clock_UsingFixedFunction)
        CY_SET_REG16(Counter_Row_Clock_COMPARE_LSB_PTR,(uint16)compare);
    #else
        CY_SET_REG8(Counter_Row_Clock_COMPARE_LSB_PTR,compare);
    #endif
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
* Reentrant
*  Yes
*
*******************************************************************************/
uint8 Counter_Row_Clock_ReadCompare(void) 
{
   return ( CY_GET_REG8(Counter_Row_Clock_COMPARE_LSB_PTR));
}


#if (Counter_Row_Clock_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_Row_Clock_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Counter_Row_Clock_CONTROL &= ~Counter_Row_Clock_CTRL_CMPMODE_MASK;
    
    /* Write the new setting */
    Counter_Row_Clock_CONTROL |= (compareMode << Counter_Row_Clock_CTRL_CMPMODE0_SHIFT);
}
#endif  /* (Counter_Row_Clock_COMPARE_MODE_SOFTWARE) */


#if (Counter_Row_Clock_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Counter_Row_Clock_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Counter_Row_Clock_CONTROL &= ~Counter_Row_Clock_CTRL_CAPMODE_MASK;
    
    /* Write the new setting */
    Counter_Row_Clock_CONTROL |= (captureMode << Counter_Row_Clock_CTRL_CAPMODE0_SHIFT);
}
#endif  /* (Counter_Row_Clock_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Counter_Row_Clock_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
* Reentrant
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_ClearFIFO(void) 
{

    while(Counter_Row_Clock_ReadStatusRegister() & Counter_Row_Clock_STATUS_FIFONEMP)
    {
        Counter_Row_Clock_ReadCapture();
    }

}
#endif  /* (!Counter_Row_Clock_UsingFixedFunction) */


/* [] END OF FILE */

