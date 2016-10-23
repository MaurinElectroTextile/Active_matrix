#ifndef _signal_h
#define _signal_h

//
//  Les colonnes (COLS) sont actives (sorties)
//  Les lignes (ROWS) sont passives (entrees)
//

/* ************************************************************************** */

#define MATRIX_CR_RESET             (0x01)
#define MATRIX_CR_INPUT_ENABLE      (0x02)
#define MATRIX_CR_OUTPUT_ENABLE     (0x04)
#define MATRIX_CR_RECT_FULL         (0x08)
#define MATRIX_CR_RECT_PHASE        (0x10)

#define MATRIX_SR_ROW_COUNT_MASK    (0x07)
#define MATRIX_SR_ROW_COUNT_SHIFT   (0)

#define MATRIX_SR_COL_COUNT_MASK    (0x38)
#define MATRIX_SR_COL_COUNT_SHIFT   (3)
#define MATRIX_SR_COL_ACCU_OVFLW    (0x40)


#define NUM_COLS                16
#define NUM_ROWS                16
#define SIG_ROW_MASK            (0x07)
#define SIG_COL_MASK            (0x78)
#define SIG_CHANNEL_MASK        (0x7F)

#define DEFAULT_SAMPLES_SKIP_COUNT          (100)
#define DEFAULT_SAMPLES_CAPTURE_COUNT       (150)
#define MAX_SAMPLES_CAPTURE_COUNT           (512)

typedef struct _SIGNAL_DATA_T
{
    uint32          usb_sof;                            // NE PAS deplacer
    volatile uint32 anFilteredData[NUM_COLS][NUM_ROWS]; // NE PAS deplacer
    volatile uint32 anRawData[NUM_COLS][NUM_ROWS];      
    volatile uint16 anDMABuffer[2][MAX_SAMPLES_CAPTURE_COUNT];
    volatile uint16 nDumpBuffer;
    volatile uint16 nFilterBuffer[2];
    volatile uint16 nChannelCur;

    uint8           hDmaFilterA;
    uint8           ahTdFilterA[2];
    uint8           hDmaFilterB;
    uint8           ahTdFilterB[2];
    
    volatile uint8   bDataReady     : 1;
} SIGNAL_DATA_T;

typedef struct _SIGNAL_CONFIG_T
{
    uint8 nCaptureCount;
    uint8 nSampleSkipCount;
} SIGNAL_CONFIG_T;


extern SIGNAL_DATA_T    sig_data;
extern SIGNAL_CONFIG_T  sig_config;

extern void SignalInit();
extern void SignalSetCapturePeriod(uint16 nBlankingPeriod, uint16 nCapturePeriod);

#endif
