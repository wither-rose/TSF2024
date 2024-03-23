// This sketch runs the program that drives the HOSE prototype.
// It is based on the Dallas Library Single example code for using the DS18B20 sensor.

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

// new LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

// boolean to check if the temperature is hot enough
bool isHot;

// linear regression variables
double m = -0.8075867411379077;
double b = 357.3532013337096;

// current temperature
float currentTemp;

// start time of heat
unsigned long start;

/*
 * Setup function.
 */
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();

  // initialize LCD
  lcd.init();
  lcd.backlight();

  // set isHot to false
  isHot = false;

}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
 
  float tempC = sensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  Serial.print("Temp C: ");
  Serial.println(tempC);

  // output temperature information on the LCD screen
  lcd.clear();
  lcd.print(tempC);
  lcd.print("C");
  currentTemp = tempC; // set global variable current temperature to the sensor temperature
}
/*
 * Main function.
 */
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  // It responds almost immediately.
  printTemperature(insideThermometer); 

  // check if temperature is over threshold, if not, check if it has reached it
  if(!isHot){
    if(currentTemp >= 90){
      // set isHot to true and mark the start time
      isHot = true;
      start = millis()/1000;
    }
  } else if(isHot) {
    // otherwise if temperature has dipped below the threshold
    if(currentTemp < 90){
      // set isHot to false
      isHot = false;
      // calculate the y value from the regression equation
      double y = m*((millis()/1000) - start) + b;
      double percentage = 100 - ((int) (y/b*100));
      // display percentage sterilized on the LCD screen
      lcd.clear();
      lcd.print(percentage);
      lcd.print("% Sterilized!");
      delay(5000);
    }
  }
}
