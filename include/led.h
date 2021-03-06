/*@filename     : led.h
 * @author      : Om Raheja & Sorabh Gandhi
 * @brief       : contains macros for led on, led off, gpio path, gpio direction for all threads/tasks 
 * @date        : 31st March 2019
 * */

#ifndef LED_H
#define LED_H

/*******************************
 * STANDARD C LIBRARAY HEADERS**
 * *****************************/
#include <unistd.h>
#include <stdio.h> 

/******************************
 * MACROS & DEFINITIONS       *
 * ****************************/
#define OFF "0"
#define ON  "1"
#define IO_DIRECTION  "out"

#define TEMP_SENSOR   "53"
#define LIGHT_SENSOR  "54"
#define SOCKET_THREAD "55"
#define LOGGER_THREAD "56"

#define TEMP_GPIO_PATH "/sys/class/gpio/gpio53/value"
#define TEMP_GPIO_DIRECTION "/sys/class/gpio/gpio53/direction"

#define LIGHT_GPIO_PATH "/sys/class/gpio/gpio54/value"
#define LIGHT_GPIO_DIRECTION "/sys/class/gpio/gpio54/direction"

#define SOCKET_GPIO_PATH "/sys/class/gpio/gpio55/value"
#define SOCKET_GPIO_DIRECTION "/sys/class/gpio/gpio55/direction"

#define LOGGER_GPIO_PATH "/sys/class/gpio/gpio56/value"
#define LOGGER_GPIO_DIRECTION "/sys/class/gpio/gpio56/direction"

#define TEMP_ERROR_LED_ON()		user_led(TEMP_SENSOR,TEMP_GPIO_DIRECTION,IO_DIRECTION,TEMP_GPIO_PATH,1)
#define TEMP_ERROR_LED_OFF()		user_led(TEMP_SENSOR,TEMP_GPIO_DIRECTION,IO_DIRECTION,TEMP_GPIO_PATH,0)

#define LIGHT_ERROR_LED_ON()		user_led(LIGHT_SENSOR,LIGHT_GPIO_DIRECTION,IO_DIRECTION,LIGHT_GPIO_PATH,1)
#define LIGHT_ERROR_LED_OFF()		user_led(LIGHT_SENSOR,LIGHT_GPIO_DIRECTION,IO_DIRECTION,LIGHT_GPIO_PATH,0)

#define SOCKET_ERROR_LED_ON()		user_led(SOCKET_THREAD,SOCKET_GPIO_DIRECTION,IO_DIRECTION,SOCKET_GPIO_PATH,1)
#define SOCKET_ERROR_LED_OFF()		user_led(SOCKET_THREAD,SOCKET_GPIO_DIRECTION,IO_DIRECTION,SOCKET_GPIO_PATH,0)

#define LOGGER_ERROR_LED_ON()		user_led(LOGGER_THREAD,LOGGER_GPIO_DIRECTION,IO_DIRECTION,LOGGER_GPIO_PATH,1)
#define LOGGER_ERROR_LED_OFF()		user_led(LOGGER_THREAD,LOGGER_GPIO_DIRECTION,IO_DIRECTION,LOGGER_GPIO_PATH,0)


/******************************
 * FUNCTION PROTOTYPE         *
 * ****************************/
/*@brief: Turns ON or OFF the LED depending upon the status
 *
 *@param: param1: gpio pin number
        : param2: gpio pin path
 *      : param3: direction of gpio (in/out)
 *      : param4: gpio pin path value
 *      : param5: status for LED on/off
 *
 *@return: No return value
 * */

void user_led(char *gpio_number,char *gpio_direction,char *direction,char *gpio_path, int status);

#endif
