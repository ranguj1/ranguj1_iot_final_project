#define BLYNK_TEMPLATE_ID "TMPL2UioWdoGg"
#define BLYNK_DEVICE_NAME "weather monitoring"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial  
#define APP_DEBUG
#include "BlynkEdgent.h"
//////////////////////////////////////// 
#include <Arduino_JSON.h>
String inputData = "";
boolean data_complete = false;
String vala;
String valb;
String valc;
String vald;
///////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  inputData.reserve(200);
  BlynkEdgent.begin();
}

void loop() 
{
 BlynkEdgent.run();
  while(Serial.available() > 0)
  {
   char inChar = Serial.read();
   if( inChar == '\r'){
   inputData = "";
  }
  else if(inChar == '\n'){
  data_complete = true;
 }
 else{
 inputData+=inChar;
 }
}
  if(data_complete)
  {
    data_complete = false;
    Serial.println(inputData);
    demoParse();
  }
  Blynk.virtualWrite(V5, vala); // Humidity for gauge
  Blynk.virtualWrite(V6, valb); // Humidity for gauge
  Blynk.virtualWrite(V7, valc);
  Blynk.virtualWrite(V8 ,vald);
}

void demoParse() 
 {
  Serial.println("parse");
  Serial.println("=====");
  JSONVar myObject = JSON.parse(inputData);
  if (JSON.typeof(myObject) == "undefined") 
  {
    Serial.println("Parsing input failed!");
    return;
  }

  Serial.print("JSON.typeof(myObject) = ");
  Serial.println(JSON.typeof(myObject)); // prints: object
  // myObject.hasOwnProperty(key) checks if the object contains an entry for key
  if (myObject.hasOwnProperty("anloga")) {
    Serial.print("myObject[\"anloga\"] = ");
     vala = (const char*) myObject["anloga"];//to get value in  vala
    Serial.println(vala);
    Serial.println((const char*) myObject["anloga"]);
  }
  if (myObject.hasOwnProperty("anlogb")) {
    Serial.print("myObject[\"anlogb\"] = ");
    valb = (const char*) myObject["anlogb"];
    Serial.println(valb); 
    Serial.println((const char*) myObject["anlogb"]);
  }
  if (myObject.hasOwnProperty("anlogc")) {
    Serial.print("myObject[\"anlogc\"] = ");
    valc = (const char*) myObject["anlogc"];
    Serial.println(valc); 
    Serial.println((const char*) myObject["anlogc"]);
  }
  if (myObject.hasOwnProperty("anlogd")) {
    Serial.print("myObject[\"anlogd\"] = ");
    vald = (const char*) myObject["anlogd"];
    Serial.println(vald); 
    Serial.println((const char*) myObject["anlogd"]);
  }
  //JSON vars can be printed using print or println
  Serial.print("myObject=");
  Serial.println(myObject);
  Serial.println();
}
