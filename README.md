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
typedef struct{
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
- i2c_address, who_am_i _val and i2c_ptr are set by ICM20948_Init() and are not to be messed around with.
- gyro/accel_datarate_sec are calculated and set by GYRO_Init() which is already implemented in ICM20948_Init();
    gyro_datarate_sec is used to calculate gyro's angle; Whereas accel_datarate_sec is not used anywhere in the code, as of this moment;
- angle_x/y/z calculated and set by ICM20948_read_data(), provides current angle of tilt;
- accel_x/y/z calculated and set by ICM20948_read_data(), provides current acceleration at all axis;
- temp calculated and set by ICM20948_read_data(), provides current sensor temperature;

### createICM()
```c
ICM20948* createICM20948( i2c_inst_t* i2c_chosen_ptr, uint8_t addr_pin_high );
```

returns a pointer to ICM struct allocated memory
needs pico-sdk i2c_inst_t pointer
either 
- [ICM20948_Init(ICM20948*)][#Initiates ICM's struct variables]


