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

    if (status != 0) {
        printf("Failed to open I2C Bus\n");
        return -1;
    }

    if ((sensor_enable()) != MRAA_SUCCESS) {

        printf("Failed to enable the sensor\n");
        return -1;
    }

    read_sensorID(&id);
    printf("ID = %x\n", id);
    if (0x50 == (id & 0xF0)) {
        printf("Sensor test passed\n");
    }

    // while (1)
    // {
    //     data = get_sensorlux();
    //     printf("Recieved data = %f\n", data);

    //     sleep(3);
    // }

    if (set_manualControl(1) != 0) {
        printf("Failed to write to timer register\n");
    }

    i2c_read(LIGHT_SENSOR_ADDR, &time_data, (TIMING_REG | COMMAND_REG));
    printf("Read %d from timing reg\n", time_data);

    if (set_integrationTime(0) != 0) {
        printf("Failed to write to timer register\n");
    }

    read_timer_reg(&time_data);
    printf("Read %d from timing reg\n", time_data);

    if (set_integrationTime(1) != 0) {
        printf("Failed to write to timer register\n");
    }

    read_timer_reg(&time_data);
    printf("Read %d from timing reg\n", time_data);

    if (set_integrationTime(2) != 0) {
        printf("Failed to write to timer register\n");
    }

    read_timer_reg(&time_data);
    printf("Read %d from timing reg\n", time_data);

    if (set_integrationTime(3) != 0) {
        printf("Failed to write to timer register\n");
    }

    read_timer_reg(&time_data);
    printf("Read %d from timing reg\n", time_data);

    if (set_manualControl(0) != 0) {
        printf("Failed to write to timer register\n");
    }

    i2c_read(LIGHT_SENSOR_ADDR, &time_data, (TIMING_REG | COMMAND_REG));
    printf("Read %d from timing regx\n", time_data);

    if ((sensor_disable()) != MRAA_SUCCESS) {
     printf("Failed to diable the sensor");
    }

    if (i2c_close() != 0) {
     printf("Failed to close I2C Bus\n");
     return -1;
    }

    return 0;
}
