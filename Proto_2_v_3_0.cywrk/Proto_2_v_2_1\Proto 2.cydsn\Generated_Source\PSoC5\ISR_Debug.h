/*******************************************************************************
* File Name: ISR_Debug.h
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
#if !defined(__ISR_Debug_INTC_H__)
#define __ISR_Debug_INTC_H__


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void ISR_Debug_Start(void);
void ISR_Debug_StartEx(cyisraddress address);
void ISR_Debug_Stop(void) ;

CY_ISR_PROTO(ISR_Debug_Interrupt);

void ISR_Debug_SetVector(cyisraddress address) ;
cyisraddress ISR_Debug_GetVector(void) ;

void ISR_Debug_SetPriority(uint8 priority) ;
uint8 ISR_Debug_GetPriority(void) ;

void ISR_Debug_Enable(void) ;
uint8 ISR_Debug_GetState(void) ;
void ISR_Debug_Disable(void) ;

void ISR_Debug_SetPending(void) ;
void ISR_Debug_ClearPending(void) ;


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the ISR_Debug ISR. */
#define ISR_Debug_INTC_VECTOR            ((reg32 *) ISR_Debug__INTC_VECT)

/* Address of the ISR_Debug ISR priority. */
#define ISR_Debug_INTC_PRIOR             ((reg8 *) ISR_Debug__INTC_PRIOR_REG)

/* Priority of the ISR_Debug interrupt. */
#define ISR_Debug_INTC_PRIOR_NUMBER      ISR_Debug__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable ISR_Debug interrupt. */
#define ISR_Debug_INTC_SET_EN            ((reg32 *) ISR_Debug__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the ISR_Debug interrupt. */
#define ISR_Debug_INTC_CLR_EN            ((reg32 *) ISR_Debug__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the ISR_Debug interrupt state to pending. */
#define ISR_Debug_INTC_SET_PD            ((reg32 *) ISR_Debug__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the ISR_Debug interrupt. */
#define ISR_Debug_INTC_CLR_PD            ((reg32 *) ISR_Debug__INTC_CLR_PD_REG)



/* __ISR_Debug_INTC_H__ */
#endif
