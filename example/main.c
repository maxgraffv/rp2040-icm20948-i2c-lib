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

	uint8_t who_am_i_addr = WHO_AM_I;
	uint8_t who_am_i_read = 0x00;
 
	who_am_i_read = ICM20948_who_am_i_check(icm1);

	if(who_am_i_read)
		printf("WHO AM I OK\n");
	else
		printf("FUCK YOU, You piece of shit = %d\n", who_am_i_read);

	uint8_t who_am_i_value = ICM20948_get_who_am_i(icm1);
	printf("who VALUE = %d\n", who_am_i_value);

	ICM20948_defaultInit(icm1);


	// if(ICM20948_isSleepMode(icm1))
	// 	printf("ICM asleep\n");
	// else
	// 	printf("ICM awake\n");

	// ICM20948_Sleep_enable(icm1, 0);
	
	// if(ICM20948_isSleepMode(icm1))
	// 	printf("ICM asleep\n");
	// else
	// 	printf("ICM awake\n");

}
