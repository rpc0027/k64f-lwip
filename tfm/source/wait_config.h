/**
 * @file wait.h
 * @author RPC0027
 * @date December 2018
 * @version 1.0
 *
 * @brief Configuration for the waiting cycles on the CPU.
 *
 */

#ifndef WAIT_CONFIG_H
#define WAIT_CONFIG_H

/**
 * If 1: use hardware cycle counter (if present, only on Cortex-M3 or higher).
 * If 0: not using hardware cycle counter.
 */
#ifndef WAIT_CONFIG_USE_CYCLE_COUNTER
#define WAIT_CONFIG_USE_CYCLE_COUNTER  (0 && (__CORTEX_M >= 3))
#endif

/**
 * If 1: use RTOS wait if RTOS is present.
 * If 0: use normal busy waiting.
 */
#ifndef WAIT_CONFIG_USE_RTOS_WAIT
#define WAIT_CONFIG_USE_RTOS_WAIT  (0 && USE_RTOS)
#endif

/**
 * Include Cortex utility functions.
 */
#if WAIT_CONFIG_USE_CYCLE_COUNTER
#include "KIN1.h"
#endif

#endif /* WAIT_CONFIG_H */

/*** end of file ***/
