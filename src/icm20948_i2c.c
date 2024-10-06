#include "icm20948_i2c.h"
#include "stdio.h"
#include <stdlib.h>

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
	i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, &reg_addr, 1, 1);
	i2c_read_blocking(icm->i2c_ptr, icm->i2c_address, &reg_val, 1, 0);

	return reg_val;
}

uint8_t ICM20948_set_register(ICM20948* icm, UserBank bank, uint8_t reg_addr, uint8_t value)
{
	ICM20948_selectBank(icm, bank);

	uint8_t buffer[2] = {reg_addr, value};

	int result = i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, buffer, 2, 0);

	// i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, &reg_addr, 1, 1);
	// i2c_write_blocking(icm->i2c_ptr, icm->i2c_address, &value, 1, 0);
	if (result == PICO_ERROR_GENERIC) {
        printf("Error writing to register 0x%02X\n", reg_addr);
        return 0;  // Indicate failure
    }

	return 1;
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

	return icm_ptr;
}

uint8_t ICM20948_get_who_am_i(ICM20948* icm)
{
	uint8_t data_read = ICM20948_get_register(icm, Bank0, WHO_AM_I);
	
	return data_read;
}

uint8_t ICM20948_who_am_i_check(ICM20948* icm)
{
	uint8_t who_am_i_val_read = ICM20948_get_who_am_i(icm);
	uint8_t who_am_i_val_isOK = 0x00;
	if(who_am_i_val_read == icm->who_am_i_val)
		who_am_i_val_isOK = 1;

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

	if(ICM20948_isSleepMode(icm))
		printf("ICM is in sleep mode\n");
	else
		printf("ICM not in sleep mode\n");

	printf("Getting Raw Gyro \n");
	ICM20948_get_GYRO_X_raw(icm);
	ICM20948_GYRO_defaultInit(icm);
	printf("Waking... \n");
	ICM20948_Sleep_enable(icm, 0);

	if(ICM20948_isSleepMode(icm))
		printf("ICM is in sleep mode\n");
	else
		printf("ICM not in sleep mode\n");

	for(int i = 0; i < 20; i++)
	{
		ICM20948_get_GYRO_X_raw(icm);
		sleep_ms(5000);
	}

	return 1;
}

uint8_t ICM20948_isSleepMode(ICM20948* icm)
{
	uint8_t isSleepMode = ICM20948_get_register(icm, Bank0, PWR_MGMT_1) & 0b01000000;

	return isSleepMode;
}

uint8_t ICM20948_Sleep_enable(ICM20948* icm, uint8_t enableSleep)
{
	uint8_t PWR_MGMT_1_val = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);
	printf("PWR MGMT 1 val read: %d\n", PWR_MGMT_1_val);

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

	printf("PWR MGMT 1 val wrtn: %d\n", PWR_MGMT_1_val);
	ICM20948_set_register(icm, Bank0, PWR_MGMT_1, PWR_MGMT_1_val);
	
	uint8_t PWR_MGMT_1_val2 = ICM20948_get_register(icm, Bank0, PWR_MGMT_1);
	printf("PWR MGMT 1 val read: %d\n", PWR_MGMT_1_val2);

	return 1;
}


uint8_t ICM20948_GYRO_defaultInit(ICM20948* icm)
{

	uint8_t GYRO_SMPLRT_DIV_val = 0x00; //Gyro samplerate divider
	uint8_t GYRO_CONFIG_1_val = 0x00;
	uint8_t GYRO_CONFIG_2_val = 0x00;
	uint8_t XG_OFFS_USRH_val = 0x00;
	uint8_t XG_OFFS_USRL_val = 0x00;
	uint8_t YG_OFFS_USRH_val = 0x00;
	uint8_t YG_OFFS_USRL_val = 0x00;
	uint8_t ZG_OFFS_USRH_val = 0x00;
	uint8_t ZG_OFFS_USRL_val = 0x00;
	uint8_t ODR_ALIGN_EN_val = 0x00; //ODR calculated as = 1.1KHz/(1+GYROSMPLRT_DIV)

	ICM20948_Sleep_enable(icm, 0);

	/*
		FS_SEL | FullScale Range (degrees/sec) | Sensitivity 
		00			250								131
		01			500								65.5
		10			1000							32.8
		11			2000							16.4
	*/

	GYRO_CONFIG_1_val = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_1);

	//Choosing 151.8 as Low Pass Filter so DLPFCFG = 2;
	GYRO_CONFIG_1_val &= ~(1<<GYRO_CONFIG_1_GYRO_DLPFCFG_2);
	GYRO_CONFIG_1_val |= (1<<GYRO_CONFIG_1_GYRO_DLPFCFG_1);
	GYRO_CONFIG_1_val &= ~(1<<GYRO_CONFIG_1_GYRO_DLPFCFG_0);

	//FCHOICE = 1
	GYRO_CONFIG_1_val |= (1<<GYRO_CONFIG_1_GYRO_FCHOICE);

	//FS_SEL = 10
	GYRO_CONFIG_1_val |= (1<<GYRO_CONFIG_1_GYRO_FS_SEL_1);
	GYRO_CONFIG_1_val &= ~(1<<GYRO_CONFIG_1_GYRO_FS_SEL_0);

	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_1, GYRO_CONFIG_1_val);


	
	GYRO_CONFIG_2_val = ICM20948_get_register(icm, Bank2, GYRO_CONFIG_2);
	GYRO_CONFIG_2_val &= 0b11000000;
	ICM20948_set_register(icm, Bank2, GYRO_CONFIG_2, GYRO_CONFIG_2_val);




	return 1;
}

//GYRO SELF-TEST

//ACCEL DEFAULT INIT

//ACCEL SELF-TEST


//TEMP DEFAULT INIT















uint16_t ICM20948_get_GYRO_X_raw(ICM20948* icm)
{
	uint8_t GYRO_XOUT_H_reg = GYRO_XOUT_H;
	uint8_t GYRO_XOUT_L_reg = GYRO_XOUT_L;
	uint8_t gyro_x_H = 0x00;
	uint8_t gyro_x_L = 0x00;
	uint16_t gyro_x_raw = 0x00;

    
	float gyro_sensitivity = 131;
	
	gyro_x_H = ICM20948_get_register(icm, Bank0, GYRO_XOUT_H_reg);
	gyro_x_L = ICM20948_get_register(icm, Bank0, GYRO_XOUT_L_reg);
	printf("GYRO L: %d\n", gyro_x_H);
	printf("GYRO H: %d\n", gyro_x_L);

	gyro_x_raw |= (gyro_x_H<<8);
	gyro_x_raw |= gyro_x_L;
	printf("GYRO RAW: %d\n", gyro_x_raw);

	// x_deg = ((float)gyro_x_raw)/gyro_sensitivity;

	return gyro_x_raw;
}
