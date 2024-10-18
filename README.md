# ICM20948 I2C Library for RP2040
## Description
The library is made to be as user friendly as possible.
Supporting basic functionality, like gyro, accelerometer and temperature sensor data reads.
All listed and described below.

## Table of Content
- [Functions description](#Functions)
- [Example](#Example)
- [To Be Dealt With](#TBDW)


## Functions
### createICM()
\'''c
ICM20948* createICM20948( i2c_inst_t* i2c_chosen_ptr, uint8_t addr_pin_high );
\'''

- [createICM(i2c_inst_t*, uint8_t)][#returns pointer to ICM struct allocated memory]
- [ICM20948_Init(ICM20948*)][#Initiates ICM's struct variables]


