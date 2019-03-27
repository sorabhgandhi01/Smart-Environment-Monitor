
#include "light_sensor.h"
#include "i2c_helper.h"


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

    status = i2c_open();

    if (status != 0) {
        printf("Failed to open I2C Bus\n");
        return -1;
    }

    if ((sensor_enable()) != MRAA_SUCCESS) {

        printf("Failed to enable the sensor\n");
        return -1;
    }

    while (1)
    {

    data = get_sensorlux();
    printf("Recieved data = %f\n", data);

   /* if ((sensor_disable()) != MRAA_SUCCESS) {

        printf("Failed to close the sensor\n");
        return -1;
    }

    status = i2c_close();

    if (status != 0) {
        printf("Failed to close I2C Bus\n");
        return -1;
    }*/

    sleep(5);

    }

    return 0;
}
