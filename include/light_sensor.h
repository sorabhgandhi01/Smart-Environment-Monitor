#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>

//#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "i2c_helper.h"

#define LIGHT_SENSOR_ADDR (0x39)

//Defining the register set of APDS-9301
#define COMMAND_REG (0x80)            //Specifies register address
#define CONTROL_REG (0x00)            //Control of basic functions
#define TIMING_REG (0x10)             //Integration time/gain control
#define THRESHLOWLOW_REG (0x20)       //Low byte of low interrupt threshold
#define THRESHLOWHIGH_REG (0x30)      //High byte of low interrupt threshold
#define THRESHHIGHLOW_REG (0x40)      //Low byte of high interrupt threshold
#define THRESHHIGHHIGH_REG (0x50)     //High byte of high interrupt threshold
#define INTERRUPT_REG (0x60)          //Interrupt control
#define CRC_REG (0x80)                //Factory test — not a user register
#define ID_REG (0xA0)                 //Part number/ Rev ID
#define DATA0LOW_REG (0xC0)           //Low byte of ADC channel 0
#define DATA0HIGH_REG (0xD0)          //High byte of ADC channel 0
#define DATA1LOW_REG (0xE0)           //Low byte of ADC channel 1
#define DATA1HIGH_REG (0xF0)          //High byte of ADC channel 1

#define POWER_ON_BIT (0x03)
#define POWER_OFF_BIT (0x00)

//Write to Control register
int sensor_enable();

int sensor_disable();

int read_sensorID(uint8_t id);

int read_channel0(uint16_t data);

int read_channel1(uint16_t data);

float get_sensorlux();
