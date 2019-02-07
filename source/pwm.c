/**
 * @file pwm.c
 * @author RPC0027
 * @date December 2018
 * @version 1.0
 *
 * @brief Controls the PWM devices.
 *
 * Devices can be controlled modifying their duty cycle.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "pwm.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Adjust the PWM duty cycle with the given percentage.
 *
 * @param[in] base             A pointer to the FTM peripheral.
 * @param[in] chnlNumber       The specific channel to modify.
 * @param[in] dutyCyclePercent New percentage of the duty cycle.
 */
void update_pwm_dutycyle(FTM_Type * base, ftm_chnl_t chnlNumber, uint8_t dutyCyclePercent)
{
	FTM_UpdateChnlEdgeLevelSelect(base, chnlNumber, 0U);
	FTM_UpdatePwmDutycycle(base, chnlNumber, kFTM_CenterAlignedPwm, dutyCyclePercent);
	FTM_SetSoftwareTrigger(base, true);
	FTM_UpdateChnlEdgeLevelSelect(base, chnlNumber, kFTM_LowTrue);
}

/*** end of file ***/
