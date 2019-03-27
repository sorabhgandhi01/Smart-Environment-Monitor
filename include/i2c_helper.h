
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "mraa/i2c.h"

#define MY_I2C_BUS 2

mraa_i2c_context i2c_context;
pthread_mutex_t i2c_bus_lock;

int i2c_open();

int i2c_close();

int i2c_write(uint8_t slave_addr, uint8_t data);

int i2c_write_byte(uint8_t slave_addr, uint8_t data, uint8_t command);

int i2c_write_word(uint8_t slave_addr, uint16_t data, uint8_t command);

int i2c_read(uint8_t slave_addr, uint8_t *data, uint8_t command);

int i2c_read_bytes(uint8_t slave_addr, uint8_t *data, uint8_t command, size_t len);
