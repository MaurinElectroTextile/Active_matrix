#ifndef _filters_h
#define _filters_h


typedef struct _IIR5_T
{
    float   A[5];
    float   B[6];
    float   x[5];
    float   y[5];
} IIR5_T;

//typedef struct _LONGLONG
//{
//    union
//    {
//        struct
//        {
//            int32   high;
//            uint32   low;
//        };
//        uint8 b[8];
//        uint16 w[4];
//    };
//} LONGLONG;
//

extern int32 HeavyFilter(int32 nLast, int32 nPrev);
extern int32 MedianFilter(int32 x1, int32 x2, int32 x3);
extern int32 AveragingFilter(int32 x1, int32 x2, int32 x3);
extern int32 WeightedAverageFilter(int32 x1, int32 x2, int32 weight);
extern int32 JitterFilter(int32 x1, int32 x2);
extern int32 NoiseGate(int32 x, int32 floor);
extern void IIR5Init(IIR5_T* p);
extern int32 IIR5Filter(int32 n, IIR5_T* p);

#endif