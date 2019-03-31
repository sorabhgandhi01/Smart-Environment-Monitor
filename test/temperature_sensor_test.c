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

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0xb063);
	//printf("Read data after set EM mode = %x\n", read_data);

	status = set_sdMode();
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0xb063);
	//printf("Read data after set SD mode = %x\n", read_data);

	status = set_comparatorMode();
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0xb061);
	//printf("Read data after set CM mode = %x\n", read_data);

	status = set_InterruptMode();
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0xb063);
	//printf("Read data after set IM mode = %x\n", read_data);

	status = set_defaultMode();
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0xa063);
	//printf("Read data after set DM mode = %x\n", read_data);

	status = set_operationFreq(0);
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0x6063);
	//printf("Read data after set 0.25Hz = %x\n", read_data);

	status = set_operationFreq(1);
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0x6063);
	//printf("Read data after set 1Hz = %x\n", read_data);

	status = set_operationFreq(2);
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0xa063);
	//printf("Read data after set 4Hz = %x\n", read_data);

	status = set_operationFreq(3);
	assert(status == 0);

	status = read_configuration_reg(&read_data);
	assert(status == 0);
	assert(read_data == 0xe063);
	//printf("Read data after set 8Hz = %x\n", read_data);

	status = set_operationFreq(2);
	assert(status == 0);

	status = set_defaultMode();
	assert(status == 0);

	status = i2c_close();
	assert(status == 0);

	return 0;
}