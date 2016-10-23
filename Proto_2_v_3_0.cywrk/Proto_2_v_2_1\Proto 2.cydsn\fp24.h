#ifndef _fp24_h
#define _fp24_h
/* ************************************************************************** */
/* ************************************************************************** */

#define FP24_USE_INLINE_ASSEMBLY    1

/* ************************************************************************** */

#ifndef _INT64_DEFINED
typedef long long int   int64;
typedef unsigned long long int uint64;
#define _INT64_DEFINED
#endif

typedef int32   FP24;

/* ************************************************************************** */

#define FP24_SHIFT      (24)
#define Q24(x)  ((int32)(((x) * (1 << FP24_SHIFT)) + (((x) >= 0) ? .5f : -.5f)))

/* ************************************************************************** */

static inline int64 FP_UU_Multiply(uint32 x, uint32 y)
{
#ifdef FP24_USE_INLINE_ASSEMBLY
    register union
    {
        int64 ll64;
        struct
        {
            uint32  wl; 
            int32   wh;
        };
    } res;

    asm ( "umull	%[rl], %[rh], %[xx], %[yy]"
          : [rl] "=r" (res.wl), [rh] "=r" (res.wh)
          : [xx] "r" (x), [yy] "r" (y) );

    return res.ll64;
#else
    return ((uint64)x) * y;
#endif
}

/* ************************************************************************** */

static inline int64 FP_SS_Multiply(int32 x, int32 y)
{
#ifdef FP24_USE_INLINE_ASSEMBLY
    register union
    {
        int64 ll64;
        struct
        {
            uint32  wl; 
            int32   wh;
        };
    } res;

    asm ( "smull	%[rl], %[rh], %[xx], %[yy]"
          : [rl] "=r" (res.wl), [rh] "=r" (res.wh)
          : [xx] "r" (x), [yy] "r" (y) );

    return res.ll64;
#else
    return ((int64)x) * y;
#endif
}

/* ************************************************************************** */

static inline FP24 FP24_FromFloat(float f)
{
    return Q24(f);
}

/* ************************************************************************** */

static inline float FP24_ToFloat(FP24 x)
{
    static const float FACTOR = 1.f / (float)(1 << FP24_SHIFT);

    float y = (float)x;
    if (x >= 0)
    {
        return FACTOR * (y + .5f);
    }
    else
    {
        return FACTOR * (y - .5f);
    }
}

#endif  /* _fp24_h  ***************************************************** EOF */
