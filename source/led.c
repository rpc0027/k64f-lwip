/**
 * @file led.c
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

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_gpio.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Turn off all LEDs.
 */
void turn_off_leds()
{
	GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 1U);
	GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 1U);
	GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 1U);
}

/**
 * @brief Turn on red color.
 */
void turn_on_red()
{
	turn_off_leds();
	GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
}

/**
 * @brief Turn on green color.
 */
void turn_on_green()
{
	turn_off_leds();
	GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
}

/**
 * @brief Turn on blue color.
 */
void turn_on_blue()
{
	turn_off_leds();
	GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

/**
 * @brief Turn on yellow color.
 */
void turn_on_yellow()
{
	turn_off_leds();
	GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
	GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
}

/**
 * @brief Turn on magenta color.
 */
void turn_on_magenta()
{
	turn_off_leds();
	GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
	GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

/**
 * @brief Turn on cyan color.
 */
void turn_on_cyan()
{
	turn_off_leds();
	GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
	GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

/**
 * @brief Turn on white color.
 */
void turn_on_white()
{
	GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
	GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
	GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

/*** end of file ***/
