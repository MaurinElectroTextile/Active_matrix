
#include <device.h>
#include "debug.h"
#include "signal.h"

#if ENABLE_DEBUG_OUTPUT

CY_ISR_PROTO(Debug_IRQ_Handler);
uint32 nDebugBuffer;

#endif  // ENABLE_DEBUG_OUTPUT

void DebugInit(void)
{
#if ENABLE_DEBUG_OUTPUT

    VDAC8_Debug_Start();
    Opamp_Debug_Start();
    
#if TRACE_ADC_OUTPUT_SIGNAL

    ISR_Debug_Fast_StartEx(Debug_IRQ_Handler);
    ISR_Debug_Fast_Enable();
    
#elif  TRACE_FILTER_A_OUTPUT
    
    ISR_Debug_Fast_2_StartEx(Debug_IRQ_Handler);
    ISR_Debug_Fast_2_Enable();

#elif  TRACE_RECTIFIER_OUTPUT
    
    {
        uint8 DMA_Debug_Chan;
        uint8 DMA_Debug_TD[2];
        
        #define DMA_Debug_BYTES_PER_BURST 1
        #define DMA_Debug_REQUEST_PER_BURST 0
        #define DMA_Debug_SRC_BASE (&Rectifier_OUTL_Status)
        #define DMA_Debug_DST_BASE (&nDebugBuffer)
        
        DMA_Debug_Chan = DMA_Debug_DmaInitialize(DMA_Debug_BYTES_PER_BURST, 
                                                 DMA_Debug_REQUEST_PER_BURST, 
                                                 HI16(DMA_Debug_SRC_BASE), 
                                                 HI16(DMA_Debug_DST_BASE));
        DMA_Debug_TD[0] = CyDmaTdAllocate();
        DMA_Debug_TD[1] = CyDmaTdAllocate();

        CyDmaTdSetConfiguration(DMA_Debug_TD[0], 
                                1, 
                                DMA_Debug_TD[1], 
                                TD_AUTO_EXEC_NEXT);
        CyDmaTdSetConfiguration(DMA_Debug_TD[1], 
                                1, 
                                DMA_Debug_TD[0], 
                                DMA_Debug__TD_TERMOUT_EN);
        CyDmaTdSetAddress  (DMA_Debug_TD[0], 
                            LO16((uint32)&Rectifier_OUTL_Status), 
                            LO16((uint32)&nDebugBuffer));
        CyDmaTdSetAddress  (DMA_Debug_TD[1], 
                            LO16((uint32)&Rectifier_OUTH_Status), 
                            LO16(((uint32)&nDebugBuffer) + 1));

        CyDmaChSetInitialTd(DMA_Debug_Chan, DMA_Debug_TD[0]);
        CyDmaChEnable(DMA_Debug_Chan, 1);
    }
        
//    ISR_Debug_Fast_2_StartEx(Debug_IRQ_Handler);
//    ISR_Debug_Fast_2_Enable();

    ISR_Debug_Fast_3_StartEx(Debug_IRQ_Handler);
    ISR_Debug_Fast_3_Enable();

#else

    ISR_Debug_Slow_StartEx(Debug_IRQ_Handler);
    ISR_Debug_Slow_Enable();
#endif    
#endif  // ENABLE_DEBUG_OUTPUT
}

#if ENABLE_DEBUG_OUTPUT

CY_ISR(Debug_IRQ_Handler)
{
#if ENABLE_DEBUG_OUTPUT

    static uint8 x, y;

#if TRACE_ADC_OUTPUT_SIGNAL

    x = (uint8)(nDebugBuffer >> 4);

#elif TRACE_FILTER_A_OUTPUT

    x = (uint8)((sig_data.nFilterBuffer >> 2) + 128);

#elif TRACE_RECTIFIER_OUTPUT
    
    x = (uint8)(nDebugBuffer >> 2);

#elif TRACE_ACCU_OUTPUT

    x = (uint8)(sig_data.nRawBuffer >> 8);

#elif TRACE_FILTERED_OUTPUT
    
    x = (uint8)(sig_data.anFilteredData[0][y] >> 8);
    if (++y >= NUM_ROWS * NUM_COLS)
    {
        y = 0;
    }

#elif TRACE_VALUE
    
    x = (uint8)(sig_data.anValue[0][y] >> 4) + 128;
    if (++y >= NUM_ROWS * NUM_COLS)
    {
        y = 0;
    }
    
#elif TRACE_VALUE_ABS

    int16 xx;
    xx = sig_data.anValue[0][y];
    if (xx < 0)
    {
        xx = -xx;
    }
    x = (uint8)(xx >> 4);
    
    if (++y >= NUM_ROWS * NUM_COLS)
    {
        y = 0;
    }

#elif TRACE_BASELINE

    x = sig_data.anBaseline[0][y] >> 7;
    if (++y >= NUM_ROWS * NUM_COLS)
    {
        y = 0;
    }

#endif

    VDAC8_Debug_SetValue(x);

#endif
}

#endif // ENABLE_DEBUG_OUTPUT
