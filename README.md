# ICM20948 I2C Library for RP2040
## Description
Library for ICM20948 9
The library is made to be as user friendly as possible.
Supporting basic functionality, like gyro, accelerometer and temperature sensor data reads.
All listed and described below.

## Table of Content
- [Functions description](#Functions)
- [Example](#Example)
- [To Be Dealt With](#TBDW)


## Functions
### ICM20948 Struct
```c
typedef struct
{
    uint8_t i2c_address;
    uint8_t who_am_i_val;
    i2c_inst_t* i2c_ptr;
    float gyro_datarate_sec;
    float accel_datarate_sec;

    float angle_x;
    float angle_y;
    float angle_z;

    float accel_x;
    float accel_y;
    float accel_z;

    float temp;
}ICM20948;
```
- **i2c_address, who_am_i _val and i2c_ptr** are set by createICM20948() and are not to be messed around with.
- **gyro/accel_datarate_sec** are calculated and set by GYRO_Init() which is already implemented in ICM20948_Init();
    gyro_datarate_sec is used to calculate gyro's angle; Whereas accel_datarate_sec is not used anywhere in the code, as of this moment;
- **angle_x/y/z** calculated and set by ICM20948_read_data(), provides current angle of tilt;
- **accel_x/y/z** calculated and set by ICM20948_read_data(), provides current acceleration at all axis;
- **temp** calculated and set by ICM20948_read_data(), provides current sensor temperature;


### createICM()
```c
ICM20948* createICM20948( i2c_inst_t* i2c_chosen_ptr, uint8_t addr_pin_high );
```

- returns a pointer to ICM struct allocated memory
- needs pico-sdk i2c_inst_t pointer and integer for whether physical address pin is high or low (1 or 0)
- implements who_am_i_check() to make sure that the sensor connected is valid

### Init()
```c
uint8_t ICM20948_Init(ICM20948* icm);
```
**Performs:**
- reset() - Setting all register to default values
- SleepMode_disable() - Waking up the sensor
- Set_Clock_Source() - Set's the default clocksource at Auto_sel = 1; Advised by ICM20948 documentation
- ODR_ALIGN_enable() - Enabling Output Datarate Alignment
- GYRO_Init() - Initiating Gyro with default values; To be configured by a user;
- ACCEL_Init() - Initiating Accelerometer with default values; To be configured by a user;
- TEMP_Init() - Initiating Temperature sensor with default values; To be configured by a user;

### SleepMode
```c
uint8_t ICM20948_SleepMode_enable(ICM20948* icm);
uint8_t ICM20948_SleepMode_disable(ICM20948* icm);
uint8_t ICM20948_isSleepMode(ICM20948* icm);
```
When in Sleep Mode - sensor does not perform any action. (Not to be confused with Low Power Mode)  
**Enable/Disable** functions do as described, enable and disable SleepMode  
Whereas **isSleepMode** checks sleepMode and returns 1 if SleepMode is enabled and 0 when disabled  


### GYRO_Init()
```c
uint8_t ICM20948_GYRO_Init(ICM20948* icm, GYRO_DLPF dlpf, GYRO_FS fs, uint8_t sample_rate, uint16_t sample_num)
```

Takes arguments as shown above, and performs the following:
- **GYRO_enable()** - Enables Gyro
- **set_DLPFCFG()** - sets Digital Low Pass Filter
- **set_FS_SEL()** - sets Full Scale Range
- **set_SAMPLE_RATE_DIV()** - sets Sample Rate Divider
- **BIAS_CONFIGURE()** - sets GYRO Bias values
- **get_GYRO_ODR_kHZ()** - reads ODR register value, calculates and sets **ICM's** ***gyro_data_s*** value based on it  



