#include "led.h"

void user_led(char *gpio_number,char *gpio_direction,char *direction,char *gpio_path, int status)
{
  FILE *export_file = NULL; //declare pointers
  FILE *IO_direction = NULL;
  FILE *IO_value = NULL;  

  export_file = fopen ("/sys/class/gpio/export", "w");
  fwrite (gpio_number, 1, sizeof(gpio_number), export_file);
  fclose (export_file);
  //this part here sets the direction of the pin
  IO_direction = fopen(gpio_direction, "w");
  fwrite(direction, 1, sizeof(direction), IO_direction); //set the pin to HIGH
  fclose(IO_direction);
 
  if(status == 1)
  {
    IO_value = fopen (gpio_path, "w");
    fwrite (ON, 1, sizeof(ON), IO_value); //set the pin to HIGH
    fclose (IO_value);
  }

  if(status == 0)
  {
    IO_value = fopen (gpio_path, "w");
    fwrite (OFF, 1, sizeof(OFF), IO_value); //set the pin to LOW
    fclose (IO_value);
  }

}
