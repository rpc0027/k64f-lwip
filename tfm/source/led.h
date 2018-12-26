/**
 * @file led.h
 * @author RPC0027
 * @date December 2018
 * @version 1.0
 *
 * @brief Turn on and off on boards LEDs.
 *
 * The colors of the on board LED are red, green and blue.
 * Mixing these basic colors more can be obtained
 * like yellow, magenta, cyan and white.
 *
 */

#ifndef LED_H
#define LED_H

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void turn_off_leds();
void turn_on_red();
void turn_on_green();
void turn_on_blue();
void turn_on_yellow();
void turn_on_magenta();
void turn_on_cyan();
void turn_on_white();

#endif /* LED_H */

/*** end of file ***/
