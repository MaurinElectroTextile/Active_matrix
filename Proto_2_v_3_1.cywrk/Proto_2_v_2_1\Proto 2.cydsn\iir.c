/* ************************************************************************** */
/* ************************************************************************** */

#include <device.h>
#include <math.h>
#include "iir.h"

/* ************************************************************************** */
/*
 *  Options
 */

#define IIR_NOTCH_FAVORS_LOW_PASS  1

/* ************************************************************************** */
/*
 *  Constantes
 */
 
static const float PI = 3.1415926535f;

/* ************************************************************************** */
/**
 *  Convertit les valeurs d'entrées vers le format interne 
 *  utilisé par le filtre. 
 *
 *  Le module IIR peut utiliser plusieurs formats de variables 
 *  à point flotant, voir IIR_FLOAT pour plus de détails.
 *
 *  @param  x   variable d'entrée en format point flotant ou entier.
 *
 *  @returns La valeur de <i>x</i> convertie en format IIR_FLOAT.
 */
 
static inline IIR_FLOAT _ConvertInput(uint32 x)
{
    return (IIR_FLOAT)x;
}

/* ************************************************************************** */

static inline uint32 _ConvertOutput(IIR_FLOAT x)
{
#if IIR_USE_FP24
    return (uint32)x;
#else
    return (uint32)(x + .5f);
#endif
}

/* ************************************************************************** */

static inline IIR_FLOAT _ConvertCoef(float x)
{
#if IIR_USE_FP24
    return (IIR_FLOAT)FP24_FromFloat(x);
#else
    return (IIR_FLOAT)(x);
#endif
}

/* ************************************************************************** */

static IIR_FLOAT _BiquadFilter (IIR_FLOAT x,
                                const BIQUAD_COEFS_T* pCoefs, 
                                BIQUAD_DATA_T* pData)
{
    IIR_FLOAT y;
    
#if IIR_USE_FP24
    
    int64 temp;
    
    temp =    FP_SS_Multiply(x,           pCoefs->B[0]) \
            + FP_SS_Multiply(pData->x[0], pCoefs->B[1]) \
            + FP_SS_Multiply(pData->x[1], pCoefs->B[2]) \
            + FP_SS_Multiply(pData->y[0], pCoefs->A[0]) \
            + FP_SS_Multiply(pData->y[1], pCoefs->A[1]);

    y = (IIR_FLOAT)(temp >> 24);

#else

    y = (x           * pCoefs->B[0]) \
      + (pData->x[0] * pCoefs->B[1]) \
      + (pData->x[1] * pCoefs->B[2]) \
      + (pData->y[0] * pCoefs->A[0]) \
      + (pData->y[1] * pCoefs->A[1]);

#endif
    
    pData->x[1] = pData->x[0];
    pData->x[0] = x;
    
    pData->y[1] = pData->y[0];
    pData->y[0] = y;
    
    return y;
}

/* ************************************************************************** */

static float _BiquadGetGainAtDC(float* a, float* b)
{
    float sa = a[0] + a[1];
    float sb = b[0] + b[1] + b[2];
    
    return (sb / (1 - sa));
}

/* ************************************************************************** */

static float _BiquadGetGainAtFMax(float* a, float* b)
{
    float sa = -a[0] + a[1];
    float sb = b[0] - b[1] + b[2];
    
    return (sb / (1 - sa));
}

/* ************************************************************************** */

static void _BiquadBuildStage  (BIQUAD_COEFS_T* pCoefs, 
                                float w, 
                                float pr, 
                                float pi, 
                                uint8 bHighPass)
{
    //
    //  z_plane transform
    //

    float x0, x1, x2, y1, y2;
    {    
        float t = 2.0 * tanf(.5f);
        float t2 = t * t;
        float m = (pr * pr) + (pi * pi);
        float mt2 = m * t2;
        float d = 4 - (4 * pr * t) + mt2;
        
        x0 = t2 / d;
        x1 = 2 * x0;
        x2 = x0;
        y1 = (8 - (2 * mt2)) / d;
        y2 = (-4 - (4 * pr * t) - (mt2)) / d;
    }
    
    //
    //  LP->LP transform
    //

    float a[2], b[3];

    {
        float k;
        if (bHighPass)
        {
            k = -cosf((w / 2) + .5) / cosf((w / 2) - .5);
        }
        else
        {
            k = sinf(.5 - (w / 2)) / sinf(.5 + (w / 2));
        }
        
        float k2 = k * k;
        float d = 1 + (y1 * k) - (y2 * k2);
        
        b[0] = (x0 - (x1 * k) + (x2 * k2)) / d;
        b[1] = ((-2 * x0 * k) + x1 + (x1 * k2) - (2 * x2 * k)) / d;
        b[2] = ((x0 * k2) - (x1 * k) + x2) / d;
        a[0] = ((2 * k) + y1 + (y1 * k2) - (2 * y2 * k)) / d;
        a[1] = (-k2 - (y1 * k) + y2) / d;
    
        if (bHighPass)
        {
            a[0] = -a[0];
            b[1] = -a[1];
        }
    }    

    //
    //  adjuster le gain
    //
    float gain;

    if (bHighPass)
    {
        gain = 1.0f / _BiquadGetGainAtFMax(a, b);
    }
    else
    {
        gain = 1.0f / _BiquadGetGainAtDC(a, b);
    }

    pCoefs->B[0] = _ConvertCoef(gain * b[0]);
    pCoefs->B[1] = _ConvertCoef(gain * b[1]);
    pCoefs->B[2] = _ConvertCoef(gain * b[2]);
    pCoefs->A[0] = _ConvertCoef(a[0]);
    pCoefs->A[1] = _ConvertCoef(a[1]);
}

/* ************************************************************************** */

void BiquadBuildCascadeLPF (float fC, 
                            int nStages, 
                            BIQUAD_CASCADE_COEFS_T* pBiquadCascade)
{
    const float w = 2 * PI * fC;
    int i;
    
    if (nStages > BIQUAD_MAX_STAGES)
    {
        nStages = BIQUAD_MAX_STAGES;
    }

    pBiquadCascade->nStages = nStages;

    if (nStages)
    {
        float PInp = PI / (nStages * 2);

        for (i = 0; i < pBiquadCascade->nStages; ++i)
        {
            float f = (i + 0.5f) * PInp;
            float pr = -cosf(f);
            float pi = sinf(f);
    
            _BiquadBuildStage(&(pBiquadCascade->coefs[i]), w, pr, pi, 0);
        }
    }
}
                                   
/* ************************************************************************** */

void BiquadBuildNotchFilter(float fC,
                            float fQ,
                            BIQUAD_COEFS_T* pCoefs)
{
    float a[2], b[3];
    float w   = 2.0f * PI * fC;
    float r   = expf(-w / (2.0f * fQ));

    float cos_w = cos(w);

    b[0] = 1.0f;
    b[1] = -(cos_w + cos_w);
    b[2] = 1.0f;

    a[0] = 2.0 * r * cos_w;
    a[1] = -(r * r);

    float gain;
    
    if (IIR_NOTCH_FAVORS_LOW_PASS || fC > 0.25f)
    {
        gain = 1.0f / _BiquadGetGainAtDC(a, b);
    }
    else
    {
        gain = 1.0f / _BiquadGetGainAtFMax(a, b);
    }

    pCoefs->B[0] = _ConvertCoef(gain * b[0]);
    pCoefs->B[1] = _ConvertCoef(gain * b[1]);
    pCoefs->B[2] = _ConvertCoef(gain * b[2]);
    pCoefs->A[0] = _ConvertCoef(a[0]);
    pCoefs->A[1] = _ConvertCoef(a[1]);
}
                                     
/* ************************************************************************** */

uint32 BiquadFilterCascade (uint32 x, 
                            const BIQUAD_CASCADE_COEFS_T* pCoefs, 
                            BIQUAD_CASCADE_DATA_T* pFilterData)
{
    int i;
    IIR_FLOAT   y;
    
    y = _ConvertInput(x);
    for (i = 0; i < pCoefs->nStages; ++i)
    {
        y = _BiquadFilter(y, &(pCoefs->coefs[i]), &((*pFilterData)[i]));
    }
    
    return _ConvertOutput(y);
}

/* ************************************************************************** */

uint32 BiquadFilter(uint32 x, 
                    const BIQUAD_COEFS_T* pCoefs, 
                    BIQUAD_DATA_T* pFilterData)
{
    IIR_FLOAT   y;
    
    y = _ConvertInput(x);
    y = _BiquadFilter(y, pCoefs, pFilterData);

    return _ConvertOutput(y);
}

/* ********************************************************************** EOF */
