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

	ICM20948_Init(icm1);

	float gx = 0;
	float gy = 0;
	float gz = 0;

	float ax = 0;
	float ay = 0;
	float az = 0;

	float t = 0;

	for(int i = 0; i < 100; i++)
	{
		gx = ICM20948_GYRO_raw_to_dps(icm1, ICM20948_get_GYRO_X_raw(icm1));
		gy = ICM20948_GYRO_raw_to_dps(icm1, ICM20948_get_GYRO_Y_raw(icm1));
		gz = ICM20948_GYRO_raw_to_dps(icm1, ICM20948_get_GYRO_Z_raw(icm1));

		ax = ICM20948_ACCEL_raw_to_g(icm1, ICM20948_get_ACCEL_X_raw(icm1));
		ay = ICM20948_ACCEL_raw_to_g(icm1, ICM20948_get_ACCEL_Y_raw(icm1));
		az = ICM20948_ACCEL_raw_to_g(icm1, ICM20948_get_ACCEL_Z_raw(icm1));

		printf("GYRO: x %f; y %f; z %f;\t\t ACCEL: x %f; y %f; z %f;\t\t TEMP: %f C\n",
		 	gx, gy, gz,
			ax, ay, az,
			ICM20948_get_TEMP_C(icm1)
		);
		sleep_ms(1000);
	}

	return 1;
}
