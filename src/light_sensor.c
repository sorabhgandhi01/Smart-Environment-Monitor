
#include "light_sensor.h"
#include <stdint.h>
#include <math.h>


int sensor_enable()
{
    int status;
    status = i2c_write_byte(LIGHT_SENSOR_ADDR, POWER_ON_BIT, (CONTROL_REG | COMMAND_REG));

    return status;
}

int sensor_disable()
{
    int status = i2c_write_byte(LIGHT_SENSOR_ADDR, POWER_OFF_BIT, (CONTROL_REG | COMMAND_REG));
    return status;
}

int read_sensorID(uint8_t *id)
{
    int status = i2c_read(LIGHT_SENSOR_ADDR, id, (DATA0LOW_REG | COMMAND_REG));

    return status;
}

int read_channel0(uint16_t *data)
{
    uint8_t ch0_MSB, ch0_LSB;

    int status = i2c_read(LIGHT_SENSOR_ADDR, &ch0_LSB, (DATA0LOW_REG | COMMAND_REG));
    if (status == -1) {
        printf("Failed to read DATA0LOW_REG\n");
        return status;
    }

    status = i2c_read(LIGHT_SENSOR_ADDR, &ch0_MSB, (DATA0HIGH_REG | COMMAND_REG));
    if (status == -1) {
        printf("Failed to read DATA1HIGH_REG\n");
        return status;
    }
    
    printf("MSB and LSB = %d    %d\n", ch0_MSB, ch0_LSB);

    *data = (ch0_MSB << 8) | ch0_LSB;

    return status;
}

int read_channel1(uint16_t *data)
{
    uint8_t ch1_MSB, ch1_LSB;

    int status = i2c_read(LIGHT_SENSOR_ADDR, &ch1_LSB, (DATA1LOW_REG | COMMAND_REG));
    if (status == -1) {
        printf("Failed to read DATA1LOW_REG\n");
        return status;
    }

    status = i2c_read(LIGHT_SENSOR_ADDR, &ch1_MSB, (DATA1HIGH_REG | COMMAND_REG));
    if (status == -1) {
        printf("Failed to read DATA1HIGH_REG\n");
        return status;
    }
    
    printf("MSB and LSB = %d    %d\n", ch1_MSB, ch1_LSB);

    *data = (ch1_MSB << 8) | ch1_LSB;

    return status;
}

float get_sensorlux()
{
    uint16_t CH0, CH1;
    float div;
    float Sensor_Lux = -1;

    int status = read_channel0(&CH0);
    if (status == -1)
        return Sensor_Lux;

    status = read_channel1(&CH1);
    if (status == -1)
        return Sensor_Lux;

    if (CH0 != 0)
        div = (float)CH1 / (float)CH0;
    else
        div = 0;

    printf("Ch0 = %f    ch1 = %f    div = %f\n", CH0, CH1, div);

        if ((div > 0) && (div <= 0.50)) {
            Sensor_Lux = (0.0304 * CH0) - (0.062 * CH0 * powf(div, 1.4));
            printf("In S1   Sensor Value = %f\n", Sensor_Lux);
        }
        else if ((div > 0.50) && (div <= 0.61)) {
            Sensor_Lux = (0.0224 * CH0) - (0.031 * CH1);
            printf("In S2   Sensor Value = %f\n", Sensor_Lux);
        }
        else if ((div > 0.61) && (div <= 0.80)) {
            Sensor_Lux = (0.0128 * CH0) - (0.0153 * CH1);
            printf("In S3   Sensor Value = %f\n", Sensor_Lux);
        }
        else if ((div > 0.80) && (div <= 1.30)) {
            Sensor_Lux = (0.00146 * CH0) - (0.00112 * CH1);
            printf("In S4   Sensor Value = %f\n", Sensor_Lux);
        }
        else if (div > 1.30) {
            Sensor_Lux = 0;
            printf("In S5   Sensor Value = %f\n", Sensor_Lux);

        }
        else {
            printf("Undefined\n");
        }

    return Sensor_Lux;
}
