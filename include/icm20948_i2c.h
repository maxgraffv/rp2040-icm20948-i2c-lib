#ifndef ICM20948_I2C_H
#define ICM20948_I2C_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdint.h>

/*
    User Bank 0
*/
#define WHO_AM_I 0x00
#define USER_CTRL 0x03
#define LP_CONFIG 0x05
#define PWR_MGMT_1 0x06
#define PWR_MGMT_2 0x07
#define INT_PIN_CFG 0x0F
#define INT_ENABLE 0x10
#define INT_ENABLE_1 0x11
#define INT_ENABLE_2 0x12
#define INT_ENABLE_3 0x13
#define I2C_MST_STATUS 0x17
#define INT_STATUS 0x19
#define INT_STATUS_1 0x1A
#define INT_STATUS_2 0x1B
#define INT_STATUS_3 0x1C
#define DELAY_TIMEH 0x28
#define DELAY_TIMEL 0x29
#define ACCEL_XOUT_H 0x2D
#define ACCEL_XOUT_L 0x2E
#define ACCEL_YOUT_H 0x2F
#define ACCEL_YOUT_L 0x30
#define ACCEL_ZOUT_H 0x31
#define ACCEL_ZOUT_L 0x32
#define GYRO_XOUT_H 0x33
#define GYRO_XOUT_L 0x34
#define GYRO_YOUT_H 0x35
#define GYRO_YOUT_L 0x36
#define GYRO_ZOUT_H 0x37
#define GYRO_ZOUT_L 0x38
#define TEMP_OUT_H 0x39
#define TEMP_OUT_L 0x3A
#define EXT_SLV_SENS_DATA_00 0x3B
#define EXT_SLV_SENS_DATA_01 0x3C
#define EXT_SLV_SENS_DATA_02 0x3D
#define EXT_SLV_SENS_DATA_03 0x3E
#define EXT_SLV_SENS_DATA_04 0x3F
#define EXT_SLV_SENS_DATA_05 0x40
#define EXT_SLV_SENS_DATA_06 0x41
#define EXT_SLV_SENS_DATA_07 0x42
#define EXT_SLV_SENS_DATA_08 0x43
#define EXT_SLV_SENS_DATA_09 0x44
#define EXT_SLV_SENS_DATA_10 0x45
#define EXT_SLV_SENS_DATA_11 0x46
#define EXT_SLV_SENS_DATA_12 0x47
#define EXT_SLV_SENS_DATA_13 0x48
#define EXT_SLV_SENS_DATA_14 0x49
#define EXT_SLV_SENS_DATA_15 0x4A
#define EXT_SLV_SENS_DATA_16 0x4B
#define EXT_SLV_SENS_DATA_17 0x4C
#define EXT_SLV_SENS_DATA_18 0x4D
#define EXT_SLV_SENS_DATA_19 0x4E
#define EXT_SLV_SENS_DATA_20 0x4F
#define EXT_SLV_SENS_DATA_21 0x50
#define EXT_SLV_SENS_DATA_22 0x51
#define EXT_SLV_SENS_DATA_23 0x52
#define FIFO_EN_1 0x66
#define FIFO_EN_2 0x67
#define FIFO_RST 0x68
#define FIFO_MODE 0x69
#define FIFO_COUNTH 0x70
#define FIFO_COUNTL 0x71
#define FIFO_R_W 0x72
#define DATA_RDY_STATUS 0x74
#define FIFO_CFG 0x76
#define REG_BANK_SEL 0x7F

/*
    User Bank 1
*/
#define SELF_TEST_X_GYRO 0x02
#define SELF_TEST_Y_GYRO 0x03
#define SELF_TEST_Z_GYRO 0x04
#define SELF_TEST_X_ACCEL 0x0E
#define SELF_TEST_Y_ACCEL 0x0F
#define SELF_TEST_Z_ACCEL 0x10
#define XA_OFFS_H 0x14
#define XA_OFFS_L 0x15
#define YA_OFFS_H 0x17
#define YA_OFFS_L 0x18
#define ZA_OFFS_H 0x1A
#define ZA_OFFS_L 0x1B
#define TIMEBASE_CORRECTION_PLL 0x28
#define REG_BANK_SEL_1 0x7F

/*
    User Bank 2
*/
#define GYRO_SMPLRT_DIV 0x00
#define GYRO_CONFIG_1 0x01
#define GYRO_CONFIG_2 0x02
#define XG_OFFS_USRH 0x03
#define XG_OFFS_USRL 0x04
#define YG_OFFS_USRH 0x05
#define YG_OFFS_USRL 0x06
#define ZG_OFFS_USRH 0x07
#define ZG_OFFS_USRL 0x08
#define ODR_ALIGN_EN 0x09
#define ACCEL_SMPLRT_DIV_1 0x10
#define ACCEL_SMPLRT_DIV_2 0x11
#define ACCEL_INTEL_CTRL 0x12
#define ACCEL_WOM_THR 0x13
#define ACCEL_CONFIG 0x14
#define ACCEL_CONFIG_2 0x15
#define FSYNC_CONFIG 0x52
#define TEMP_CONFIG 0x53
#define MOD_CTRL_USR 0x54
#define REG_BANK_SEL_2 0x7F

/*
    User Bank 3
*/
#define I2C_MST_ODR_CONFIG 0x00
#define I2C_MST_CTRL 0x01
#define I2C_MST_DELAY_CTRL 0x02
#define I2C_SLV0_ADDR 0x03
#define I2C_SLV0_REG 0x04
#define I2C_SLV0_CTRL 0x05
#define I2C_SLV0_DO 0x06
#define I2C_SLV1_ADDR 0x07
#define I2C_SLV1_REG 0x08
#define I2C_SLV1_CTRL 0x09
#define I2C_SLV1_DO 0x0A
#define I2C_SLV2_ADDR 0x0B
#define I2C_SLV2_REG 0x0C
#define I2C_SLV2_CTRL 0x0D
#define I2C_SLV2_DO 0x0E
#define I2C_SLV3_ADDR 0x0F
#define I2C_SLV3_REG 0x10
#define I2C_SLV3_CTRL 0x11
#define I2C_SLV3_DO 0x12
#define I2C_SLV4_ADDR 0x13
#define I2C_SLV4_REG 0x14
#define I2C_SLV4_CTRL 0x15
#define I2C_SLV4_DO 0x16
#define I2C_SLV4_DI 0x17
#define REG_BANK_SEL_3 0x7F

/*
    Magnetometer Registers
*/
#define MAG_WIA2 0x01
#define MAG_ST1 0x10
#define MAG_HXL 0x11
#define MAG_HXH 0x12
#define MAG_HYL 0x13
#define MAG_HYH 0x14
#define MAG_HZL 0x15
#define MAG_HZH 0x16
#define MAG_ST2 0x18
#define MAG_CNTL2 0x31
#define MAG_CNTL3 0x32

typedef enum{ 
	Bank0 = 0, 
	Bank1 = 1, 
	Bank2 = 2, 
	Bank3 = 3
} UserBank;

typedef struct{
	uint8_t i2c_address;
	uint8_t who_am_i_val;
	i2c_inst_t i2c;
}ICM20948;

int ICM20948_selectBank( ICM20948* icm, UserBank bank );

/*
    To initiate 
        USER_CTRL
        LP_CONFIG
        PWR_MGMT_1
        PWR_MGMT_2
        INT_PIN_CFG
        INT_ENABLE
        INT_ENABLE_1
        INT_ENABLE_2
        INT_ENABLE_3
        DELAY_TIMEH
        DELAY_TIMEL
        FIFO_EN_1
        FIFO_EN_2
        FIFO_RST
        FIFO_MODE
        FIFO_R_W
        FIFO_CFG
        SELF_TEST_X_GYRO
        SELF_TEST_Y_GYRO
        SELF_TEST_Z_GYRO
        SELF_TEST_X_ACCEL
        SELF_TEST_Y_ACCEL
        SELF_TEST_Z_ACCEL
        A_OFFS_H
        ...
        TIMEBASE_CORRECTION_PLL
        GYRO_SMPLRT_DIV
        GYRO_CONFIG_1
        GYRO_CONFIG_2
        G_OFFS_USR
        ...
        ODR_ALIGN_EN
        ACCEL_SMPLRT_DIV_1
        ACCEL_SMPLRT_DIV_2
        ACCEL_INTEL_CTRL
        ACCEL_WOM_THR
        ACCEL_CONFIG
        ACCEL_CONFIG_2
        FSYNC_CONFIG
        TEMP_CONFIG
        MOD_CTRL_USR

        I2C ???? - whole bank3

*/
uint8_t ICM20948_init(ICM20948* icm, uint8_t addr_pin_high, i2c_inst_t* i2c_chosen);

uint8_t ICM20948_who_am_i_check(ICM20948* icm);
uint8_t ICM20948_get_who_i_am(ICM20948* icm);

/*
	USER_CTRL
*/
uint8_t ICM20948_get_USER_CTRL(ICM20948* icm);
uint8_t ICM20948_set_USER_CTRL(ICM20948* icm, uint8_t val_to_set);

uint8_t ICM20948_set_DMP_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_FIFO_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_I2C_MST_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_I2C_IF_DIS(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_DMP_RST(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_SRAM_RST(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_I2C_MST_RST(ICM20948* icm, uint8_t enable_bit);

/*
	LP_CONFIG - Low Power Mode Configuration
*/
uint8_t ICM20948_get_LP_CONFIG(ICM20948* icm);
uint8_t ICM20948_set_LP_CONFIG(ICM20948* icm, uint8_t val_to_set);

uint8_t ICM20948_set_I2C_MST_CYCLE(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_ACCEL_CYCLE(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_GYRO_CYCLE(ICM20948* icm, uint8_t enable_bit);


/*
	PWR_MGMT_1 - Power Management Register 1
*/
uint8_t ICM20948_get_PWR_MGMT_1(ICM20948* icm);
uint8_t ICM20948_set_PWR_MGMT_1(ICM20948* icm, uint8_t val_to_set);

uint8_t ICM20948_set_DEVICE_RESET(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_SLEEP(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_LP_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_TEMP_DIS(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_CLKSEL(ICM20948* icm, uint8_t enable_bit);

/*
	PWR_MGMT_2 - Power Management Register 2
*/
uint8_t ICM20948_get_PWR_MGMT_2(ICM20948* icm);
uint8_t ICM20948_set_PWR_MGMT_2(ICM20948* icm, uint8_t val_to_set);

uint8_t ICM20948_set_DISABLE_ACCEL(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_DISABLE_GYRO(ICM20948* icm, uint8_t enable_bit);

/*
	INT_PIN_CONFIG
*/
uint8_t ICM20948_get_INT_PIN_CONFIG(ICM20948* icm);
uint8_t ICM20948_set_INT_PIN_CONFIG(ICM20948* icm, uint8_t val_to_set);

uint8_t ICM20948_set_INT1_ACTL(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_INT1_OPEN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_INT1_LATCH_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_INT_ANYRD_2CLEAR(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_ACTL_FSYNC(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_FSYNC_INT_MODE_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_BYPASS_EN(ICM20948* icm, uint8_t enable_bit);

/*
	INT_ENABLE
*/
uint8_t ICM20948_get_INT_ENABLE(ICM20948* icm);
uint8_t ICM20948_set_INT_ENABLE(ICM20948* icm, uint8_t val_to_set);

uint8_t ICM20948_set_REG_WOF_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_WOM_INT_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_PLL_RDY_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_DMP_INT1_EN(ICM20948* icm, uint8_t enable_bit);
uint8_t ICM20948_set_I2C_MST_INT_EN(ICM20948* icm, uint8_t enable_bit);

/*
	INT_ENABLE_1
*/
uint8_t ICM20948_get_INT_ENABLE_1(ICM20948* icm);
uint8_t ICM20948_set_INT_ENABLE_1(ICM20948* icm, uint8_t val_to_set);

/*
	INT_ENABLE_2
*/
uint8_t ICM20948_get_INT_ENABLE_2(ICM20948* icm);
uint8_t ICM20948_set_INT_ENABLE_2(ICM20948* icm, uint8_t val_to_set);

/*
	INT_ENABLE_3
*/
uint8_t ICM20948_get_INT_ENABLE_3(ICM20948* icm);
uint8_t ICM20948_set_INT_ENABLE_3(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_MST_STATUS
*/
uint8_t ICM20948_get_I2C_MST_STATUS(ICM20948* icm);

/*
    INT_STATUS
*/
uint8_t ICM20948_get_INT_STATUS(ICM20948* icm);

/*
    INT_STATUS_1
*/
uint8_t ICM20948_get_INT_STATUS_1(ICM20948* icm);

/*
    INT_STATUS_2
*/
uint8_t ICM20948_get_INT_STATUS_2(ICM20948* icm);

/*
    INT_STATUS_3
*/
uint8_t ICM20948_get_INT_STATUS_3(ICM20948* icm);

/*
    DELAY_TIMEH
*/
uint8_t ICM20948_get_DELAY_TIMEH(ICM20948* icm);
void ICM20948_set_DELAY_TIMEH(ICM20948* icm, uint8_t val_to_set);

/*
    DELAY_TIMEL
*/
uint8_t ICM20948_get_DELAY_TIMEL(ICM20948* icm);
void ICM20948_set_DELAY_TIMEL(ICM20948* icm, uint8_t val_to_set);

/*
    ACCEL
*/
uint8_t ICM20948_get_ACCEL_X(ICM20948* icm);
uint8_t ICM20948_get_ACCEL_Y(ICM20948* icm);
uint8_t ICM20948_get_ACCEL_Z(ICM20948* icm);

/*
	GYRO
*/
uint8_t ICM20948_get_GYRO_X(ICM20948* icm);
uint8_t ICM20948_get_GYRO_Y(ICM20948* icm);
uint8_t ICM20948_get_GYRO_Z(ICM20948* icm);

/*
	TEMP
*/
float ICM20948_get_TEMP(ICM20948* icm);
uint8_t ICM20948_get_TEMP_OUT_H(ICM20948* icm);
uint8_t ICM20948_get_TEMP_OUT_L(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_00
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_00(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_01
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_01(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_02
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_02(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_03
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_03(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_04
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_04(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_05
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_05(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_06
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_06(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_07
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_07(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_08
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_08(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_09
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_09(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_10
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_10(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_11
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_11(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_12
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_12(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_13
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_13(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_14
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_14(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_15
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_15(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_16
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_16(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_17
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_17(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_18
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_18(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_19
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_19(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_20
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_20(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_21
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_21(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_22
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_22(ICM20948* icm);

/*
    EXT_SLV_SENS_DATA_23
*/
uint8_t ICM20948_get_EXT_SLV_SENS_DATA_23(ICM20948* icm);

/*
    FIFO_EN_1
*/
uint8_t ICM20948_get_FIFO_EN_1(ICM20948* icm);
void ICM20948_set_FIFO_EN_1(ICM20948* icm, uint8_t val_to_set);

/*
    FIFO_EN_2
*/
uint8_t ICM20948_get_FIFO_EN_2(ICM20948* icm);
void ICM20948_set_FIFO_EN_2(ICM20948* icm, uint8_t val_to_set);

/*
    FIFO_RST
*/
uint8_t ICM20948_get_FIFO_RST(ICM20948* icm);
void ICM20948_set_FIFO_RST(ICM20948* icm, uint8_t val_to_set);

/*
    FIFO_MODE
*/
uint8_t ICM20948_get_FIFO_MODE(ICM20948* icm);
void ICM20948_set_FIFO_MODE(ICM20948* icm, uint8_t val_to_set);

/*
    FIFO_COUNTH
*/
uint8_t ICM20948_get_FIFO_COUNTH(ICM20948* icm);

/*
    FIFO_COUNTL
*/
uint8_t ICM20948_get_FIFO_COUNTL(ICM20948* icm);

/*
    FIFO_R_W
*/
uint8_t ICM20948_get_FIFO_R_W(ICM20948* icm);
void ICM20948_set_FIFO_R_W(ICM20948* icm, uint8_t val_to_set);

/*
    DATA_RDY_STATUS
*/
uint8_t ICM20948_get_DATA_RDY_STATUS(ICM20948* icm);

/*
    FIFO_CFG
*/
uint8_t ICM20948_get_FIFO_CFG(ICM20948* icm);
void ICM20948_set_FIFO_CFG(ICM20948* icm, uint8_t val_to_set);

/*
    SELF_TEST_X_GYRO
*/
uint8_t ICM20948_get_SELF_TEST_X_GYRO(ICM20948* icm);
void ICM20948_set_SELF_TEST_X_GYRO(ICM20948* icm, uint8_t val_to_set);

/*
    SELF_TEST_Y_GYRO
*/
uint8_t ICM20948_get_SELF_TEST_Y_GYRO(ICM20948* icm);
void ICM20948_set_SELF_TEST_Y_GYRO(ICM20948* icm, uint8_t val_to_set);

/*
    SELF_TEST_Z_GYRO
*/
uint8_t ICM20948_get_SELF_TEST_Z_GYRO(ICM20948* icm);
void ICM20948_set_SELF_TEST_Z_GYRO(ICM20948* icm, uint8_t val_to_set);

/*
    SELF_TEST_X_ACCEL
*/
uint8_t ICM20948_get_SELF_TEST_X_ACCEL(ICM20948* icm);
void ICM20948_set_SELF_TEST_X_ACCEL(ICM20948* icm, uint8_t val_to_set);

/*
    SELF_TEST_Y_ACCEL
*/
uint8_t ICM20948_get_SELF_TEST_Y_ACCEL(ICM20948* icm);
void ICM20948_set_SELF_TEST_Y_ACCEL(ICM20948* icm, uint8_t val_to_set);

/*
    SELF_TEST_Z_ACCEL
*/
uint8_t ICM20948_get_SELF_TEST_Z_ACCEL(ICM20948* icm);
void ICM20948_set_SELF_TEST_Z_ACCEL(ICM20948* icm, uint8_t val_to_set);

/*
    XA_OFFS_H
*/
uint8_t ICM20948_get_XA_OFFS_H(ICM20948* icm);
void ICM20948_set_XA_OFFS_H(ICM20948* icm, uint8_t val_to_set);

/*
    XA_OFFS_L
*/
uint8_t ICM20948_get_XA_OFFS_L(ICM20948* icm);
void ICM20948_set_XA_OFFS_L(ICM20948* icm, uint8_t val_to_set);

/*
    YA_OFFS_H
*/
uint8_t ICM20948_get_YA_OFFS_H(ICM20948* icm);
void ICM20948_set_YA_OFFS_H(ICM20948* icm, uint8_t val_to_set);

/*
    YA_OFFS_L
*/
uint8_t ICM20948_get_YA_OFFS_L(ICM20948* icm);
void ICM20948_set_YA_OFFS_L(ICM20948* icm, uint8_t val_to_set);

/*
    ZA_OFFS_H
*/
uint8_t ICM20948_get_ZA_OFFS_H(ICM20948* icm);
void ICM20948_set_ZA_OFFS_H(ICM20948* icm, uint8_t val_to_set);

/*
    ZA_OFFS_L
*/
uint8_t ICM20948_get_ZA_OFFS_L(ICM20948* icm);
void ICM20948_set_ZA_OFFS_L(ICM20948* icm, uint8_t val_to_set);

/*
    TIMEBASE_CORRECTION_PLL
*/
uint8_t ICM20948_get_TIMEBASE_CORRECTION_PLL(ICM20948* icm);
void ICM20948_set_TIMEBASE_CORRECTION_PLL(ICM20948* icm, uint8_t val_to_set);

/*
    GYRO_SMPLRT_DIV
*/
uint8_t ICM20948_get_GYRO_SMPLRT_DIV(ICM20948* icm);
void ICM20948_set_GYRO_SMPLRT_DIV(ICM20948* icm, uint8_t val_to_set);

/*
    GYRO_CONFIG_1
*/
uint8_t ICM20948_get_GYRO_CONFIG_1(ICM20948* icm);
void ICM20948_set_GYRO_CONFIG_1(ICM20948* icm, uint8_t val_to_set);

/*
    GYRO_CONFIG_2
*/
uint8_t ICM20948_get_GYRO_CONFIG_2(ICM20948* icm);
void ICM20948_set_GYRO_CONFIG_2(ICM20948* icm, uint8_t val_to_set);

/*
    XG_OFFS_USRH
*/
uint8_t ICM20948_get_XG_OFFS_USRH(ICM20948* icm);
void ICM20948_set_XG_OFFS_USRH(ICM20948* icm, uint8_t val_to_set);

/*
    XG_OFFS_USRL
*/
uint8_t ICM20948_get_XG_OFFS_USRL(ICM20948* icm);
void ICM20948_set_XG_OFFS_USRL(ICM20948* icm, uint8_t val_to_set);

/*
    YG_OFFS_USRH
*/
uint8_t ICM20948_get_YG_OFFS_USRH(ICM20948* icm);
void ICM20948_set_YG_OFFS_USRH(ICM20948* icm, uint8_t val_to_set);

/*
    YG_OFFS_USRL
*/
uint8_t ICM20948_get_YG_OFFS_USRL(ICM20948* icm);
void ICM20948_set_YG_OFFS_USRL(ICM20948* icm, uint8_t val_to_set);

/*
    ZG_OFFS_USRH
*/
uint8_t ICM20948_get_ZG_OFFS_USRH(ICM20948* icm);
void ICM20948_set_ZG_OFFS_USRH(ICM20948* icm, uint8_t val_to_set);

/*
    ZG_OFFS_USRL
*/
uint8_t ICM20948_get_ZG_OFFS_USRL(ICM20948* icm);
void ICM20948_set_ZG_OFFS_USRL(ICM20948* icm, uint8_t val_to_set);

/*
    ODR_ALIGN_EN
*/
uint8_t ICM20948_get_ODR_ALIGN_EN(ICM20948* icm);
void ICM20948_set_ODR_ALIGN_EN(ICM20948* icm, uint8_t val_to_set);

/*
    ACCEL_SMPLRT_DIV_1
*/
uint8_t ICM20948_get_ACCEL_SMPLRT_DIV_1(ICM20948* icm);
void ICM20948_set_ACCEL_SMPLRT_DIV_1(ICM20948* icm, uint8_t val_to_set);

/*
    ACCEL_SMPLRT_DIV_2
*/
uint8_t ICM20948_get_ACCEL_SMPLRT_DIV_2(ICM20948* icm);
void ICM20948_set_ACCEL_SMPLRT_DIV_2(ICM20948* icm, uint8_t val_to_set);

/*
    ACCEL_INTEL_CTRL
*/
uint8_t ICM20948_get_ACCEL_INTEL_CTRL(ICM20948* icm);
void ICM20948_set_ACCEL_INTEL_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    ACCEL_WOM_THR
*/
uint8_t ICM20948_get_ACCEL_WOM_THR(ICM20948* icm);
void ICM20948_set_ACCEL_WOM_THR(ICM20948* icm, uint8_t val_to_set);

/*
    ACCEL_CONFIG
*/
uint8_t ICM20948_get_ACCEL_CONFIG(ICM20948* icm);
void ICM20948_set_ACCEL_CONFIG(ICM20948* icm, uint8_t val_to_set);

/*
    ACCEL_CONFIG_2
*/
uint8_t ICM20948_get_ACCEL_CONFIG_2(ICM20948* icm);
void ICM20948_set_ACCEL_CONFIG_2(ICM20948* icm, uint8_t val_to_set);

/*
    FSYNC_CONFIG
*/
uint8_t ICM20948_get_FSYNC_CONFIG(ICM20948* icm);
void ICM20948_set_FSYNC_CONFIG(ICM20948* icm, uint8_t val_to_set);

/*
    TEMP_CONFIG
*/
uint8_t ICM20948_get_TEMP_CONFIG(ICM20948* icm);
void ICM20948_set_TEMP_CONFIG(ICM20948* icm, uint8_t val_to_set);

/*
    MOD_CTRL_USR
*/
uint8_t ICM20948_get_MOD_CTRL_USR(ICM20948* icm);
void ICM20948_set_MOD_CTRL_USR(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_MST_ODR_CONFIG
*/
uint8_t ICM20948_get_I2C_MST_ODR_CONFIG(ICM20948* icm);
void ICM20948_set_I2C_MST_ODR_CONFIG(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_MST_CTRL
*/
uint8_t ICM20948_get_I2C_MST_CTRL(ICM20948* icm);
void ICM20948_set_I2C_MST_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_MST_DELAY_CTRL
*/
uint8_t ICM20948_get_I2C_MST_DELAY_CTRL(ICM20948* icm);
void ICM20948_set_I2C_MST_DELAY_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV0_ADDR
*/
uint8_t ICM20948_get_I2C_SLV0_ADDR(ICM20948* icm);
void ICM20948_set_I2C_SLV0_ADDR(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV0_REG
*/
uint8_t ICM20948_get_I2C_SLV0_REG(ICM20948* icm);
void ICM20948_set_I2C_SLV0_REG(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV0_CTRL
*/
uint8_t ICM20948_get_I2C_SLV0_CTRL(ICM20948* icm);
void ICM20948_set_I2C_SLV0_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV0_DO
*/
uint8_t ICM20948_get_I2C_SLV0_DO(ICM20948* icm);
void ICM20948_set_I2C_SLV0_DO(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV1_ADDR
*/
uint8_t ICM20948_get_I2C_SLV1_ADDR(ICM20948* icm);
void ICM20948_set_I2C_SLV1_ADDR(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV1_REG
*/
uint8_t ICM20948_get_I2C_SLV1_REG(ICM20948* icm);
void ICM20948_set_I2C_SLV1_REG(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV1_CTRL
*/
uint8_t ICM20948_get_I2C_SLV1_CTRL(ICM20948* icm);
void ICM20948_set_I2C_SLV1_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV1_DO
*/
uint8_t ICM20948_get_I2C_SLV1_DO(ICM20948* icm);
void ICM20948_set_I2C_SLV1_DO(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV2_ADDR
*/
uint8_t ICM20948_get_I2C_SLV2_ADDR(ICM20948* icm);
void ICM20948_set_I2C_SLV2_ADDR(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV2_REG
*/
uint8_t ICM20948_get_I2C_SLV2_REG(ICM20948* icm);
void ICM20948_set_I2C_SLV2_REG(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV2_CTRL
*/
uint8_t ICM20948_get_I2C_SLV2_CTRL(ICM20948* icm);
void ICM20948_set_I2C_SLV2_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV2_DO
*/
uint8_t ICM20948_get_I2C_SLV2_DO(ICM20948* icm);
void ICM20948_set_I2C_SLV2_DO(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV3_ADDR
*/
uint8_t ICM20948_get_I2C_SLV3_ADDR(ICM20948* icm);
void ICM20948_set_I2C_SLV3_ADDR(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV3_REG
*/
uint8_t ICM20948_get_I2C_SLV3_REG(ICM20948* icm);
void ICM20948_set_I2C_SLV3_REG(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV3_CTRL
*/
uint8_t ICM20948_get_I2C_SLV3_CTRL(ICM20948* icm);
void ICM20948_set_I2C_SLV3_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV3_DO
*/
uint8_t ICM20948_get_I2C_SLV3_DO(ICM20948* icm);
void ICM20948_set_I2C_SLV3_DO(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV4_ADDR
*/
uint8_t ICM20948_get_I2C_SLV4_ADDR(ICM20948* icm);
void ICM20948_set_I2C_SLV4_ADDR(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV4_REG
*/
uint8_t ICM20948_get_I2C_SLV4_REG(ICM20948* icm);
void ICM20948_set_I2C_SLV4_REG(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV4_CTRL
*/
uint8_t ICM20948_get_I2C_SLV4_CTRL(ICM20948* icm);
void ICM20948_set_I2C_SLV4_CTRL(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV4_DO
*/
uint8_t ICM20948_get_I2C_SLV4_DO(ICM20948* icm);
void ICM20948_set_I2C_SLV4_DO(ICM20948* icm, uint8_t val_to_set);

/*
    I2C_SLV4_DI
*/
uint8_t ICM20948_get_I2C_SLV4_DI(ICM20948* icm);


	
#endif