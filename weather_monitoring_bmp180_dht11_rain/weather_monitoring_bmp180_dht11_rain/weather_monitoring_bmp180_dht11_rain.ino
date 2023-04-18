#include <SFE_BMP180.h>
#include <Wire.h>
#include <SoftwareSerial.h> 
SoftwareSerial IOT(3 ,4);
// You will need to create an SFE_BMP180 object, here called "pressure":
SFE_BMP180 pressure;
double T,P,p0,a;
double baseline; // baseline pressure
#define ALTITUDE 1655.0
#include<LiquidCrystal.h>
LiquidCrystal lcd(8,9,10,11,12,13);//rs en d4 d5 d6 d7 
#include <stdlib.h>
#define rain A0

/*------------------------DHT SENSOR------------------------*/
#include "DHT.h"
#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
int h;
int t,rain_status;
int reg=0;
int error;

void setup()
{
  Serial.begin(115200); //or use default 115200.
  IOT.begin(115200);
  dht.begin();
  lcd.begin(16,2);
  lcd.setCursor(0,0);//col row
  lcd.print("SMART-WEATHER");
  Serial.println("AT");
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }
  delay(1000);
  //////////////////////////////////////////////////////////////////////////////
}
void loop()
{  
 //Read temperature and humidity values from DHT sensor:
 start: //label 
 error=0;
 ////////////////////////////////////////////////////
 rain_status=digitalRead(rain);
 rain_status=1-rain_status;
 t = dht.readHumidity();
 h = dht.readTemperature();
 if (isnan(t) || isnan(h)) 
 {      
  return;
 }
 baseline = getPressure();
 ///////////////////////////////////////////////////////
 lcd.setCursor(0,0);
 lcd.print("T  H  Rain  P");
 lcd.setCursor(0,1);
 lcd.print(t,1);
 lcd.setCursor(3,1);
 lcd.print(h,1);
 lcd.setCursor(7,1);
 lcd.print(rain_status);
 lcd.setCursor(11,1);
 lcd.print(baseline);
 Serial.print("Rain_status=");Serial.println(rain_status);
 Serial.print("Atmospheric pr.=");Serial.println(baseline);
 Serial.print("Temp=");Serial.println(t);
 Serial.print("Hum=");Serial.println(h);
 reg=t;
 String data = "";
 data+= "{";
 data+= "\"anloga\":";
 data+= "\""+String(t)+"\",";
 data+= "\"anlogb\":";
 data+= "\""+String(h)+"\",";
 data+= "\"anlogc\":";
 data+= "\""+String(rain_status)+"\",";
 data+= "\"anlogd\":";
 data+= "\""+String(baseline)+"\"";
 data+= "}";
 IOT.print('\r');
 IOT.print(data);
 delay(10);
 IOT.print('\n');delay(500);
}

double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else{} //Serial.println("error retrieving pressure measurement\n");
      }
      else{} //Serial.println("error starting pressure measurement\n");
    }
    else{} //Serial.println("error retrieving temperature measurement\n");
  }
  else{}// Serial.println("error starting temperature measurement\n");
}
