/*******************************************************************************
* File Name: USBFS_episr.c
* Version 1.50
*
* Description:
*  Data endpoint Interrupt Service Routines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "USBFS.h"


/***************************************
* Custom Declratations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
* External function references
***************************************/

void USBFS_InitComponent(uint8 device, uint8 mode);
void USBFS_ReInitComponent(void);


/***************************************
* External references
***************************************/

extern volatile T_USBFS_EP_CTL_BLOCK USBFS_EP[];
extern volatile uint8 USBFS_device;


#if(USBFS_EP1_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_1_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 1 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_1_ISR)
    {
        /* `#START EP1_USER_CODE` Place your code here */

        /* `#END` */
    
        CY_GET_REG8(USBFS_SIE_EP1_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP1].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP1].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                            & ~USBFS_SIE_EP_INT_EP1_MASK);
    
        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_1__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }

#endif   /* End USBFS_EP1_ISR_REMOVE */


#if(USBFS_EP2_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_2_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 2 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_2_ISR)
    {
        /* `#START EP2_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USBFS_SIE_EP2_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP2].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP2].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                        & ~USBFS_SIE_EP_INT_EP2_MASK);

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_2__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }

#endif   /* End USBFS_EP2_ISR_REMOVE */


#if(USBFS_EP3_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_3_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 3 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_3_ISR)
    {
        /* `#START EP3_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USBFS_SIE_EP3_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP3].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP3].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                        & ~USBFS_SIE_EP_INT_EP3_MASK);

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_3__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }

#endif   /* End USBFS_EP3_ISR_REMOVE */


#if(USBFS_EP4_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_4_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 4 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_4_ISR)
    {
        /* `#START EP4_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USBFS_SIE_EP4_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP4].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP4].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                        & ~USBFS_SIE_EP_INT_EP4_MASK);

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_4__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }

#endif   /* End USBFS_EP4_ISR_REMOVE */


#if(USBFS_EP5_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_5_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 5 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_5_ISR)
    {
        /* `#START EP5_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USBFS_SIE_EP5_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP5].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP5].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                        & ~USBFS_SIE_EP_INT_EP5_MASK);

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_5__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }
#endif   /* End USBFS_EP5_ISR_REMOVE */


#if(USBFS_EP6_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_6_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 6 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_6_ISR)
    {
        /* `#START EP6_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USBFS_SIE_EP6_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP6].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP6].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                        & ~USBFS_SIE_EP_INT_EP6_MASK);

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_6__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }

#endif   /* End USBFS_EP6_ISR_REMOVE */


#if(USBFS_EP7_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_7_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 7 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_7_ISR)
    {
        /* `#START EP7_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USBFS_SIE_EP7_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP7].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP7].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                        & ~USBFS_SIE_EP_INT_EP7_MASK);

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_7__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }

#endif   /* End USBFS_EP7_ISR_REMOVE */


#if(USBFS_EP8_ISR_REMOVE == 0u)

    /*******************************************************************************
    * Function Name: USBFS_EP_8_ISR
    ********************************************************************************
    *
    * Summary:
    *  Endpoint 8 Interrupt Service Routine
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(USBFS_EP_8_ISR)
    {
        /* `#START EP8_USER_CODE` Place your code here */

        /* `#END` */

        CY_GET_REG8(USBFS_SIE_EP8_CR0_PTR); /* Must read the mode reg */
        USBFS_EP[USBFS_EP8].epToggle ^= USBFS_EPX_CNT_DATA_TOGGLE;
        USBFS_EP[USBFS_EP8].apiEpState = USBFS_EVENT_PENDING;
        CY_SET_REG8(USBFS_SIE_EP_INT_SR_PTR, CY_GET_REG8(USBFS_SIE_EP_INT_SR_PTR) \
                                                                        & ~USBFS_SIE_EP_INT_EP8_MASK);

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(USBFS_PSOC3_ES2 && (USBFS_ep_8__ES2_PATCH))
            USBFS_ISR_PATCH();
        #endif /* End USBFS_PSOC3_ES2*/
    }

#endif   /* End USBFS_EP8_ISR_REMOVE */


/*******************************************************************************
* Function Name: USBFS_SOF_ISR
********************************************************************************
*
* Summary:
*  Start of Frame Interrupt Service Routine
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USBFS_SOF_ISR)
{
    /* `#START SOF_USER_CODE` Place your code here */

    /* `#END` */

    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(USBFS_PSOC3_ES2 && (USBFS_sof_int__ES2_PATCH))
        USBFS_ISR_PATCH();
    #endif /* End USBFS_PSOC3_ES2*/
}


/*******************************************************************************
* Function Name: USBFS_BUS_RESET_ISR
********************************************************************************
*
* Summary:
*  USB Bus Reset Interrupt Service Routine.  Calls _Start with the same
*  parameters as the last USER call to _Start
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
CY_ISR(USBFS_BUS_RESET_ISR)
{
    /* `#START BUS_RESET_USER_CODE` Place your code here */

    /* `#END` */

    USBFS_ReInitComponent();

    /* PSoC3 ES1, ES2 RTC ISR PATCH  */
    #if(USBFS_PSOC3_ES2 && (USBFS_bus_reset__ES2_PATCH))
        USBFS_ISR_PATCH();
    #endif /* End USBFS_PSOC3_ES2*/
}


/* [] END OF FILE */
