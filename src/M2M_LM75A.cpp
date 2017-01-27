//---------------------------------------------------------------------------------------------
//
// Library for the LM75A sensor by NXP, TI, et al.
// Copyright 2016, M2M Solutions AB
//
// Jonny Bergdahl, 2016-11-18
//
//---------------------------------------------------------------------------------------------
// TODO: 
//
//---------------------------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////////////////////////
#include <Wire.h>
#include "M2M_LM75A.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
LM75A::LM75A()
{
	_i2cAddress = LM75A_DEFAULT_ADDRESS;
}

LM75A::LM75A (uint8_t i2cAddress)
{
	_i2cAddress = i2cAddress;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public functions
//
void LM75A::shutdown()
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111110) | 0b1);
}

void LM75A::wakeup()
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, config & 0b11111110);
}

bool LM75A::isShutdown()
{
	return (read8bitRegister(LM75A_REGISTER_CONFIG) & 0b1) == 1;	
}

float LM75A::getTemperature()
{
	return (int16_t)read16bitRegister(LM75A_REGISTER_TEMP) / 256.0f;
}

float LM75A::getTemperatureInFarenheit()
{
	return getTemperature() * 1.8f + 32.0f;
}

float LM75A::getHysterisisTemperature()
{
	return (float)read16bitRegister(LM75A_REGISTER_THYST) / 256.0f;
}

FaultQueueValue LM75A::getFaultQueueValue()
{
	return (FaultQueueValue)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b00011000);
}

float LM75A::getOSTripTemperature()
{
	return (float)read16bitRegister(LM75A_REGISTER_TOS) / 256.0f;
}

OsPolarity LM75A::getOsPolarity()
{
	return (OsPolarity)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b100);
}

DeviceMode LM75A::getDeviceMode()
{
	return (DeviceMode)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b010);
}

uint8_t LM75A::getConfig()
{
	return read8bitRegister(LM75A_REGISTER_CONFIG);
}

float LM75A::getProdId()
{
	uint8_t value = read8bitRegister(LM75A_REGISTER_PRODID);
	return (float)(value >> 4) + (value & 0x0F) / 10.0f;
} 

void LM75A::setHysterisisTemperature(float temperature)
{
	write16bitRegister(LM75A_REGISTER_THYST, temperature * 256);
}

void LM75A::setOsTripTemperature(float temperature)
{
	write16bitRegister(LM75A_REGISTER_TOS, temperature * 256);
}

void LM75A::setFaultQueueValue(FaultQueueValue value)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11100111) | value);
}

void LM75A::setOsPolarity(OsPolarity polarity)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111011) | polarity);
}

void LM75A::setDeviceMode(DeviceMode deviceMode)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111101) | deviceMode);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private functions
//

uint8_t LM75A::read8bitRegister(const uint8_t reg)
{
	uint8_t result;
	int8_t response;

	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	result = Wire.endTransmission();
	if (result != 0)
	{
		return result;
	}

	Wire.requestFrom(_i2cAddress, (uint8_t)1);
	if (Wire.available() != 1)
	{
		return 0;
	}

	response = Wire.read();

	return response;
}

uint16_t LM75A::read16bitRegister(const uint8_t reg)
{
	uint8_t result;
	int16_t response;
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	result = Wire.endTransmission();
	if (result != 0)
	{
		return result;
	}

	Wire.requestFrom(_i2cAddress, (uint8_t)2);
	if (Wire.available() != 2)
	{
		Serial.println("ERROR READING");
		
		return 0;
	}
	response = (Wire.read() << 8) | Wire.read();
	return response;
}

bool LM75A::write16bitRegister(const uint8_t reg, const uint16_t value)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.write((byte)(value >> 8));
	Wire.write((byte)value);
	return Wire.endTransmission() == 0;
}

bool LM75A::write8bitRegister(const uint8_t reg, const uint8_t value)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.write(value);
	return Wire.endTransmission() == 0;
}

