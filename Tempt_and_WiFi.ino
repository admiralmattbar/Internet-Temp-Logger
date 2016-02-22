#include 
#include 
#include
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#include <LiquidCrystal.h>

//Thingspeak settings
#define SSID "Condorico"
#define PASS "sandpiper"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=[THINGSPEAK_KEY]&field1=";

//Setup the LCD pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  Serial.begin(9600);
  sensors.begin();
  Serial.println("AT");
  delay(5000);
  if(Serial.find("OK")){
    connectWiFi();
  }
  lcd.begin(16, 2);    //Establish the number of columns and rows on the LCD
  lcd.print("Temperature:"); //Print a constant string, I'm indicating that the numbers are temperatures.
}

void loop(){
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  tempC = DallasTemperature::toFahrenheit(tempC);
  char buffer[10];
  String tempF = dtostrf(tempC, 4, 1, buffer);
  updateTemp(tempF);
  delay(1000);                                     //waiting a second
  lcd.setCursor(0,1); //Setting cursor for temp
  lcd.print(tempF); //printing the temp in F
  lcd.print("F");
  delay(60000);

  void updateTemp(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }else{
    Serial.println("AT+CIPCLOSE");
  }
}

 
boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}
