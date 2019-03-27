#ifndef _LIGHT_SENSOR_H_
#define _LIGHT_SENSOR_H_

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

//Write to Control register
int sensor_enable();

int sensor_disable();

int read_sensorID(uint8_t *id);

int read_channel0(uint16_t *data);

int read_channel1(uint16_t *data);

float get_sensorlux();

#endif
