#include "icm20948_i2c.h"
#include <stdio.h>

void printICM_data(ICM20948* icm);

void printExactGyroAngle(ICM20948* icm);

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

	printExactGyroAngle(icm1);

	

	return 1;
}

void printExactGyroAngle(ICM20948* icm)
{

	ICM20948_Init(icm);
	ICM20948_ODR_ALIGN_enable(icm);
	ICM20948_set_GYRO_SAMPLE_RATE_DIV(icm, (uint8_t)16);
	ICM20948_set_ACCEL_SAMPLE_RATE_DIV(icm, 255);

	float gyro_data_ms = 0;
	float accel_data_ms = 0;

	for(int i = 0; i < 2; i++)
	{
		gyro_data_ms = (ICM20948_get_GYRO_ODR_kHz(icm)*1000);
		accel_data_ms = (ICM20948_get_ACCEL_ODR_kHz(icm)*1000);

		printf("GYRO: %f ms; ACCEL: %f ms \n", gyro_data_ms, accel_data_ms);

	}










}





void printICM_data(ICM20948* icm)
{
	ICM20948_Init(icm);
	float gx = 0;
	float gy = 0;
	float gz = 0;

	float ax = 0;
	float ay = 0;
	float az = 0;

	float t = 0;

	for(int i = 0; i < 100; i++)
	{
		gx = ICM20948_GYRO_raw_to_dps(icm, ICM20948_get_GYRO_X_raw(icm));
		gy = ICM20948_GYRO_raw_to_dps(icm, ICM20948_get_GYRO_Y_raw(icm));
		gz = ICM20948_GYRO_raw_to_dps(icm, ICM20948_get_GYRO_Z_raw(icm));

		ax = ICM20948_ACCEL_raw_to_g(icm, ICM20948_get_ACCEL_X_raw(icm));
		ay = ICM20948_ACCEL_raw_to_g(icm, ICM20948_get_ACCEL_Y_raw(icm));
		az = ICM20948_ACCEL_raw_to_g(icm, ICM20948_get_ACCEL_Z_raw(icm));

		printf("GYRO: x %f; y %f; z %f;\t\t ACCEL: x %f; y %f; z %f;\t\t TEMP: %f C\n",
		 	gx, gy, gz,
			ax, ay, az,
			ICM20948_get_TEMP_C(icm)
		);
		sleep_ms(1000);
	}




}