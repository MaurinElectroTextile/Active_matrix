/*******************************************************************************
* File Name: Counter_Accu.h  
* Version 1.50
*
*  Description:
*   Contains the function prototypes and constants available to the counter
*   user module.
*
*   Note:
*    None
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
    
#if !defined(CY_COUNTER_Counter_Accu_H)
#define CY_COUNTER_Counter_Accu_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* PSoC3 ES2 or early */
#define Counter_Accu_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES1 or early */
#define Counter_Accu_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define Counter_Accu_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))
/* PSoC5 ES2 or later */
#define Counter_Accu_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A)    && \
                                    (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))


/**************************************
*           Parameter Defaults        
**************************************/

#define Counter_Accu_Resolution            16u
#define Counter_Accu_UsingFixedFunction    0u
#define Counter_Accu_ControlRegRemoved     0u
#define Counter_Accu_COMPARE_MODE_SOFTWARE 0u
#define Counter_Accu_CAPTURE_MODE_SOFTWARE 0u
#define Counter_Accu_RunModeUsed           0u


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct Counter_Accu_backupStruct
{
    #if (!Counter_Accu_ControlRegRemoved)
        #if (!Counter_Accu_UsingFixedFunction)
            uint8 control;
        #endif
        
        uint8 enableState;
    #endif   
    
    #if (!Counter_Accu_UsingFixedFunction)
        uint16 counterUdb;
        uint16 captureValue;
    #endif
}Counter_Accu_backupStruct;


/**************************************
 *  Function Prototypes
 *************************************/
void    Counter_Accu_Start(void);
void    Counter_Accu_Stop(void) ;
void    Counter_Accu_SetInterruptMode(uint8 interruptsMask) ;
uint8   Counter_Accu_GetInterruptSource(void) ;
uint8   Counter_Accu_ReadStatusRegister(void) ;
#if(!Counter_Accu_ControlRegRemoved)
    uint8   Counter_Accu_ReadControlRegister(void) ;
    void    Counter_Accu_WriteControlRegister(uint8 control) \
        ;
#endif
void    Counter_Accu_WriteCounter(uint16 counter) \
    ; 
uint16  Counter_Accu_ReadCounter(void) ;
uint16  Counter_Accu_ReadCapture(void) ;
void    Counter_Accu_WritePeriod(uint16 period) \
    ;
uint16  Counter_Accu_ReadPeriod( void ) ;
#if (!Counter_Accu_UsingFixedFunction)
    void    Counter_Accu_WriteCompare(uint16 compare) \
        ;
    uint16  Counter_Accu_ReadCompare( void ) \
        ;
#endif

#if (Counter_Accu_COMPARE_MODE_SOFTWARE)
    void    Counter_Accu_SetCompareMode(uint8 comparemode) ;
#endif
#if (Counter_Accu_CAPTURE_MODE_SOFTWARE)
    void    Counter_Accu_SetCaptureMode(uint8 capturemode) ;
#endif
void Counter_Accu_ClearFIFO(void)     ;
void Counter_Accu_Init(void)          ;
void Counter_Accu_Enable(void)        ;
void Counter_Accu_SaveConfig(void)    ;
void Counter_Accu_RestoreConfig(void) ;
void Counter_Accu_Sleep(void)         ;
void Counter_Accu_Wakeup(void)        ;

/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Counter__CompareModes, Used in Compare Mode */
#define Counter_Accu__B_COUNTER__LESS_THAN 1
#define Counter_Accu__B_COUNTER__LESS_THAN_OR_EQUAL 2
#define Counter_Accu__B_COUNTER__EQUAL 0
#define Counter_Accu__B_COUNTER__GREATER_THAN 3
#define Counter_Accu__B_COUNTER__GREATER_THAN_OR_EQUAL 4
#define Counter_Accu__B_COUNTER__SOFTWARE 5


/* Enumerated Type B_Counter__CaptureModes, Used in Capture Mode */
#define Counter_Accu__B_COUNTER__NONE 0
#define Counter_Accu__B_COUNTER__RISING_EDGE 1
#define Counter_Accu__B_COUNTER__FALLING_EDGE 2
#define Counter_Accu__B_COUNTER__EITHER_EDGE 3
#define Counter_Accu__B_COUNTER__SOFTWARE_CONTROL 4


/***************************************
 *  Initialization Values
 **************************************/
#define Counter_Accu_INIT_PERIOD_VALUE       255u
#define Counter_Accu_INIT_COUNTER_VALUE      0u
#if (Counter_Accu_UsingFixedFunction)
#define Counter_Accu_INIT_INTERRUPTS_MASK    ((0u << Counter_Accu_STATUS_ZERO_INT_EN_MASK_SHIFT))
#else
#define Counter_Accu_INIT_COMPARE_VALUE      128u
#define Counter_Accu_INIT_INTERRUPTS_MASK ((0u << Counter_Accu_STATUS_ZERO_INT_EN_MASK_SHIFT) | \
        (0u << Counter_Accu_STATUS_CAPTURE_INT_EN_MASK_SHIFT) | \
        (0u << Counter_Accu_STATUS_CMP_INT_EN_MASK_SHIFT) | \
        (0u << Counter_Accu_STATUS_OVERFLOW_INT_EN_MASK_SHIFT) | \
        (0u << Counter_Accu_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT))
#define Counter_Accu_DEFAULT_COMPARE_MODE    (3u << Counter_Accu_CTRL_CMPMODE0_SHIFT)
#define Counter_Accu_DEFAULT_CAPTURE_MODE    (0u << Counter_Accu_CTRL_CAPMODE0_SHIFT)
#endif /* (Counter_Accu_UsingFixedFunction) */

/**************************************
 *  Registers
 *************************************/
#if (Counter_Accu_UsingFixedFunction)
    #define Counter_Accu_STATICCOUNT_LSB     (*(reg16 *) Counter_Accu_CounterHW__CAP0 )
    #define Counter_Accu_STATICCOUNT_LSB_PTR ( (reg16 *) Counter_Accu_CounterHW__CAP0 )
    #define Counter_Accu_PERIOD_LSB          (*(reg16 *) Counter_Accu_CounterHW__PER0 )
    #define Counter_Accu_PERIOD_LSB_PTR      ( (reg16 *) Counter_Accu_CounterHW__PER0 )
    /* MODE must be set to 1 to set the compare value */
    #define Counter_Accu_COMPARE_LSB         (*(reg16 *) Counter_Accu_CounterHW__CNT_CMP0 )
    #define Counter_Accu_COMPARE_LSB_PTR     ( (reg16 *) Counter_Accu_CounterHW__CNT_CMP0 )
    /* MODE must be set to 0 to get the count */
    #define Counter_Accu_COUNTER_LSB         (*(reg16 *) Counter_Accu_CounterHW__CNT_CMP0 )
    #define Counter_Accu_COUNTER_LSB_PTR     ( (reg16 *) Counter_Accu_CounterHW__CNT_CMP0 )
#else
    #define Counter_Accu_STATICCOUNT_LSB     (*(reg16 *) \
        Counter_Accu_CounterUDB_sC16_counterdp_u0__F0_REG )
    #define Counter_Accu_STATICCOUNT_LSB_PTR ( (reg16 *) \
        Counter_Accu_CounterUDB_sC16_counterdp_u0__F0_REG )
    #define Counter_Accu_PERIOD_LSB          (*(reg16 *) \
        Counter_Accu_CounterUDB_sC16_counterdp_u0__D0_REG )
    #define Counter_Accu_PERIOD_LSB_PTR      ( (reg16 *) \
        Counter_Accu_CounterUDB_sC16_counterdp_u0__D0_REG )
    #define Counter_Accu_COMPARE_LSB         (*(reg16 *) \
        Counter_Accu_CounterUDB_sC16_counterdp_u0__D1_REG )
    #define Counter_Accu_COMPARE_LSB_PTR     ( (reg16 *) \
        Counter_Accu_CounterUDB_sC16_counterdp_u0__D1_REG )
    #define Counter_Accu_COUNTER_LSB         (*(reg16 *) \
        Counter_Accu_CounterUDB_sC16_counterdp_u0__A0_REG )
    #define Counter_Accu_COUNTER_LSB_PTR     ( (reg16 *)\
        Counter_Accu_CounterUDB_sC16_counterdp_u0__A0_REG )

	#define Counter_Accu_AUX_CONTROLDP0 \
        (*(reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u0__DP_AUX_CTL_REG)
    #define Counter_Accu_AUX_CONTROLDP0_PTR \
        ( (reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u0__DP_AUX_CTL_REG)
    #if (Counter_Accu_Resolution == 16 || Counter_Accu_Resolution == 24 || Counter_Accu_Resolution == 32)
       #define Counter_Accu_AUX_CONTROLDP1 \
           (*(reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u1__DP_AUX_CTL_REG)
       #define Counter_Accu_AUX_CONTROLDP1_PTR \
           ( (reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u1__DP_AUX_CTL_REG)
    #endif
    #if (Counter_Accu_Resolution == 24 || Counter_Accu_Resolution == 32)
       #define Counter_Accu_AUX_CONTROLDP2 \
           (*(reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u2__DP_AUX_CTL_REG)
       #define Counter_Accu_AUX_CONTROLDP2_PTR \
           ( (reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u2__DP_AUX_CTL_REG)
    #endif
    #if (Counter_Accu_Resolution == 32)
       #define Counter_Accu_AUX_CONTROLDP3 \
           (*(reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u3__DP_AUX_CTL_REG)
       #define Counter_Accu_AUX_CONTROLDP3_PTR \
           ( (reg8 *) Counter_Accu_CounterUDB_sC16_counterdp_u3__DP_AUX_CTL_REG)
    #endif
#endif  /* (Counter_Accu_UsingFixedFunction) */

#if (Counter_Accu_UsingFixedFunction)
    #define Counter_Accu_STATUS         (*(reg8 *) Counter_Accu_CounterHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Counter_Accu_STATUS_MASK             (*(reg8 *) Counter_Accu_CounterHW__SR0 )
    #define Counter_Accu_STATUS_MASK_PTR         ( (reg8 *) Counter_Accu_CounterHW__SR0 )
    #define Counter_Accu_CONTROL                 (*(reg8 *) Counter_Accu_CounterHW__CFG0)
    #define Counter_Accu_CONTROL_PTR             ( (reg8 *) Counter_Accu_CounterHW__CFG0)
    #define Counter_Accu_CONTROL2                (*(reg8 *) Counter_Accu_CounterHW__CFG1)
    #define Counter_Accu_CONTROL2_PTR            ( (reg8 *) Counter_Accu_CounterHW__CFG1)
    #if (Counter_Accu_PSOC3_ES3 || Counter_Accu_PSOC5_ES2)
        #define Counter_Accu_CONTROL3       (*(reg8 *) Counter_Accu_CounterHW__CFG2)
        #define Counter_Accu_CONTROL3_PTR   ( (reg8 *) Counter_Accu_CounterHW__CFG2)
    #endif
    #define Counter_Accu_GLOBAL_ENABLE           (*(reg8 *) Counter_Accu_CounterHW__PM_ACT_CFG)
    #define Counter_Accu_GLOBAL_ENABLE_PTR       ( (reg8 *) Counter_Accu_CounterHW__PM_ACT_CFG)
    #define Counter_Accu_GLOBAL_STBY_ENABLE      (*(reg8 *) Counter_Accu_CounterHW__PM_STBY_CFG)
    #define Counter_Accu_GLOBAL_STBY_ENABLE_PTR  ( (reg8 *) Counter_Accu_CounterHW__PM_STBY_CFG)
    
    /********************************
    *    Constants
    ********************************/
    /* Fixed Function Block Chosen */
    #define Counter_Accu_BLOCK_EN_MASK          Counter_Accu_CounterHW__PM_ACT_MSK
    #define Counter_Accu_BLOCK_STBY_EN_MASK     Counter_Accu_CounterHW__PM_STBY_MSK 
    
    /* Control Register Bit Locations */    
    /* As defined in Register Map, part of TMRX_CFG0 register */
    #define Counter_Accu_CTRL_ENABLE_SHIFT      0x00u
    /* Control Register Bit Masks */
    #define Counter_Accu_CTRL_ENABLE            (0x01u << Counter_Accu_CTRL_ENABLE_SHIFT)         

    /* Control2 Register Bit Masks */
    /* Set the mask for run mode */
    #if (Counter_Accu_PSOC3_ES2 || Counter_Accu_PSOC5_ES1)
        /* Use CFG1 Mode bits to set run mode */
        #define Counter_Accu_CTRL_MODE_SHIFT        0x01u    
        #define Counter_Accu_CTRL_MODE_MASK         (0x07u << Counter_Accu_CTRL_MODE_SHIFT)
    #endif
    #if (Counter_Accu_PSOC3_ES3 || Counter_Accu_PSOC5_ES2)
        /* Use CFG2 Mode bits to set run mode */
        #define Counter_Accu_CTRL_MODE_SHIFT        0x00u    
        #define Counter_Accu_CTRL_MODE_MASK         (0x03u << Counter_Accu_CTRL_MODE_SHIFT)
    #endif
    /* Set the mask for interrupt (raw/status register) */
    #define Counter_Accu_CTRL2_IRQ_SEL_SHIFT     0x00u
    #define Counter_Accu_CTRL2_IRQ_SEL          (0x01u << Counter_Accu_CTRL2_IRQ_SEL_SHIFT)     
    
    /* Status Register Bit Locations */
    #define Counter_Accu_STATUS_ZERO_SHIFT      0x07u  /* As defined in Register Map, part of TMRX_SR0 register */ 

    /* Status Register Interrupt Enable Bit Locations */
    #define Counter_Accu_STATUS_ZERO_INT_EN_MASK_SHIFT      (Counter_Accu_STATUS_ZERO_SHIFT - 0x04u)

    /* Status Register Bit Masks */                           
    #define Counter_Accu_STATUS_ZERO            (0x01u << Counter_Accu_STATUS_ZERO_SHIFT)

    /* Status Register Interrupt Bit Masks*/
    #define Counter_Accu_STATUS_ZERO_INT_EN_MASK       (Counter_Accu_STATUS_ZERO >> 0x04u)

#else /* !Counter_Accu_UsingFixedFunction */
    #define Counter_Accu_STATUS               (* (reg8 *) Counter_Accu_CounterUDB_sSTSReg_nrstSts_stsreg__STATUS_REG )
    #define Counter_Accu_STATUS_PTR           (  (reg8 *) Counter_Accu_CounterUDB_sSTSReg_nrstSts_stsreg__STATUS_REG )
    #define Counter_Accu_STATUS_MASK          (* (reg8 *) Counter_Accu_CounterUDB_sSTSReg_nrstSts_stsreg__MASK_REG )
    #define Counter_Accu_STATUS_MASK_PTR      (  (reg8 *) Counter_Accu_CounterUDB_sSTSReg_nrstSts_stsreg__MASK_REG )
    #define Counter_Accu_STATUS_AUX_CTRL      (*(reg8 *) Counter_Accu_CounterUDB_sSTSReg_nrstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Counter_Accu_STATUS_AUX_CTRL_PTR  ( (reg8 *) Counter_Accu_CounterUDB_sSTSReg_nrstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Counter_Accu_CONTROL              (* (reg8 *) Counter_Accu_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )
    #define Counter_Accu_CONTROL_PTR          (  (reg8 *) Counter_Accu_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )

    /********************************
    *    Constants
    ********************************/
    /* Control Register Bit Locations */
    #define Counter_Accu_CTRL_CMPMODE0_SHIFT    0x00u       /* As defined by Verilog Implementation */
    #define Counter_Accu_CTRL_CAPMODE0_SHIFT    0x03u       /* As defined by Verilog Implementation */
    #define Counter_Accu_CTRL_RESET_SHIFT       0x06u       /* As defined by Verilog Implementation */
    #define Counter_Accu_CTRL_ENABLE_SHIFT      0x07u       /* As defined by Verilog Implementation */
    /* Control Register Bit Masks */
    #define Counter_Accu_CTRL_CMPMODE_MASK      (0x07u << Counter_Accu_CTRL_CMPMODE0_SHIFT)  
    #define Counter_Accu_CTRL_CAPMODE_MASK      (0x03u << Counter_Accu_CTRL_CAPMODE0_SHIFT)  
    #define Counter_Accu_CTRL_RESET             (0x01u << Counter_Accu_CTRL_RESET_SHIFT)  
    #define Counter_Accu_CTRL_ENABLE            (0x01u << Counter_Accu_CTRL_ENABLE_SHIFT) 

    /* Status Register Bit Locations */
    #define Counter_Accu_STATUS_CMP_SHIFT       0x00u       /* As defined by Verilog Implementation */
    #define Counter_Accu_STATUS_ZERO_SHIFT      0x01u       /* As defined by Verilog Implementation */
    #define Counter_Accu_STATUS_OVERFLOW_SHIFT  0x02u       /* As defined by Verilog Implementation */
    #define Counter_Accu_STATUS_UNDERFLOW_SHIFT 0x03u       /* As defined by Verilog Implementation */
    #define Counter_Accu_STATUS_CAPTURE_SHIFT   0x04u       /* As defined by Verilog Implementation */
    #define Counter_Accu_STATUS_FIFOFULL_SHIFT  0x05u       /* As defined by Verilog Implementation */
    #define Counter_Accu_STATUS_FIFONEMP_SHIFT  0x06u       /* As defined by Verilog Implementation */
    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define Counter_Accu_STATUS_CMP_INT_EN_MASK_SHIFT       Counter_Accu_STATUS_CMP_SHIFT       
    #define Counter_Accu_STATUS_ZERO_INT_EN_MASK_SHIFT      Counter_Accu_STATUS_ZERO_SHIFT      
    #define Counter_Accu_STATUS_OVERFLOW_INT_EN_MASK_SHIFT  Counter_Accu_STATUS_OVERFLOW_SHIFT  
    #define Counter_Accu_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT Counter_Accu_STATUS_UNDERFLOW_SHIFT 
    #define Counter_Accu_STATUS_CAPTURE_INT_EN_MASK_SHIFT   Counter_Accu_STATUS_CAPTURE_SHIFT   
    #define Counter_Accu_STATUS_FIFOFULL_INT_EN_MASK_SHIFT  Counter_Accu_STATUS_FIFOFULL_SHIFT  
    #define Counter_Accu_STATUS_FIFONEMP_INT_EN_MASK_SHIFT  Counter_Accu_STATUS_FIFONEMP_SHIFT  
    /* Status Register Bit Masks */                
    #define Counter_Accu_STATUS_CMP             (0x01u << Counter_Accu_STATUS_CMP_SHIFT)  
    #define Counter_Accu_STATUS_ZERO            (0x01u << Counter_Accu_STATUS_ZERO_SHIFT) 
    #define Counter_Accu_STATUS_OVERFLOW        (0x01u << Counter_Accu_STATUS_OVERFLOW_SHIFT) 
    #define Counter_Accu_STATUS_UNDERFLOW       (0x01u << Counter_Accu_STATUS_UNDERFLOW_SHIFT) 
    #define Counter_Accu_STATUS_CAPTURE         (0x01u << Counter_Accu_STATUS_CAPTURE_SHIFT) 
    #define Counter_Accu_STATUS_FIFOFULL        (0x01u << Counter_Accu_STATUS_FIFOFULL_SHIFT)
    #define Counter_Accu_STATUS_FIFONEMP        (0x01u << Counter_Accu_STATUS_FIFONEMP_SHIFT)
    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define Counter_Accu_STATUS_CMP_INT_EN_MASK            Counter_Accu_STATUS_CMP                    
    #define Counter_Accu_STATUS_ZERO_INT_EN_MASK           Counter_Accu_STATUS_ZERO            
    #define Counter_Accu_STATUS_OVERFLOW_INT_EN_MASK       Counter_Accu_STATUS_OVERFLOW        
    #define Counter_Accu_STATUS_UNDERFLOW_INT_EN_MASK      Counter_Accu_STATUS_UNDERFLOW       
    #define Counter_Accu_STATUS_CAPTURE_INT_EN_MASK        Counter_Accu_STATUS_CAPTURE         
    #define Counter_Accu_STATUS_FIFOFULL_INT_EN_MASK       Counter_Accu_STATUS_FIFOFULL        
    #define Counter_Accu_STATUS_FIFONEMP_INT_EN_MASK       Counter_Accu_STATUS_FIFONEMP         
    

    /* StatusI Interrupt Enable bit Location in the Auxilliary Control Register */
    #define Counter_Accu_STATUS_ACTL_INT_EN     0x10u /* As defined for the ACTL Register */
    
    /* Datapath Auxillary Control Register definitions */
    #define Counter_Accu_AUX_CTRL_FIFO0_CLR         0x01u   /* As defined by Register map */
    #define Counter_Accu_AUX_CTRL_FIFO1_CLR         0x02u   /* As defined by Register map */
    #define Counter_Accu_AUX_CTRL_FIFO0_LVL         0x04u   /* As defined by Register map */
    #define Counter_Accu_AUX_CTRL_FIFO1_LVL         0x08u   /* As defined by Register map */
    #define Counter_Accu_STATUS_ACTL_INT_EN_MASK    0x10u   /* As defined for the ACTL Register */
    
#endif /* Counter_Accu_UsingFixedFunction */

#endif  /* CY_COUNTER_Counter_Accu_H */


/* [] END OF FILE */

