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

    status = sensor_enable();
    assert(status == 0);

    read_sensorID(&id);
    assert(id == 0x50);

    status = set_manualControl(1);
    assert(status == 0);

    read_timer_reg(&time_data);
    assert(time_data == 11);

    status = set_integrationTime(0);
    assert(status == 0);

    read_timer_reg(&time_data);
    assert(time_data == 8);

    status = set_integrationTime(1);
    assert(status == 0);

    read_timer_reg(&time_data);
    assert(time_data == 9);

    status = set_integrationTime(2);
    assert(status == 0);

    read_timer_reg(&time_data);
    assert(time_data == 10);

    status = set_integrationTime(3);
    assert(status == 0);

    read_timer_reg(&time_data);
    assert(time_data == 11);

    status = set_manualControl(0);
    assert(status == 0);

    read_timer_reg(&time_data);
    assert(time_data == 3);

    status = sensor_disable();
    assert(status == 0);

    status = i2c_close();
    assert(status == 0);

    return 0;
}
