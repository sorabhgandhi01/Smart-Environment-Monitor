# _*_ Makefile _*_

CC = arm-linux-gcc

CFLAGS = -Iinclude/

vpath %.h include/
vpath %.c src/

OBJ = main_task.o temp_task.o light_task.o socket_task.o logger_task.o i2c_helper.o temperature_sensor.o light_sensor.o led.o

main_task: $(OBJ)
	$(CC) $(CFLAGS) -o main_task $(OBJ) -lpthread -g -lrt -lmraa -lm

main_task.o: src/main_task.c
	$(CC) -c src/main_task.c $(CFLAGS)


temp_task.o: src/temp_task.c
	$(CC) -c src/temp_task.c $(CFLAGS) -lmraa


light_task.o: src/light_task.c
	$(CC) -c src/light_task.c $(CFLAGS) -lmraa


socket_task.o: src/socket_task.c
	$(CC) -c src/socket_task.c $(CFLAGS)


logger_task.o: src/logger_task.c
	$(CC) -c src/logger_task.c $(CFLAGS)

i2c_helper.o: src/i2c_helper.c
	$(CC) -c src/i2c_helper.c $(CFLAGS) -lmraa


temperature_sensor.o: src/temperature_sensor.c
	$(CC) -c src/temperature_sensor.c $(CFLAGS) -lmraa

light_sensor.o: src/light_sensor.c
	$(CC) -c src/light_sensor.c $(CFLAGS) -lmraa

led.o: src/led.c
	$(CC) -c src/led.c $(CFLAGS)


clean:
	rm *.o main_task *.txt
