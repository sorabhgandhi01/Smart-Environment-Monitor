
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

int read_configuration_reg(uint16_t *data)
{
	uint16_t value = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&value, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	*data = value;
	return status;
}

int set_extendedMode()
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	//printf("Read data = %x\n", data);

	data |= ((uint16_t)EXTENDED_MODE_SET_BIT);

	status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, data, CONFIGURATION_REG);

	return status;
}

int set_defaultMode()
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	//printf("Read data = %x\n", data);

	data &= ~((uint16_t)EXTENDED_MODE_SET_BIT);

	status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, data, CONFIGURATION_REG);

	return status;
}

/*value = 1; for high
value = 0; for active low*/
int set_alert(uint8_t value)
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	//printf("Read data = %x\n", data);

	if (value) {
		data |= ((uint16_t)POLARITY_SET_BIT);
	} else {
		data &= ~((uint16_t)POLARITY_SET_BIT);
	}

	status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, data, CONFIGURATION_REG);

	return status;
}

int set_sdMode()
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	//printf("Read data = %x\n", data);

	data |= ((uint16_t)SHUTDOWN_MODE_SET_BIT);

	status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, data, CONFIGURATION_REG);

	return status;
}

int set_comparatorMode()
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	//printf("Read data = %x\n", data);

	data &= ~((uint16_t)THERMOSTAT_MODE_SET_BIT);

	status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, data, CONFIGURATION_REG);

	return status;	
}

int set_InterruptMode()
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	//printf("Read data = %x\n", data);

	data |= ((uint16_t)THERMOSTAT_MODE_SET_BIT);

	status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, data, CONFIGURATION_REG);

	return status;	
}

int set_operationFreq(uint8_t freq)
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	//printf("Read data = %x\n", data);

	data &= ~((uint16_t)(3<<14));

	if (freq == 1) {
		data |= ((uint16_t)(1<<14));
	}
	else if (freq == 2) {
		data |= ((uint16_t)(2<<14));
	}
	else if (freq == 3) {
		data |= ((uint16_t)(3<<14));
	}
	else {
		data |= ((uint16_t)(1<<14));
	}

	status = i2c_write_word(TEMPERATURE_SENSOR_ADDR, data, CONFIGURATION_REG);

	return status;	
}

int read_alertBit(uint8_t *value)
{
	uint16_t data = 0;

	int status = i2c_read_bytes(TEMPERATURE_SENSOR_ADDR, (uint8_t *)&data, CONFIGURATION_REG, sizeof(status));
	if (status == -1)
		return status;

	*value = ((data & ((uint16_t)CONFIGURATION_REG)) >> 13);
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

	MSB = buff[0];
	LSB = buff[1];

	data = ((MSB << 8) | LSB) >> 4;
	*temp = data*0.0625;

	return status;
}