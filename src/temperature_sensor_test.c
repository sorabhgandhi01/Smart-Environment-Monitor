#include <unistd.h>
#include <stdint.h>
#include "temperature_sensor.h"

#include <assert.h>

int main()
{
	int status;
	uint16_t write_data = 0;
	uint16_t read_data = 0;

	status = i2c_open();
	assert(status == 0);

	write_data = 75;
	status = write_tlow_reg(write_data);
	assert(status == 0);

	status = read_tlow_reg(&read_data);
	assert(status == 0);
	assert(write_data == read_data);

	write_data = 80;
	status = write_thigh_reg(write_data);
	assert(status == 0);

	status = read_thigh_reg(&read_data);
	assert(status == 0);
	assert(write_data == read_data);

	status = set_extendedMode();
	assert(status == 0);

	status = i2c_close();
	assert(status == 0);

	return 0;
}