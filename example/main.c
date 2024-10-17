#include "icm20948_i2c.h"
#include <stdio.h>
#include <stdlib.h>

void printICM_data(ICM20948* icm);

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

	ICM20948_Init(icm1);
	ICM20948_RAW_DATA_RDY_INT_enable(icm1);
	
	for(int i = 0; i < 1000; i++)
	{
		ICM20948_read_data(icm1);
		printICM_data(icm1);
	}

	return 1;
}

void printICM_data(ICM20948* icm)
{
	float gx = icm->angle_x;
	float gy = icm->angle_y;
	float gz = icm->angle_z;

	float ax = icm->accel_x;
	float ay = icm->accel_y;
	float az = icm->accel_z;

	float t = icm->temp;

	printf("GYRO: x %f; y %f; z %f;\t\t ACCEL: x %f; y %f; z %f;\t\t TEMP: %f C\n",
		gx, gy, gz,
		ax, ay, az,
		t
	);
}