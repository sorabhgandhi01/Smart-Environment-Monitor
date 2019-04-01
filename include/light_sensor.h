#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "i2c_helper.h"

#define LIGHT_SENSOR_ADDR (0x39)

//Defining the register set of APDS-9301
#define COMMAND_REG (0x80)            //Specifies register address
#define CONTROL_REG (0x00)            //Control of basic functions
#define TIMING_REG (0x01)             //Integration time/gain control
#define THRESHLOWLOW_REG (0x02)       //Low byte of low interrupt threshold
#define THRESHLOWHIGH_REG (0x03)      //High byte of low interrupt threshold
#define THRESHHIGHLOW_REG (0x04)      //Low byte of high interrupt threshold
#define THRESHHIGHHIGH_REG (0x05)     //High byte of high interrupt threshold
#define INTERRUPT_REG (0x06)          //Interrupt control
#define CRC_REG (0x08)                //Factory test — not a user register
#define ID_REG (0x0A)                 //Part number/ Rev ID
#define DATA0LOW_REG (0x0C)           //Low byte of ADC channel 0
#define DATA0HIGH_REG (0x0D)          //High byte of ADC channel 0
#define DATA1LOW_REG (0x0E)           //Low byte of ADC channel 1
#define DATA1HIGH_REG (0x0F)          //High byte of ADC channel 1

#define POWER_ON_BIT (0x03)
#define POWER_OFF_BIT (0x00)
#define WORD_SET_BIT (1<<5)
#define MANUAL_CONTROL_ENABLE_BIT(x) (x<<3)
#define INTG_TIME_SET_BIT (0x03)
#define CLEAR_PENDING_INT_BIT (1<<6)


/**
--------------------------------------------------------------------------------------------
write_int_tlow
--------------------------------------------------------------------------------------------
*	This function sets Lower byte of the low interrupt threshold register
*
* 	@\param	tlow	Two byte value to lower threshold value
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int write_int_tlow(uint16_t tlow);


/**
--------------------------------------------------------------------------------------------
read_int_tlow
--------------------------------------------------------------------------------------------
*	This function reads Lower byte of the low interrupt threshold register
*
* 	@\param	tlow	Read the lower threshold value from threshold register
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int read_int_tlow(uint16_t *tlow);


/**
--------------------------------------------------------------------------------------------
write_int_thigh
--------------------------------------------------------------------------------------------
*	This function sets Higher byte of the high interrupt threshold register
*
* 	@\param	tlow	Write two byte value to higher threshold register
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int write_int_thigh(uint16_t thigh);


/**
--------------------------------------------------------------------------------------------
read_int_thigh
--------------------------------------------------------------------------------------------
*	This function reads Higher byte of the high interrupt threshold register
*
* 	@\param	tlow	Copy two byte value obtained from the higher threshold register
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int read_int_thigh(uint16_t *thigh);


/**
--------------------------------------------------------------------------------------------
sensor_enable
--------------------------------------------------------------------------------------------
*	This function powers on the slave light sensor
*
* 	@\param	void
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int sensor_enable();


/**
--------------------------------------------------------------------------------------------
sensor_disable
--------------------------------------------------------------------------------------------
*	This function powers off the slave light sensor
*
* 	@\param	void
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int sensor_disable();


/**
--------------------------------------------------------------------------------------------
read_sensorID
--------------------------------------------------------------------------------------------
*	This function reads the sensor ID
*
* 	@\param	ID 		Obtains the ID fetched from the register
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int read_sensorID(uint8_t *id);


/**
--------------------------------------------------------------------------------------------
read_timer_reg
--------------------------------------------------------------------------------------------
*	This function reads the timer register
*
* 	@\param	ID 		Obtains the eight bit value fetched from the register
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int read_timer_reg(uint8_t *data);


/**
--------------------------------------------------------------------------------------------
enable_interrupt
--------------------------------------------------------------------------------------------
*	This function enables the peripheral interrupt
*
* 	@\param	set 	Write "1" to enable the interrupt and "0" to disable the interrupt
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int enable_interrupt(uint8_t set);


/**
--------------------------------------------------------------------------------------------
clear_pendingInterrupt
--------------------------------------------------------------------------------------------
*	This function clears the pending interrupt from the command register
*
* 	@\param	void
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int clear_pendingInterrupt();


/**
--------------------------------------------------------------------------------------------
set_integrationTime
--------------------------------------------------------------------------------------------
*	This function sets the intergation time
*
* 	@\param	value	two bit value to set the Integration time (Refer the datasheet for different configurations)
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int set_integrationTime(uint8_t value);


/**
--------------------------------------------------------------------------------------------
set_manualControl
--------------------------------------------------------------------------------------------
*	This function sets the device in manual control mode
*
* 	@\param	value	two bit value to set the Integration time (Refer the datasheet for different configurations)
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int set_manualControl(uint8_t on);


/**
--------------------------------------------------------------------------------------------
read_channel0
--------------------------------------------------------------------------------------------
*	This function reads two byte of data of Channel0
*
* 	@\param	data	Obtains two byte value from lower and higher ADC register
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int read_channel0(uint16_t *data);


/**
--------------------------------------------------------------------------------------------
read_channel1
--------------------------------------------------------------------------------------------
*	This function reads two byte of data of Channel1
*
* 	@\param	data	Obtains two byte value from lower and higher ADC register
*
* 	@\return		Returns 0 on Success and -1 on Failure
***/
int read_channel1(uint16_t *data);


/**
--------------------------------------------------------------------------------------------
get_sensorlux
--------------------------------------------------------------------------------------------
*	This function returns the actuall sensor lux data obtained after conversion
*
* 	@\param	void
*
* 	@\return		Returns (> 0) on Success and -1 on Failure
***/
float get_sensorlux();

#endif