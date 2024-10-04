#include "icm20948_i2c.h"
#include "stdio.h"

uint8_t ICM20948_selectBank( ICM20948* icm, UserBank bank )
{
	uint8_t icm_i2c_address = icm->i2c_address;
	uint8_t bank_select_register_addr = 127;
	uint8_t bank_selected = 0x00;


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

	i2c_write_blocking( icm->i2c_ptr, icm->i2c_address, &bank_select_register_addr, 1, 1 );
	i2c_write_blocking( icm->i2c_ptr, icm->i2c_address, &bank_selected, 1, 0 );

	return 1;
}

uint8_t ICM20948_get_register(ICM20948* icm, UserBank bank, uint8_t reg_addr)
{
	uint8_t reg_val = 0x00;
	ICM20948_selectBank(icm, bank);
	i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, reg_addr, 1, 1);
	i2c_read_blocking(icm->i2c_ptr, icm->i2c_address, &reg_val, 1, 0);

	return reg_val;
}

uint8_t ICM20948_set_register(ICM20948* icm, UserBank bank, uint8_t reg_addr, uint8_t value)
{
	ICM20948_selectBank(icm, bank);
	i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, reg_addr, 1, 1);
	i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, &value, 1, 0);

	return 1;
}

ICM20948* createICM20948( i2c_inst_t* i2c_chosen_ptr, uint8_t addr_pin_high )
{
	ICM20948* icm_ptr = (ICM20948*)malloc(sizeof(ICM20948));

	icm_ptr->i2c_ptr = i2c_chosen_ptr;
	icm_ptr->who_am_i_val = WHO_AM_I;
	if(addr_pin_high)
		icm_ptr->i2c_address = 0b1101001;
	else
		icm_ptr->i2c_address = 0b1101000;
}

uint8_t ICM20948_get_who_am_i(ICM20948* icm);
{
	uint8_t data_read = ICM20948_get_register(icm, Bank0, WHO_AM_I);
	
	return data_read;
}

uint8_t ICM20948_who_am_i_check(ICM20948* icm)
{
	uint8_t who_am_i_val_read = ICM20948_get_who_am_i(icm);
	uint8_t who_am_i_val_isOK = 0x00;
	if(who_am_i_val_read == icm->who_am_i_val)
		who_am_i_val_OK = 1;

	return who_am_i_val_isOK;
}

uint8_t ICM20948_defaultInit(ICM20948* icm)
{


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