#ifndef _cfg_h
#define _cfg_h
/* ************************************************************************** */
/* ************************************************************************** */

#ifdef _MSC_VER
#pragma pack(push, 1)
typedef WORD uint16;
typedef DWORD uint32;
#endif

/* ************************************************************************** */

#define PROTO2_BLANKING_PERIOD_MIN     (36)
#define PROTO2_SAMPLING_RATE           (777778.0)
#define PROTO2_CFG_SOF_V1              (0xEEAAEE01)
#define PROTO2_CFG_SOF_V2              (0xEEAAEE02)
#define PROTO2_CFG_SOF                 (0xEEAAEE03)

/* -------------------------------------------------------------------------- */

typedef enum
{
    GainStage1_1,
    GainStage1_3,
    GainStage1_7,
    GainStage1_15,
    GainStage1_22,
    GainStage1_24,
    GainStage1_31,
    GainStage1_47,
    GainStage1_49,
    GainStage1_Max = GainStage1_49,
    GainStage1_Default = GainStage1_15,
} GainStage1;

/* -------------------------------------------------------------------------- */

typedef enum
{
    GainStage2_1,
    GainStage2_2,
    GainStage2_4,
    GainStage2_8,
    GainStage2_16,
    GainStage2_24,
    GainStage2_32,
    GainStage2_48,
    GainStage2_50,
    GainStage2_Max = GainStage2_50,
    GainStage2_Default = GainStage2_8,
} GainStage2;

/* -------------------------------------------------------------------------- */

typedef enum
{
    RectifierFullWave,
    RectifierHalfWavePos,
    RectifierHalfWaveNeg,
    Rectifier_Max = RectifierHalfWaveNeg,
    Rectifier_Default = RectifierFullWave,
} RectStyle;

/* -------------------------------------------------------------------------- */

typedef enum
{
    FilterNone,
    FilterLPF,
    FilterIIR5 = FilterLPF,
    FilterNotch,
    FilterLPF_Notch,
    Filter_Max = FilterLPF_Notch,
    Filter_Default = FilterLPF_Notch,
} FilterStyle;

/* -------------------------------------------------------------------------- */

#define BlankingPeriod_Default      (100)
#define CapturePeriod_Default       (125)
#define LPFStages_Default           (2)
#define LPFStages_Max               (6)
#define LPFCutOff_Shift             (30)
#define LPFCutOff_Default           ((uint32)(.08f * (1 << LPFCutOff_Shift)))
#define LPFCutOff_Max               ((uint32)(.5f * (1 << LPFCutOff_Shift)))
#define NotchCutOff_Shift           (30)
#define NotchCutOff_Max             ((uint32)(.5f * (1 << NotchCutOff_Shift)))
#define NotchCutOff_Default         ((uint32)(.095f * (1 << NotchCutOff_Shift)))
#define NotchQ_Shift                (24)
#define NotchQ_Max                  ((uint32)(255.0f * (1 << NotchQ_Shift)))
#define NotchQ_Default              ((uint32)(10.0f *  (1 << NotchQ_Shift)))

/* -------------------------------------------------------------------------- */

typedef struct _PROTO2_CFG_V1_T
{
    uint16      gainStage1;
    uint16      gainStage2;
    uint16      nBlankingPeriod;
    uint16      nCapturePeriod;
    uint16      nRectifierStyle;
} PROTO2_CFG_V1_T;

/* -------------------------------------------------------------------------- */

typedef struct _PROTO2_CFG_V2_T
{
    uint16      gainStage1;
    uint16      gainStage2;
    uint16      nBlankingPeriod;
    uint16      nCapturePeriod;
    uint16      nRectifierStyle;
    uint16      nFilterStyle;
} PROTO2_CFG_V2_T;

/* -------------------------------------------------------------------------- */

typedef struct _PROTO2_CFG_T 
{
    uint16      gainStage1;
    uint16      gainStage2;
    uint16      nBlankingPeriod;
    uint16      nCapturePeriod;
    uint16      nRectifierStyle;
    uint16      nFilterStyle;
    uint16      nLPFStages;
    uint16      nReserved;      ///< l'ARM ne supporte pas les DWORD mal alignés
    uint32      fpLPFCutOff;
    uint32      fpNotchCutOff;
    uint32      fpNotchQ;
} PROTO2_CFG_T;

/* ************************************************************************** */

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif /* _cfg_h ************************************************************ */