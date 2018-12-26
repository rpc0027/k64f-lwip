/**
 * @file i2c.c
 * @author RPC0027
 * @date December 2018
 * @version 1.0
 *
 * @brief Interface between serial bus I2C and LCD.
 *
 * Send bytes of data to the device at the given slave address.
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "peripherals.h"

#include "wait.h"
#include "i2c.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_slave_address = 0x00; /**< Address of the slave device. */

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * @brief Specify the slave device by its hexadecimal address.
 *
 * @param[in] slave_address The address of the I2C slave in the bus.
 */
void select_slave(uint8_t slave_address)
{
    g_slave_address = slave_address;
}

/**
 * @brief Send a byte over the I2C bus.
 *
 * @param[in] chr the byte to be send.
 */
void send_byte(uint8_t chr)
{
    I2C_MasterStart(I2C, g_slave_address, kI2C_Write);

    wait_Waitus(8);

    I2C_MasterWriteBlocking(I2C, &chr, TX_SIZE, kI2C_TransferDefaultFlag);

    I2C_MasterStop(I2C);
}

/*** end of file ***/
