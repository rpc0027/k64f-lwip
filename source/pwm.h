/**
 * @file pwm.h
 * @author RPC0027
 * @date December 2018
 * @version 1.0
 *
 * @brief Controls the PWM devices.
 *
 * Devices can be controlled modifying their duty cycle.
 */

#ifndef PWM_H
#define PWM_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

#include "fsl_ftm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define WHITE_PWM  FTM0_PERIPHERAL /**< FlexTimer Module of white LED. */
#define GREEN_PWM  FTM0_PERIPHERAL /**< FlexTimer Module of green LED. */
#define YELLOW_PWM FTM3_PERIPHERAL /**< FlexTimer Module of yellow LED. */
#define RED_PWM    FTM3_PERIPHERAL /**< FlexTimer Module of red LED. */

#define WHITE_CHANNEL  kFTM_Chnl_1 /**< Channel of white LED. */
#define GREEN_CHANNEL  kFTM_Chnl_3 /**< Channel of green LED. */
#define YELLOW_CHANNEL kFTM_Chnl_0 /**< Channel of yellow LED. */
#define RED_CHANNEL    kFTM_Chnl_2 /**< Channel of red LED. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void update_pwm_dutycyle(FTM_Type * base, ftm_chnl_t chnlNumber, uint8_t dutyCyclePercent);

#endif /* PWM_H */

/*** end of file ***/
