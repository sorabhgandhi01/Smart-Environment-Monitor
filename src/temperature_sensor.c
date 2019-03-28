
#include "temperature_sensor.h"

int write_tlow_reg(uint16_t temp)
{

    if ((temp < -55) || (temp > 150))
        temp = 75;

    int status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, temp, TLOW_REG);

    return status;
}

int write_thigh_reg(uint16_t temp)
{   
    if ((temp <= -55) || (temp >= 150))
        temp = 80;

    int status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, temp, THIGH_REG);

    return status;
}

int read_tlow_reg(uint16_t *temp)
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, TLOW_REG, sizeof(status));
	if (status == -1)
		return status;

	*temp = data;

	return status;
}

int read_thigh_reg(uint16_t *temp)
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, THIGH_REG, sizeof(status));
	if (status == -1)
		return status;

	*temp = data;
	return status;
}

int get_sensortemp(float *temp)
{
	uint8_t buff[2] = {0};
	unsigned char MSB, LSB;
	int data;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, buff, TEMPERATURE_REG, sizeof(buff));
	if (status == -1)
		return status;

	data = ((MSB << 8) | LSB) >> 4;
	*temp = data*0.0625;

	return status;
}