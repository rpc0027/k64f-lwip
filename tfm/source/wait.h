/*!
 ** @file wait.h
 ** @version 01.00
 ** @brief
 **          Implements busy waiting routines.
 */

/** @file wait.h
*
* @brief Header file for wait.
*
* @par
* This header file declares the functions of the busy waiting module.
*/
#ifndef WAIT_H
#define WAIT_H

#include <stdint.h>

#include "wait_config.h"

// Includes needed if RTOS is used.
//
#if WAIT_CONFIG_USE_RTOS_WAIT
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock; /* clock frequency variable defined system_<device>.h of the SDK */
#define WAIT_INSTR_CLOCK_HZ       SystemCoreClock  /* core clock frequency in Hz */
#define WAIT_NofCyclesMs(ms, hz)  ((ms)*((hz)/1000)) /* calculates the needed cycles based on bus clock frequency */
#define WAIT_NofCyclesUs(us, hz)  ((us)*(((hz)/1000)/1000)) /* calculates the needed cycles based on bus clock frequency */
#define WAIT_NofCyclesNs(ns, hz)  (((ns)*(((hz)/1000)/1000))/1000) /* calculates the needed cycles based on bus clock frequency */

#define WAIT_WAIT_C(cycles) \
        ( (cycles)<=10 ? \
                wait_wait0Cycles() \
                : wait_WaitCycles((uint16_t)cycles) \
        )                                      /*!< wait for some cycles */


void wait_wait0Cycles(void);
void wait_wait00Cycles(void);
void wait_WaitCycles(uint16_t cycles);
void wait_Waitms(uint16_t ms);

#define wait_Waitus(us)  \
        /*lint -save -e(505,506,522) Constant value Boolean, Redundant left argument to comma. */\
        (  ((WAIT_NofCyclesUs((us),WAIT_INSTR_CLOCK_HZ)==0)||(us)==0) ? \
                (void)0 : \
                ( ((us)/1000)==0 ? (void)0 : wait_Waitms((uint16_t)((us)/1000))) \
                , (WAIT_NofCyclesUs(((us)%1000), WAIT_INSTR_CLOCK_HZ)==0) ? (void)0 : \
                        WAIT_WAIT_C(WAIT_NofCyclesUs(((us)%1000), WAIT_INSTR_CLOCK_HZ)) \
                        /*lint -restore */\
        )

#define wait_Waitns(ns)  \
        /*lint -save -e(505,506,522) Constant value Boolean, Redundant left argument to comma. */\
        (  ((wait_NofCyclesNs((ns), wait_INSTR_CLOCK_HZ)==0)||(ns)==0) ? \
                (void)0 : \
                wait_Waitus((ns)/1000) \
                , (wait_NofCyclesNs((ns)%1000, wait_INSTR_CLOCK_HZ)==0) ? \
                        (void)0 : \
                        wait_WAIT_C(wait_NofCyclesNs(((ns)%1000), wait_INSTR_CLOCK_HZ)) \
                        /*lint -restore */\
        )

#if wait_CONFIG_USE_RTOS_WAIT
#define wait_WaitOSms(ms) vTaskDelay(pdMS_TO_TICKS(ms)) /* use FreeRTOS API */
#else
#define wait_WaitOSms(ms)  wait_Waitms(ms) /* use normal wait */
#endif

void wait_WaitLongCycles(uint32_t cycles);
void wait_Init(void);
void wait_DeInit(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* WAIT_H */

/*** end of file ***/
