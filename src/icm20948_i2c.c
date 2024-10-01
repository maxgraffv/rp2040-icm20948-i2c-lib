#include "icm20948_i2c.h"
#include "stdio.h"

void imu_selectBank( IMU* imu ,UserBank bank )
{
	uint8_t imu_i2c_address = imu->i2c_address;
	uint8_t bank_select_register_addr = 127;
	uint8_t bank_selected = 0x00;

	i2c_write_blocking( i2c0, imu_i2c_address, &bank_select_register_addr, 1, 1 );

	switch (bank)
	{
		case Bank0 : 
			bank_selected = 0x00;
			break;
		case Bank1 : 
			bank_selected = 0b00010000;
			break;
		case Bank2 : 
			bank_selected = 0b00100000;
			break;
		case Bank3 : 
			bank_selected = 0b00110000;
			break;
		default:
			bank_selected = 0x00;
			break;
	}

	i2c_write_blocking( i2c0, imu_i2c_address, &bank_selected, 1, 0 );

}

int imu_who_am_i_check(IMU* imu)
{
	uint8_t data_read;
	imu_selectBank(imu, Bank0);
	i2c_write_blocking( i2c0, imu->i2c_address, WHO_AM_I , 1, 1 );
	i2c_read_blocking( i2c0, imu->i2c_address, &data_read, 1, 0 );
	
	if(data_read == imu->who_am_i_val)
		return 1;
	else
		return 0;
}



float imu_get_gyro_x_deg(IMU* imu)
{
	uint8_t GYRO_XOUT_H_reg = GYRO_XOUT_H;
	uint8_t GYRO_XOUT_L_reg = GYRO_XOUT_L;
	uint8_t gyro_x_H = 0x00;
	uint8_t gyro_x_L = 0x00;
	uint16_t gyro_x_raw = 0x00;
	float x_deg = 0x00;

    
	float gyro_sensitivity = 131;
	
	i2c_write_blocking( i2c0, imu->i2c_address, &GYRO_XOUT_H_reg , 1, 1 );
	i2c_read_blocking( i2c0, imu->i2c_address, &gyro_x_H, 1, 1 );
	i2c_write_blocking( i2c0, imu->i2c_address, &GYRO_XOUT_L_reg, 1, 1 );
	i2c_read_blocking( i2c0, imu->i2c_address, &gyro_x_L, 1, 0 );
	printf("GYRO L: %d\n", gyro_x_H);
	printf("GYRO H: %d\n", gyro_x_L);

	gyro_x_raw |= (gyro_x_H<<8);
	gyro_x_raw |= gyro_x_L;
	printf("GYRO RAW: %d\n", gyro_x_raw);

	x_deg = ((float)gyro_x_raw)/gyro_sensitivity;

	return x_deg;
}