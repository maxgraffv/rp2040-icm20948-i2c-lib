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


	ICM20948_SleepMode_disable(icm);
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

uint8_t ICM20948_DMP_reset(ICM20948*)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_DMP_RST);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_SRAM_reset(ICM20948*)
{
	uint8_t usr_ctrl = ICM20948_get_register(icm, Bank0, USER_CTRL);
	usr_ctrl |= (1<<USER_CTRL_SRAM_RST);
	ICM20948_set_register(icm, Bank0, USER_CTRL, usr_ctrl);

	return 1;
}

uint8_t ICM20948_I2C_MST_reset(ICM20948*)
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

uint8_t ICM20948_FIFO_OVERFLOW_INT_enable(ICM20948* icm);
{
	uint8_t int_enable_2 = ICM20948_get_register(icm, Bank0, INT_ENABLE_2);
	int_enable_2 |= (1<<INT_ENABLE_2_FIFO_OVERFLOW_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_2, int_enable_2);

	return 1;
}

uint8_t ICM20948_FIFO_OVERFLOW_INT_disable(ICM20948* icm);
{
	uint8_t int_enable_2 = ICM20948_get_register(icm, Bank0, INT_ENABLE_2);
	int_enable_2 &= ~(1<<INT_ENABLE_2_FIFO_OVERFLOW_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_2, int_enable_2);

	return 1;
}

uint8_t ICM20948_FIFO_WATERMARK_INT_enable(ICM20948* icm);
{
	uint8_t int_enable_3 = ICM20948_get_register(icm, Bank0, INT_ENABLE_3);
	int_enable_3 |= (1<<INT_ENABLE_3_FIFO_WM_EN);
	ICM20948_set_register(icm, Bank0, INT_ENABLE_3, int_enable_3);

	return 1;
}

uint8_t ICM20948_FIFO_WATERMARK_INT_disable(ICM20948* icm);
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
	int16_t accel_x_raw = (int16_t)ICM20948_get_register_16b(icm, Bank0, ACCEL_ZOUT_H, ACCEL_ZOUT_L);

	return accel_y_raw;
}








































