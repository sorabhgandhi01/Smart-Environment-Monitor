#include <unistd.h>
#include "temperature_sensor.h"


int main()
{
	float data = 0;

	i2c_open();

	while (1)
	{

		get_sensortemp(&data);
		printf("Temp Date = %f\n", data);

		sleep(5);
	}


	i2c_close();

	return 0;
}