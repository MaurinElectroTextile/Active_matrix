/*******************************************************************************
* File Name: `$INSTANCE_NAME`_episr.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
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
#include "`$INSTANCE_NAME`.h"
#include "`$INSTANCE_NAME`_API.h"

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
void `$INSTANCE_NAME`_InitComponent(uint8 bDevice, uint8 bMode);
/*******************************************************************************
* External references
********************************************************************************/
extern T_`$INSTANCE_NAME`_EP_CTL_BLOCK `$INSTANCE_NAME`_EP[];
extern uint8 `$INSTANCE_NAME`_bDevice;
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_1_ISR
********************************************************************************
* Summary:
*   Endpoint 1 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_1_ISR)
{
/* `#START EP1_USER_CODE` Place your code here */

/* `#END` */
   CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP1_CR0); /* Must read the mode reg */
   `$INSTANCE_NAME`_EP[1].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
   `$INSTANCE_NAME`_EP[1].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x01);
   
   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_1__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	
}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_2_ISR
********************************************************************************
* Summary:
*   Endpoint 2 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_2_ISR)
{
/* `#START EP2_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP2_CR0); /* Must read the mode reg */
    `$INSTANCE_NAME`_EP[2].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
    `$INSTANCE_NAME`_EP[2].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x02);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_2__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_3_ISR
********************************************************************************
* Summary:
*   Endpoint 3 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_3_ISR)
{
/* `#START EP3_USER_CODE` Place your code here */

/* `#END` */
	uint8 bCount;
	/* Read the data, but do not re-enable EP3 */
	bCount = (uint8)`$INSTANCE_NAME`_wGetEPCount(3);
	`$INSTANCE_NAME`_ReadOutEP( 3, ISRBuffer, bCount);
	FIFO_Write( &USBRxFIFO, ISRBuffer, bCount);
	
    CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP3_CR0); /* Must read the mode reg */
    `$INSTANCE_NAME`_EP[3].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
    `$INSTANCE_NAME`_EP[3].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x04);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_3__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_4_ISR
********************************************************************************
* Summary:
*   Endpoint 4 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_4_ISR)
{
/* `#START EP4_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP4_CR0); /* Must read the mode reg */
    `$INSTANCE_NAME`_EP[4].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
    `$INSTANCE_NAME`_EP[4].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x08);
   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 

	#if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_4__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	
}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_5_ISR
********************************************************************************
* Summary:
*   Endpoint 5 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_5_ISR)
{
/* `#START EP5_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP5_CR0); /* Must read the mode reg */
    `$INSTANCE_NAME`_EP[5].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
    `$INSTANCE_NAME`_EP[5].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x10);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_5__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_6_ISR
********************************************************************************
* Summary:
*   Endpoint 6 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_6_ISR)
{
/* `#START EP6_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP6_CR0); /* Must read the mode reg */
    `$INSTANCE_NAME`_EP[6].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
    `$INSTANCE_NAME`_EP[6].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x20);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_6__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_7_ISR
********************************************************************************
* Summary:
*   Endpoint 7 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_7_ISR)
{
/* `#START EP7_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP7_CR0); /* Must read the mode reg */
    `$INSTANCE_NAME`_EP[7].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
    `$INSTANCE_NAME`_EP[7].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x40);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_7__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EP_8_ISR
********************************************************************************
* Summary:
*   Endpoint 8 Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_EP_8_ISR)
{
/* `#START EP8_USER_CODE` Place your code here */

/* `#END` */
    CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP8_CR0); /* Must read the mode reg */
    `$INSTANCE_NAME`_EP[8].bEPToggle ^= `$INSTANCE_NAME`_EPX_CNT_DATA_TOGGLE;
    `$INSTANCE_NAME`_EP[8].bAPIEPState = `$INSTANCE_NAME`_EVENT_PENDING;
    CY_SET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR, CY_GET_REG8(`$INSTANCE_NAME`_SIE_EP_INT_SR) & ~0x80);

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_ep_8__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SOF_ISR
********************************************************************************
* Summary:
*   Start of Frame Interrupt Service Routine
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_SOF_ISR)
{
/* `#START SOF_USER_CODE` Place your code here */

/* `#END` */

   /* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_sof_int__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_BUS_RESET_ISR
********************************************************************************
* Summary:
*   USB Bus Reset Interrupt Service Routine.  Calls _Start with the same 
*   parameters as the last USER call to _Start
*   
* Parameters:  
*   None
*******************************************************************************/
CY_ISR(`$INSTANCE_NAME`_BUS_RESET_ISR)
{
/* `#START BUS_RESET_USER_CODE` Place your code here */

/* `#END` */
	`$INSTANCE_NAME`_InitComponent(`$INSTANCE_NAME`_bDevice, CY_GET_REG8(`$INSTANCE_NAME`_CR1));

	/* PSoC3 ES1, ES2 RTC ISR PATCH  */ 
    #if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
        #if((CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2) && (`$INSTANCE_NAME`_bus_reset__ES2_PATCH))      
            `$INSTANCE_NAME`_ISR_PATCH();
        #endif
    #endif	

}
