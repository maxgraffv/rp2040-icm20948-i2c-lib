# ICM20948 I2C Library for RP2040
## Description
Library for ICM20948 - a 9axis Motion Sensor - configuration and communication using I2C protocol.  
The library is made to be as user friendly as possible.   
Supporting basic functionality, like gyro, accelerometer and temperature sensor data reads.
All listed and described in [here](#Functions).  
**IMPORTANT** - the library is not fully complete, lacks many features that are to be done.  
missing features listed [here](#to-be-dealt-with)  


## Table of Content
- [Features](#features)
- [Simple Code Example](#Example)
- [To Be Dealt With](#to-be-dealt-with)


## Features
- [General Overview](#general)
- [Data Rate Alignment](#data-rate-alignment)
- [SleepMode](#sleepmode)
- [Low Power Mode](#low-power-mode)
- [Clock Source](#clock-source)

- [Gyro Sensor](#gyro-sensor)

### General Overview
- [ICM20948 Struct](#icm20948-struct)
- [createICM()](#createicm)
- [Init()](#init)
- [Who Am I](#who-am-i)
- [Reset()](#reset)
- [Read Data](#read_data)


#### ICM20948 Struct
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


#### createICM()
```c
ICM20948* createICM20948( i2c_inst_t* i2c_chosen_ptr, uint8_t addr_pin_high );
```

- returns a pointer to ICM struct allocated memory
- needs pico-sdk i2c_inst_t pointer and integer for whether physical address pin is high or low (1 or 0)
- implements who_am_i_check() to make sure that the sensor connected is valid

#### Init()
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

#### Who Am I
Who_Am_I value, stored in a Bank 0, in Register WHO_AM_I at address 0x00.  
ICM20948's valid Who_Am_I value is 0xEA or 234 in decimal.  
<br>

```c
uint8_t ICM20948_get_who_am_i(ICM20948* icm);
```
Function returns read Who_Am_I value. Should be 0xEA.  
<br>

```c
uint8_t ICM20948_who_am_i_check(ICM20948* icm);
```
Function returns *** 1 *** if the Who_Am_I value is valid for ICM20948.   
Returns *** 0 *** if not valid. As if the device is not recognized as ICM20948.  
<br>


#### reset()
```c
uint8_t ICM20948_reset(ICM20948* icm);
```
Resets every register value to a default.  
Implements 10 milisecond sleep/delay for RP2040, so the sensor can initialize every register.  
Otherwise other functions occuring immediately after restart may not be performed and even end up in a stall.   
It's possible to lower the delay, for a user to play around with.  
<br>

#### read_data()
```c
uint8_t ICM20948_read_data(ICM20948* icm)
```
Acts as a "data update". Updating values held in a allocated icm struct with fresh ones.  
***Doesn't return the data***  
Use icm pointers to read.  
<br>



### Data Rate Alignment
```c
uint8_t ICM20948_ODR_ALIGN_enable(ICM20948* icm);
uint8_t ICM20948_ODR_ALIGN_disable(ICM20948* icm);
```
Enables/Disables Output Data Rate Alignment.  
It Aligns the data rate of used sensors, so that they produce "fresh" data, at the same time.  
Recommended to be enabled.  
<br>


### SleepMode
```c
uint8_t ICM20948_SleepMode_enable(ICM20948* icm);
uint8_t ICM20948_SleepMode_disable(ICM20948* icm);
```
When in Sleep Mode - sensor does not perform any action. (Not to be confused with Low Power Mode)  
**Enable/Disable** functions do as described, enable and disable SleepMode  
<br>

```c
uint8_t ICM20948_isSleepMode(ICM20948* icm)
```
**isSleepMode** checks sleepMode and returns 1 if SleepMode is enabled and 0 when disabled.  
<br>



### Low Power Mode
```c
uint8_t ICM20948_LowPowerMode_enable(ICM20948* icm);
uint8_t ICM20948_LowPowerMode_disable(ICM20948* icm);
```
Enables/Disables Low Power Mode.  
<br>

```c
uint8_t ICM20948_isLowPowerpMode(ICM20948* icm)
```
Checks for Low Power Mode status.  
Returns 1 if enabled, 0 if disabled.  
<br>

```c
uint8_t ICM20948_GYRO_CYCLE_enable(ICM20948* icm)
uint8_t ICM20948_GYRO_CYCLE_disable(ICM20948* icm)
```
Reduces Gyro power consumption, periodically powering down and waking it up.  
Requires further configuration, not yet implemented.  
<br>

<u>IMPORTANT</u>
Although those are fully functional.  
ICM needs further configuration for working in LP Mode.  
Which has not been implemented by this library, at the moment.  
Recommended to be disabled, until further update.  
See [TBDW](#to-be-dealt-with).  



### Clock Source
```c
uint8_t ICM20948_set_CLOCK_SRC(ICM20948* icm, CLOCK_SRC clk_src);
```
Sets Clock Source for the sensor.  
Recommended to be set to **CLOCK_SRC_Auto_Sel_1** as per Invensense Official Documentation.  
<br>

```c
CLOCK_SRC ICM20948_get_CLOCK_SRC(ICM20948* icm)
```
Returns current clock source selected.  
<br>

**Possible values defined in a following enum**
```c
typedef enum{
    CLOCK_SRC_Internal_20MHz = 0,
    CLOCK_SRC_Auto_Sel_1 = 1,
    CLOCK_SRC_Auto_Sel_2 = 2,
    CLOCK_SRC_Auto_Sel_3 = 3,
    CLOCK_SRC_Auto_Sel_4 = 4,
    CLOCK_SRC_Auto_Sel_5 = 5,
    // CLOCK_SRC_Internal_20MHz = 6, SAME as option = 0
    CLOCK_SRC_STOP = 7
} CLOCK_SRC;

```

<br>





### Gyro Sensor
- [Init()](#gyro-init)
- [Configuration](#gyro-config)
- [Data Output](#gyro-data)

#### Init() <a id="gyro-init" ></a>
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
<br>


#### Configuration <a id="gyro-config"></a>
```c
uint8_t ICM20948_GYRO_enable(ICM20948* icm);
uint8_t ICM20948_GYRO_disable(ICM20948* icm);
```
Enables/Disables Gyro sensor  
<br>


##### Full Scale Range <a id="gyro-fs"></a>
```c
GYRO_FS ICM20948_get_GYRO_FS_SEL(ICM20948* icm);
uint8_t ICM20948_set_GYRO_FS_SEL(ICM20948* icm, GYRO_FS fs_sel);
```
Gets and Sets Gyro Full Scale Range.  
<br>

```c
float ICM20948_getGyroSensitivity(GYRO_FS FS)
```
Returns Gyro Sensor sensitivity, based on Full Scale Range selected.  
<br>

##### Digital Low Pass Filter <a id="gyro-dlpf"></a>
```c
uint8_t ICM20948_GYRO_DLPF_enable(ICM20948* icm)
uint8_t ICM20948_GYRO_DLPF_disable(ICM20948* icm)
```
Enables/Disables Gyro Digital Low Pass Filter.  
<br>

```c
GYRO_DLPF ICM20948_get_GYRO_DLPFCFG(ICM20948* icm )
uint8_t ICM20948_set_GYRO_DLPFCFG(ICM20948* icm, GYRO_DLPF dlpf_sel)
```
Gets/Sets Gyro Digital Low Pass Filter
<br>


##### Averaging Filter <a id="gyro-avgf"></a>
```c
uint8_t ICM20948_set_GYRO_AVG_FILTER_CFG(ICM20948* icm, GYRO_AVG_FILTER avg_filter)
```
Average Filter Config applies averaging to gyroscope data, smoothing out short-term fluctuations and noise. This feature improves the stability and accuracy of the gyroscope readings, especially when measuring slow or gradual movements.
<br>


#### Data Output <a id="gyro-data"></a>
<br>

##### Bias <a id="gyro-bias"></a>
```c
uint8_t ICM20948_set_GYRO_X_BIAS(ICM20948* icm, int16_t bias)
uint8_t ICM20948_set_GYRO_Y_BIAS(ICM20948* icm, int16_t bias)
uint8_t ICM20948_set_GYRO_Z_BIAS(ICM20948* icm, int16_t bias)
```
Sets physical bias for gyro readings.  
Meaning, the bias is applied by the sensor itself, no nned to do it in software.  
<br>

```c
uint8_t ICM20948_GYRO_BIAS_CONFIGURE(ICM20948* icm, int16_t samples)
```
Function reads Gyro data for number defined by ***sample*** value and sets Gyro Bias using functions above.  
<br>


```c
int16_t ICM20948_get_GYRO_X_raw(ICM20948* icm);
int16_t ICM20948_get_GYRO_Y_raw(ICM20948* icm);
int16_t ICM20948_get_GYRO_Z_raw(ICM20948* icm);
```
Reads and returns Gyro Raw Data from registers.  
<br>

```c
float ICM20948_GYRO_raw_to_dps(ICM20948* icm, int16_t gyro_raw);
```
Converts raw data from Gyro to Degrees Per Second [dps]
<br>


```c
uint8_t ICM20948_set_GYRO_SAMPLE_RATE_DIV(ICM20948* icm, uint8_t sample_rate)
uint8_t ICM20948_get_GYRO_SAMPLE_RATE_DIV(ICM20948* icm)
```
Gets/Sets Sample Rate for Gyro sensor.
<br>


```c
float ICM20948_get_GYRO_ODR_kHz(ICM20948* icm)
```
Returns Gyro Output Data Rate in kHz.  
<br>



### Acceleration Sensor

### Temperature Sensor



<!-- 
### DMP

### FIFO

### SPI Mode

### SRAM

### I2C Master


### Wake On Motion
 -->

### Interrupts



### Register read/write
For people looking to expand on the exisitng library.  
Few functions for "moving around the sensor" have been provided.

```c
uint8_t ICM20948_selectBank( ICM20948* icm, UserBank bank )
```
As ICM's registers are devided into 4 Banks, a user needs to select one to write or read data from.  
Hence this function is provided.  


```c
uint8_t ICM20948_get_register(ICM20948* icm, UserBank bank, uint8_t reg_addr);
uint8_t ICM20948_set_register(ICM20948* icm, UserBank bank, uint8_t reg_addr, uint8_t value);
```
For reading or writing values to certain registers.  
Already implements ***selectBank*** function.  

```c
uint16_t ICM20948_get_register_16b(ICM20948* icm, UserBank bank, uint8_t reg_addr_H, uint8_t reg_addr_L)
```
Reads data from two 8bit registers and returns the value as 16bit value.  
Mostly used for reading GYRO, ACCEL and TEMP data, as they're spread across two registers.  










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








## To Be Dealt With
Many ICM20948 features available have not been implemented, mostly because of no need to do so and a short timeline for completing this project.  
To be completed in near future:  ~~hopefully~~  

- [ ] Low Power Mode - only enable/disable functions; no configuration available
- [ ] Lacks ICM as I2C Master Functionality
- [ ] Accelerometer Bias gives weird values; hence ACCEL_set_BIAS() commented out
- [ ] No Interrupt Pin Configuration functionality, besides Raw Data Rady Interrupt
- [ ] No FSYNC Functionality, although many functions declared;
- [ ] No FIFO Functionality, although many functions declared;
- [ ] No Accel and Gyro self-test functionality
- [ ] No magnetometer functionality, needs ICM as I2C Master Operating
- [ ] Error Handling - at the moment functions return 1 as default
- [ ] Infinity Loops - when reading data based on INT status

***DISCLAIMER***
Most of functions performing those features have been declared, few have been defined, but are **not advised** to be used by an unexperienced user, as the're not fully functioning.
