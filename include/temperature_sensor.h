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


int write_tlow_reg(uint16_t temp);

int read_tlow_reg(uint16_t *temp);

int write_thigh_reg(uint16_t temp);

int read_thigh_reg(uint16_t *temp);

int read_configuration_reg(uint16_t *data);

int set_alert(uint8_t value);

int set_defaultMode();

int set_extendedMode();

int set_sdMode();

int set_comparatorMode();

int set_InterruptMode();

int set_operationFreq(uint8_t freq);

int read_alertBit(uint8_t *value);

int get_sensortemp(float *temp);

#endif
