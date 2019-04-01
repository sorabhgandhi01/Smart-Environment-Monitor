/*@filename	: i2c_helper.h
 * @author	: Sorabh Gandhi & Om Raheja
 * @brief	: Contains all the functions related to i2c operations 
 * @date	: 31st March 2019
 * */

#ifndef I2C_HELPER_H
#define I2C_HELPER_H


/***********************************************************
 * USER DEFINED HEADER FILES & STANDARD C LIBRARY HEADERS  *
 * *********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "mraa/i2c.h"

/******************************
 * MACROS & DEFINITIONS       *
 * ****************************/
#define MY_I2C_BUS 2
mraa_i2c_context i2c_context;


/******************************
 * FUNCTION PROTOTYPES        *
 * ****************************/


/******************************
 * I2C OPEN                   *
 * ****************************/
/*@brief: Opens the I2C bus
 *
 *@param: void
 *
 *@return: 0 on SUCCESS
 *	 :-1 on FAILURE
 * */
int i2c_open();


/******************************
 * I2C CLOSE                  *
 * ****************************/
/*@brief: Closes the I2C bus
 *
 *@param: void
 *
 *@return: 0 on SUCCESS
 *	 :-1 on FAILURE
 * */
int i2c_close();


/******************************
 * I2C_WRITE                  *
 * ****************************/
/*@brief: sets slave address and writes 1 byte data to the slave device.
 *
 *@param: param1: slave address of the device
 	: param2: data to be written to the slave device
 *
 *@return: 0 on SUCCESS
 	 :-1 on FAILURE
 * */
int i2c_write(uint8_t slave_addr, uint8_t data);


/******************************
 * I2C_WRITE_BYTE             *
 * ****************************/
/*@brief: writes a byte to the slave device
 *
 *@param: param1: slave address of the device
        : param2: data to be written to the slave device
 *	: param3: 8 bit register of slave device in which data is to be written
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int i2c_write_byte(uint8_t slave_addr, uint8_t data, uint8_t command);


/******************************
 * I2C_WRITE_WORD             *
 * ****************************/
/*@brief: writes 2 byte to the slave device
 *
 *@param: param1: slave address of the device
        : param2: data to be written to the slave device
 *      : param3: 8 bit register of slave device in which data is to be written
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int i2c_write_word(uint8_t slave_addr, uint16_t data, uint8_t command);


/******************************
 * I2C_READ                   *
 * ****************************/
/*@brief: reads a byte to the slave device
 *
 *@param: param1: slave address of the device
        : param2: data to be read from the slave device
 *      : param3: 8 bit register of slave device from which data is to be read
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int i2c_read(uint8_t slave_addr, uint8_t *data, uint8_t command);


/******************************
 * I2C_READ_BYTES             *
 * ****************************/
/*@brief: reads 'n' bytes to the slave device
 *
 *@param: param1: slave address of the device
        : param2: data to be read from the slave device
 *      : param3: 8 bit register of slave device from which data is to be read
 *	: param4: number of bytes to be read
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int i2c_read_bytes(uint8_t slave_addr, uint8_t *data, uint8_t command, size_t len);

#endif
