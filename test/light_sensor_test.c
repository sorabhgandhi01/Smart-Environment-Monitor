#include "light_sensor.h"
#include "i2c_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void signal_handler(int signum)
{
    sensor_disable();
    i2c_close();
}


int main()
{
    signal(SIGINT, signal_handler);
    float data;
    int status;
    uint8_t id;
    uint8_t time_data;

    status = i2c_open();
    assert(status == 0);

    /*if (status != 0) {
        printf("Failed to open I2C Bus\n");
        return -1;
    }*/

    status = sensor_enable();
    assert(status == 0);

    /*if ((sensor_enable()) != MRAA_SUCCESS) {

        printf("Failed to enable the sensor\n");
        return -1;
    }*/

    read_sensorID(&id);
    assert(id == 0x50);
    
    /*printf("ID = %x\n", id);
    if (0x50 == (id & 0xF0)) {
        printf("Sensor test passed\n");
    }*/

    status = set_manualControl(1);
    assert(status == 0);

    /*if (set_manualControl(1) != 0) {
        printf("Failed to write to timer register\n");
    }*/

    //i2c_read(LIGHT_SENSOR_ADDR, &time_data, (TIMING_REG | COMMAND_REG));
    //printf("Read %d from timing reg\n", time_data);

    read_timer_reg(&time_data);
    assert(time_data == 11);

    status = set_integrationTime(0);
    assert(status == 0);

    /*if (set_integrationTime(0) != 0) {
        printf("Failed to write to timer register\n");
    }*/

    read_timer_reg(&time_data);
    assert(time_data == 8);
    //printf("Read %d from timing reg\n", time_data);

    status = set_integrationTime(1);
    assert(status == 0);

    /*if (set_integrationTime(1) != 0) {
        printf("Failed to write to timer register\n");
    }*/

    read_timer_reg(&time_data);
    assert(time_data == 9);
    //printf("Read %d from timing reg\n", time_data);

    status = set_integrationTime(2);
    assert(status == 0);

    /*if (set_integrationTime(2) != 0) {
        printf("Failed to write to timer register\n");
    }*/

    read_timer_reg(&time_data);
    assert(time_data == 10);
    //printf("Read %d from timing reg\n", time_data);

    status = set_integrationTime(3);
    assert(status == 0);

    /*if (set_integrationTime(3) != 0) {
        printf("Failed to write to timer register\n");
    }*/

    read_timer_reg(&time_data);
    assert(time_data == 11);
    //printf("Read %d from timing reg\n", time_data);

    status = set_manualControl(0);
    assert(status == 0);

    /*if (set_manualControl(0) != 0) {
        printf("Failed to write to timer register\n");
    }*/


    read_timer_reg(&time_data);
    assert(time_data == 3);

    //i2c_read(LIGHT_SENSOR_ADDR, &time_data, (TIMING_REG | COMMAND_REG));
    //printf("Read %d from timing reg\n", time_data);

    status = sensor_disable();
    assert(status == 0);

    status = i2c_close();
    assert(status == 0);

    // if ((sensor_disable()) != MRAA_SUCCESS) {
    //  printf("Failed to diable the sensor");
    // }

    // if (i2c_close() != 0) {
    //  printf("Failed to close I2C Bus\n");
    //  return -1;
    // }

    return 0;
}
