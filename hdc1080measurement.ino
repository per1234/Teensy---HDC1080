#include <i2c_t3.h>

/**************************************************************************************

This is example for ClosedCube HDC1080 Humidity and Temperature Sensor breakout booard

Initial Date: 07-Jun-2017

Hardware connections for Arduino Uno:
VDD to 3.3V DC
SCL to A5
SDA to A4
GND to common ground

Written by AA for ClosedCube

MIT License

**************************************************************************************/

//#include <Wire.h>

#include "ClosedCube_HDC1080.h"

ClosedCube_HDC1080 hdc1080;
const int ledPin = 13;

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);   // set the LED on
	Serial.begin(115200);
//	Serial.println("ClosedCube HDC1080 Measurement Resolutions Arduino Test");

//Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_29_30, I2C_PULLUP_EXT, 100000);

  //Wire.setSDA(30);
  //Wire.setSCL(29);

/*
  Wire.begin();

*/

	hdc1080.begin(0x40);



	Serial.print("Manufacturer ID=0x");
	Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
	Serial.print("Device ID=0x");
	Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device
	
	printTandRH(HDC1080_RESOLUTION_8BIT, HDC1080_RESOLUTION_11BIT);
	printTandRH(HDC1080_RESOLUTION_11BIT, HDC1080_RESOLUTION_11BIT);
	printTandRH(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_11BIT);
	printTandRH(HDC1080_RESOLUTION_8BIT, HDC1080_RESOLUTION_14BIT);
	printTandRH(HDC1080_RESOLUTION_11BIT, HDC1080_RESOLUTION_14BIT);
	printTandRH(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_14BIT);

}

void loop()
{	
  Serial.println("I am alive");
  delay(1000);
  
  Serial.print("Manufacturer ID=0x");
  Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
  Serial.print("Device ID=0x");
  Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device
  
  printTandRH(HDC1080_RESOLUTION_8BIT, HDC1080_RESOLUTION_11BIT);
  printTandRH(HDC1080_RESOLUTION_11BIT, HDC1080_RESOLUTION_11BIT);
  printTandRH(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_11BIT);
  printTandRH(HDC1080_RESOLUTION_8BIT, HDC1080_RESOLUTION_14BIT);
  printTandRH(HDC1080_RESOLUTION_11BIT, HDC1080_RESOLUTION_14BIT);
  printTandRH(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_14BIT);
}


void printTandRH(HDC1080_MeasurementResolution humidity, HDC1080_MeasurementResolution temperature) {
	hdc1080.setResolution(humidity, temperature);

	HDC1080_Registers reg = hdc1080.readRegister();
	printRegister(reg);

	Serial.print("T=");
	Serial.print(hdc1080.readTemperature());
	Serial.print("C, RH=");
	Serial.print(hdc1080.readHumidity());
	Serial.println("%");
}

void printRegister(HDC1080_Registers reg) {
	Serial.print("Measurement Resolution: T=");
	Serial.print(reg.TemperatureMeasurementResolution, BIN);
	Serial.print(" (0=14 bit, 1=11 bit)");

	Serial.print(" RH=");
	Serial.print(reg.HumidityMeasurementResolution, BIN);
	Serial.println(" (00=14 bit, 01=11 bit, 10=8 bit)");
}

