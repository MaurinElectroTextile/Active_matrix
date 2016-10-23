/*******************************************************************************
* File Name: Filter.c
* Version 1.50
*
* Description:
*  This file provides the API source code for the FILT component.
*
* Note:
*  
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Filter_PVT.h"


/*******************************************************************************
* FILT component internal variables.
*******************************************************************************/

uint8 XDATA Filter_initVar = 0u;


/*******************************************************************************
* Function Name: Filter_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  Filter_Start().
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Reentrance:
*  Yes
*
*******************************************************************************/
void Filter_Init(void) 
{
        /* Power on DFB before initializing the RAMs */
    Filter_PM_ACT_CFG_REG |= Filter_PM_ACT_MSK;

    /* Turn off Run Bit */
    Filter_CR_REG &= ~Filter_RUN_MASK;
                
    /* Enable the DFB RAMS */
    Filter_RAM_EN_REG = Filter_RAM_DIR_BUS;
        
    /* Put DFB RAM on the bus */
    Filter_RAM_DIR_REG = Filter_RAM_DIR_BUS;
        
    /* Write DFB RAMs */
    /* Control Store RAMs */
    cymemcpy( Filter_CSA_RAM,
        Filter_control, Filter_CSA_RAM_SIZE); 
    cymemcpy(Filter_CSB_RAM,
        Filter_control, Filter_CSB_RAM_SIZE); 
    /* CFSM RAM */
    cymemcpy(Filter_CFSM_RAM,
        Filter_cfsm, Filter_CFSM_RAM_SIZE); 
    /* DAta RAMs */
    cymemcpy(Filter_DA_RAM,
        Filter_data_a, Filter_DA_RAM_SIZE); 
    cymemcpy(Filter_DB_RAM,
        Filter_data_b, Filter_DB_RAM_SIZE); 
    /* ACU RAM */
    cymemcpy(Filter_ACU_RAM,
        Filter_acu, Filter_ACU_RAM_SIZE); 

    /* Take DFB RAM off the bus */
    Filter_RAM_DIR_REG = Filter_RAM_DIR_DFB;

    /* Set up interrupt and DMA events */
    Filter_SetInterruptMode(Filter_INIT_INTERRUPT_MODE);
    Filter_SetDMAMode(Filter_INIT_DMA_MODE);
        
    /* Clear any pending interrupts */
    Filter_SR_REG = 0xff;
}


/*******************************************************************************
* Function Name: Filter_Enable
********************************************************************************
*  
* Summary: 
*  Enables the DFB run bit.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Reentrance:
*  Yes
*
*******************************************************************************/
void Filter_Enable(void) 
{
    /* Power on DFB in Active mode */
    Filter_PM_ACT_CFG_REG |= Filter_PM_ACT_MSK;
        
    /* Power on DFB in Alternative Active mode */
    Filter_PM_STBY_CFG_REG |= Filter_PM_STBY_MSK;

    /* Turn on Run Bit */
    Filter_CR_REG |= Filter_RUN_MASK;
}


/*******************************************************************************
* Function Name: Filter_Start
********************************************************************************
*
* Summary:
*  This method does the prep work necessary to setup DFB.  This includes loading 
*
* Parameters:  
*  void
* 
* Return: 
*  void
*
* Global variables:
*  Filter_initVar:  Used to check the initial configuration,
*  modified when this function is called for the first time.
*
* Note: 
*  Use Filter_InterruptConfig to control which events trigger 
*  interrupts in the DFB. 
*
* Reentrance:
*  No
*
*******************************************************************************/
void Filter_Start()
{
     /* If not Initialized then initialize all required hardware and software */
    if(Filter_initVar == 0u)
    {
        Filter_Init();
        Filter_initVar = 1u;
    }

    /* Enable the DFB block */
    Filter_Enable();
}


/*******************************************************************************
* Function Name: Filter_Stop
********************************************************************************
*
* Summary:
*  Turn off the run bit.  If DMA control is used to feed the channels, allow 
*  arguments to turn one of the TD channels off. 
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Reentrance:
*  Yes
*
*******************************************************************************/
void Filter_Stop() 
{
    /* Power off DFB in Active mode */
    Filter_PM_ACT_CFG_REG &= ~Filter_PM_ACT_MSK;
    
    /* Power off DFB in Alternative Active mode */
    Filter_PM_STBY_CFG_REG &= ~Filter_PM_STBY_MSK;
        
    Filter_CR_REG &= ~(Filter_RUN_MASK);
}


/*******************************************************************************
* Function Name: Filter_Read8
********************************************************************************
*
* Summary:
*  Get the value in one of the DFB Output Holding Registers 
*
* Parameters:  
*  channel:  Filter_CHANNEL_A or Filter_CHANNEL_B
*            
* Return: 
*  The most significant 8 bits of the current output value sitting in the 
*  selected channel's holding register or 0x00 for invalid channel numbers.
*
* Reentrance:
*  Yes
*
*******************************************************************************/
uint8 Filter_Read8(uint8 channel) 
{
    if (channel == Filter_CHANNEL_A)
    {
        return Filter_HOLDAH_REG;
    }
    else if (channel == Filter_CHANNEL_B)
    {
        return Filter_HOLDBH_REG;
    }
    else
    {
        return 0;
    }
}


/*******************************************************************************
* Function Name: Filter_Read16
********************************************************************************
*
* Summary:
*  Get the value in one of the DFB Output Holding Registers 
*
* Parameters:  
*  channel:  Filter_CHANNEL_A or Filter_CHANNEL_B
*            
* Return: 
*  The most significant 16 bits of the current output value sitting in the 
*  selected channel's holding register or 0x0000 for invalid channel numbers
*
* Note:
*  Order of the read is important.  On the read of the high byte, the DFB clears
*  the data ready bit.
*
* Reentrance:
*  Yes
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__) 

    uint16 Filter_Read16(uint8 channel) 
    {
        uint16 val;
    
        if (channel == Filter_CHANNEL_A)
        {        
            val = Filter_HOLDAM_REG;
            val |= (uint16)(Filter_HOLDAH_REG) << 8;
            return val;
        }
        else if (channel == Filter_CHANNEL_B)
        {      
            val = Filter_HOLDBM_REG;
            val |= (uint16)Filter_HOLDBH_REG << 8;
            return val;
        }
        else
        {
            return 0;
        }
    }

#else

    uint16 Filter_Read16(uint8 channel) 
    {
        if (channel == Filter_CHANNEL_A)
        {        
            return Filter_HOLDA16_REG;
        }
        else if (channel == Filter_CHANNEL_B)
        {      
            return Filter_HOLDB16_REG;
        }
        else
        {
            return 0;
        }
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/*******************************************************************************
* Function Name: Filter_Read24
********************************************************************************
*
* Summary:
*  Get the value in one of the DFB Output Holding Registers 
*
* Parameters:  
*  channel:  Filter_CHANNEL_A or Filter_CHANNEL_B
*            
* Return: 
*  The current 24-bit output value sitting in the selected channel's
*  holding register or 0x00000000 for invalid channel numbers
*
* Note:
*  Order of the read is important.  On the read of the high byte, the DFB clears
*  the data ready bit.
*
* Reentrance:
*  Yes
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__)

    uint32 Filter_Read24(uint8 channel) 
    {
        uint32 val;
    
        if (channel == Filter_CHANNEL_A)
        {        
            val = Filter_HOLDA_REG;
            val |= (uint32)(Filter_HOLDAM_REG) << 8;
            val |= (uint32)(Filter_HOLDAH_REG) << 16;
            
            /* SignExtend */
            if(val & Filter_SIGN_BIT)
                val |= Filter_SIGN_BYTE;
                
            return val;
        }
        else if (channel == Filter_CHANNEL_B)
        {      
            val = Filter_HOLDB_REG;
            val |= (uint32)Filter_HOLDBM_REG << 8;
            val |= (uint32)Filter_HOLDBH_REG << 16;
            
            /* SignExtend */
            if(val & Filter_SIGN_BIT)
                val |= Filter_SIGN_BYTE;
            
            return val;
        }
        else
        {
            return 0;
        }
    }

#else

    uint32 Filter_Read24(uint8 channel) 
    {
        if (channel == Filter_CHANNEL_A)
        {        
            return Filter_HOLDA24_REG;
        }
        else if (channel == Filter_CHANNEL_B)
        {      
            return Filter_HOLDB24_REG;
        }
        else
        {
            return 0;
        }
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/*******************************************************************************
* Function Name: Filter_Write8
********************************************************************************
*
* Summary:
*  Set the value in one of the DFB Input Staging Registers 
*
* Parameters:  
*  channel:  Use either Filter_CHANNEL_A or 
*            Filter_CHANNEL_B as arguments to the function.  
*  sample:   The 8-bit, right justified input sample. 
*
* Return: 
*  void
*
* Note:
*  Order of the write is important.  On the load of the high byte, the DFB sets
*  the input ready bit.
*
* Reentrance:
*  Yes
*
*******************************************************************************/
void Filter_Write8(uint8 channel, uint8 sample) 
{
    if (channel == Filter_CHANNEL_A)
    {
        Filter_STAGEAH_REG = sample;
    }
    else if (channel == Filter_CHANNEL_B)
    {
        Filter_STAGEBH_REG = sample;
    }
    /* No Final else statement: No value is loaded on bad channel input */
}


/*******************************************************************************
* Function Name: Filter_Write16
********************************************************************************
*
* Summary:
*  Set the value in one of the DFB Input Staging Registers 
*
* Parameters:  
*  channel:  Use either Filter_CHANNEL_A or 
*            Filter_CHANNEL_B as arguments to the function.  
*  sample:   The 16-bit, right justified input sample. 
*
* Return: 
*  void
*
* Note:
*  Order of the write is important.  On the load of the high byte, the DFB sets
*  the input ready bit.
*
* Reentrance:
*  Yes
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__)

    void Filter_Write16(uint8 channel, uint16 sample) 
    {
        /* Write the STAGE MSB reg last as it signals a complete wrote to the DFB.*/
        if (channel == Filter_CHANNEL_A)
        {
            Filter_STAGEAM_REG = (uint8)(sample);
            Filter_STAGEAH_REG = (uint8)(sample >> 8 );
        }
        else if (channel == Filter_CHANNEL_B)
        {
            Filter_STAGEBM_REG = (uint8)(sample);
            Filter_STAGEBH_REG = (uint8)(sample >> 8);
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#else

    void Filter_Write16(uint8 channel, uint16 sample) 
    {
        if (channel == Filter_CHANNEL_A)
        {
            Filter_STAGEA16_REG = sample;
        }
        else if (channel == Filter_CHANNEL_B)
        {
            Filter_STAGEB16_REG = sample;
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/*******************************************************************************
* Function Name: Filter_Write24
********************************************************************************
*
* Summary:
*  Set the value in one of the DFB Input Staging Registers 
*
* Parameters:  
*  channel:  Use either Filter_CHANNEL_A or 
*            Filter_CHANNEL_B as arguments to the function.  
*  sample:   The 24-bit, right justified input sample inside of a uint32. 
*
* Return: 
*  void
*
* Note:
*  Order of the write is important.  On the load of the high byte, the DFB sets
*  the input ready bit.
*
* Reentrance:
*  Yes
*
*******************************************************************************/
#if defined(__C51__) || defined(__CX51__)

    void Filter_Write24(uint8 channel, uint32 sample) 
    {
        /* Write the STAGE LSB reg last as it signals a complete wrote to the DFB.*/
        if (channel == Filter_CHANNEL_A)
        {
            Filter_STAGEA_REG  = (uint8)(sample);
            Filter_STAGEAM_REG = (uint8)(sample >> 8 );
            Filter_STAGEAH_REG = (uint8)(sample >> 16);
        }
        else if (channel == Filter_CHANNEL_B)
        {
            Filter_STAGEB_REG = (uint8)(sample);
            Filter_STAGEBM_REG = (uint8)(sample >> 8);
            Filter_STAGEBH_REG = (uint8)(sample >> 16);
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#else

    void Filter_Write24(uint8 channel, uint32 sample) 
    {
        if (channel == Filter_CHANNEL_A)
        {
            Filter_STAGEA24_REG = sample;
        }
        else if (channel == Filter_CHANNEL_B)
        {
            Filter_STAGEB24_REG = sample;
        }
        /* No Final else statement: No value is loaded on bad channel input */
    }

#endif /* defined(__C51__) || defined(__CX51__) */


/* [] END OF FILE */
