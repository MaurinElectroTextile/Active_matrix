/* ************************************************************************** */
/* ************************************************************************** */

#include <device.h>
#include "signal.h"
#include "dfb_extra.h"
#include "debug.h"
#include "cfg.h"
#include "filters.h"
#include "iir.h"

/* ************************************************************************** */

#define APP_VERSION     "3.1"

/* ************************************************************************** */

#define USB_ENABLE      1
#define LCD_ENABLE      1
#define UART_ENABLE     (0 && (!USB_ENABLE))

#define USBUART_TX_BUFFER_SIZE      (64)
#define USBUART_RX_BUFFER_SIZE      (255)

/* ************************************************************************** */

typedef enum
{
    rxStateIdle,
    rxStateReadCfgV2,
    rxStateReadCfgV1,
    rxStateReadCfg,
} RxState;

/* ************************************************************************** */

static const uint8 GAIN_STAGE1_A_CONV[] = 
{
    PGA_Stage1_A_GAIN_01,
    PGA_Stage1_A_GAIN_03,
    PGA_Stage1_A_GAIN_07,
    PGA_Stage1_A_GAIN_15,
    PGA_Stage1_A_GAIN_22,
    PGA_Stage1_A_GAIN_24,
    PGA_Stage1_A_GAIN_31,
    PGA_Stage1_A_GAIN_47,
    PGA_Stage1_A_GAIN_49,
};

/* ************************************************************************** */

static const uint8 GAIN_STAGE2_A_CONV[] = 
{
    PGA_Stage2_A_GAIN_01,
    PGA_Stage2_A_GAIN_02,
    PGA_Stage2_A_GAIN_04,
    PGA_Stage2_A_GAIN_08,
    PGA_Stage2_A_GAIN_16,
    PGA_Stage2_A_GAIN_24,
    PGA_Stage2_A_GAIN_32,
    PGA_Stage2_A_GAIN_48,
    PGA_Stage2_A_GAIN_50,
};

/* ************************************************************************** */

static const uint8 GAIN_STAGE1_B_CONV[] = 
{
    PGA_Stage1_B_GAIN_01,
    PGA_Stage1_B_GAIN_03,
    PGA_Stage1_B_GAIN_07,
    PGA_Stage1_B_GAIN_15,
    PGA_Stage1_B_GAIN_22,
    PGA_Stage1_B_GAIN_24,
    PGA_Stage1_B_GAIN_31,
    PGA_Stage1_B_GAIN_47,
    PGA_Stage1_B_GAIN_49,
};

/* ************************************************************************** */

static const uint8 GAIN_STAGE2_B_CONV[] = 
{
    PGA_Stage2_B_GAIN_01,
    PGA_Stage2_B_GAIN_02,
    PGA_Stage2_B_GAIN_04,
    PGA_Stage2_B_GAIN_08,
    PGA_Stage2_B_GAIN_16,
    PGA_Stage2_B_GAIN_24,
    PGA_Stage2_B_GAIN_32,
    PGA_Stage2_B_GAIN_48,
    PGA_Stage2_B_GAIN_50,
};

/* ************************************************************************** */

static PROTO2_CFG_T cfgNext = 
{
    GainStage1_Default,     //  gainStage1
    GainStage2_Default,     //  gainStage2
    BlankingPeriod_Default, //  nBlankingPeriod
    CapturePeriod_Default,  //  nCapturePeriod
    Rectifier_Default,      //  nRectifierStyle
                            //  (End V1)
    Filter_Default,         //  nFilterStyle
                            //  (End V2)
    LPFStages_Default,      //  nLPFStages
    0,                      //  nReserved
    LPFCutOff_Default,      //  fpLPFCutOff
    NotchCutOff_Default,    //  fpNotchCutOff
    NotchQ_Default,         //  fpNotchQ
                            //  (End V3)
};

static PROTO2_CFG_T cfgCur;

/* ************************************************************************** */

//static IIR5_T fltIIR[NUM_ROWS * NUM_COLS];

static BIQUAD_CASCADE_COEFS_T   iirCoefs;
static BIQUAD_CASCADE_DATA_T    iirData[NUM_ROWS * NUM_COLS];

static BIQUAD_COEFS_T           notchCoefs;
static BIQUAD_DATA_T            notchData[NUM_ROWS * NUM_COLS];

/* ************************************************************************** */

static void SetConfig(const PROTO2_CFG_T* pCfg)
{
//#if LCD_ENABLE
//    LCD_Position(1,0);
//    LCD_PrintString("CFG Rx.         ");
//#endif

    //
    //  Stop operations
    //

    Matrix_CR_Control &= ~(MATRIX_CR_INPUT_ENABLE | MATRIX_CR_OUTPUT_ENABLE);
    
    //
    //  Set parameters
    //
    
    if (pCfg->gainStage1 <= GainStage1_Max)
    {
        PGA_Stage1_A_SetGain(GAIN_STAGE1_A_CONV[pCfg->gainStage1]);
        PGA_Stage1_B_SetGain(GAIN_STAGE1_B_CONV[pCfg->gainStage1]);
        cfgCur.gainStage1 = pCfg->gainStage1;
    }
    
    if (pCfg->gainStage2 <= GainStage2_Max)
    {
        PGA_Stage2_A_SetGain(GAIN_STAGE1_A_CONV[pCfg->gainStage2]);
        PGA_Stage2_B_SetGain(GAIN_STAGE1_B_CONV[pCfg->gainStage2]);
        cfgCur.gainStage2 = pCfg->gainStage2;
    }
    
    if ((pCfg->nBlankingPeriod != cfgCur.nBlankingPeriod) \
        || (pCfg->nCapturePeriod != cfgCur.nCapturePeriod))
    {
        if (pCfg->nBlankingPeriod < PROTO2_BLANKING_PERIOD_MIN)
        {
            cfgCur.nBlankingPeriod = PROTO2_BLANKING_PERIOD_MIN;
        }
        else
        {
            cfgCur.nBlankingPeriod = pCfg->nBlankingPeriod;
        }

        if (pCfg->nCapturePeriod > MAX_SAMPLES_CAPTURE_COUNT)
        {
            cfgCur.nCapturePeriod = MAX_SAMPLES_CAPTURE_COUNT;
        }
        else
        {
            cfgCur.nCapturePeriod = pCfg->nCapturePeriod;
        }
        
        SignalSetCapturePeriod(cfgCur.nBlankingPeriod, cfgCur.nCapturePeriod);
        
    }
    
    switch(pCfg->nRectifierStyle)
    {
    case RectifierFullWave:
        Matrix_CR_Control |= MATRIX_CR_RECT_FULL;
        Matrix_CR_Control &= ~MATRIX_CR_RECT_PHASE;
        cfgCur.nRectifierStyle = pCfg->nRectifierStyle;
        break;
    
    case RectifierHalfWavePos:
        Matrix_CR_Control &= ~MATRIX_CR_RECT_FULL;
        Matrix_CR_Control |= MATRIX_CR_RECT_PHASE;
        cfgCur.nRectifierStyle = pCfg->nRectifierStyle;
        break;

    case RectifierHalfWaveNeg:
        Matrix_CR_Control &= ~(MATRIX_CR_RECT_FULL | MATRIX_CR_RECT_PHASE);
        cfgCur.nRectifierStyle = pCfg->nRectifierStyle;
        break;
    }

    uint8 bResetFilters = 0;
    if (cfgCur.nFilterStyle != pCfg->nFilterStyle)
    {
        switch(pCfg->nFilterStyle)
        {
        case FilterNone:
        case FilterLPF:
        case FilterNotch:
        case FilterLPF_Notch:
            cfgCur.nFilterStyle = pCfg->nFilterStyle;
            break;
        default:
            cfgCur.nFilterStyle = Filter_Default;
            break;
        }
        bResetFilters = 1;
    }
    
    if ((cfgCur.nLPFStages != pCfg->nLPFStages) \
        || (cfgCur.fpLPFCutOff != pCfg->fpLPFCutOff))
    {
        cfgCur.nLPFStages = pCfg->nLPFStages;
        cfgCur.fpLPFCutOff = pCfg->fpLPFCutOff;
        
        if (cfgCur.nLPFStages > LPFStages_Max)
        {
            cfgCur.nLPFStages = LPFStages_Max;
        }
        if (cfgCur.fpLPFCutOff > LPFCutOff_Max)
        {
            cfgCur.fpLPFCutOff = LPFCutOff_Max;
        }
        
        float fC = ((float)cfgCur.fpLPFCutOff + .5f) / (float)(1 << LPFCutOff_Shift);
        
        BiquadBuildCascadeLPF(fC, cfgCur.nLPFStages, &iirCoefs);
        bResetFilters = 1;
    }
    
    if ((cfgCur.fpNotchCutOff != pCfg->fpNotchCutOff) \
        || (cfgCur.fpNotchQ != pCfg->fpNotchQ))
    {
        cfgCur.fpNotchCutOff = pCfg->fpNotchCutOff;
        cfgCur.fpNotchQ = pCfg->fpNotchQ;

        if (cfgCur.fpNotchCutOff > NotchCutOff_Max)
        {
            cfgCur.fpNotchCutOff = NotchCutOff_Max;
        }
        if (cfgCur.fpNotchQ > NotchQ_Max)
        {
            cfgCur.fpNotchQ = NotchQ_Max;
        }

        
        float fC = ((float)cfgCur.fpNotchCutOff + .5f) / (float)(1 << NotchCutOff_Shift);
        float fQ = ((float)cfgCur.fpNotchQ + .5f) / (float)(1 << NotchQ_Shift);
        
        BiquadBuildNotchFilter(fC, fQ, &notchCoefs);
        bResetFilters = 1;
    }

    if (bResetFilters)
    {
        memset(&iirData, 0, sizeof(iirData));
        memset(&notchData, 0, sizeof(notchData));
    }

    //
    //  Resume ops
    //

    Matrix_CR_Control |= (MATRIX_CR_INPUT_ENABLE | MATRIX_CR_OUTPUT_ENABLE);

//#if LCD_ENABLE
//    LCD_Position(1,0);
//    LCD_PrintString("CFG Rx...  OK   ");
//#endif
}

/* ************************************************************************** */

void main()
{
    int bDataReady = 0;
    int i;
    int nBytesToSend = 0;
    
#if USB_ENABLE
    int nTxKick = 0;
#endif

#if LCD_ENABLE
    LCD_Start();
    LCD_Position(0,0);
    LCD_PrintString("XYi Active " APP_VERSION "  ");
#endif

    Opamp_Vref_Start();

    Opamp_A_Start();
    Opamp_B_Start();

    PGA_Stage1_A_Start();
    PGA_Stage1_A_CR1_REG |= PGA_Stage1_A_COMP_MASK;
    
    PGA_Stage2_A_Start();
    PGA_Stage2_A_CR2_REG |= PGA_Stage2_A_COMP_MASK;

    PGA_Stage1_B_Start();
    PGA_Stage1_B_CR1_REG |= PGA_Stage1_B_COMP_MASK;
    
    PGA_Stage2_B_Start();
    PGA_Stage2_B_CR2_REG |= PGA_Stage2_B_COMP_MASK;

    Matrix_CR_Control = MATRIX_CR_RESET;

    CYGlobalIntEnable;

    DebugInit();
    SignalInit();

    SetConfig(&cfgNext);
        
    Matrix_CR_Control = MATRIX_CR_INPUT_ENABLE | MATRIX_CR_OUTPUT_ENABLE;


#if USB_ENABLE

#if LCD_ENABLE
    LCD_Position(1,0);
    LCD_PrintString("Init. USB...    ");
#endif

    USBUART_Start(0, USBUART_5V_OPERATION);
    while(!USBUART_bGetConfiguration())
    {
        continue;
    }
    USBUART_Init();
    
#endif

#if UART_ENABLE

#if LCD_ENABLE
    LCD_Position(1,0);
    LCD_PrintString("Init. UART      ");
#endif

    UART_Start();

#endif
    
#if LCD_ENABLE
    LCD_Position(1,0);
    LCD_PrintString("Running...      ");
#endif

//    InitializeBaseline();
    
    for(;;)
    {
        if (sig_data.bDataReady)
        {
            Pin_Soft_Filter_Time_Write(1);
            sig_data.bDataReady = 0;

            memcpy(&(sig_data.anFilteredData), &(sig_data.anRawData), sizeof(sig_data.anFilteredData));

            if (FilterNone != cfgCur.nFilterStyle)
            {
                for (i = 0; i < NUM_ROWS * NUM_COLS; ++i)
                {
                    if (FilterNotch != cfgCur.nFilterStyle)
                    {
                        sig_data.anFilteredData[0][i] = BiquadFilterCascade(sig_data.anFilteredData[0][i], &iirCoefs, &(iirData[i]));
                    }
                    if (FilterLPF != cfgCur.nFilterStyle)
                    {
                        sig_data.anFilteredData[0][i] = BiquadFilter(sig_data.anFilteredData[0][i], &notchCoefs, &notchData[i]);
                    }
                }
            }
            bDataReady = 1;
            Pin_Soft_Filter_Time_Write(0);
        }

#if USB_ENABLE || UART_ENABLE

#if USB_ENABLE
        if (bDataReady && (!nTxKick || USBUART_bTxIsReady()))
#elif UART_ENABLE
        if (bDataReady && !(UART_TX_STS_FIFO_FULL & UART_ReadTxStatus()))
#endif
        {
            int nBytesSent;
            int nBytesThisRun;
            uint8* pData;
            
            if (!nBytesToSend)
            {
                nBytesToSend = sizeof(sig_data.anRawData) + sizeof(uint32);
            }

            nBytesSent = sizeof(sig_data.anRawData) + sizeof(uint32) - nBytesToSend;
            nBytesThisRun = nBytesToSend;
#if USB_ENABLE
            if (nBytesThisRun > USBUART_TX_BUFFER_SIZE)
            {
                nBytesThisRun = USBUART_TX_BUFFER_SIZE;
            }
#elif UART_ENABLE
            int n = UART_GetTxBufferSize();
            if (n == 0) 
            {
                n = 255;
            }
            if (nBytesThisRun > n)
            {
                nBytesThisRun = n;
            }
#endif
            pData = ((uint8*)&sig_data.usb_sof) + nBytesSent;

#if USB_ENABLE
            USBUART_Write(pData, nBytesThisRun);
            nTxKick = 1;
#elif UART_ENABLE
            UART_PutArray(pData, nBytesThisRun);
#endif
            nBytesToSend -= nBytesThisRun;
            if (!nBytesToSend)
            {
                bDataReady = 0;
            }
        }
#if USB_ENABLE
        uint8 nRxCount = USBUART_bGetRxCount();
#elif UART_ENABLE
        uint8 nRxCount = UART_GetRxBufferSize();
#endif
        if (0 != nRxCount && nRxCount <= USBUART_RX_BUFFER_SIZE)
        {
            static uint8  pchSig[4];
            static uint8 pchRxBuffer[USBUART_RX_BUFFER_SIZE];
            static RxState rxState = rxStateIdle;
            static int nRxPtr = 0;
            uint8* pch;

#if USB_ENABLE
            USBUART_ReadAll(pchRxBuffer);
#elif UART_ENABLE
            for (i = 0; i < nRxCount; ++i)
            {
                pchRxBuffer[i] = UART_GetChar();
            }
#endif

            pch = pchRxBuffer;

            while (nRxCount > 0)
            {
                pchSig[0] = pchSig[1];
                pchSig[1] = pchSig[2];
                pchSig[2] = pchSig[3];
                pchSig[3] = *pch;

                ((uint8*)&cfgNext)[nRxPtr] = *pch;
                ++nRxPtr;
                ++pch;
                --nRxCount;

                switch(rxState)
                {
                case rxStateIdle:
                    nRxPtr = 0;
                    switch(*((uint32*)pchSig))
                    {
                    case PROTO2_CFG_SOF_V1:
                        rxState = rxStateReadCfgV1;
                        break;
                    case PROTO2_CFG_SOF_V2:
                        rxState = rxStateReadCfgV2;
                        break;
                    case PROTO2_CFG_SOF:
                        rxState = rxStateReadCfg;
                        break;
                    }
                    break;

                case rxStateReadCfgV2:
                case rxStateReadCfgV1:
                case rxStateReadCfg:
                
                    if ((rxState == rxStateReadCfgV1 && nRxPtr >= sizeof(PROTO2_CFG_V1_T)) \
                        || (rxState == rxStateReadCfgV2 && nRxPtr >= sizeof(PROTO2_CFG_V2_T))\
                        || (rxState == rxStateReadCfg && nRxPtr >= sizeof(PROTO2_CFG_T)))
                    {
                        SetConfig(&cfgNext);
                        nRxPtr = 0;
                        rxState = rxStateIdle;
                    }
                    else if (nRxPtr >= sizeof(PROTO2_CFG_T))
                    {
                        nRxPtr = 0;
                        rxState = rxStateIdle;
                    }
                    break;

                default:
                    rxState = rxStateIdle;
                    nRxPtr = 0;
                    break;
                }
            }
        }
#endif  //  USB_ENABLE || UART_ENABLE
    }
}

/* ********************************************************************** EOF */
