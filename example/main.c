#include "icm20948_i2c.h"
#include <stdio.h>

int main()
{
    int pin = 25;
    gpio_init(pin);
    printf("gpio: %d", pin );



	/*
		I2C Initialization
	*/
    i2c_init(i2c0, 100000);
	gpio_set_function(4, GPIO_FUNC_I2C);
	gpio_set_function(5, GPIO_FUNC_I2C);
	gpio_pull_up(4);
	gpio_pull_up(5);




}
