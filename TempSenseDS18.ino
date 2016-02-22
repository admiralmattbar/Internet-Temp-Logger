#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
//Setup the LCD pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Create global variable for timing last temperature reading
long lastTempRead = 0;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  lcd.begin(16, 2);    //Establish the number of columns and rows on the LCD
  lcd.print("Temperature:"); //Print a constant string, I'm indicating that the numbers are temperatures.
  
}
 
float getFar(){  //Function that returns sensor reading in Farenheit
  sensors.requestTemperatures(); // Send the command to get temperatures
  return sensors.getTempCByIndex(0)*1.8+32;
} 
void loop(void)
{
  if (Serial.available()){  //Set up response to ping from ESP
    char fromesp = Serial.read();
    if (fromesp == '<'){  //Special character to identify a ping from ESP
      Serial.println(getFar());
    }
  }
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  //Serial.print(" Requesting temperatures...");
  //Serial.println("DONE");
  //Serial.print("Temperature for Device 1 is: ");
  //Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? 
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
  //Serial.print(sensors.getTempCByIndex(0)*1.8+32);
  long currentTime = millis();  //Gets current time
  if (currentTime - lastTempRead > 500){
    //float tempF = sensors.getTempCByIndex(0)*1.8+32; //create variable to be printed in LCD
    float tempF = getFar();  
    lcd.setCursor(0,1); //Setting cursor for temp
    lcd.print(tempF); //printing the temp in F
    lcd.print("F");
    lastTempRead = millis();
  }

}
