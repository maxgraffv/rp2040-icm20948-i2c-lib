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

uint8_t ICM20948_Init(ICM20948* icm)
{
	printf("ICM Default Init...\n");

	
	// ICM20948_reset(icm);
	ICM20948_SleepMode_disable(icm);
	ICM20948_set_CLOCK_SRC(icm, CLOCK_SRC_Auto_Sel_1);
	ICM20948_GYRO_Init(icm, GYRO_DLPF_NBW_154_3, GYRO_FS_1000);
	ICM20948_ACCEL_Init(icm, ACCEL_DLPF_NBW_68_8, ACCEL_FS_4);
	ICM20948_TEMP_Init(icm, TEMP_DLPF_NBW_8_8);

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

uint8_t ICM20948_SleepMode_enable(ICM20948* icm)
{
	uint8_t PWR_MGMT_1_val = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	PWR_MGMT_1_val |= (1<<PWR_MGMT_1_SLEEP);
	printf("ICM Sleep Mode enabled\n");

	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, PWR_MGMT_1_val);
	
	return 1;
}

uint8_t ICM20948_SleepMode_disable(ICM20948* icm)
{
	uint8_t PWR_MGMT_1_val = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	PWR_MGMT_1_val &= ~(1<<PWR_MGMT_1_SLEEP);
	printf("ICM Sleep Mode disabled\n");

	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, PWR_MGMT_1_val);
	
	return 1;
}

uint8_t ICM20948_GYRO_Init(ICM20948* icm, GYRO_DLPF dlpf, GYRO_FS fs)
{
	//Choosing DLPF NBW 
	ICM20948_set_GYRO_DLPFCFG(icm, dlpf);

	//FS_SEL
	ICM20948_set_GYRO_FS_SEL(icm, fs);


	return 1;
}

uint8_t ICM20948_reset(ICM20948* icm)
{
	uint8_t pwr_mgmt_1 = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	pwr_mgmt_1 |= (1<<PWR_MGMT_1_DEVICE_RESET);
	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, pwr_mgmt_1);

	return 1;
}

uint8_t ICM20948_LowPowerMode_enable(ICM20948* icm)
{
	uint8_t pwr_mgmt_1 = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	pwr_mgmt_1 |= (1<<PWR_MGMT_1_LP_EN);
	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, pwr_mgmt_1);

	return 1;
}

uint8_t ICM20948_LowPowerMode_disable(ICM20948* icm)
{
	uint8_t pwr_mgmt_1 = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	pwr_mgmt_1 &= ~(1<<PWR_MGMT_1_LP_EN);
	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, pwr_mgmt_1);

	return 1;
}

uint8_t ICM20948_isLowPowerpMode(ICM20948* icm)
{
	uint8_t isLP = ICM20948_get_register(icm, Bank0, PWR_MGMT_1) & 0b00100000;
	if(isLP)
		printf("ICM IS in low power mode\n");
	else
		printf("ICM NOT in low power mode\n");

	return isLP;
}

uint8_t ICM20948_Temp_enable(ICM20948* icm)
{
	uint8_t pwr_mgmt_1 = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	pwr_mgmt_1 &= ~(1<<PWR_MGMT_1_TEMP_DIS);
	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, pwr_mgmt_1);

	return 1;
}

uint8_t ICM20948_Temp_disable(ICM20948* icm)
{
	uint8_t pwr_mgmt_1 = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);

	pwr_mgmt_1 |= (1<<PWR_MGMT_1_TEMP_DIS);
	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, pwr_mgmt_1);

	return 1;
}

uint8_t ICM20948_isTemp(ICM20948* icm)
{
	uint8_t isTemp_disabled = ICM20948_get_register(icm, Bank0, PWR_MGMT_1) & 0b00100000;
	if(isTemp_disabled)
		printf("Temp Sensor IS DISABLED\n");
	else
		printf("Temp Sensor IS ENABLED\n");

	return (!isTemp_disabled);
}

uint8_t ICM20948_set_CLOCK_SRC(ICM20948* icm, CLOCK_SRC clk_src)
{
	uint8_t pwr_mgmt_1 = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);
	pwr_mgmt_1 &= 0b11111000;
	uint8_t clk = clk_src;

	pwr_mgmt_1 |= clk;

	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, pwr_mgmt_1);

	return 1;
}

CLOCK_SRC ICM20948_get_CLOCK_SRC(ICM20948* icm)
{
	CLOCK_SRC clk_sel = CLOCK_SRC_Internal_20MHz;
	uint8_t clk_val = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);
	clk_val &= 0b00000111;

	switch(clk_val)
	{
		case 0:
			clk_sel = CLOCK_SRC_Internal_20MHz;
		break;
		case 1:
			clk_sel = CLOCK_SRC_Auto_Sel_1;
		break;
		case 2:
			clk_sel = CLOCK_SRC_Auto_Sel_1;
		break;
		case 3:
			clk_sel = CLOCK_SRC_Auto_Sel_1;
		break;
		case 4:
			clk_sel = CLOCK_SRC_Auto_Sel_1;
		break;
		case 5:
			clk_sel = CLOCK_SRC_Auto_Sel_1;
		break;
		case 6:
			clk_sel = CLOCK_SRC_Internal_20MHz;
		break;
		case 7:
			clk_sel = CLOCK_SRC_STOP;
		break;
	}

	return clk_sel;
}

uint8_t ICM20948_ACCEL_enable(ICM20948* icm)
{
	uint8_t pwr_mgmt_2 = ICM20948_get_register(icm, Bank0, PWR_MGMT_2);
	pwr_mgmt_2 |= 0b00111000;
	ICM20948_set_register(icm, Bank0, PWR_MGMT_2, pwr_mgmt_2);

	return 1;
}

uint8_t ICM20948_ACCEL_disable(ICM20948* icm)
{
	uint8_t pwr_mgmt_2 = ICM20948_get_register(icm, Bank0, PWR_MGMT_2);
	pwr_mgmt_2 &= 0b11000111;
	ICM20948_set_register(icm, Bank0, PWR_MGMT_2, pwr_mgmt_2);

	return 1;
}

uint8_t ICM20948_GYRO_enable(ICM20948* icm)
{
	uint8_t pwr_mgmt_2 = ICM20948_get_register(icm, Bank0, PWR_MGMT_2);
	pwr_mgmt_2 |= 0b00000111;
	ICM20948_set_register(icm, Bank0, PWR_MGMT_2, pwr_mgmt_2);

	return 1;
}

uint8_t ICM20948_GYRO_disable(ICM20948* icm)
{
	uint8_t pwr_mgmt_2 = ICM20948_get_register(icm, Bank0, PWR_MGMT_2);
	pwr_mgmt_2 &= 0b11111000;
	ICM20948_set_register(icm, Bank0, PWR_MGMT_2, pwr_mgmt_2);

	return 1;
}

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

int8_t ICM20948_get_GYRO_raw_arr(ICM20948* icm, uint16_t* gyro_array_ptr)
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

GYRO_FS ICM20948_get_GYRO_FS_SEL(ICM20948* icm)
{
	GYRO_FS FS_sel = GYRO_FS_250;
	uint8_t fs_sel_val = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);
	fs_sel_val &= 0b00000110;
	fs_sel_val >>= 1;

	switch(fs_sel_val)
	{
		case 0:
			FS_sel = GYRO_FS_250;
		break;
		case 1:
			FS_sel = GYRO_FS_500;
		break;
		case 2:
			FS_sel = GYRO_FS_1000;
		break;
		case 3:
			FS_sel = GYRO_FS_2000;
		break;
	}

	return FS_sel;
}

uint8_t ICM20948_set_GYRO_FS_SEL(ICM20948* icm, GYRO_FS fs_sel)
{
	uint8_t fs_sel_val = 0b00000000;

	switch(fs_sel)
	{
		case (GYRO_FS_250):
			fs_sel_val = 0b00000000;
		break;
		case (GYRO_FS_500):
			fs_sel_val = 0b00000010;
		break;
		case GYRO_FS_1000:
			fs_sel_val = 0b00000100;
		break;
		case GYRO_FS_2000:
			fs_sel_val = 0b00000110;
		break;
	}

	uint8_t gyro_config_1 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);
	gyro_config_1 &= 0b11111001;
	gyro_config_1 |= fs_sel_val;
	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_1, gyro_config_1);

	return 1;
}

float ICM20948_getGyroSensitivity(GYRO_FS FS)
{
	float sensitivity = 131;
	switch(FS)
	{
		case GYRO_FS_250:
			sensitivity = 131;
		break;
		case GYRO_FS_500:
			sensitivity = 65.5;
		break;
		case GYRO_FS_1000:
			sensitivity = 32.8;
		break;
		case GYRO_FS_2000:
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
		ICM20948_GYRO_DLPF_disable(icm);
	}
	else
	{
		ICM20948_GYRO_DLPF_enable(icm);
		gyro_config_1 &= 0b11000111;
		dlpf_val <<= 3;
		gyro_config_1 |= dlpf_val;
		gyro_config_1 |= 0x01;
		ICM20948_set_register(icm, Bank2, GYRO_CONFIG_1, gyro_config_1);
	}

	return 1;
}

uint8_t ICM20948_GYRO_DLPF_enable(ICM20948* icm)
{
	uint8_t gyro_config_1 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);

	gyro_config_1 |= (1<<GYRO_CONFIG_1_GYRO_FCHOICE);

	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_1,gyro_config_1);

	return 1;
}

uint8_t ICM20948_GYRO_DLPF_disable(ICM20948* icm)
{
	uint8_t gyro_config_1 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);
	gyro_config_1 |= (1<<GYRO_CONFIG_1_GYRO_FCHOICE);
	gyro_config_1 &= ~(1<<GYRO_CONFIG_1_GYRO_FCHOICE);

	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_1,gyro_config_1);
	return 1;
}

uint8_t ICM20948_set_GYRO_AVG_FILTER_CFG(ICM20948* icm, GYRO_AVG_FILTER avg_filter)
{
	uint8_t gyro_config_2 = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_2);	
	uint8_t filter_sel = avg_filter;
	gyro_config_2 &= 0b11111000;
	gyro_config_2 |= filter_sel;
	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_2, gyro_config_2);

	return 1;
}

uint8_t ICM20948_set_GYRO_SAMPLE_RATE_DIV(ICM20948* icm, uint8_t sample_rate)
{
	ICM20948_set_register(icm, Bank2, GYRO_SMPLRT_DIV, sample_rate);

	return 1;
}

uint8_t ICM20948_get_GYRO_SAMPLE_RATE_DIV(ICM20948* icm)
{
	uint8_t smplrt = ICM20948_get_register(icm, Bank2, GYRO_SMPLRT_DIV);
	return smplrt;
}

float ICM20948_get_GYRO_ODR_kHz(ICM20948* icm)
{
	float gyro_odr = 1.1/(1 + (float)ICM20948_get_GYRO_SAMPLE_RATE_DIV(icm));

	return gyro_odr;
}

uint8_t ICM20948_DMP_enable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_DMP_EN);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_DMP_disable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl &= ~(1<<USER_CTRL_DMP_EN);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_FIFO_enable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_FIFO_EN);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_FIFO_disable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl &= ~(1<<USER_CTRL_FIFO_EN);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_I2C_MST_enable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_I2C_MST_EN);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_I2C_MST_disable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl &= ~(1<<USER_CTRL_I2C_MST_EN);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_SPI_MODE_ONLY_enable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_I2C_IF_DIS);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_SPI_MODE_ONLY_disable(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl &= ~(1<<USER_CTRL_I2C_IF_DIS);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_DMP_reset(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_DMP_RST);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_SRAM_reset(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_SRAM_RST);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_I2C_MST_reset(ICM20948* icm)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_I2C_MST_RST);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_I2C_MST_CYCLE_enable(ICM20948* icm)
{
	uint8_t lp_config = ICM20948_get_register(icm, Bank0, LP_CONFIG);
	lp_config |= (1<<LP_CONFIG_I2C_MST_CYCLE);
	ICM20948_set_register(icm, Bank0, LP_CONFIG, lp_config);

	return 1;
}

uint8_t ICM20948_I2C_MST_CYCLE_disable(ICM20948* icm)
{
	uint8_t lp_config = ICM20948_get_register(icm, Bank0, LP_CONFIG);
	lp_config &= ~(1<<LP_CONFIG_I2C_MST_CYCLE);
	ICM20948_set_register(icm, Bank0, LP_CONFIG, lp_config);

	return 1;
}

uint8_t ICM20948_ACCEL_CYCLE_enable(ICM20948* icm)
{
	uint8_t lp_config = ICM20948_get_register(icm, Bank0, LP_CONFIG);
	lp_config |= (1<<LP_CONFIG_ACCEL_CYCLE);
	ICM20948_set_register(icm, Bank0, LP_CONFIG, lp_config);

	return 1;
}

uint8_t ICM20948_ACCEL_CYCLE_disable(ICM20948* icm)
{
	uint8_t lp_config = ICM20948_get_register(icm, Bank0, LP_CONFIG);
	lp_config &= ~(1<<LP_CONFIG_ACCEL_CYCLE);
	ICM20948_set_register(icm, Bank0, LP_CONFIG, lp_config);

	return 1;
}

uint8_t ICM20948_GYRO_CYCLE_enable(ICM20948* icm)
{
	uint8_t lp_config = ICM20948_get_register(icm, Bank0, LP_CONFIG);
	lp_config |= (1<<LP_CONFIG_GYRO_CYCLE);
	ICM20948_set_register(icm, Bank0, LP_CONFIG, lp_config);

	return 1;
}

uint8_t ICM20948_GYRO_CYCLE_disable(ICM20948* icm)
{
	uint8_t lp_config = ICM20948_get_register(icm, Bank0, LP_CONFIG);
	lp_config &= ~(1<<LP_CONFIG_GYRO_CYCLE);
	ICM20948_set_register(icm, Bank0, LP_CONFIG, lp_config);

	return 1;
}

uint8_t ICM20948_WOF_enable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable |= (1<<INT_ENABLE_REG_WOF_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_WOF_disable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable &= ~(1<<INT_ENABLE_REG_WOF_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_WOM_enable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable |= (1<<INT_ENABLE_WOM_INT_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_WOM_disable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable &= ~(1<<INT_ENABLE_WOM_INT_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_PLL_RDY_enable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable |= (1<<INT_ENABLE_PLL_RDY_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_PLL_RDY_disable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable &= ~(1<<INT_ENABLE_PLL_RDY_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_DMP_INT1_enable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable |= (1<<INT_ENABLE_DMP_INT1_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_DMP_INT1_disable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable &= ~(1<<INT_ENABLE_DMP_INT1_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_I2C_MST_INT_enable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable |= (1<<INT_ENABLE_I2C_MST_INT_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_I2C_MST_INT_disable(ICM20948* icm)
{
	uint8_t int_enable = ICM20948_get_register(icm, Bank0, INT_ENABLE);
	int_enable &= ~(1<<INT_ENABLE_I2C_MST_INT_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE, int_enable);

	return 1;
}

uint8_t ICM20948_RAW_DATA_RDY_INT_enable(ICM20948* icm)
{
	uint8_t int_enable_1 = ICM20948_get_register(icm, Bank0, INT_ENABLE_1);
	int_enable_1 |= (1<<INT_ENABLE_1_RAW_DATA_0RDY_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_1, int_enable_1);

	return 1;
}

uint8_t ICM20948_RAW_DATA_RDY_INT_disable(ICM20948* icm)
{
	uint8_t int_enable_1 = ICM20948_get_register(icm, Bank0, INT_ENABLE_1);
	int_enable_1 &= ~(1<<INT_ENABLE_1_RAW_DATA_0RDY_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_1, int_enable_1);

	return 1;
}

uint8_t ICM20948_FIFO_OVERFLOW_INT_enable(ICM20948* icm)
{
	uint8_t int_enable_2 = ICM20948_get_register(icm, Bank0, INT_ENABLE_2);
	int_enable_2 |= (1<<INT_ENABLE_2_FIFO_OVERFLOW_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_2, int_enable_2);

	return 1;
}

uint8_t ICM20948_FIFO_OVERFLOW_INT_disable(ICM20948* icm)
{
	uint8_t int_enable_2 = ICM20948_get_register(icm, Bank0, INT_ENABLE_2);
	int_enable_2 &= ~(1<<INT_ENABLE_2_FIFO_OVERFLOW_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_2, int_enable_2);

	return 1;
}

uint8_t ICM20948_FIFO_WATERMARK_INT_enable(ICM20948* icm)
{
	uint8_t int_enable_3 = ICM20948_get_register(icm, Bank0, INT_ENABLE_3);
	int_enable_3 |= (1<<INT_ENABLE_3_FIFO_WM_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_3, int_enable_3);

	return 1;
}

uint8_t ICM20948_FIFO_WATERMARK_INT_disable(ICM20948* icm)
{
	uint8_t int_enable_3 = ICM20948_get_register(icm, Bank0, INT_ENABLE_3);
	int_enable_3 &= ~(1<<INT_ENABLE_3_FIFO_WM_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_3, int_enable_3);

	return 1;
}









uint8_t ICM20948_WakeOnMotion_occured(ICM20948* icm)
{
	uint8_t int_status = ICM20948_get_register(icm, Bank0, INT_STATUS);
	uint8_t WOM = int_status & 0b00001000;
	WOM >>= 3;

	return WOM;
}

uint8_t ICM20948_PLL_Ready(ICM20948* icm) // PLL Enabled and Ready Status
{
	uint8_t int_status = ICM20948_get_register(icm, Bank0, INT_STATUS);
	uint8_t PLL_RDY = int_status & 0b00000100;
	PLL_RDY >>= 2;

	return PLL_RDY;
}

uint8_t ICM20948_DMP_INT1_occured(ICM20948* icm) 
{
	uint8_t int_status = ICM20948_get_register(icm, Bank0, INT_STATUS);
	uint8_t DMP_INT = int_status & 0b00000010;
	DMP_INT >>= 1;

	return DMP_INT;
}

uint8_t ICM20948_I2C_MST_INT_occured(ICM20948* icm) 
{
	uint8_t int_status = ICM20948_get_register(icm, Bank0, INT_STATUS);
	uint8_t I2C_MST_INT = int_status & 0b00000001;

	return I2C_MST_INT;
}






float ICM20948_get_DELAY_TIME_ms(ICM20948* icm)
{
	float delay_time_H = (float)ICM20948_get_register(icm, Bank0, DELAY_TIMEH);
	float delay_time_L = (float)ICM20948_get_register(icm, Bank0, DELAY_TIMEL);
	float ms = (delay_time_H * 256 + delay_time_L) * 0.9645;

	return ms;
}







uint8_t ICM20948_ACCEL_Init(ICM20948* icm, ACCEL_DLPF dlpf , ACCEL_FS fs)
{
	//DLPF
	ICM20948_set_ACCEL_DLPFCFG(icm, dlpf);

	//ACCEL Full Scale
	ICM20948_set_ACCEL_FS_SEL(icm, fs);

	return 1;
}

int16_t ICM20948_get_ACCEL_X_raw(ICM20948* icm)
{
	int16_t accel_x_raw = (int16_t)ICM20948_get_register_16b(icm, Bank0, ACCEL_XOUT_H, ACCEL_XOUT_L);

	return accel_x_raw;
}

int16_t ICM20948_get_ACCEL_Y_raw(ICM20948* icm)
{
	int16_t accel_y_raw = (int16_t)ICM20948_get_register_16b(icm, Bank0, ACCEL_YOUT_H, ACCEL_YOUT_L);

	return accel_y_raw;
}

int16_t ICM20948_get_ACCEL_Z_raw(ICM20948* icm)
{
	int16_t accel_z_raw = (int16_t)ICM20948_get_register_16b(icm, Bank0, ACCEL_ZOUT_H, ACCEL_ZOUT_L);

	return accel_z_raw;
}

float ICM20948_ACCEL_raw_to_g(ICM20948* icm, int16_t accel_raw)
{
	float accel = (float)(accel_raw);
	float sensitivity = ICM20948_getAccelSensitivity(ICM20948_get_ACCEL_FS_SEL(icm));
	float accel_g = accel/sensitivity;

	return accel_g;
}

uint8_t ICM20948_set_ACCEL_FS_SEL(ICM20948* icm, ACCEL_FS accel_fs_sel )
{
	uint8_t accel_config = ICM20948_get_register(icm, Bank2, ACCEL_CONFIG);
	accel_config &= 0b11111001;

	switch(accel_fs_sel)
	{
		case ACCEL_FS_2:
			accel_config |= (ACCEL_FS_2<<1);
		break;	 
		case ACCEL_FS_4:
			accel_config |= (ACCEL_FS_4<<1);
		break; 
		case ACCEL_FS_8:
			accel_config |= (ACCEL_FS_8<<1);
		break; 
		case ACCEL_FS_16:
			accel_config |= (ACCEL_FS_16<<1);
		break; 
	}

	ICM20948_set_register(icm, Bank2, ACCEL_CONFIG, accel_config);

	return 1;
}

ACCEL_FS ICM20948_get_ACCEL_FS_SEL(ICM20948* icm)
{

	uint8_t accel_config = ICM20948_get_register(icm, Bank2, ACCEL_CONFIG);
	accel_config &= 0b00000110;
	accel_config >>= 1;

	ACCEL_FS accel_fs = ACCEL_FS_2;

	switch(accel_config)
	{
		case 0:
			accel_fs = ACCEL_FS_2;
		break;	
		case 1:
			accel_fs = ACCEL_FS_4;
		break;
		case 2:
			accel_fs = ACCEL_FS_8;
		break;
		case 3:
			accel_fs = ACCEL_FS_16;
		break;
	}

	return accel_fs;
}

float ICM20948_getAccelSensitivity(ACCEL_FS accel_fs)
{
	float sensitivity = 16384;
	switch(accel_fs)
	{
		case ACCEL_FS_2:
			sensitivity = 16384;
		break;
		case ACCEL_FS_4:
			sensitivity = 8192;
		break;
		case ACCEL_FS_8:
			sensitivity = 4096 ;
		break;
		case ACCEL_FS_16:
			sensitivity = 2048;
		break;
	}


	return sensitivity;
}

uint8_t ICM20948_ACCEL_DLPF_enable(ICM20948* icm)
{
	uint8_t accel_config = ICM20948_get_register(icm, Bank2, ACCEL_CONFIG);
	accel_config |= (1<<ACCEL_CONFIG_FCHOICE);
	ICM20948_set_register(icm, Bank2, ACCEL_CONFIG, accel_config);

	return 1;
}

uint8_t ICM20948_ACCEL_DLPF_disable(ICM20948* icm )
{
	uint8_t accel_config = ICM20948_get_register(icm, Bank2, ACCEL_CONFIG);
	accel_config &= ~(1<<ACCEL_CONFIG_FCHOICE);
	ICM20948_set_register(icm, Bank2, ACCEL_CONFIG, accel_config);

	return 1;
}

uint8_t ICM20948_set_ACCEL_DLPFCFG(ICM20948* icm, ACCEL_DLPF accel_dlpf )
{
	uint8_t accel_config = 0x00;
	uint8_t dlpf_val =  accel_dlpf;

	if(accel_dlpf == -1)
	{
		ICM20948_ACCEL_DLPF_disable(icm);
	}
	else
	{
		ICM20948_ACCEL_DLPF_enable(icm);
		accel_config = ICM20948_get_register(icm, Bank2, ACCEL_CONFIG);
		accel_config &= 0b11000111;
		dlpf_val <<= 3;
		accel_config |= dlpf_val;
		ICM20948_set_register(icm, Bank2, ACCEL_CONFIG, accel_config);
	}
}

ACCEL_DLPF ICM20948_get_ACCEL_DLPFCFG(ICM20948* icm)
{
	uint8_t accel_config = ICM20948_get_register(icm, Bank2, ACCEL_CONFIG);

	uint8_t fchoice = accel_config & 0x01;
	ACCEL_DLPF dlpf = ACCEL_DLPF_NBW_1248;

	if(fchoice)
	{
		accel_config &= 0b00111000;
		accel_config >>= 3;

		switch(accel_config)
		{
			case ACCEL_DLPF_NBW_265 :
				dlpf = ACCEL_DLPF_NBW_265 ;
			break;
			case ACCEL_DLPF_NBW_265_2 :
				dlpf = ACCEL_DLPF_NBW_265_2 ;
			break;
			case ACCEL_DLPF_NBW_136 :
				dlpf = ACCEL_DLPF_NBW_136 ;
			break;
			case ACCEL_DLPF_NBW_68_8 :
				dlpf = ACCEL_DLPF_NBW_68_8 ;
			break;
			case ACCEL_DLPF_NBW_34_4 :
				dlpf = ACCEL_DLPF_NBW_34_4 ;
			break;
			case ACCEL_DLPF_NBW_17 :
				dlpf = ACCEL_DLPF_NBW_17 ;
			break;
			case ACCEL_DLPF_NBW_8_3 :
				dlpf = ACCEL_DLPF_NBW_8_3 ;
			break;
			case ACCEL_DLPF_NBW_499 :
				dlpf = ACCEL_DLPF_NBW_499 ;
			break;
		}

	}

	return dlpf;
}

uint8_t ICM20948_set_ACCEL_AVG_SAMPLES(ICM20948* icm, ACCEL_AVG_SAMPLES avg_sample)
{
	uint8_t accel_config_2 = ICM20948_get_register(icm, Bank2, ACCEL_CONFIG_2);
	uint8_t sample_val = avg_sample;

	accel_config_2 &= 0b11111100;
	accel_config_2 |= sample_val;

	ICM20948_set_register(icm, Bank2, ACCEL_CONFIG_2, accel_config_2);

	return 1;
}

uint16_t ICM20948_set_ACCEL_SAMPLE_RATE_DIV(ICM20948* icm, uint16_t samplerate)
{
	if(samplerate >= 4096)
		samplerate = 4095;

	uint16_t samplerate_l_16 = samplerate & 0b0000000011111111;

	uint16_t samplerate_h_16 = samplerate & 0b1111111100000000;
	samplerate_h_16 >>= 8;

	uint8_t samplerate_h_8 = (uint8_t)(samplerate_h_16);
	uint8_t samplerate_l_8 = (uint8_t)(samplerate_l_16);

	uint8_t samplerate_h = ICM20948_get_register(icm, Bank2, ACCEL_SMPLRT_DIV_1);
	samplerate_h &= 0b11110000;
	samplerate_h |= samplerate_h_8;

	ICM20948_set_register(icm, Bank2, ACCEL_SMPLRT_DIV_1, samplerate_h);
	ICM20948_set_register(icm, Bank2, ACCEL_SMPLRT_DIV_2, samplerate_l_8);

	return 1;
}

uint16_t ICM20948_get_ACCEL_SAMPLE_RATE_DIV(ICM20948* icm)
{
	uint16_t samplerate = ICM20948_get_register_16b(icm, Bank2, ACCEL_SMPLRT_DIV_1, ACCEL_SMPLRT_DIV_2);
	samplerate &= 0b0000111111111111;

	return samplerate;
}

float ICM20948_get_ACCEL_ODR_kHz(ICM20948* icm)
{
	float sample_rate = (float)ICM20948_get_ACCEL_SAMPLE_RATE_DIV(icm);

	float accel_odr = 1.125/(1+sample_rate);

	return accel_odr;
}

uint8_t ICM20948_WOM_Logic_enable(ICM20948* icm)
{
	uint8_t accel_intel = ICM20948_get_register(icm, Bank2, ACCEL_INTEL_CTRL);
	accel_intel |= (1<<ACCEL_INTEL_CTRL_EN);
	ICM20948_set_register(icm, Bank2, ACCEL_INTEL_CTRL, accel_intel);

	return 1;
}

uint8_t ICM20948_WOM_Logic_disable(ICM20948* icm)
{
	uint8_t accel_intel = ICM20948_get_register(icm, Bank2, ACCEL_INTEL_CTRL);
	accel_intel &= ~(1<<ACCEL_INTEL_CTRL_EN);
	ICM20948_set_register(icm, Bank2, ACCEL_INTEL_CTRL, accel_intel);

	return 1;
}

uint8_t ICM20948_WOM_Algorithm_select(ICM20948* icm, WOM_ALGORITHM wom_alg)
{
	uint8_t accel_intel = ICM20948_get_register(icm, Bank2, ACCEL_INTEL_CTRL);
	uint8_t selected = wom_alg;
	accel_intel &= 0b11111110;
	accel_intel |= selected;

	ICM20948_set_register(icm, Bank2, ACCEL_INTEL_CTRL, accel_intel);

	return 1;
}

uint8_t ICM20948_set_WOM_THRESHOLD(ICM20948* icm, uint8_t threshold)
{
	ICM20948_set_register(icm, Bank2, ACCEL_WOM_THR, threshold);

	return 1;
}

uint8_t ICM20948_set_TEMP_DLPFCFG(ICM20948* icm, TEMP_DLPF dlpf)
{
	uint8_t temp_config = dlpf;
	ICM20948_set_register(icm, Bank2, TEMP_CONFIG, temp_config);

	return 1;
}

uint8_t ICM20948_TEMP_Init(ICM20948* icm, TEMP_DLPF dlpf)
{
	ICM20948_Temp_enable(icm);
	ICM20948_set_TEMP_DLPFCFG(icm, dlpf);

	return 1;
}

uint16_t ICM20948_get_TEMP_raw(ICM20948* icm)
{
	uint16_t temp_raw = ICM20948_get_register_16b(icm, Bank0, TEMP_OUT_H, TEMP_OUT_L);

	return temp_raw;
}

float ICM20948_get_TEMP_C(ICM20948* icm)
{
	uint16_t temp_raw = ICM20948_get_TEMP_raw(icm);
	float temp_f = (float)temp_raw;
	float sensitivity = 333.87;
	float RoomTemp_Offset = 21;
	float temp_C = ((temp_raw - RoomTemp_Offset)/sensitivity) + 21;
	float temp_offset = 10;
	temp_C -= temp_offset;

	return temp_C;
}

uint8_t ICM20948_FIFO_reset(ICM20948* icm)
{
	uint8_t fifo_rst = ICM20948_get_register(icm, Bank0, FIFO_RST);
	fifo_rst |= 0x01;
	ICM20948_set_register(icm, Bank0, FIFO_RST, fifo_rst);

	fifo_rst = ICM20948_get_register(icm, Bank0, FIFO_RST);
	fifo_rst &= 0b11100000;
	ICM20948_set_register(icm, Bank0, FIFO_RST, fifo_rst);

	return 1;
}

uint8_t ICM20948_set_FIFO_MODE(ICM20948* icm, FIFO_MODE_ENUM mode_sel)
{
	uint8_t fifo_mode = ICM20948_get_register(icm, Bank0, FIFO_MODE);
	uint8_t mode = mode_sel;
	fifo_mode &= 0b11111110;
	fifo_mode |= mode;
	ICM20948_set_register(icm, Bank0, FIFO_MODE, fifo_mode);

	return 1;
}

uint16_t ICM20948_get_FIFO_COUNT(ICM20948* icm)
{
	uint16_t fifo_count = ICM20948_get_register_16b(icm, Bank0, FIFO_COUNTH, FIFO_COUNTL);
	fifo_count &= 0b0001111111111111;

	return fifo_count;
}

uint8_t ICM20948_SLV_0_FIFO_enable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 |= (1<<FIFO_EN_1_SLV_0_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_SLV_0_FIFO_disable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 &= ~(1<<FIFO_EN_1_SLV_0_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_SLV_1_FIFO_enable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 |= (1<<FIFO_EN_1_SLV_1_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_SLV_1_FIFO_disable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 &= ~(1<<FIFO_EN_1_SLV_1_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_SLV_2_FIFO_enable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 |= (1<<FIFO_EN_1_SLV_2_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_SLV_2_FIFO_disable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 &= ~(1<<FIFO_EN_1_SLV_2_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_SLV_3_FIFO_enable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 |= (1<<FIFO_EN_1_SLV_3_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_SLV_3_FIFO_disable(ICM20948* icm)
{
	uint8_t fifo_en_1 = ICM20948_get_register(icm, Bank0, FIFO_EN_1);
	fifo_en_1 &= ~(1<<FIFO_EN_1_SLV_3_FIFO_EN);
	ICM20948_set_register(icm, Bank0, FIFO_EN_1, fifo_en_1);

	return 1;
}

uint8_t ICM20948_ODR_ALIGN_enable(ICM20948* icm)
{
	uint8_t odr_align_en = ICM20948_get_register(icm, Bank2, ODR_ALIGN_EN);
	odr_align_en |= (1<<ODR_ALIGN_EN_bit);
	ICM20948_set_register(icm, Bank2, ODR_ALIGN_EN, odr_align_en);

	return 1;
}

uint8_t ICM20948_ODR_ALIGN_disable(ICM20948* icm)
{
	uint8_t odr_align_en = ICM20948_get_register(icm, Bank2, ODR_ALIGN_EN);
	odr_align_en &= ~(1<<ODR_ALIGN_EN_bit);
	ICM20948_set_register(icm, Bank2, ODR_ALIGN_EN, odr_align_en);

	return 1;
}

uint8_t ICM20948_get_RAW_DATA_RDY_INT_status(ICM20948* icm)
{
	uint8_t int_status_1 = ICM20948_get_register(icm, Bank0, INT_STATUS_1);
	int_status_1 &= 0x01;

	return int_status_1;
}

uint8_t ICM20948_set_GYRO_X_BIAS(ICM20948* icm, int16_t bias)
{
	uint16_t bias_16_h = bias & 0b1111111100000000;
		bias_16_h >>=8;
	uint16_t bias_16_l = bias & 0b0000000011111111;

	uint8_t bias_8_h = (uint8_t)bias_16_h;
	uint8_t bias_8_l = (uint8_t)bias_16_l;

	ICM20948_set_register(icm, Bank2, XG_OFFS_USRH, bias_8_h);
	ICM20948_set_register(icm, Bank2, XG_OFFS_USRL, bias_8_l);
}

uint8_t ICM20948_set_GYRO_Y_BIAS(ICM20948* icm, int16_t bias)
{
	uint16_t bias_16_h = bias & 0b1111111100000000;
		bias_16_h >>=8;
	uint16_t bias_16_l = bias & 0b0000000011111111;

	uint8_t bias_8_h = (uint8_t)bias_16_h;
	uint8_t bias_8_l = (uint8_t)bias_16_l;

	ICM20948_set_register(icm, Bank2, YG_OFFS_USRH, bias_8_h);
	ICM20948_set_register(icm, Bank2, YG_OFFS_USRL, bias_8_l);
}

uint8_t ICM20948_set_GYRO_Z_BIAS(ICM20948* icm, int16_t bias)
{
	uint16_t bias_16_h = bias & 0b1111111100000000;
		bias_16_h >>=8;
	uint16_t bias_16_l = bias & 0b0000000011111111;

	uint8_t bias_8_h = (uint8_t)bias_16_h;
	uint8_t bias_8_l = (uint8_t)bias_16_l;

	ICM20948_set_register(icm, Bank2, ZG_OFFS_USRH, bias_8_h);
	ICM20948_set_register(icm, Bank2, ZG_OFFS_USRL, bias_8_l);
}




