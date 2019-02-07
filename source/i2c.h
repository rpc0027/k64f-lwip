/**
 * @file i2c.h
 * @author RPC0027
 * @date December 2018
 * @version 1.0
 *
 * @brief Interface between serial bus I2C and LCD.
 *
 * Send bytes of data to the device at the given slave address.
 *
 */

#ifndef I2C_H
#define I2C_H

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TX_SIZE (sizeof(uint8_t)) /**< Size of the data to transmit. */
#define I2C I2C_1_PERIPHERAL /**< Enabled peripheral on the board.*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void select_slave(uint8_t slave_address);
void send_byte(uint8_t chr);

#endif /* I2C_H */

/*** end of file ***/
