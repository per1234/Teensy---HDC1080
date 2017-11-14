/*

Arduino Library for Texas Instruments HDC1080 Digital Humidity and Temperature Sensor
Written by AA for ClosedCube
---

The MIT License (MIT)

Copyright (c) 2016-2017 ClosedCube Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

//#include <Wire.h>

#include <i2c_t3.h>

#include "ClosedCube_HDC1080.h"


ClosedCube_HDC1080::ClosedCube_HDC1080()
{
}

void ClosedCube_HDC1080::begin(uint8_t address) {
	_address = address;
	//Wire1.begin();
	Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_29_30, I2C_PULLUP_EXT, 100000);

	
	setResolution(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_14BIT);
}

void ClosedCube_HDC1080::setResolution(HDC1080_MeasurementResolution humidity, HDC1080_MeasurementResolution temperature) {
	HDC1080_Registers reg;
	reg.HumidityMeasurementResolution = 0;
	reg.TemperatureMeasurementResolution = 0;

	if (temperature == HDC1080_RESOLUTION_11BIT)
		reg.TemperatureMeasurementResolution = 0x01;

	switch (humidity)
	{
		case HDC1080_RESOLUTION_8BIT:
			reg.HumidityMeasurementResolution = 0x02;
			break;
		case HDC1080_RESOLUTION_11BIT:
			reg.HumidityMeasurementResolution = 0x01;
			break;
		default:
			break;
	}

	writeRegister(reg);
}

HDC1080_SerialNumber ClosedCube_HDC1080::readSerialNumber() {
	HDC1080_SerialNumber sernum;
	sernum.serialFirst = readData(HDC1080_SERIAL_ID_FIRST);
	sernum.serialMid = readData(HDC1080_SERIAL_ID_MID);
	sernum.serialLast = readData(HDC1080_SERIAL_ID_LAST);
	return sernum;
}

HDC1080_Registers ClosedCube_HDC1080::readRegister() {
	HDC1080_Registers reg;
	reg.rawData = (readData(HDC1080_CONFIGURATION) >> 8);
	return reg;
}

void ClosedCube_HDC1080::writeRegister(HDC1080_Registers reg) {
	Wire1.beginTransmission(_address);
	Wire1.write(HDC1080_CONFIGURATION);
	Wire1.write(reg.rawData);
	Wire1.write(0x00);
	Wire1.endTransmission();
	delay(10);
}

void ClosedCube_HDC1080::heatUp(uint8_t seconds) {
	HDC1080_Registers reg = readRegister();
	reg.Heater = 1;
	reg.ModeOfAcquisition = 1;
	writeRegister(reg);

	uint8_t buf[4];
	for (int i = 1; i < (seconds*66); i++) {
		Wire1.beginTransmission(_address);
		Wire1.write(0x00);
		Wire1.endTransmission();
		delay(20);
		Wire1.requestFrom(_address, (uint8_t)4);
		Wire1.readBytes(buf, (size_t)4);
	}
	reg.Heater = 0;
	reg.ModeOfAcquisition = 0;
	writeRegister(reg);
}


float ClosedCube_HDC1080::readT() {
	return readTemperature();
}

float ClosedCube_HDC1080::readTemperature() {
	uint16_t rawT = readData(HDC1080_TEMPERATURE);
	return (rawT / pow(2, 16)) * 165 - 40;
}

float ClosedCube_HDC1080::readH() {
	return readHumidity();
}

float ClosedCube_HDC1080::readHumidity() {
	uint16_t rawH = readData(HDC1080_HUMIDITY);
	return (rawH / pow(2, 16)) * 100;
}

uint16_t ClosedCube_HDC1080::readManufacturerId() {
	return readData(HDC1080_MANUFACTURER_ID);
}

uint16_t ClosedCube_HDC1080::readDeviceId() {
	return readData(HDC1080_DEVICE_ID);
}

uint16_t ClosedCube_HDC1080::readData(uint8_t pointer) {
	Wire1.beginTransmission(_address);
	Wire1.write(pointer);
	Wire1.endTransmission();
	
	delay(9);
	Wire1.requestFrom(_address, (uint8_t)2);

	byte msb = Wire1.read();
	byte lsb = Wire1.read();

	return msb << 8 | lsb;
}


