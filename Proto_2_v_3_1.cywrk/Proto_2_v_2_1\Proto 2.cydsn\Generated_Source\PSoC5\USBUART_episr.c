/*******************************************************************************
* File Name: USBUART_episr.c
* Version 0.0
*
*  Description:
*    Data endpoint Interrupt Service Routines
*
*   Note:
*
********************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cyfitter.h"
#include "USBUART.h"
#include "USBUART_API.h"

/*******************************************************************************
* Custom Declratations
********************************************************************************/
extern FIFO USBRxFIFO;
uint8 ISRBuffer[64];
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */
/*******************************************************************************
* External function references
********************************************************************************/
void USBUART_InitComponent(uint8 bDevice, uint8 bMode);
/*******************************************************************************
* External references
********************************************************************************/
extern T_USBUART_EP_CTL_BLOCK USBUART_EP[];
extern uint8 USBUART_bDevice;
/*******************************************************************************
* Function Name: USBUART_EP_1_ISR
********************************************************************************
* Summary:
*   Endpoint 1 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_1_ISR)
{
/* `#START EP1_USER_CODE` Place your code here */

/* `#END` */
   CY_GET_REG8(USBUART_SIE_EP1_CR0); /* Must read the mode reg */
   USBUART_EP[1].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
   USBUART_EP[1].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x01);
   
   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_1__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	
}
/*******************************************************************************
* Function Name: USBUART_EP_2_ISR
********************************************************************************
* Summary:
*   Endpoint 2 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_2_ISR)
{
/* `#START EP2_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(USBUART_SIE_EP2_CR0); /* Must read the mode reg */
    USBUART_EP[2].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
    USBUART_EP[2].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x02);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_2__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: USBUART_EP_3_ISR
********************************************************************************
* Summary:
*   Endpoint 3 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_3_ISR)
{
/* `#START EP3_USER_CODE` Place your code here */

/* `#END` */
	uint8 bCount;
	/* Read the data, but do not re-enable EP3 */
	bCount = (uint8)USBUART_wGetEPCount(3);
	USBUART_ReadOutEP( 3, ISRBuffer, bCount);
	FIFO_Write( &USBRxFIFO, ISRBuffer, bCount);
	
    CY_GET_REG8(USBUART_SIE_EP3_CR0); /* Must read the mode reg */
    USBUART_EP[3].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
    USBUART_EP[3].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x04);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_3__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: USBUART_EP_4_ISR
********************************************************************************
* Summary:
*   Endpoint 4 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_4_ISR)
{
/* `#START EP4_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(USBUART_SIE_EP4_CR0); /* Must read the mode reg */
    USBUART_EP[4].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
    USBUART_EP[4].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x08);
   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 

	#if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_4__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	
}
/*******************************************************************************
* Function Name: USBUART_EP_5_ISR
********************************************************************************
* Summary:
*   Endpoint 5 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_5_ISR)
{
/* `#START EP5_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(USBUART_SIE_EP5_CR0); /* Must read the mode reg */
    USBUART_EP[5].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
    USBUART_EP[5].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x10);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_5__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: USBUART_EP_6_ISR
********************************************************************************
* Summary:
*   Endpoint 6 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_6_ISR)
{
/* `#START EP6_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(USBUART_SIE_EP6_CR0); /* Must read the mode reg */
    USBUART_EP[6].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
    USBUART_EP[6].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x20);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_6__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: USBUART_EP_7_ISR
********************************************************************************
* Summary:
*   Endpoint 7 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_7_ISR)
{
/* `#START EP7_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(USBUART_SIE_EP7_CR0); /* Must read the mode reg */
    USBUART_EP[7].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
    USBUART_EP[7].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x40);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_7__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: USBUART_EP_8_ISR
********************************************************************************
* Summary:
*   Endpoint 8 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_EP_8_ISR)
{
/* `#START EP8_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(USBUART_SIE_EP8_CR0); /* Must read the mode reg */
    USBUART_EP[8].bEPToggle ^= USBUART_EPX_CNT_DATA_TOGGLE;
    USBUART_EP[8].bAPIEPState = USBUART_EVENT_PENDING;
    CY_SET_REG8(USBUART_SIE_EP_INT_SR, CY_GET_REG8(USBUART_SIE_EP_INT_SR) & ~0x80);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_ep_8__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: USBUART_SOF_ISR
********************************************************************************
* Summary:
*   Start of Frame Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_SOF_ISR)
{
/* `#START SOF_USER_CODE` Place your code here */

/* `#END` */

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_sof_int__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: USBUART_BUS_RESET_ISR
********************************************************************************
* Summary:
*   USB Bus Reset Interrupt Service Routine.  Calls _Start with the same 
*   parameters as the last USER call to _Start
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(USBUART_BUS_RESET_ISR)
{
/* `#START BUS_RESET_USER_CODE` Place your code here */

/* `#END` */
	USBUART_InitComponent(USBUART_bDevice, CY_GET_REG8(USBUART_CR1));

	/* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (USBUART_bus_reset__ES2_PATCH))      
            USBUART_ISR_PATCH();
        #endif
    #endif	

}
