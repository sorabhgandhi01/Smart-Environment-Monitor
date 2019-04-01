                **SMART ENVIRONMENT MONITORING DEVICE**

INTRODUCTION
------------
A Multithreaded Application for BeagleBone Green (Linux) using two offboard sensors (Temperature sensor & Light sensor).
The project consists of 5 threads in total. There is 1 parent thread which is the main task. The main task spawns 4 threads, namely Temperature Thread, Light Thread, Socket Thread and Logger Thread.

Main Thread: This thread/task is responsible to keep a check on all of its child threads(ALIVE/DEAD).If the main thread dies or is terminated, it gracefully closes all the threads and terminates the program. 
Temperature Thread: This thread/task is responsible for communicating with the TMP102 temperature sensor.
Light Thread: This thread/task is responsible for communicating with the APDS9301 light sensor.
Socket Thread: This thread/task is responsible for connecting with remote clients in order to provide them with Temperature & Lux data as and when requested by the client.
Logger Thread: This thread/task is responsible for logging all the data/error messages/debug messages/information messages to a log file.


PROJECT FOLDER
--------------
|
|- doc
.....|- .keep
.....|- Project Architecture.jpg
|- include
.....|- .keep
.....|- i2c_helper.h
.....|- led.h
.....|- light_sensor.h
.....|- light_task.h
.....|- logger_task.h
.....|- main_task.h
.....|- socket_task.h
.....|- temp_task.h
.....|- temperature_sensor.h
|- src
.....|- i2c_helper.c
.....|- led.c
.....|- light_sensor.c
.....|- light_task.c
.....|- logger_task.c
.....|- main_task.c
.....|- socket_task.c
.....|- temp_task.c
.....|- temperature_sensor.c
|- test
.....|- .keep
.....|- light_sensor_test.c
.....|- temperature_sensor_test.c
|- Makefile
|- README.md

BUILD AND RUN STEPS
-------------------
    TBD


