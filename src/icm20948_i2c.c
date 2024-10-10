#include "icm20948_i2c.h"
#include "stdio.h"
#include <stdlib.h>

uint8_t ICM20948_selectBank( ICM20948* icm, UserBank bank )
{
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

	uint8_t data[] = {bank_select_register_addr, bank_selected};
    int result = i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, data, sizeof(data), 0);

    // Return success (1) or failure (0) based on the I2C operation result
    return (result == sizeof(data)) ? 1 : 0;
}

uint8_t ICM20948_get_register(ICM20948* icm, UserBank bank, uint8_t reg_addr)
{
	uint8_t reg_val = 0x00;
	ICM20948_selectBank(icm, bank);
	i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, &reg_addr, 1, 1);
	i2c_read_blocking(icm->i2c_ptr, icm->i2c_address, &reg_val, 1, 0);

	return reg_val;
}

uint8_t ICM20948_set_register(ICM20948* icm, UserBank bank, uint8_t reg_addr, uint8_t value)
{
	ICM20948_selectBank(icm, bank);

	uint8_t buffer[2] = {reg_addr, value};

	int result = i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, buffer, 2, 0);

	if (result == PICO_ERROR_GENERIC) {
        printf("Error writing to register 0x%02X\n", reg_addr);
        return 0;  // Indicate failure
    }

	return 1;
}

uint16_t ICM20948_get_register_16b(ICM20948* icm, UserBank bank, uint8_t reg_addr_H, uint8_t reg_addr_L)
{
	uint8_t H = 0x00;
	uint8_t L = 0x00;
	uint16_t val = 0x00;


	H = ICM20948_get_register(icm, bank, reg_addr_H);
	L = ICM20948_get_register(icm, bank, reg_addr_L);

	val |= (uint16_t)((H<<8)| L );

	return val;
}

ICM20948* createICM20948( i2c_inst_t* i2c_chosen, uint8_t addr_pin_high )
{
	printf("ICM20948 Creating\n");
	ICM20948* icm_ptr = (ICM20948*)malloc(sizeof(ICM20948));

	icm_ptr->i2c_ptr = i2c_chosen;
	icm_ptr->who_am_i_val = WHO_AM_I_VALUE ;
	if(addr_pin_high)
		icm_ptr->i2c_address = 0b1101001;
	else
		icm_ptr->i2c_address = 0b1101000;

	
	if(ICM20948_who_am_i_check(icm_ptr))
		return icm_ptr;
	else
		return 0x00; // Indicate failure
}

uint8_t ICM20948_get_who_am_i(ICM20948* icm)
{
	uint8_t data_read = ICM20948_get_register(icm, Bank0, WHO_AM_I);
	printf("WHO_AM_I value: %d\n", data_read);
	
	return data_read;
}

uint8_t ICM20948_who_am_i_check(ICM20948* icm)
{
	uint8_t who_am_i_val_read = ICM20948_get_who_am_i(icm);
	uint8_t who_am_i_val_isOK = 0x00;
	if(who_am_i_val_read == icm->who_am_i_val)
	{
		who_am_i_val_isOK = 1;
		printf("WHO_AM_I check: POSITIVE\n");
	}
	else
		printf("WHO_AM_I check: NEGATIVE\n");

	return who_am_i_val_isOK;
}

uint8_t ICM20948_defaultInit(ICM20948* icm)
{
	printf("ICM Default Init...\n");

	uint8_t USER_CTRL_reg = 0x00;
	uint8_t LP_CONFIG_reg = 0x00;
	uint8_t PWR_MGMT_1_reg = 0x00;
	uint8_t PWR_MGMT_2_reg = 0x00;
	uint8_t INT_PIN_CFG_reg = 0x00;
	uint8_t INT_ENABLE_reg = 0x00;
	uint8_t INT_ENABLE_1_reg = 0x00;
	uint8_t INT_ENABLE_2_reg = 0x00;
	uint8_t INT_ENABLE_3_reg = 0x00;
	uint8_t FIFO_EN_1_reg = 0x00;
	uint8_t FIFO_EN_2_reg = 0x00;
	uint8_t FIFO_RST_reg = 0x00;
	uint8_t FIFO_MODE_reg = 0x00;
	uint8_t FIFO_CFG_reg = 0x00;
	uint8_t ODR_ALIGN_EN_reg = 0x00;
	uint8_t ACCEL_SMPLRT_DIV_1_reg = 0x00;
	uint8_t ACCEL_SMPLRT_DIV_2_reg = 0x00;
	uint8_t ACCEL_INTEL_CTRL_reg = 0x00;
	uint8_t ACCEL_WOM_THR_reg = 0x00;
	uint8_t ACCEL_CONFIG_reg = 0x00;
	uint8_t ACCEL_CONFIG_2_reg = 0x00;
	uint8_t FSYNC_CONFIG_reg = 0x00;
	uint8_t TEMP_CONFIG_reg = 0x00;
	uint8_t MOD_CTRL_USR_reg = 0x00;


	ICM20948_Sleep_enable(icm, 0);
	ICM20948_GYRO_init(icm, GYRO_DLPF_NBW_154_3, FS_1000);

	float deg = 0;
	float dps = 0;

	printf("DLPF: %d \t\t FS: %d \t\t Sensitivity: %f\n", ICM20948_get_GYRO_DLPFCFG(icm), ICM20948_get_GYRO_FS_SEL(icm), ICM20948_getGyroSensitivity(ICM20948_get_GYRO_FS_SEL(icm)));
	for(int i = 0; i < 2000; i++)
	{
		sleep_ms(10);
		dps = ICM20948_GYRO_raw_to_dps( icm, ICM20948_get_GYRO_X_raw(icm) );
		deg += (dps*0.01); 
		printf("Deg: %f\n", deg);
	}

	return 1;
}

uint8_t ICM20948_isSleepMode(ICM20948* icm)
{
	uint8_t isSleepMode = ICM20948_get_register(icm, Bank0, PWR_MGMT_1) & 0b01000000;
	if(isSleepMode)
		printf("ICM IS in sleep mode\n");
	else
		printf("ICM NOT in sleep mode\n");

	return isSleepMode;
}

uint8_t ICM20948_Sleep_enable(ICM20948* icm, uint8_t enableSleep)
{
	uint8_t PWR_MGMT_1_val = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	if(enableSleep)
	{
		PWR_MGMT_1_val |= (1<<PWR_MGMT_1_SLEEP);
		printf("ICM Sleep Mode enabled\n");
	}
	else
	{
		PWR_MGMT_1_val &= ~(1<<PWR_MGMT_1_SLEEP);
		printf("ICM Sleep Mode disabled\n");
	}

	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, PWR_MGMT_1_val);
	
	return 1;
}


uint8_t ICM20948_GYRO_init(ICM20948* icm, GYRO_DLPF dlpf, FullScaleRange fs)
{

	uint8_t GYRO_SMPLRT_DIV_val = 0x00; //Gyro samplerate divider
	uint8_t GYRO_CONFIG_2_val = 0x00;
	uint8_t XG_OFFS_USRH_val = 0x00;
	uint8_t XG_OFFS_USRL_val = 0x00;
	uint8_t YG_OFFS_USRH_val = 0x00;
	uint8_t YG_OFFS_USRL_val = 0x00;
	uint8_t ZG_OFFS_USRH_val = 0x00;
	uint8_t ZG_OFFS_USRL_val = 0x00;
	uint8_t ODR_ALIGN_EN_val = 0x00; //ODR calculated as = 1.1KHz/(1+GYROSMPLRT_DIV)

	//Choosing DLPF NBW 
	ICM20948_set_GYRO_DLPFCFG(icm, dlpf);

	//FS_SEL
	ICM20948_set_GYRO_FS_SEL(icm, fs);

	//GYRO CONFIG 2 - default
	GYRO_CONFIG_2_val = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_2);
	GYRO_CONFIG_2_val &= 0b11000000;
	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_2, GYRO_CONFIG_2_val);

	return 1;
}

//GYRO SELF-TEST

//ACCEL DEFAULT INIT

//ACCEL SELF-TEST

//accel enable
//gyro enable
//TEMP DEFAULT INIT
//dmp enable
//dmp init
//fifo enable
//fifo init
//Low Power Init
//Low power enable
//device_reset()
//Temp init

//select ClockSource

// Wake On Motion enable





int16_t ICM20948_get_GYRO_X_raw(ICM20948* icm)
{
	int16_t gyro_x_raw = (int16_t)(ICM20948_get_register_16b(icm, Bank0, GYRO_XOUT_H, GYRO_XOUT_L));

	return gyro_x_raw;
}

int16_t ICM20948_get_GYRO_Y_raw(ICM20948* icm)
{
	int16_t gyro_y_raw = (int16_t)(ICM20948_get_register_16b(icm, Bank0, GYRO_YOUT_H, GYRO_YOUT_L));

	return gyro_y_raw;
}

int16_t ICM20948_get_GYRO_Z_raw(ICM20948* icm)
{
	int16_t gyro_z_raw = (int16_t)(ICM20948_get_register_16b(icm, Bank0, GYRO_ZOUT_H, GYRO_ZOUT_L));

	return gyro_z_raw;
}

int8_t ICM20948_get_GYRO_raw_arr(ICM20948* icm, uint16_t* gyro_array_ptr);
{
	/*
		array has to be 3 index size
	*/

	gyro_array_ptr[0] = ICM20948_get_GYRO_X_raw(icm);
	gyro_array_ptr[1] = ICM20948_get_GYRO_Y_raw(icm);
	gyro_array_ptr[2] = ICM20948_get_GYRO_Z_raw(icm);

	return 1;
}

float ICM20948_GYRO_raw_to_dps(ICM20948* icm, int16_t gyro_raw)
{
	float gyro_sensitivity = ICM20948_getGyroSensitivity(ICM20948_get_GYRO_FS_SEL(icm));
	float dps = ((float)(gyro_raw))/gyro_sensitivity;

	return dps;
}

FullScaleRange ICM20948_get_GYRO_FS_SEL(ICM20948* icm)
{
	FullScaleRange FS_sel = FS_250;
	uint8_t fs_sel_val = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);
	fs_sel_val &= 0b00000110;
	fs_sel_val >>= 1;

	switch(fs_sel_val)
	{
		case 0:
			FS_sel = FS_250;
		break;
		case 1:
			FS_sel = FS_500;
		break;
		case 2:
			FS_sel = FS_1000;
		break;
		case 3:
			FS_sel = FS_2000;
		break;
	}

	return FS_sel;
}

uint8_t ICM20948_set_GYRO_FS_SEL(ICM20948* icm, FullScaleRange fs_sel)
{
	uint8_t fs_sel_val = 0b00000000;

	switch(fs_sel)
	{
		case (FS_250):
			fs_sel_val = 0b00000000;
		break;
		case (FS_500):
			fs_sel_val = 0b00000010;
		break;
		case FS_1000:
			fs_sel_val = 0b00000100;
		break;
		case FS_2000:
			fs_sel_val = 0b00000110;
		break;
	}

	uint8_t gyro_config_1 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);
	gyro_config_1 &= 0b11111001;
	gyro_config_1 |= fs_sel_val;
	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_1, gyro_config_1);

	return 1;
}

float ICM20948_getGyroSensitivity(FullScaleRange FS)
{
	float sensitivity = 131;
	switch(FS)
	{
		case FS_250:
			sensitivity = 131;
		break;
		case FS_500:
			sensitivity = 65.5;
		break;
		case FS_1000:
			sensitivity = 32.8;
		break;
		case FS_2000:
			sensitivity = 16.4;
		break;
	}

	// printf("GYRO Sensitivity: %f\n", sensitivity);
	return sensitivity;
}

GYRO_DLPF ICM20948_get_GYRO_DLPFCFG(ICM20948* icm )
{
	GYRO_DLPF dlpf_sel = GYRO_DLPF_NBW_229_8;
	uint8_t gyro_config_1 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);

	uint8_t fchoice = gyro_config_1 & 0x01;

	if(!fchoice)
	{
		dlpf_sel = GYRO_DLPF_NBW_12316;
	}
	else
	{
		gyro_config_1 &= 0b00111000;
		gyro_config_1 >>= 3;

		switch(gyro_config_1)
		{
			case 0:
				dlpf_sel = GYRO_DLPF_NBW_229_8;
			break;
			case 1:
				dlpf_sel = GYRO_DLPF_NBW_187_6;
			break;
			case 2:
				dlpf_sel = GYRO_DLPF_NBW_154_3;
			break;
			case 3:
				dlpf_sel = GYRO_DLPF_NBW_73_3;
			break;
			case 4:
				dlpf_sel = GYRO_DLPF_NBW_35_9;
			break;
			case 5:
				dlpf_sel = GYRO_DLPF_NBW_17_8;
			break;
			case 6:
				dlpf_sel = GYRO_DLPF_NBW_8_9;
			break;
			case 7:
				dlpf_sel = GYRO_DLPF_NBW_376_5;
			break;
		}
	}

	printf("GYRO DLPF value: %d\n", dlpf_sel);
	return dlpf_sel;
}

uint8_t ICM20948_set_GYRO_DLPFCFG(ICM20948* icm, GYRO_DLPF dlpf_sel)
{
	uint8_t gyro_config_1 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);
	uint8_t dlpf_val = dlpf_sel;

	if(dlpf_sel == GYRO_DLPF_NBW_12316)
	{
		ICM20948_GYRO_DLPF_enable(icm, 0);
	}
	else
	{
		gyro_config_1 &= 0b11000111;
		dlpf_val <<= 3;
		gyro_config_1 |= dlpf_val;
		gyro_config_1 |= 0x01;
		ICM20948_set_register(icm, Bank2, GYRO_CONFIG_1, gyro_config_1);
	}

	return 1;
}

uint8_t ICM20948_GYRO_DLPF_enable(ICM20948* icm, uint8_t enable)
{
	uint8_t gyro_config_1 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);

	switch(enable)
	{
		case 0:
			gyro_config_1 &= ~(1<<GYRO_CONFIG_1_GYRO_FCHOICE);
		break;
		default:
			gyro_config_1 |= (1<<GYRO_CONFIG_1_GYRO_FCHOICE);
	}

	return 1;
}


