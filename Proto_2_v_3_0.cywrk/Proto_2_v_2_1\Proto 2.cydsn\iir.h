#ifndef _iir_h
#define _iir_h
/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */
/*
 *  Options de compilation
 */

#define IIR_USE_FP24    1

/* ************************************************************************** */

#if IIR_USE_FP24
#include "fp24.h"
#endif

/* ************************************************************************** */
/*
 *  Constantes
 */

#define BIQUAD_MAX_STAGES   (3)

/* ************************************************************************** */
/*
 *  Typedefs
 */

#if IIR_USE_FP24

typedef FP24    IIR_FLOAT;

#else

typedef float IIR_FLOAT;

#endif

/* ************************************************************************** */

typedef struct _BIQUAD_DATA_T
{
    IIR_FLOAT   x[2];
    IIR_FLOAT   y[2];
} BIQUAD_DATA_T;

/* -------------------------------------------------------------------------- */

typedef BIQUAD_DATA_T BIQUAD_CASCADE_DATA_T[BIQUAD_MAX_STAGES];

/* -------------------------------------------------------------------------- */

typedef struct _BIQUAD_COEFS_T
{
    IIR_FLOAT   B[3];
    IIR_FLOAT   A[2];
} BIQUAD_COEFS_T;

/* -------------------------------------------------------------------------- */

typedef struct _BIQUAD_CASCADE_COEFS_T
{
    uint32          nStages;
    BIQUAD_COEFS_T  coefs[BIQUAD_MAX_STAGES];
} BIQUAD_CASCADE_COEFS_T;

/* ************************************************************************** */
/*
 *  Procedures et fonctions
 */

extern void BiquadBuildCascadeLPF (float fC, 
                                   int nStages, 
                                   BIQUAD_CASCADE_COEFS_T* pBiquadCascade);
                                   
/* -------------------------------------------------------------------------- */

extern void BiquadBuildNotchFilter  (float fC,
                                     float fQ,
                                     BIQUAD_COEFS_T* pCoefs);
                                     
/* -------------------------------------------------------------------------- */

extern uint32 BiquadFilterCascade (uint32 x, 
                                   const BIQUAD_CASCADE_COEFS_T* pCoefs, 
                                   BIQUAD_CASCADE_DATA_T* pFilterData);

/* -------------------------------------------------------------------------- */

extern uint32 BiquadFilter (uint32 x, 
                            const BIQUAD_COEFS_T* pCoefs, 
                            BIQUAD_DATA_T* pFilterData);

#endif  /*  _iir_h  ***************************************************** EOF */