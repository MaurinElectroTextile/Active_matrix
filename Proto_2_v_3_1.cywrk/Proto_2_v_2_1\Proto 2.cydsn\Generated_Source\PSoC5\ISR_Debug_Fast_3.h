/*******************************************************************************
* File Name: ISR_Debug_Fast_3.h
* Version 1.50
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#if !defined(__ISR_Debug_Fast_3_INTC_H__)
#define __ISR_Debug_Fast_3_INTC_H__


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ISR_Debug_Fast_3_Start(void);
void ISR_Debug_Fast_3_StartEx(cyisraddress address);
void ISR_Debug_Fast_3_Stop(void) ;

CY_ISR_PROTO(ISR_Debug_Fast_3_Interrupt);

void ISR_Debug_Fast_3_SetVector(cyisraddress address) ;
cyisraddress ISR_Debug_Fast_3_GetVector(void) ;

void ISR_Debug_Fast_3_SetPriority(uint8 priority) ;
uint8 ISR_Debug_Fast_3_GetPriority(void) ;

void ISR_Debug_Fast_3_Enable(void) ;
uint8 ISR_Debug_Fast_3_GetState(void) ;
void ISR_Debug_Fast_3_Disable(void) ;

void ISR_Debug_Fast_3_SetPending(void) ;
void ISR_Debug_Fast_3_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ISR_Debug_Fast_3 ISR. */
#define ISR_Debug_Fast_3_INTC_VECTOR            ((reg32 *) ISR_Debug_Fast_3__INTC_VECT)

/* Address of the ISR_Debug_Fast_3 ISR priority. */
#define ISR_Debug_Fast_3_INTC_PRIOR             ((reg8 *) ISR_Debug_Fast_3__INTC_PRIOR_REG)

/* Priority of the ISR_Debug_Fast_3 interrupt. */
#define ISR_Debug_Fast_3_INTC_PRIOR_NUMBER      ISR_Debug_Fast_3__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ISR_Debug_Fast_3 interrupt. */
#define ISR_Debug_Fast_3_INTC_SET_EN            ((reg32 *) ISR_Debug_Fast_3__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ISR_Debug_Fast_3 interrupt. */
#define ISR_Debug_Fast_3_INTC_CLR_EN            ((reg32 *) ISR_Debug_Fast_3__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ISR_Debug_Fast_3 interrupt state to pending. */
#define ISR_Debug_Fast_3_INTC_SET_PD            ((reg32 *) ISR_Debug_Fast_3__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ISR_Debug_Fast_3 interrupt. */
#define ISR_Debug_Fast_3_INTC_CLR_PD            ((reg32 *) ISR_Debug_Fast_3__INTC_CLR_PD_REG)



/* __ISR_Debug_Fast_3_INTC_H__ */
#endif
