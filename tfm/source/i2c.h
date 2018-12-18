/** @file i2c.h
 *
 * @brief Interface between I2C and LCD.
 *
 */

#ifndef I2C_H
#define I2C_H

#include <stdint.h>

typedef unsigned char byte;

byte select_slave(byte slv);
byte send_byte(uint8_t chr);
byte check_bus();

#endif /* I2C_H */

/*** end of file ***/
