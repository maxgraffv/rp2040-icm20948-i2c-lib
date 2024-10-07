#include "icm20948_i2c.h"
#include <stdio.h>
void blink()
{
	gpio_put(25, 1);
	sleep_ms(300);
	gpio_put(25, 0);
	sleep_ms(300);
}






int main()
{
	stdio_init_all();
    int pin = 25;
    gpio_init(pin);
	gpio_set_dir(pin, 1);
	sleep_ms(3000);
    printf("gpio: %d\n", pin );


	/*
		I2C Initialization
	*/
    i2c_init(i2c0, 100000);
	gpio_set_function(4, GPIO_FUNC_I2C);
	gpio_set_function(5, GPIO_FUNC_I2C);
	gpio_pull_up(4);
	gpio_pull_up(5);


    ICM20948* icm1 = createICM20948(i2c0, 1);
	printf("ICM created\n");

	ICM20948_defaultInit(icm1);

}
