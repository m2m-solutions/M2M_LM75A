//---------------------------------------------------------------------------------------------
//
// Library for the LM75A temperature sensor by NXP and Texas Instruments.
//
// Copyright 2016, M2M Solutions AB
// Jonny Bergdahl, 2016-11-18
//
//---------------------------------------------------------------------------------------------
// LM75A is a temperature sensor communicating over the I2C protocol.
//
// The NXP version of LM75A has 11 significant bits of the temperature, which gives a 
// resolution of 0.125 °C. The standard version only uses 9 bits, which gives a resuolution 
// of 0.5 °C.
//
// I2C address span is 0x48 - 0x4F, and is set with pins A0/A1/A2.
//
// Power up mode:
// Comparator mode
// Tos = 80 °C
// Thyst = 75 °C
// OS active low
//
// Pin 3 is the OS (overtemperature shutdown). It can be used to to trigger a pin interrupt.
// Check the functional mode, Tos and Thyst if you use this.
//
// http://www.nxp.com/documents/data_sheet/LM75A.pdf
// http://www.ti.com/lit/ds/symlink/lm75a.pdf
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef M2M_LM75_h
#define M2M_LM75_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LM75A_DEFAULT_ADDRESS		0x48	// Address is configured with pins A0-A2
#define LM75A_INVALID_TEMPERATURE	-1000	// Just an arbritary value outside of the sensor limits
#define LM75A_REGISTER_TEMP			0		// Temperature register (read-only)
#define LM75A_REGISTER_CONFIG		1		// Configuration register
#define LM75A_REGISTER_THYST		2		// Hysterisis register
#define LM75A_REGISTER_TOS			3		// OS register
#define LM75A_REGISTER_PRODID		7		// Product ID register - Only valid for Texas Instruments

#define LM75_CONF_OS_COMP_INT		1
#define LM75_CONF_OS_POL			2
#define LM75_CONF_OS_F_QUE			3

enum FaultQueueValue : uint8_t
{
	NUMBER_OF_FAULTS_1 = 0,
	NUMBER_OF_FAULTS_2 = 0b01000,
	NUMBER_OF_FAULTS_4 = 0b10000,
	NUMBER_OF_FAULTS_6 = 0b11000
};

enum OsPolarity : uint8_t
{
	OS_POLARITY_ACTIVELOW = 0,
	OS_POLARITY_ACTIVEHIGH = 0b100
};

enum DeviceMode : uint8_t
{
	DEVICE_MODE_COMPARATOR = 0,
	DEVICE_MODE_INTERRUPT = 0b10
};

class LM75A 
{
	// Private variables
	uint8_t _i2cAddress;

	// Private functions
	uint8_t read8bitRegister(uint8_t register);
	uint16_t read16bitRegister(uint8_t register);
	bool write16bitRegister(const uint8_t reg, const uint16_t value);
	bool write8bitRegister(const uint8_t reg, const uint8_t value);

public:
	// Constructors
    LM75A();
    LM75A(byte address);

	// Power management
	void shutdown();
	void wakeup();
	bool isShutdown();

	// Temperature functions
    float getTemperature();
	float getTemperatureInFarenheit();

	// Read config
	float getHysterisisTemperature();	
	FaultQueueValue getFaultQueueValue();
	float getOSTripTemperature();
	OsPolarity getOsPolarity();
	DeviceMode getDeviceMode();

	// Other
	uint8_t getConfig();
	float getProdId();

	// Write config
	void setHysterisisTemperature(float temperature);
	void setOsTripTemperature(float temperature);
	void setFaultQueueValue(FaultQueueValue value);
	void setOsPolarity(OsPolarity polarity);
	void setDeviceMode(DeviceMode deviceMode);	
};

#endif
