/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLbFAQWVlt"
#define BLYNK_DEVICE_NAME "project"
#define BLYNK_AUTH_TOKEN "3a59IWEggET5lWL0IVcjD5SydJZTkvsP"
#include "TRIGGER_WIFI.h"
#include "TRIGGER_GOOGLESHEETS.h"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define pump D2

int sense_pin = 0;
int dryness = 0;
char column_name_in_sheets[ ][6] = {"val1","val2","val3"};
String Sheets_GAS_ID = "AKfycbz-BusRbG_9k-TEn1tonQHoCz_sOKW_IqqvG_CcHShbG3cXTEZJAZdmJ21HskNExZVV";
int No_of_Parameters = 1;
const char* webhost = "maker.ifttt.com";
String mailurl1 = "trigger/plant/json/with/key/i_LsKp_4CycX-c_QYxm0HJhH8SQk2J2fuXtjetChWnM?value1=Automatic\nhttps://blynk.cloud/dashboard/91487/global/filter/520421/organization/91487/devices/347759/dashboard";
String mailurl2 = "trigger/plant/json/with/key/i_LsKp_4CycX-c_QYxm0HJhH8SQk2J2fuXtjetChWnM?value1=Manual\nhttps://blynk.cloud/dashboard/91487/global/filter/520421/organization/91487/devices/347759/dashboard";

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Deez_nuts";
//char pass[] = "Advaita2711";

//char ssid[] = "MilkyWay";
//char pass[] = "Disc0iit";

//char ssid[] = "D-Link_DIR-600M";
//char pass[] = "12345678";

char ssid[] = "POCO M2 Pro";
char pass[] = "e51311cfc8dc";
int automatic = 1;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  if(automatic == 0){
    if(value == 1)
    {
      digitalWrite(pump,LOW);
    }
    else{
      digitalWrite(pump, HIGH);
    }
  }

  // Update state
//  Blynk.virtualWrite(V3, value);
}

BLYNK_WRITE(V1){
  int value = param.asInt();
  if(value == 1){
    automatic = 1;
  }
  else{
    automatic = 0;
  }
}

BlynkTimer timer; 

void myTimer() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  Blynk.virtualWrite(V2, dryness);  
}


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.virtualWrite(V0,LOW);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(pump, OUTPUT);
  digitalWrite(pump,HIGH);
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V1, HIGH);
  WIFI_Connect(ssid,pass);    
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  timer.setInterval(2000L, myTimer);
  Google_Sheets_Init(column_name_in_sheets, Sheets_GAS_ID, No_of_Parameters );
}

void loop()
{
  Blynk.run();
  Serial.println("Starting...");
  dryness = analogRead(sense_pin);
  Blynk.virtualWrite(V2, dryness);
  Serial.println(dryness);
  Serial.println(automatic);
  float a = dryness;
  Data_to_Sheets(No_of_Parameters,  a);
  if(dryness > 700){
    if(automatic == 1){
      Blynk.logEvent("email", "Your plants are thirsty! We are watering them.");
//      client.print(String("GET ") + mailurl1 + " HTTP/1.1\r\n" +
//         "Host: " + webhost + "\r\n" +
//         "User-Agent: BuildFailureDetectorESP8266\r\n" +
//         "Connection: close\r\n\r\n");
      digitalWrite(pump, LOW);
      Blynk.virtualWrite(V0, HIGH);
//      delay(2000);
      }
    else{
      Blynk.logEvent("email", "Your plants are thirsty! Would you like to water them?");
//      client.print(String("GET ") + mailurl2 + " HTTP/1.1\r\n" +
//         "Host: " + webhost + "\r\n" +
//         "User-Agent: BuildFailureDetectorESP8266\r\n" +
//         "Connection: close\r\n\r\n");
    }
  }
  else{
    if(automatic == 1){
      digitalWrite(pump, HIGH);
      Blynk.virtualWrite(V0, LOW);
    }
   }
  delay(2000);
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
//  timer.run();
}
