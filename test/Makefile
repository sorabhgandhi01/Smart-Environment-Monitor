# _*_ Makefile _*_

CC = arm-linux-gcc

CFLAGS = -I../include/

vpath %.h ../include/
vpath %.c ../src/

light_test: light_sensor_test.c
	$(CC) light_sensor_test.c ../src/light_sensor.c ../src/i2c_helper.c -o light_test -lmraa -lm $(CFLAGS)

temp_test: temperature_sensor_test.c
	$(CC) temperature_sensor_test.c ../src/temperature_sensor.c ../src/i2c_helper.c -o temp_test -lmraa $(CFLAGS)

logger_test: logger_task_test.c
	$(CC) logger_task_test.c -o logger_test $(CFLAGS)
