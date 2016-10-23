/*******************************************************************************
* File Name: UART_1_INT.c
* Version 2.0
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
* Note:
*  Any unusual or non-standard behavior should be noted here. Other-
*  wise, this section should remain blank.
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "UART_1.h"


/***************************************
* Custom Declratations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if( (UART_1_RX_ENABLED || UART_1_HD_ENABLED) && \
     (UART_1_RXBUFFERSIZE > UART_1_FIFO_LENGTH))

    extern volatile uint8 UART_1_rxBuffer[];
    extern volatile uint8 UART_1_rxBufferRead;
    extern volatile uint8 UART_1_rxBufferWrite;
    extern volatile uint8 UART_1_rxBufferLoopDetect;
    extern volatile uint8 UART_1_rxBufferOverflow;


    /*******************************************************************************
    * Function Name: UART_1_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_1_rxBuffer - RAM buffer pointer for save received data.
    *  UART_1_rxBufferWrite - cyclic index for write to rxBuffer, 
    *     increments after each byte saved to buffer.
    *  UART_1_rxBufferRead - cyclic index for read from rxBuffer, 
    *     checked to detect overflow condition.
    *  UART_1_rxBufferOverflow - software overflow flag. Set to one
    *     when UART_1_rxBufferWrite index overtakes 
    *     UART_1_rxBufferRead index.
    *  UART_1_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when UART_1_rxBufferWrite is equal to 
    *    UART_1_rxBufferRead
    *
    *******************************************************************************/
    CY_ISR(UART_1_RXISR)
    {
        uint8 readData;

        /* User code required at start of ISR */
        /* `#START UART_1_RXISR_START` */

        /* `#END` */

        readData = UART_1_RXSTATUS_REG;

        if(readData & (UART_1_RX_STS_BREAK | UART_1_RX_STS_PAR_ERROR | \
                       UART_1_RX_STS_STOP_ERROR | UART_1_RX_STS_OVERRUN))
        {
            /* ERROR handling. */
            /* `#START UART_1_RXISR_ERROR` */

            /* `#END` */
        }

        while(readData & UART_1_RX_STS_FIFO_NOTEMPTY)
        {
            /* Check pointer. */
            if(UART_1_rxBufferWrite >= UART_1_RXBUFFERSIZE)
            {
                UART_1_rxBufferWrite = 0u;
                if(UART_1_rxBufferWrite == UART_1_rxBufferRead)
                {   /* Set Software Buffer status Overflow */
                    UART_1_rxBufferOverflow = 1u;
                }
            }

            UART_1_rxBuffer[UART_1_rxBufferWrite] = UART_1_RXDATA_REG;

            if(UART_1_rxBufferLoopDetect)
            {   /* Set Software Buffer status Overflow */
                UART_1_rxBufferOverflow = 1u;
            }
            /* Set next pointer. */
            UART_1_rxBufferWrite++;

            if(UART_1_rxBufferWrite == UART_1_rxBufferRead)
            {
                UART_1_rxBufferLoopDetect = 1u;
            }

            /* Check again if there is data. */
            readData = UART_1_RXSTATUS_REG;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_1_RXISR_END` */

        /* `#END` */

        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(UART_1_PSOC3_ES2 && (UART_1_RXInternalInterrupt__ES2_PATCH))
            UART_1_ISR_PATCH();
        #endif /* End UART_1_PSOC3_ES2*/
    }

#endif /* End UART_1_RX_ENABLED && (UART_1_RXBUFFERSIZE > UART_1_FIFO_LENGTH) */


#if(UART_1_TX_ENABLED && (UART_1_TXBUFFERSIZE > UART_1_FIFO_LENGTH))

    extern volatile uint8 UART_1_txBuffer[];
    extern volatile uint8 UART_1_txBufferRead;
    extern uint8 UART_1_txBufferWrite;


    /*******************************************************************************
    * Function Name: UART_1_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_1_txBuffer - RAM buffer pointer for transmit data from.
    *  UART_1_txBufferRead - cyclic index for read and transmit data 
    *     from txBuffer, increments after each transmited byte.
    *  UART_1_rxBufferWrite - cyclic index for write to txBuffer, 
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_1_TXISR)
    {

        /* User code required at start of ISR */
        /* `#START UART_1_TXISR_START` */

        /* `#END` */

        while((UART_1_txBufferRead != UART_1_txBufferWrite) && \
             !(UART_1_TXSTATUS_REG & UART_1_TX_STS_FIFO_FULL))
        {
            /* Check pointer. */
            if(UART_1_txBufferRead >= UART_1_TXBUFFERSIZE)
            {
                UART_1_txBufferRead = 0u;
            }

            UART_1_TXDATA_REG = UART_1_txBuffer[UART_1_txBufferRead];

            /* Set next pointer. */
            UART_1_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_1_TXISR_END` */

        /* `#END` */
        
        /* PSoC3 ES1, ES2 RTC ISR PATCH  */
        #if(UART_1_PSOC3_ES2 && (UART_1_TXInternalInterrupt__ES2_PATCH))
            UART_1_ISR_PATCH();
        #endif /* End UART_1_PSOC3_ES2*/
    }

#endif /* End UART_1_TX_ENABLED && (UART_1_TXBUFFERSIZE > UART_1_FIFO_LENGTH) */


/* [] END OF FILE */
