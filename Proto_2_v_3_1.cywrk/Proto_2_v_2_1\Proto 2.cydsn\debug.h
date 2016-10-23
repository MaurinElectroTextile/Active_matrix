#ifndef _debug_h
#define _debug_h


#define ENABLE_DEBUG_OUTPUT         0

//
//  un seul des suivants a la fois !!
//
#define TRACE_ADC_OUTPUT_SIGNAL     0   /* ...1 */
#define TRACE_FILTER_A_OUTPUT       0
#define TRACE_RECTIFIER_OUTPUT      1
#define TRACE_ACCU_OUTPUT           0
#define TRACE_FILTERED_OUTPUT       0
#define TRACE_VALUE                 0
#define TRACE_VALUE_ABS             0
#define TRACE_BASELINE              0

#if ENABLE_DEBUG_OUTPUT

extern uint32 nDebugBuffer;

#endif  //  ENABLE_DEBUG_OUTPUT

extern void DebugInit(void);


#endif