#include <ESP8266WiFi.h>
#include <PubSubClient.h>
int k=100;
String data;
const char* ssid = "Oppo A12";
const char* password = "oppo A12";
#define TOKEN "Messi879" //Access token of device Display
char ThingsboardHost[] = "thingsboard.cloud";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("connected to");
  Serial.println(ssid);
  client.setServer( ThingsboardHost, 1883 );
}

void loop()
{
if ( !client.connected() ) 
{
    reconnect();
}
getAndSendTemperatureAndHumidityData();
  delay(1000);
}

void getAndSendTemperatureAndHumidityData()
{
  
    if (Serial.available()) {
   //delay(1000); 
    data = Serial.readString();         
       String payload = "{";
     payload += "\"FireTest\":";payload += data.substring(0,4); payload += ",";
   payload += "\"TempTest\":";payload +=data.substring(4,8); 
    payload += "}";

  char attributes[1000];
  payload.toCharArray( attributes, 1000 );
  client.publish( "v1/devices/me/telemetry",attributes);
  Serial.println( attributes );
}
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESPTESTk", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 500 );
    }
  }
}