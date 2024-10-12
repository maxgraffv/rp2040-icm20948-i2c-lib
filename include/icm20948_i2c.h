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

//WHO_AM_I Register
#define WHO_AM_I_VALUE 0xEA

// USER_CTRL Register (0x03)
#define USER_CTRL_DMP_EN         7  // Enable DMP
#define USER_CTRL_FIFO_EN        6  // Enable FIFO
#define USER_CTRL_I2C_MST_EN     5  // Enable I2C Master Module
#define USER_CTRL_I2C_IF_DIS     4  // Disable I2C interface, enable SPI Mode Only
#define USER_CTRL_DMP_RST        3  // Reset DMP
#define USER_CTRL_SRAM_RST       2  // Reset SRAM
#define USER_CTRL_I2C_MST_RST    1  // Reset I2C Master

// LP_CONFIG Register (0x05)
#define LP_CONFIG_I2C_MST_CYCLE  6  // I2C Master cycle mode
#define LP_CONFIG_ACCEL_CYCLE    5  // Accelerometer in duty cycle mode
#define LP_CONFIG_GYRO_CYCLE     4  // Gyroscope in duty cycle mode

// PWR_MGMT_1 Register (0x06)
#define PWR_MGMT_1_DEVICE_RESET  7  // Reset the entire device
#define PWR_MGMT_1_SLEEP         6  // Put device to sleep
#define PWR_MGMT_1_LP_EN         5  // Enable low power mode
#define PWR_MGMT_1_TEMP_DIS      3  // Disable temperature sensor
#define PWR_MGMT_1_CLKSEL2       2  // Clock source select bit 2
#define PWR_MGMT_1_CLKSEL1       1  // Clock source select bit 1
#define PWR_MGMT_1_CLKSEL0       0  // Clock source select bit 0

// PWR_MGMT_2 Register (0x07)
#define PWR_MGMT_2_DISABLE_XA    5  // Disable accelerometer X-axis
#define PWR_MGMT_2_DISABLE_YA    4  // Disable accelerometer Y-axis
#define PWR_MGMT_2_DISABLE_ZA    3  // Disable accelerometer Z-axis
#define PWR_MGMT_2_DISABLE_XG    2  // Disable gyroscope X-axis
#define PWR_MGMT_2_DISABLE_YG    1  // Disable gyroscope Y-axis
#define PWR_MGMT_2_DISABLE_ZG    0  // Disable gyroscope Z-axis

// INT_PIN_CFG Register (0x0F)
#define INT_PIN_CFG_ACTL         7  // Active low for INT pin
#define INT_PIN_CFG_OPEN         6  // Open drain INT pin
#define INT_PIN_CFG_LATCH_INT_EN 5  // Latch INT pin until cleared
#define INT_PIN_CFG_INT_ANYRD_2CLEAR 4  // Clear INT status on any read
#define INT_PIN_CFG_ACTL_FSYNC	 3	// FSYNC pin as interrupt is active low
#define INT_PIN_CFG_FSYNC_INT_MODE_EN 2 // FSYNC pin used as an Interrupt
#define INT_PIN_CFG_BYPASS_EN 	 1 // BYPASS MODE enable

// INT_ENABLE Register (0x10)
#define INT_ENABLE_REG_WOF_EN    7  // Enable wake on FSYNC interrupt
#define INT_ENABLE_WOM_INT_EN    3  // Enable wake on motion interrupt
#define INT_ENABLE_PLL_RDY_EN    2  // Enable PLLY RDY
#define INT_ENABLE_DMP_INT1_EN	 1  // Enable DMP interrupt to pin 1
#define INT_ENABLE_I2C_MST_INT_EN 0 //Enable I2C master Interrupt to pin 1

// INT_ENABLE_1 Register (0x11)
#define INT_ENABLE_1_RAW_DATA_0RDY_EN 0  // Enable raw data ready interrupt to pin 1

// INT_ENABLE_2 Register (0x12)
#define INT_ENABLE_2_FIFO_OVERFLOW_EN 3  // Enable FIFO overflow interrupt to pin 1

// INT_ENABLE_3 Register (0x13)
#define INT_ENABLE_3_FIFO_WM_EN  0  // Enable FIFO watermark interrupt to pin 1

// FIFO_EN_1 Register (0x66)
#define FIFO_EN_1_SLV_3_FIFO_EN  3  // Enable Slave 3 data in FIFO
#define FIFO_EN_1_SLV_2_FIFO_EN  2  // Enable Slave 2 data in FIFO
#define FIFO_EN_1_SLV_1_FIFO_EN  1  // Enable Slave 1 data in FIFO
#define FIFO_EN_1_SLV_0_FIFO_EN  0  // Enable Slave 0 data in FIFO

// FIFO_EN_2 Register (0x67)
#define FIFO_EN_2_ACCEL_FIFO_EN  4  // Enable accelerometer data in FIFO
#define FIFO_EN_2_GYRO_Z_FIFO_EN 3  // Enable gyroscope Z-axis data in FIFO
#define FIFO_EN_2_GYRO_Y_FIFO_EN 2  // Enable gyroscope Y-axis data in FIFO
#define FIFO_EN_2_GYRO_X_FIFO_EN 1  // Enable gyroscope X-axis data in FIFO
#define FIFO_EN_2_TEMP_FIFO_EN   0  // Enable temperature data in FIFO

// FIFO_RST Register (0x68)
#define FIFO_RST_FIFO_RESET_4      4  // Reset FIFO
#define FIFO_RST_FIFO_RESET_3      3  // Reset FIFO
#define FIFO_RST_FIFO_RESET_2      2  // Reset FIFO
#define FIFO_RST_FIFO_RESET_1      1  // Reset FIFO
#define FIFO_RST_FIFO_RESET_0      0  // Reset FIFO

// FIFO_MODE Register (0x69)
#define FIFO_MODE_STREAM_MODE    0  // FIFO stream mode

// FIFO_CFG Register (0x76)
#define FIFO_CFG_FIFO_CFG        0  // FIFO configuration, stop on full

// ACCEL_CONFIG Register (0x14, Bank 2)
#define ACCEL_CONFIG_ACCEL_FS_SEL1 4  // Accelerometer full-scale select bit 1
#define ACCEL_CONFIG_ACCEL_FS_SEL0 3  // Accelerometer full-scale select bit 0

//GYRO_CONFIG_1 Register
#define GYRO_CONFIG_1_GYRO_DLPFCFG_2 5 // Gyro low Pass Filter 2
#define GYRO_CONFIG_1_GYRO_DLPFCFG_1 4 // Gyro low Pass Filter 1
#define GYRO_CONFIG_1_GYRO_DLPFCFG_0 3 // Gyro low Pass Filter 0
#define GYRO_CONFIG_1_GYRO_FS_SEL_1	 2 //Gyro Full Scale Select 1
#define GYRO_CONFIG_1_GYRO_FS_SEL_0	 1 //Gyro Full Scale Select 0
#define GYRO_CONFIG_1_GYRO_FCHOICE 	 0 // Enable gyro DLPF

//GYRO_CONFIG_2 Register
#define GYRO_CONFIG_2_XGYRO_CTEN	 5 // X Gyro Self-Test Enable
#define GYRO_CONFIG_2_YGYRO_CTEN	 4 // Y Gyro Self-Test Enable
#define GYRO_CONFIG_2_ZGYRO_CTEN	 3 // Z Gyro Self-Test Enable
#define GYRO_CONFIG_2_GYRO_AVGCFG_2  2 // Avrg filter Config for LP mode	 
#define GYRO_CONFIG_2_GYRO_AVGCFG_1  1 // Avrg filter Config for LP mode	 
#define GYRO_CONFIG_2_GYRO_AVGCFG_0  0 // Avrg filter Config for LP mode	 

//ODR_ALIGN_EN Register
#define ODR_ALIGN_EN_bit 	0 // Enable ODR start-time alignment



// ACCEL_CONFIG_2 Register (0x15, Bank 2)
#define ACCEL_CONFIG_2_DEC2_CFG  4  // Decimator for low-power mode

typedef enum{ 
	Bank0 = 0, 
	Bank1 = 1, 
	Bank2 = 2, 
	Bank3 = 3
} UserBank;

typedef struct{
	uint8_t i2c_address;
	uint8_t who_am_i_val;
	i2c_inst_t* i2c_ptr;
}ICM20948;

uint8_t ICM20948_selectBank( ICM20948* icm, UserBank bank );//ok

ICM20948* createICM20948( i2c_inst_t* i2c_chosen_ptr, uint8_t addr_pin_high );//ok
uint8_t ICM20948_Init(ICM20948* icm);//ok

uint8_t ICM20948_get_register(ICM20948* icm, UserBank bank, uint8_t reg_addr);//ok
uint8_t ICM20948_set_register(ICM20948* icm, UserBank bank, uint8_t reg_addr, uint8_t value);//ok
uint16_t ICM20948_get_register_16b(ICM20948* icm, UserBank bank, uint8_t reg_addr_H, uint8_t reg_addr_L);//ok

/*
    WHO_AM_I
*/
uint8_t ICM20948_get_who_am_i(ICM20948* icm);
uint8_t ICM20948_who_am_i_check(ICM20948* icm);

/*
	USER_CTRL
*/
uint8_t ICM20948_DMP_enable(ICM20948*);
uint8_t ICM20948_DMP_disable(ICM20948*);

uint8_t ICM20948_FIFO_enable(ICM20948*);
uint8_t ICM20948_FIFO_disable(ICM20948*);

uint8_t ICM20948_I2C_MST_enable(ICM20948*);
uint8_t ICM20948_I2C_MST_disable(ICM20948*);

uint8_t ICM20948_SPI_MODE_ONLY_enable(ICM20948*);
uint8_t ICM20948_SPI_MODE_ONLY_disable(ICM20948*);

uint8_t ICM20948_DMP_reset(ICM20948*);
uint8_t ICM20948_SRAM_reset(ICM20948*);
uint8_t ICM20948_I2C_MST_reset(ICM20948*);



/*
	LP_CONFIG - Low Power Mode Configuration
*/
uint8_t ICM20948_I2C_MST_CYCLE_enable(ICM20948*);
uint8_t ICM20948_I2C_MST_CYCLE_disable(ICM20948*);

uint8_t ICM20948_ACCEL_CYCLE_enable(ICM20948*);
uint8_t ICM20948_ACCEL_CYCLE_disable(ICM20948*);

uint8_t ICM20948_GYRO_CYCLE_enable(ICM20948*);
uint8_t ICM20948_GYRO_CYCLE_disable(ICM20948*);

/*
	PWR_MGMT_1 - Power Management Register 1
*/
uint8_t ICM20948_reset(ICM20948* icm);//ok

uint8_t ICM20948_SleepMode_enable(ICM20948* icm);//OK
uint8_t ICM20948_SleepMode_disable(ICM20948* icm);//OK
uint8_t ICM20948_isSleepMode(ICM20948* icm);//OK

uint8_t ICM20948_LowPowerMode_enable(ICM20948* icm);//ok
uint8_t ICM20948_LowPowerMode_disable(ICM20948* icm);//ok
uint8_t ICM20948_isLowPowerpMode(ICM20948* icm);//ok

uint8_t ICM20948_Temp_enable(ICM20948* icm);//ok
uint8_t ICM20948_Temp_disable(ICM20948* icm);//ok
uint8_t ICM20948_isTemp(ICM20948* icm);//ok


typedef enum{
    CLOCK_SRC_Internal_20MHz = 0,
    CLOCK_SRC_Auto_Sel_1 = 1,
    // CLOCK_SRC_Auto_Sel = 2,
    // CLOCK_SRC_Auto_Sel = 3,
    // CLOCK_SRC_Auto_Sel = 4,
    // CLOCK_SRC_Auto_Sel = 5,
    CLOCK_SRC_STOP = 7
} CLOCK_SRC;

uint8_t ICM20948_set_CLOCK_SRC(ICM20948*, CLOCK_SRC);//ok
CLOCK_SRC ICM20948_get_CLOCK_SRC(ICM20948*);//ok

/*
	PWR_MGMT_2 - Power Management Register 2
*/
uint8_t ICM20948_ACCEL_enable(ICM20948* icm);//ok
uint8_t ICM20948_ACCEL_disable(ICM20948* icm);//ok

uint8_t ICM20948_GYRO_enable(ICM20948* icm);//ok
uint8_t ICM20948_GYRO_disable(ICM20948* icm);//ok

/*
  !!!!!!!!!!!!!!!!!!!!!
 !! TODO: INT_PIN_CFG !!
  !!!!!!!!!!!!!!!!!!!!!
*/


/*
    INT_ENABLE
*/
uint8_t ICM20948_WOF_enable(ICM20948* icm);
uint8_t ICM20948_WOF_disable(ICM20948* icm);

uint8_t ICM20948_WOM_enable(ICM20948* icm);
uint8_t ICM20948_WOM_disable(ICM20948* icm);

uint8_t ICM20948_PLL_RDY_enable(ICM20948* icm);
uint8_t ICM20948_PLL_RDY_disable(ICM20948* icm);

uint8_t ICM20948_DMP_INT1_enable(ICM20948* icm);
uint8_t ICM20948_DMP_INT1_disable(ICM20948* icm);

uint8_t ICM20948_I2C_MST_INT_enable(ICM20948* icm);
uint8_t ICM20948_I2C_MST_INT_disable(ICM20948* icm);

uint8_t ICM20948_RAW_DATA_RDY_INT_enable(ICM20948* icm);
uint8_t ICM20948_RAW_DATA_RDY_INT_disable(ICM20948* icm);

uint8_t ICM20948_FIFO_OVERFLOW_INT_enable(ICM20948* icm);
uint8_t ICM20948_FIFO_OVERFLOW_INT_disable(ICM20948* icm);

uint8_t ICM20948_FIFO_WATERMARK_INT_enable(ICM20948* icm);
uint8_t ICM20948_FIFO_WATERMARK_INT_disable(ICM20948* icm);

/*
    !!!!!!!!!!!
    TODO
    !!!!!!!!

    I2C_MST_STATUS
*/
uint8_t ICM20948_get_i2C_MST_status(ICM20948* icm);

/*
    INT_STATUS 1/2/3
*/
uint8_t ICM20948_WakeOnMotion_occured(ICM20948*);
uint8_t ICM20948_PLL_Ready(ICM20948*);
uint8_t ICM20948_DMP_INT1_occured(ICM20948* icm) 
uint8_t ICM20948_get_I2C_MST_INT_status(ICM20948* icm);

uint8_t ICM20948_get_RAW_DATA_RDY_INT_status(ICM20948* icm);
uint8_t ICM20948_get_FIFO_OVERFLOW_INT_status(ICM20948* icm);
uint8_t ICM20948_get_FIFO_WATERMARK_INT_status(ICM20948* icm);


/*
    DELAY_TIME
*/
uint16_t ICM20948_get_DELAY_TIME(ICM20948* icm);


/*
    ACCEL
*/
uint8_t ICM20948_ACCEL_Init(ICM20948*, ACCEL_DLPF, AccelFullScale);

int8_t ICM20948_get_ACCEL_raw_arr(ICM20948* icm, uint16_t* array_ptr);

int16_t ICM20948_get_ACCEL_X_raw(ICM20948* icm);
int16_t ICM20948_get_ACCEL_Y_raw(ICM20948* icm);
int16_t ICM20948_get_ACCEL_Z_raw(ICM20948* icm);

float ICM20948_ACCEL_raw_to_mps(ICM20948* icm, int16_t gyro_raw);

uint8_t ICM20948_ACCEL_X_SELF_TEST(ICM20948*);
uint8_t ICM20948_ACCEL_Y_SELF_TEST(ICM20948*);
uint8_t ICM20948_ACCEL_Z_SELF_TEST(ICM20948*);

uint8_t ICM20948_set_ACCEL_AVG_SAMPLES(ICM20948*, ACCEL_AVG_SAMPLES);
//TODO: X/Y/Z A offset get/set

uint16_t ICM20948_set_ACCEL_SAMPLE_RATE_DIV(ICM20948*, uint8_t);
uint16_t ICM20948_get_ACCEL_SAMPLE_RATE_DIV(ICM20948*);

uint8_t ICM20948_WOM_Logic_enable(ICM20948*);
uint8_t ICM20948_WOM_Logic_disable(ICM20948*);

uint8_t ICM20948_WOM_Algorithm_select(ICM20948*, WOM_ALGORITHM);

uint16_t ICM20948_set_WOM_THRESHOLD(ICM20948*, uint8_t);



/*
	GYRO
*/
uint8_t ICM20948_GYRO_init(ICM20948*, GYRO_DLPF, FullScaleRange);//OK

/*
    FS_SEL | FullScale Range (degrees/sec) | Sensitivity 
    00			250								131
    01			500								65.5
    10			1000							32.8
    11			2000							16.4
*/
typedef enum{
    FS_250 = 250,
    FS_500 = 500,
    FS_1000 = 1000,
    FS_2000 = 2000
} FullScaleRange;

float ICM20948_getGyroSensitivity(FullScaleRange FS);//OK
FullScaleRange ICM20948_get_GYRO_FS_SEL(ICM20948*);//OK
uint8_t ICM20948_set_GYRO_FS_SEL(ICM20948*, FullScaleRange );//OK


typedef enum{
    GYRO_DLPF_NBW_12316 = -1,
    GYRO_DLPF_NBW_229_8 = 0,
    GYRO_DLPF_NBW_187_6 = 1,
    GYRO_DLPF_NBW_154_3 = 2,
    GYRO_DLPF_NBW_73_3 = 3,
    GYRO_DLPF_NBW_35_9 = 4,
    GYRO_DLPF_NBW_17_8 = 5,
    GYRO_DLPF_NBW_8_9 = 6,
    GYRO_DLPF_NBW_376_5 = 7
}GYRO_DLPF;

GYRO_DLPF ICM20948_get_GYRO_DLPFCFG(ICM20948* );//OK
uint8_t ICM20948_set_GYRO_DLPFCFG(ICM20948*, GYRO_DLPF );//OK

uint8_t ICM20948_GYRO_DLPF_enable(ICM20948*);//OK
uint8_t ICM20948_GYRO_DLPF_disable(ICM20948* );//OK

int8_t ICM20948_get_GYRO_raw_arr(ICM20948* icm, uint16_t* array_ptr);//OK

int16_t ICM20948_get_GYRO_X_raw(ICM20948* icm);//ok
int16_t ICM20948_get_GYRO_Y_raw(ICM20948* icm);//ok
int16_t ICM20948_get_GYRO_Z_raw(ICM20948* icm);//ok

float ICM20948_GYRO_raw_to_dps(ICM20948* icm, int16_t gyro_raw);//ok

uint8_t ICM20948_GYRO_X_SELF_TEST(ICM20948*);
uint8_t ICM20948_GYRO_Y_SELF_TEST(ICM20948*);
uint8_t ICM20948_GYRO_Z_SELF_TEST(ICM20948*);

uint8_t ICM20948_set_GYRO_AVG_FILTER_CFG(ICM20948*, GYRO_AVGCFG);

uint8_t ICM20948_set_GYRO_SAMPLE_RATE_DIV(ICM20948*, uint8_t);
uint8_t ICM20948_get_GYRO_SAMPLE_RATE_DIV(ICM20948*);

//TODO: X/Y/Z G offset get/set
//TODO: X/Y/Z G User OFFSET

/*
	TEMP
*/
uint8_t ICM20948_TEMP_Init(ICM20948*, TEMP_DLPF);

//degC = ((RAW - RoomTempOffset)/Temp_sensitivity)+21degC
uint16_t ICM20948_get_TEMP_raw(ICM20948* icm);
float ICM20948_get_TEMP(ICM20948* icm); 

/*
    EXT_SLV_SENS_DATA 00-23
*/
uint8_t ICM20948_get_EXT_SLV_DATA(ICM20948*, EXT_SLV_SENS_DATA);
uint8_t ICM20948_get_EXT_SLV_DATA_arr(ICM20948*, EXT_SLV_SENS_DATA, uint8_t* ptr);

/*
    FIFO
*/
uint8_t ICM20948_FIFO_reset(ICM20948* icm);
uint8_t ICM20948_set_FIFO_MODE(ICM20948* icm, FIFO_MODE);

uint8_t ICM20948_get_FIFO_COUNT(ICM20948* icm, FIFO_MODE);


uint8_t ICM20948_SLV_FIFO_enable(ICM20948* icm, EXT_SLV_SENS);

uint8_t ICM20948_FIFO_ACCEL_enable(ICM20948* icm);
uint8_t ICM20948_FIFO_GYRO_X_enable(ICM20948* icm);
uint8_t ICM20948_FIFO_GYRO_Y_enable(ICM20948* icm);
uint8_t ICM20948_FIFO_GYRO_Z_enable(ICM20948* icm);
uint8_t ICM20948_FIFO_TEMP_enable(ICM20948* icm);

uint8_t ICM20948_FIFO_write(ICM20948*, uint8_t);
uint8_t ICM20948_FIFO_read(ICM20948*);

uint8_t ICM20948_FIFO_CFG_enable(ICM20948*);
uint8_t ICM20948_FIFO_CFG_disable(ICM20948*);


/*
    DATA_RDY_STATUS
*/
uint8_t ICM20948_get_WOF_status(ICM20948*);
uint8_t ICM20948_get_RAW_DATA_RDY_status(ICM20948*);

//TODO: TIMEBASE_CORRECTION_PLL

/*
    ODR_ALIGN_EN
*/
uint8_t ICM20948_ODR_ALIGN_enable(ICM20948*);
uint8_t ICM20948_ODR_ALIGN_disable(ICM20948*);


/*
    FSYNC_CONFIG
*/
uint8_t ICM20948_Delay_Time_enable(ICM20948*);
uint8_t ICM20948_Delay_Time_disable(ICM20948*);

uint8_t ICM20948_WOF_DEGLITCH_enable(ICM20948*);
uint8_t ICM20948_WOF_DEGLITCH_disable(ICM20948*);

uint8_t ICM20948_WOF_EDGE_INT_enable(ICM20948*);
uint8_t ICM20948_WOF_EDGE_INT_disable(ICM20948*);

uint8_t ICM20948_set_EXT_SYNC(ICM20948*, EXT_SYNC);

/*
    MOD_CTR_USR
*/
uint8_t ICM20948_DMP_IN_LP_enable(ICM20948*);
uint8_t ICM20948_DMP_IN_LP_disable(ICM20948*);

/*
    I2C_MST
*/
uint8_t ICM20948_set_i2C_MST_ODR_CONFIG(ICM20948*, uint8_t);

uint8_t ICM20948_I2C_MultiMaster_enable(ICM20948*);
uint8_t ICM20948_I2C_MultiMaster_disable(ICM20948*);