/*******************************************************************************
* File Name: USBFS_pm.c
* Version 1.50
*
* Description:
*  This file provides Suspend/Resume APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include <device.h>
#include "CyLib.h"
#include "USBFS.h"


/***************************************
* Custom Declratations
***************************************/
/* `#START PM_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* Local data allocation
***************************************/

static USBFS_BACKUP_STRUCT  USBFS_backup;


/*******************************************************************************
* Function Name: USBFS_DP_Interrupt
********************************************************************************
*
* Summary:
*  This Interrupt Service Routine handles DP pin changes for wake-up from 
*  the sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USBFS_DP_ISR)
{
    /* `#START DP_USER_CODE` Place your code here */

    /* `#END` */

    /* Clears active interrupt */
    CY_GET_REG8(USBFS_DP_INTSTAT_PTR);
    
    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(USBFS_PSOC3_ES2 && (USBFS_dp_int__ES2_PATCH))
        USBFS_ISR_PATCH();
    #endif /* End USBFS_PSOC3_ES2*/
}

/*******************************************************************************
* Function Name: USBFS_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
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
void USBFS_SaveConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: USBFS_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
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
void USBFS_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: USBFS_Suspend
********************************************************************************
*
* Summary:
*  This function disables the USBFS block. 
*
* Parameters:
*  None.
*
* Return:
*   None.
*
* Global variables:
*  USBFS_backup.enable:  modified.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_Suspend(void)
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    if((CY_GET_REG8(USBFS_CR0_PTR) & USBFS_CR0_ENABLE) != 0u)
    {   /* USB block is enabled */
        USBFS_backup.enableState = 1u;
        
        /* Disable USBIO for TO3 */
        #if(USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1)
            USBFS_PM_AVAIL_CR_REG &= ~USBFS_PM_AVAIL_EN_FSUSBIO;
        #endif /* End USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1 */

        /* Power Down Sequencing for USBIO for TO4*/
        #if(USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2)
            /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled */
            USBFS_USBIO_CR0_REG &= ~USBFS_USBIO_CR0_TEN;
            CyDelayUs(0);  /*~50ns delay */

            /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted) and pd_pullup_hv(Inverted) high. */
            USBFS_PM_USB_CR0_REG &= \
                                    ~(USBFS_PM_USB_CR0_PD_N | USBFS_PM_USB_CR0_PD_PULLUP_N);
         #endif /* End USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2 */
    
        /* Disable the SIE */
        USBFS_CR0_REG &= ~USBFS_CR0_ENABLE;

        #if(USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2)
            CyDelayUs(0);  /*~50ns delay */
            /* Store mode and Disable VRegulator*/
            USBFS_backup.mode = USBFS_CR1_REG & USBFS_CR1_REG_ENABLE;
            USBFS_CR1_REG &= ~USBFS_CR1_REG_ENABLE;

            CyDelayUs(1);  /* 0.5 us min delay */
            /* Disable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
            USBFS_PM_USB_CR0_REG &= ~USBFS_PM_USB_CR0_REF_EN;

            /* Switch DP and DM terminals to GPIO mode and disconnect 1.5k pullup*/
            USBFS_USBIO_CR1_REG |= USBFS_USBIO_CR1_IOMODE;
            
        #endif /* End USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2*/

        /* Disable USB in ACT PM */
        USBFS_PM_ACT_CFG_REG &= ~USBFS_PM_ACT_EN_FSUSB;
                                                                     
        CyDelayUs(1); /* min  0.5us delay required */

    }
    else 
    {
        USBFS_backup.enableState = 0u;
    }
    CyExitCriticalSection(enableInterrupts);
    
    /* Set the DP Interrupt for wake-up from sleep mode. */
    CyIntSetVector(USBFS_DP_INTC_VECT_NUM,   USBFS_DP_ISR);
    CyIntSetPriority(USBFS_DP_INTC_VECT_NUM, USBFS_DP_INTC_PRIOR);
    CyIntClearPending(USBFS_DP_INTC_VECT_NUM);
    CyIntEnable(USBFS_DP_INTC_VECT_NUM);

}


/*******************************************************************************
* Function Name: USBFS_Resume
********************************************************************************
*
* Summary:
*  This function enables the USBFS block
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  USBFS_backup - cheked.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_Resume(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    if(USBFS_backup.enableState != 0u)
    {
        CyIntDisable(USBFS_DP_INTC_VECT_NUM);
        
        /* Enable USBIO for TO3 */
        #if(USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1)
            USBFS_PM_AVAIL_CR_REG |= USBFS_PM_AVAIL_EN_FSUSBIO;
        #endif /* End USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1 */

        /* Enable USB block */
        USBFS_PM_ACT_CFG_REG |= USBFS_PM_ACT_EN_FSUSB;
        /* Enable core clock */
        USBFS_USB_CLK_EN_REG |= USBFS_USB_CLK_ENABLE;
    
        /* USBIO Wakeup Sequencing for TO4 */
        #if (USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2)
            /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
            USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_REF_EN;
            /* The reference will be available ~40us after power restored */
            CyDelayUs(40); 
            /* Return VRegulator*/
            USBFS_CR1_REG |= USBFS_backup.mode;
            CyDelayUs(0);  /*~50ns delay */
            /* Enable USBIO */
            USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_PD_N;
            CyDelayUs(2); 
            /* Set the USBIO pull-up enable */
            USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_PD_PULLUP_N;
        #endif /* End USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2 */
    
        /* STALL_IN_OUT */
        CY_SET_REG8(USBFS_EP0_CR_PTR, USBFS_MODE_STALL_IN_OUT);
        /* Enable the SIE with a last address */
        USBFS_CR0_REG |= USBFS_CR0_ENABLE;
        /* Finally, Enable d+ pullup and select iomode to USB mode*/
        CY_SET_REG8(USBFS_USBIO_CR1_PTR, USBFS_USBIO_CR1_USBPUEN);

    }
    CyExitCriticalSection(enableInterrupts);
}


/* [] END OF FILE */
