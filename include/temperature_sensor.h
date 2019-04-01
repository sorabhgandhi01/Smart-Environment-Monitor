/*@filename     : temperature_sensor.h
 * @author      : Sorabh Gandhi & Om Raheja
 * @brief       : Contains all the function prototypes realted to temperature sensor initialization
 * @date        : 31st March 2019
 * */



#ifndef TEMPERATURE_SENSOR
#define TEMPERATURE_SENSOR

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "i2c_helper.h"

#define TEMPERATURE_SENSOR_ADDR (0x48)

#define TEMPERATURE_REG         (0x00)
#define CONFIGURATION_REG       (0X01)
#define TLOW_REG                (0X02)
#define THIGH_REG               (0x03)

#define EXTENDED_MODE_SET_BIT				(1<<12)
#define THERMOSTAT_MODE_SET_BIT				(1<<1)
#define SHUTDOWN_MODE_SET_BIT				(1)
#define POLARITY_SET_BIT					(1<<2)

/*****************************************
 * FUNCTION PROTOTYPES                   *
 * ***************************************/

/********************************************
 * WRITE TLOW REG                            *
 * ******************************************/
/*@brief: This function sets the lower order threshold.
 *
 *@param: temp (2 byte lower threshold data)
 *
 *@return: 0 on SUCCESS
 	 :-1 on FAILURE
 * */

int write_tlow_reg(uint16_t temp);


/********************************************
 * READ TLOW REG                            *
 * ******************************************/
/*@brief: This function reads the lower order threshold.
 *
 *@param: temp (2 byte lower threshold data)
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */

int read_tlow_reg(uint16_t *temp);

/********************************************
 * WRITE THIGH REG                          *
 * ******************************************/
/*@brief: This function sets the higher order threshold.
 *
 *@param: temp (2 byte higher threshold data)
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */

int write_thigh_reg(uint16_t temp);



/********************************************
 * READ THIGH REG                           *
 * ******************************************/
/*@brief: This function reads the higher order threshold.
 *
 *@param: temp (2 byte higher threshold data)
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */

int read_thigh_reg(uint16_t *temp);



/********************************************
 * READ CONFIGURATION REG                   *
 * ******************************************/
/*@brief: This function reads the configuration register
 *
 *@param: *data (2 byte of data obtained from configuration register)
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */

int read_configuration_reg(uint16_t *data);



/********************************************
 * SET ALERT                                *
 * ******************************************/
/*@brief: This function enables the alert functionality.
 *
 *@param: value 1 to enable alert
 		0 to disable alert
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */

int set_alert(uint8_t value);


/********************************************
 * SET DEFAULT MODE                         *
 * ******************************************/
/*@brief: This function sets the slave device in the default mode.
 *
 *@param: void
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */

int set_defaultMode();


/********************************************
 * SET EXTENDED MODE                         *
 * ******************************************/
/*@brief: This function sets the slave device in the EM mode.
 *
 *@param: void
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int set_extendedMode();


/********************************************
 * SET SD MODE                         *
 * ******************************************/
/*@brief: This function sets the slave device in the shutdown(low power) mode.
 *
 *@param: void
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int set_sdMode();


/********************************************
 * SET COMPARATOR MODE                      *
 * ******************************************/
/*@brief: This function sets the slave device in the comparator mode.
 *
 *@param: void
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int set_comparatorMode();


/********************************************
 * SET INTERRUPT MODE                       *
 * ******************************************/
/*@brief: This function sets the slave device in the interrupt mode.
 *
 *@param: void
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int set_InterruptMode();


/********************************************
 * SET OPERATION FREQ                       *
 * ******************************************/
/*@brief: This function sets the conversion rate of the slave device.
 *
 *@param: freq 0 to operate in 0.25Hz
 	       1 to operate in 1Hz
	       2 to operate in 4Hz
	       3 to operate in 8Hz
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int set_operationFreq(uint8_t freq);


/********************************************
 * READ ALERT BIT                           *
 * ******************************************/
/*@brief: This function reads the alert bit.
 *
 *@param: *value Obtains the alert bit
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int read_alertBit(uint8_t *value);

/********************************************
 * GET SENSOR TEMP                          *
 * ******************************************/
/*@brief: This function reads the temperature data from the temperature register.
 *
 *@param: *temp Obtains temperature in Celcius
 *
 *@return: 0 on SUCCESS
         :-1 on FAILURE
 * */
int get_sensortemp(float *temp);

#endif
