#include "ESP8266WiFi.h"
#include "PubSubClient.h"

String TempONOFF, HumONOFF;             // will get datas to this buffers

const char* ssid = "Arslan 2.4 GHz";   //Entering WiFi SSID
const char* password = "1971001971ma"; //Entering WiFi Password

#define mqtt_server "160.75.154.101"  //We are team 8, so we need to connect with 101 ending.  
#define mqtt_temp "TemperatureONOFF_team8"   // Getting topics
#define mqtt_humidity "HumidityONOFF_team8"  // Getting topics

#define Temprelay_REDled_pin 0          //Relay1 pin (Heater)
#define Humrelay_YELLOWled_pin 2        //Relay1 pin (Humidifier)

WiFiClient espClient;            //Connection to mqtt server.
PubSubClient client(espClient);



void WifiandMqttConnection()                    //Our wifi connection function for connect our wifi.
{    
 WiFi.begin(ssid,password);
 while (WiFi.status() != WL_CONNECTED){delay(500); Serial.println("* Trying to connect...");}
 Serial.println("");
 Serial.println("WiFi connection Successful!!");
 Serial.print("The IP Address of ESP8266 Module is: ");
 Serial.println(WiFi.localIP());// Print the IP address
 WiFi.mode(WIFI_STA);
}

void mqttCallback(char* topic, byte* message, unsigned int length)  //This function is for getting data from mqtt broker (subscribed data).
{
 String messagetemp;
 for (int i=0; i<length;i++)
 { 
   messagetemp += (char)message[i]; 
 }
 Serial.println();
 if(String(topic) =="TemperatureONOFF_team8")TempONOFF = messagetemp; Serial.print("Temp.Control:");Serial.println(TempONOFF); // giving the recieved value to output
 if(String(topic) =="HumidityONOFF_team8")HumONOFF = messagetemp; Serial.print("Hum.Control:");Serial.println(HumONOFF);
 if(TempONOFF == "1") digitalWrite(Temprelay_REDled_pin,LOW); //If "1" recieved, turning on red led with LOW signal (because its relay.)
 if(TempONOFF == "0") digitalWrite(Temprelay_REDled_pin,HIGH);
 if(HumONOFF == "1") digitalWrite(Humrelay_YELLOWled_pin,LOW);  //If "1" recieved, turning on yellow led with LOW signal (because relay.)
 if(HumONOFF == "0") digitalWrite(Humrelay_YELLOWled_pin,HIGH);
 //delay(100);
}
void reconnect()          //Reconnect algorithm if lose connection.
{
  int counter = 0;
  while(!client.connected())
  {
    if(counter==5)
    {
      ESP.restart();
      }
    counter+=1;
    Serial.println("Attempting MQTT connection...");
    if(client.connect("Team8_device2"))
    {
      Serial.println("connected!");
    }
    else
    {
      Serial.print("failed:");
      Serial.print(client.state());
      delay(1000);
    }
    }  
}
void setup() 
{
 pinMode(Temprelay_REDled_pin, OUTPUT);
 pinMode(Humrelay_YELLOWled_pin, OUTPUT); 
 Serial.begin(115200);
 WifiandMqttConnection();  //Calling for wifi connection.
 Serial.println("Connected to the WiFi network");
 client.setServer(mqtt_server, 1884); //connecting to a mqtt broker
 client.setCallback(mqttCallback);
 client.connect("Team8_device2","iturockwell","963258741");
 if(client.connected() == 1){Serial.println((" *MQTT CONNECTED* "));}
}

void loop() 
{
 reconnect();
 client.loop();
 client.subscribe(mqtt_temp);     //Subscribing the data topics for getting the data.
 client.subscribe(mqtt_humidity);
 delay(100);
 }
