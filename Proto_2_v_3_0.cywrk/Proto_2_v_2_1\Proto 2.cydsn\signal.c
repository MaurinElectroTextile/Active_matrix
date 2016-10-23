/* ************************************************************************** */

#include <device.h>
#include "signal.h"
#include "dfb_extra.h"

/* ************************************************************************** */

SIGNAL_DATA_T       sig_data;
SIGNAL_CONFIG_T     sig_config = { 125, 100 };

/* ************************************************************************** */

const uint32 USB_SOF = 0xEEAA1010;

/* ************************************************************************** */

CY_ISR_PROTO(Matrix_Switch_Handler);

/* ************************************************************************** */

static inline uint16 abs16(int16 x)
{
    return (x >= 0) ? x : -x;
}

/* ************************************************************************** */

static void Setup_DMA_ADC_A (void)
{
    #define DMA_ADC_A_BYTES_PER_BURST 2
    #define DMA_ADC_A_REQUEST_PER_BURST  1
    #define DMA_ADC_A_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_ADC_A_SRC_ADDR (ADC_SAR_A_SAR_WRK0_PTR)

#if !TRACE_ADC_A_OUTPUT_SIGNAL
    #define DMA_ADC_A_DST_BASE (CYDEV_PERIPH_BASE)
    #define DMA_ADC_A_DST_ADDR (Filter_STAGEA_PTR)
#else
    #define DMA_ADC_A_DST_BASE (&nDebugBuffer)
    #define DMA_ADC_A_DST_ADDR (&nDebugBuffer)
#endif

    uint8 nChannel;
    uint8 aTD[1];
    
    nChannel = DMA_ADC_A_DmaInitialize(DMA_ADC_A_BYTES_PER_BURST, 
                                       DMA_ADC_A_REQUEST_PER_BURST, 
                                       HI16(DMA_ADC_A_SRC_BASE), 
                                       HI16(DMA_ADC_A_DST_BASE));
    aTD[0] = CyDmaTdAllocate();

    CyDmaTdSetConfiguration(aTD[0], 
                            sizeof(uint16), 
                            aTD[0], 
                            DMA_ADC_A__TD_TERMOUT_EN);

    CyDmaTdSetAddress  (aTD[0], 
                        LO16((uint32)DMA_ADC_A_SRC_ADDR), 
                        LO16((uint32)DMA_ADC_A_DST_ADDR));

    CyDmaChSetInitialTd(nChannel, 
                        aTD[0]);
    CyDmaChEnable(nChannel, 1);
}

/* ************************************************************************** */

static void Setup_DMA_ADC_B (void)
{
    #define DMA_ADC_B_BYTES_PER_BURST 2
    #define DMA_ADC_B_REQUEST_PER_BURST  1
    #define DMA_ADC_B_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_ADC_B_SRC_ADDR (ADC_SAR_B_SAR_WRK0_PTR)

#if !TRACE_ADC_B_OUTPUT_SIGNAL
    #define DMA_ADC_B_DST_BASE (CYDEV_PERIPH_BASE)
    #define DMA_ADC_B_DST_ADDR (Filter_STAGEB_PTR)
#else
    #define DMA_ADC_B_DST_BASE (&nDebugBuffer)
    #define DMA_ADC_B_DST_ADDR (&nDebugBuffer)
#endif

    uint8 nChannel;
    uint8 aTD[1];
    
    nChannel = DMA_ADC_B_DmaInitialize(DMA_ADC_B_BYTES_PER_BURST, 
                                       DMA_ADC_B_REQUEST_PER_BURST, 
                                       HI16(DMA_ADC_B_SRC_BASE), 
                                       HI16(DMA_ADC_B_DST_BASE));
    aTD[0] = CyDmaTdAllocate();

    CyDmaTdSetConfiguration(aTD[0], 
                            sizeof(uint16), 
                            aTD[0], 
                            DMA_ADC_B__TD_TERMOUT_EN);

    CyDmaTdSetAddress  (aTD[0], 
                        LO16((uint32)DMA_ADC_B_SRC_ADDR), 
                        LO16((uint32)DMA_ADC_B_DST_ADDR));

    CyDmaChSetInitialTd(nChannel, 
                        aTD[0]);
    CyDmaChEnable(nChannel, 1);
}

/* ************************************************************************** */

static void Setup_DMA_Filter_A (void)
{
    #define DMA_Filter_A_BYTES_PER_BURST 2
    #define DMA_Filter_A_REQUEST_PER_BURST 1
    #define DMA_Filter_A_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_Filter_A_SRC_ADDR (Filter_HOLDA_PTR)
    #define DMA_Filter_A_DUMP_BASE (&(sig_data.nDumpBuffer))
    #define DMA_Filter_A_DUMP_ADDR (&(sig_data.nDumpBuffer))
    #define DMA_Filter_A_DST_BASE (&(sig_data.anDMABuffer[0][0]))
    #define DMA_Filter_A_DST_ADDR (&(sig_data.anDMABuffer[0][0]))

    sig_data.hDmaFilterA = DMA_Filter_A_DmaInitialize(DMA_Filter_A_BYTES_PER_BURST, 
                                                      DMA_Filter_A_REQUEST_PER_BURST, 
                                                      HI16(DMA_Filter_A_SRC_BASE), 
                                                      HI16(DMA_Filter_A_DST_BASE));
    sig_data.ahTdFilterA[0] = CyDmaTdAllocate();
    sig_data.ahTdFilterA[1] = CyDmaTdAllocate();

    CyDmaTdSetConfiguration(sig_data.ahTdFilterA[0], 
                            sig_config.nSampleSkipCount * sizeof(uint16), 
                            sig_data.ahTdFilterA[1], 
                            0);

    CyDmaTdSetAddress  (sig_data.ahTdFilterA[0], 
                        LO16((uint32)DMA_Filter_A_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_A_DUMP_ADDR));

    CyDmaTdSetConfiguration(sig_data.ahTdFilterA[1], 
                            sig_config.nCaptureCount * sizeof(uint16), 
                            sig_data.ahTdFilterA[0], 
                            TD_INC_DST_ADR | DMA_Filter_A__TD_TERMOUT_EN);

    CyDmaTdSetAddress  (sig_data.ahTdFilterA[1], 
                        LO16((uint32)DMA_Filter_A_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_A_DST_ADDR));

    CyDmaChSetInitialTd(sig_data.hDmaFilterA, sig_data.ahTdFilterA[0]);
    CyDmaChEnable(sig_data.hDmaFilterA, 1);
}

/* ************************************************************************** */

static void Setup_DMA_Filter_B (void)
{
    #define DMA_Filter_B_BYTES_PER_BURST 2
    #define DMA_Filter_B_REQUEST_PER_BURST 1
    #define DMA_Filter_B_SRC_BASE (CYDEV_PERIPH_BASE)
    #define DMA_Filter_B_SRC_ADDR (Filter_HOLDB_PTR)
    #define DMA_Filter_B_DUMP_BASE (&(sig_data.nDumpBuffer))
    #define DMA_Filter_B_DUMP_ADDR (&(sig_data.nDumpBuffer))
    #define DMA_Filter_B_DST_BASE (&(sig_data.anDMABuffer[1][0]))
    #define DMA_Filter_B_DST_ADDR (&(sig_data.anDMABuffer[1][0]))

    sig_data.hDmaFilterB = DMA_Filter_B_DmaInitialize(DMA_Filter_B_BYTES_PER_BURST, 
                                                      DMA_Filter_B_REQUEST_PER_BURST, 
                                                      HI16(DMA_Filter_B_SRC_BASE), 
                                                      HI16(DMA_Filter_B_DST_BASE));
    sig_data.ahTdFilterB[0] = CyDmaTdAllocate();
    sig_data.ahTdFilterB[1] = CyDmaTdAllocate();

    CyDmaTdSetConfiguration(sig_data.ahTdFilterB[0], 
                            sig_config.nSampleSkipCount * sizeof(uint16), 
                            sig_data.ahTdFilterB[1], 
                            0);

    CyDmaTdSetAddress  (sig_data.ahTdFilterB[0], 
                        LO16((uint32)DMA_Filter_B_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_B_DUMP_ADDR));

    CyDmaTdSetConfiguration(sig_data.ahTdFilterB[1], 
                            sig_config.nCaptureCount * sizeof(uint16), 
                            sig_data.ahTdFilterB[0], 
                            TD_INC_DST_ADR | DMA_Filter_B__TD_TERMOUT_EN);

    CyDmaTdSetAddress  (sig_data.ahTdFilterB[1], 
                        LO16((uint32)DMA_Filter_B_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_B_DST_ADDR));

    CyDmaChSetInitialTd(sig_data.hDmaFilterB, sig_data.ahTdFilterB[0]);
    CyDmaChEnable(sig_data.hDmaFilterB, 1);
}

/* ************************************************************************** */

static void Reset_DMA_Filter_A(void)
{
    CyDmaChDisable(sig_data.hDmaFilterA);

    CyDmaChSetRequest(sig_data.hDmaFilterA, CPU_TERM_TD);
    
    while (CPU_TERM_CHAIN & CyDmaChGetRequest(sig_data.hDmaFilterA))
    {
        continue;
    }
    
    CyDmaTdSetConfiguration(sig_data.ahTdFilterA[0], 
                            sig_config.nSampleSkipCount * sizeof(uint16), 
                            sig_data.ahTdFilterA[1], 
                            0);

    CyDmaTdSetAddress  (sig_data.ahTdFilterA[0], 
                        LO16((uint32)DMA_Filter_A_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_A_DUMP_ADDR));

    CyDmaTdSetConfiguration(sig_data.ahTdFilterA[1], 
                            sig_config.nCaptureCount * sizeof(uint16), 
                            sig_data.ahTdFilterA[0], 
                            TD_INC_DST_ADR | DMA_Filter_A__TD_TERMOUT_EN);

    CyDmaTdSetAddress  (sig_data.ahTdFilterA[1], 
                        LO16((uint32)DMA_Filter_A_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_A_DST_ADDR));

    CyDmaChSetInitialTd(sig_data.hDmaFilterA, sig_data.ahTdFilterA[0]);
    CyDmaChEnable(sig_data.hDmaFilterA, 1);
}

/* ************************************************************************** */

static void Reset_DMA_Filter_B(void)
{
    CyDmaChDisable(sig_data.hDmaFilterB);

    CyDmaChSetRequest(sig_data.hDmaFilterB, CPU_TERM_TD);
    while (CPU_TERM_CHAIN & CyDmaChGetRequest(sig_data.hDmaFilterB))
    {
        continue;
    }

    CyDmaTdSetConfiguration(sig_data.ahTdFilterB[0], 
                            sig_config.nSampleSkipCount * sizeof(uint16), 
                            sig_data.ahTdFilterB[1], 
                            0);

    CyDmaTdSetAddress  (sig_data.ahTdFilterB[0], 
                        LO16((uint32)DMA_Filter_B_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_B_DUMP_ADDR));

    CyDmaTdSetConfiguration(sig_data.ahTdFilterB[1], 
                            sig_config.nCaptureCount * sizeof(uint16), 
                            sig_data.ahTdFilterB[0], 
                            TD_INC_DST_ADR | DMA_Filter_B__TD_TERMOUT_EN);

    CyDmaTdSetAddress  (sig_data.ahTdFilterB[1], 
                        LO16((uint32)DMA_Filter_B_SRC_ADDR), 
                        LO16((uint32)DMA_Filter_B_DST_ADDR));

    CyDmaChSetInitialTd(sig_data.hDmaFilterB, sig_data.ahTdFilterB[0]);
    CyDmaChEnable(sig_data.hDmaFilterB, 1);
}

/* ************************************************************************** */

void SignalInit(void)
{
    sig_data.usb_sof = USB_SOF;

    //
    //  Préparation du DFB.
    //

    Filter_Start();

    Filter_COHER_REG  = STAGEA_KEY_LOW | HOLDA_KEY_LOW | STAGEB_KEY_LOW | HOLDB_KEY_LOW;
    Filter_DALIGN_REG = STAGEA_DALIGN_HIGH | HOLDA_DALIGN_HIGH | STAGEB_DALIGN_HIGH | HOLDB_DALIGN_HIGH;

    //
    //  Préparation des canaux DMA.
    //
    
    Setup_DMA_ADC_A();
    Setup_DMA_ADC_B();
    Setup_DMA_Filter_A();
    Setup_DMA_Filter_B();

    sig_data.nChannelCur = 0;
    sig_data.bDataReady = 0;
    
    //
    //  D?marrage des ADC.
    //
    
    ADC_SAR_A_Start();
    ADC_SAR_A_IRQ_Disable();
    ADC_SAR_A_StartConvert();
    
    ADC_SAR_B_Start();
    ADC_SAR_B_IRQ_Disable();
    ADC_SAR_B_StartConvert();

    //
    //  Demarrage des multiplexeurs.
    //

    AMux_Row_A_Start();
    AMux_Row_B_Start();
    AMux_Row_A_Next();
    AMux_Row_B_Next();

    //
    //  Demarrage du signal.
    //

    Clock_Signal_Out_Start();
    
    ISR_Matrix_StartEx(Matrix_Switch_Handler);
    ISR_Matrix_Enable();
}

/* ************************************************************************** */
/* ************************************************************************** */

void SignalSetCapturePeriod(uint16 nBlankingPeriod, uint16 nCapturePeriod)
{
    uint8 nCtlOld = Matrix_CR_Control;

    Matrix_CR_Control = MATRIX_CR_RESET;

    sig_config.nSampleSkipCount = nBlankingPeriod;
    sig_config.nCaptureCount = nCapturePeriod;
    
    Reset_DMA_Filter_A();
    Reset_DMA_Filter_B();

    sig_data.nChannelCur = 0;
    sig_data.bDataReady = 0;

    //
    //  redemarrage des multiplexeurs.
    //

    AMux_Row_A_Start();
    AMux_Row_B_Start();
    AMux_Row_A_Next();
    AMux_Row_B_Next();


    Matrix_CR_Control = nCtlOld;
}

/* ************************************************************************** */
/* ************************************************************************** */

CY_ISR(Matrix_Switch_Handler)
{
    //
    //  Faire avancer le scanning
    //
    
    Pin_Matrix_Switch_ISR_Time_Write(1);

    uint8 nChannelLast = sig_data.nChannelCur++;
    sig_data.nChannelCur &= SIG_CHANNEL_MASK;

    AMux_Row_A_Next();
    AMux_Row_B_Next();
    
    Matrix_Channel_Control = sig_data.nChannelCur;
    
    //
    //  Stocker l'intégrale des lectures pour les deux canaux.
    //
    
    uint16 i;
    uint32 sumA = 0, sumB = 0;
    for (i = 0; i < sig_config.nCaptureCount; ++i)
    {
        sumA += (uint32)(abs16(sig_data.anDMABuffer[0][i]));
        sumB += (uint32)(abs16(sig_data.anDMABuffer[1][i]));
    }
    
    int nRow = nChannelLast & 0x07;
    int nCol = nChannelLast >> 3;

    sig_data.anRawData[nCol][nRow] = sumA;
    sig_data.anRawData[nCol][nRow + (NUM_ROWS / 2)] = sumB;
    
    //
    //  indiquer la fin de trame.
    //
    
    if (SIG_CHANNEL_MASK == nChannelLast)
    {
        sig_data.bDataReady = 1;
    }

    Pin_Matrix_Switch_ISR_Time_Write(0);
}

/* ********************************************************************** EOF */
