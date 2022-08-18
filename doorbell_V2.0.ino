/*
This is the code i have made to conet my doorbel upto a MQTT server 

replace **** with your creds

if you are useing MQTT without password chang 



*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//------------------------------------------------------------------------------------------------------------------------
//WIFI
const char* wifi_ssid = "****";
const char* wifi_password = "****";
//------------------------------------------------------------------------------------------------------------------------
//MQTT
//const char* mqtt_server = "192.168.69.4";
//const char* clientID = "Doorbell";


// MQTT Broker
const char *mqtt_broker = "192.168.69.4";
const char *topic = "Doorbel";
const char *mqtt_username = "****";
const char *mqtt_password = "****";
const int mqtt_port = 1883;
//------------------------------------------------------------------------------------------------------------------------
//VARS
const char* doorbell_topic = "doorbell";
const int doorbellPin = 4;
int doorbellState = 0;
//------------------------------------------------------------------------------------------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);
//------------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  pinMode(doorbellPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
//------------------------------------------------------------------------------------------------------------------------
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
//  setup_wifi();
      //  client.setServer(mqtt_server, 1883);
//------------------------------------------------------------------------------------------------------------------------
}
void blink_now()
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
}
//------------------------------------------------------------------------------------------------------------------------
//void setup_wifi(){
  //Turn off Access Point
  //WiFi.mode(WIFI_STA);
  //delay(10);
  //------------------------------------------------------------------------------------------------------------------------
  /*/ We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    blink_now();
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
*/

//------------------------------------------------------------------------------------------------------------------------
//void setup() 

  // Set software serial baud to 115200;
  //Serial.begin(115200);
  // connecting to a WiFi network
  
//------------------------------------------------------------------------------------------------------------------------  

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
//    Serial.print("Attempting MQTT connection...");

    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);    
    
    blink_now();
    // Attempt to connect
      if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(doorbell_topic, "Doorbell connected to MQTT");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  doorbellState = digitalRead(doorbellPin);

  if ( doorbellState == LOW )
  {
    // Put your code here.  e.g. connect, send, disconnect.
    Serial.println("Doorbell is pressed!");
    client.publish(doorbell_topic, "doorbell presed", true);
    blink_now();

    delay( 5000 );
  }
}

//------------------------------------------------------------------------------------------------------------------------
