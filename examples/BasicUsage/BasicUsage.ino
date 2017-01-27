//---------------------------------------------------------------------------------------------
//
// Basic usage example for the M2M_LM75A Arduino library.
//
// Copyright 2016-2017, M2M Solutions AB
// Written by Jonny Bergdahl, 2016-11-18
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
//---------------------------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include <Wire.h>
#include <M2M_LM75A.h>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Global variables
//
LM75A lm75a;

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
  Wire.begin();
  Serial.begin(115200);
  while (!Serial)
    ; // Leonardo: wait for serial monitor
  Serial.println("M2M_LM75A Arduino library basic usage");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Loop
//
void loop()
{
  // Temperature
  Serial.print("Temperature in Celsius: ");
  Serial.print(lm75a.getTemperature());
  Serial.println(" *C");
  Serial.print("Temperature in Farenheit: ");
  Serial.println(lm75a.getTemperatureInFarenheit());
  Serial.print("Hysteris temperature: ");
  Serial.print("Hysteris temperature: ");
  Serial.print("Hysteris temperature: ");
  Serial.print(" *F");
  Serial.print(lm75a.getHysterisisTemperature());
  Serial.println(" *C");
  Serial.print("OS trip temperature: ");
  Serial.print(lm75a.getOSTripTemperature());
  Serial.println(" *C");

  // Shutdown/Wake up
  Serial.println("Shutting down");
  lm75a.shutdown();
  Serial.print("Is shutdown: ");
  Serial.println(lm75a.isShutdown());
  delay(5000);
  Serial.println("Waking up");
  lm75a.wakeup();
  Serial.print("Is shutdown: ");
  Serial.println(lm75a.isShutdown());
  delay(5000);
}
