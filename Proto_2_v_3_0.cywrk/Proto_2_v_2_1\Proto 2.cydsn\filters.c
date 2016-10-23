
#include "device.h"
#include <math.h>
#include "filters.h"


int32 HeavyFilter(int32 nLast, int32 nPrev)
{
    #define AVG_WEIGHT  128
    return (nLast + (nPrev * (AVG_WEIGHT - 1))) / AVG_WEIGHT;
}

int32 MedianFilter(int32 x1, int32 x2, int32 x3)
{
    register int32 tmp;

    if (x1 > x2)
    {
        tmp = x2;
        x2 = x1;
        x1 = tmp;
    }

    if(x2 > x3)
    {
        x2 = x3;
    }

    return ((x1 > x2) ? x1 : x2);
}

int32 AveragingFilter(int32 x1, int32 x2, int32 x3)
{
    return ((int32)x1 + (int32)x2 + (int32)x3) / 3;
}

int32 WeightedAverageFilter(int32 x1, int32 x2, int32 weight)
{
    register int32 tmp;

    if (weight < 0)
    {
        weight = -weight;
        tmp = ((weight * x1) + x2) / (weight + 1);
    }
    else if (weight == 1)
    {
        tmp = (x1 + x2) >> 1;
    }
    else if (weight == 3)
    {
        tmp = (x1 + (x2 * 3)) >> 2;
    }
    else if (weight == 7)
    {
        tmp = (x1 + (x2 * 7)) >> 3;
    }
    else
    {
        tmp = (x1 + (weight * x2)) / (weight + 1);
    }

    return tmp;
}


int32 JitterFilter(int32 x1, int32 x2)
{
    if (x1 > x2)
    {
        x1--;
    }
    else
    {
        if (x1 < x2)
        {
            x1++;
        }
    }

    return x1;
}

int32 NoiseGate(int32 x, int32 floor)
{
    if (x > 0)
    {
        if (x <= floor)
        {
            x = 0;
        }
        else
        {
            x -= floor;
        }
    }
    else if (x < 0)
    {
        if (x >= -floor)
        {
            x = 0;
        }
        else
        {
            x += floor;
        }
    }
    return x;
}


void IIR5Init(IIR5_T* p)
{
    memset(&(p->x), 0, sizeof(p->x));
    memset(&(p->y), 0, sizeof(p->y));

    p->B[0] = 0.0067191488810539454;     
	p->B[1] = 0.033595744405269724;
	p->B[2] = 0.067191488810539449;     
    p->B[3] = 0.067191488810539449;
	p->B[4] = 0.033595744405269724;    
    p->B[5] = 0.0067191488810539454;
    
    p->A[0] = -1.7281636238732112;       
    p->A[1] = 1.4824985464323421;
    p->A[2] = -0.71101884322104281;      
    p->A[3] = 0.18557516416835768;
    p->A[4] = -0.020664278454164568;
}

int32 IIR5Filter(int32 n, IIR5_T* p)
{
    float x, y;
    
    x = n;
    
    y = (x * p->B[0]) + (p->x[0] * p->B[1]) + (p->x[1] * p->B[2]) + (p->x[2] * p->B[3]) + (p->x[3] * p->B[4]) + (p->x[4] * p->B[5]);
    y -= ((p->y[0] * p->A[0]) + (p->y[1] * p->A[1]) + (p->y[2] * p->A[2]) + (p->y[3] * p->A[3]) + (p->y[4] * p->A[4]));

    p->x[4] = p->x[3];
    p->x[3] = p->x[2];
    p->x[2] = p->x[1];
    p->x[1] = p->x[0];
    p->x[0] = x;
    
    p->y[4] = p->y[3];
    p->y[3] = p->y[2];
    p->y[2] = p->y[1];
    p->y[1] = p->y[0];
    p->y[0] = y;

    return (int32)(y + .5);
}
