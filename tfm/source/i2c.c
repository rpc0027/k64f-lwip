/** @file i2c.c
 *
 * @brief Interface between I2C and LCD.
 *
 **/
#include "fsl_debug_console.h"
#include "peripherals.h"

#include "wait.h"
#include "i2c.h"

#define TX_SIZE (sizeof(uint8_t))

uint8_t g_slave_address = 0x00;

/*!
 * @brief Select the slave by its hexadecimal address.
 * @param[in] slave Address of the I2C slave in the bus.
 * @return The stored address.
 */
byte select_slave(uint8_t slave)
{
    g_slave_address = slave;

    return (byte) g_slave_address;
}

/*!
 * @brief Send a byte over the I2C bus.
 * @param[in] chr slave Byte to be send.
 * @return The transfer status.
 */
byte send_byte(uint8_t chr)
{
    status_t status;

    I2C_MasterStart(I2C_1_PERIPHERAL, g_slave_address, kI2C_Write);

    wait_Waitus(8);

    status = I2C_MasterWriteBlocking(I2C_1_PERIPHERAL, &chr, TX_SIZE, kI2C_TransferDefaultFlag);

    I2C_MasterStop(I2C_1_PERIPHERAL);

    return (byte) status;
}

/*** end of file ***/
