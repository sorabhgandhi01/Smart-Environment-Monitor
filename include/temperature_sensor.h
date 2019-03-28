
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

int write_tlow_reg(uint16_t temp);

int read_tlow_reg(uint16_t *temp);

int write_thigh_reg(uint16_t temp);

int read_thigh_reg(uint16_t *temp);

int get_sensortemp(float *temp);

#endif
