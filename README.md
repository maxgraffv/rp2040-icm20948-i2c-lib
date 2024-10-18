# ICM20948 I2C Library for RP2040
## Description
Library for ICM20948 9
The library is made to be as user friendly as possible.
Supporting basic functionality, like gyro, accelerometer and temperature sensor data reads.
All listed and described below.

## Table of Content
- [Functions](#Functions)
- [Example](#Example)
- [To Be Dealt With](#tbdw)


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



















## Example
Simplified example, as much as possible;

```c
#include "icm20948_i2c.h"
#include <stdio.h>
#include <stdlib.h>

void printICM_data(ICM20948* icm);

int main()
{
	stdio_init_all();

    i2c_init(i2c0, 100000);
	gpio_set_function(4, GPIO_FUNC_I2C);
	gpio_set_function(5, GPIO_FUNC_I2C);
	gpio_pull_up(4);
	gpio_pull_up(5);

    ICM20948* icm1 = createICM20948(i2c0, 1);

	ICM20948_Init(icm1);
	
	for(int i = 0; i < 1000; i++)
	{
		ICM20948_read_data(icm1);
		printICM_data(icm1);
	}

	return 1;
}

void printICM_data(ICM20948* icm)
{
	float gx = icm->angle_x;
	float gy = icm->angle_y;
	float gz = icm->angle_z;

	float ax = icm->accel_x;
	float ay = icm->accel_y;
	float az = icm->accel_z;

	float t = icm->temp;

	printf("GYRO: x %f; y %f; z %f;\t\t ACCEL: x %f; y %f; z %f;\t\t TEMP: %f C\n",
		gx, gy, gz,
		ax, ay, az,
		t
	);
}

```

0. **stdio_init_all()** for usb and uart communication
1. RP2040 I2C Set up at Pins 4 and 5; Using i2c_inst_t* = i2c0; at 100kHz;
2. Receiving pointer to ICM struct allocated memory; with i2c0 and address_pin HIGH
3. Perforiming a default Initialization of ICM
4. Reading data and printing it for 1000 samples;


**Results in an output like shown below**
```bash
GYRO: x 16.409292; y 23.096174; z -357.978241;           ACCEL: x -0.158813; y 0.031372; z 1.215576;             TEMP: 27.568424 C
GYRO: x 12.281576; y 22.242516; z -357.774261;           ACCEL: x -0.125000; y -0.090576; z 1.254639;            TEMP: 27.604366 C
GYRO: x 10.723261; y 21.076218; z -357.475800;           ACCEL: x -0.111572; y -0.112305; z 1.218506;            TEMP: 27.571421 C
GYRO: x 9.855855; y 19.738745; z -357.017273;            ACCEL: x -0.093140; y -0.158569; z 1.071533;            TEMP: 27.580406 C
GYRO: x 8.759624; y 18.312138; z -356.444763;            ACCEL: x -0.055542; y -0.199463; z 0.944702;            TEMP: 27.595383 C
GYRO: x 7.987562; y 17.065575; z -356.125031;            ACCEL: x 0.001221; y -0.339355; z 0.913330;             TEMP: 27.589392 C
GYRO: x 7.078471; y 15.992847; z -355.686462;            ACCEL: x 0.042969; y -0.318604; z 0.854370;             TEMP: 27.586395 C
GYRO: x 4.719713; y 15.211473; z -355.625275;            ACCEL: x 0.064819; y -0.372559; z 0.955566;             TEMP: 27.577412 C
GYRO: x 1.587118; y 14.270452; z -355.719299;            ACCEL: x 0.071045; y -0.420532; z 0.877441;             TEMP: 27.607363 C
```








## To Be Dealt With {#tbdw}
Many ICM20948 features available have not been implemented, mostly because of no need to do so and a short timeline for completing this project.  
To be completed in near future:  ~~hopefully~~  

- [ ] Low Power Mode - although some functions declared; No Implementations as of this time
- [ ] Lacks ICM as I2C Master Functionality
- [ ] Accelerometer Bias gives weird values; hence ACCEL_set_BIAS() commented out
- [ ] No Interrupt Pin Configuration functionality, besides Raw Data Rady Interrupt
- [ ] No FSYNC Functionality, although many functions declared;
- [ ] No FIFO Functionality, although many functions declared;
- [ ] No Accel and Gyro self-test functionality
- [ ] No magnetometer functionality, needs ICM as I2C Master Operating

***DISCLAIMER***
Most of functions performing those features have been declared, few have been defined, but are **not advised** to be used by an unexperienced user, as the're not fully functioning.
