/** @file led.c
*
* @brief Turn on and off on boards LEDs.
*
*/
#include "fsl_gpio.h"
#include "pin_mux.h"

void turn_off_leds()
{
    GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 1U);
    GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 1U);
    GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 1U);
}

void turn_on_red()
{
    turn_off_leds();
    GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
}

void turn_on_green()
{
    turn_off_leds();
    GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
}

void turn_on_blue()
{
    turn_off_leds();
    GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

void turn_on_yellow()
{
    turn_off_leds();
    GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
    GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
}

void turn_on_magenta()
{
    turn_off_leds();
    GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
    GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

void turn_on_cyan()
{
    turn_off_leds();
    GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
    GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

void turn_on_white()
{
    GPIO_PinWrite(LED_RED_GPIO, LED_RED_PIN, 0U);
    GPIO_PinWrite(LED_GREEN_GPIO, LED_GREEN_PIN, 0U);
    GPIO_PinWrite(LED_BLUE_GPIO, LED_BLUE_PIN, 0U);
}

/*** end of file ***/
