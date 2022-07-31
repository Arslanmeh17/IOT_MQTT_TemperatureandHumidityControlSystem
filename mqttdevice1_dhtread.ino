#include <DHT.h>
#include "ESP8266WiFi.h"
#include "PubSubClient.h"

const char* ssid = "Arslan 2.4 GHz"; //Entering WiFi SSID
const char* password = "1971001971ma"; //Entering Password of WiFi

#define mqtt_server "160.75.154.101"  //We are team 8, so we need to connect with 101 ending.  
#define mqtt_temp "Temperature_TEAM8"   //Our first topic for publishing
#define mqtt_humidity "Humidity_TEAM8"  //Our second topic for publishing

WiFiClient espClient;            //Connection to mqrr server.
PubSubClient client(espClient);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 

float h,t;


void WifiandMqttConnection()                    //Our wifi connection function for connect our wifi.
{    
 WiFi.begin(ssid,password);                     //Connectiong to wifi
 while (WiFi.status() != WL_CONNECTED){delay(500); Serial.println("* Trying to connect...");}
 Serial.println("");
 Serial.println("WiFi connection Successful!!");         //Prints this when connection is OK.
 Serial.print("The IP Address of ESP8266 Module is: ");
 Serial.println(WiFi.localIP()); // Print the IP address
 WiFi.mode(WIFI_STA);
}

void ReadTemp_Hum_print()   //For reading temperature, humidity and printing them to serial port screen.
{
  h = dht.readHumidity();  
  t = dht.readTemperature(); 
  Serial.println(("********************"));
  Serial.print((" Humidity: "));
  Serial.println(h);
  Serial.print((" Temperature: "));
  Serial.println(t); 
}

void reconnect()               //Reconnect algorithm if lose connection.
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
    if(client.connect("Team8_device1"))
    {
      Serial.println("connected!");
    }
    else
    {
      Serial.print("failed:");
      Serial.print(client.state());
      delay(1000);        //wait to retry for 1 seconds.
    }
    }  
}


void setup() {
 Serial.begin(115200);
 WifiandMqttConnection();  //Calling for wifi connection.
 dht.begin(); 
 Serial.println("Connected to the WiFi network");
 client.setServer(mqtt_server, 1884);                 //connecting to a mqtt broker
 client.connect("Team8_device1","iturockwell","963258741");   //Entering broker id and password
}

void loop() {
  ReadTemp_Hum_print();   //Calling for printing temperature and humidity to serial port screen.
  client.publish(mqtt_temp, String(t).c_str(),true);         //Publishing our data to mqtt broker with these codes.
  client.publish(mqtt_humidity, String(h).c_str(),true);
  reconnect();
  if(WiFi.status() != WL_CONNECTED){ Serial.println((" Wifi connecion loss "));}
  if(client.connected() == 1){Serial.println((" *MQTT CONNECTED* "));}
  delay(500);               
            
}
