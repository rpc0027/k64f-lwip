/** @file pwm.h
*
* @brief Header file for PWM functionality.
*
*/

#ifndef PWM_H
#define PWM_H


#include <stdint.h>

#include "fsl_ftm.h"

// FlexTimer Modules of each LED.
//
#define WHITE_PWM  FTM0_PERIPHERAL
#define GREEN_PWM  FTM0_PERIPHERAL
#define YELLOW_PWM FTM3_PERIPHERAL
#define RED_PWM    FTM3_PERIPHERAL

// Channel associated to each LED.
//
#define WHITE_CHANNEL  kFTM_Chnl_1
#define GREEN_CHANNEL  kFTM_Chnl_3
#define YELLOW_CHANNEL kFTM_Chnl_0
#define RED_CHANNEL    kFTM_Chnl_2

// Functions to adjust the duty cycle of the PWM devices.
//
void update_pwm_dutycyle(FTM_Type * base, ftm_chnl_t chnlNumber, uint8_t dutyCyclePercent);


#endif /* PWM_H */

/*** end of file ***/
