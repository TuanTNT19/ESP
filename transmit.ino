#include <ESP8266WiFi.h>
#include <PubSubClient.h>

String data;
const char* ssid = "Oppo A12";
const char* password = "oppo A12";

#define TOKEN "Messi87" //Access token of device Display
char ThingsboardHost[] = "thingsboard.cloud";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(9600);
 
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
 // delay(1000);
}

void getAndSendTemperatureAndHumidityData()
{
  
   if (Serial.available()) {
    // delay(1000);          // Kiểm tra nếu có dữ liệu đang chờ đọc
    data = Serial.readString();  // Đọc dữ liệu vào biến String
    //Serial.println(data);
   }
  // Prepare a JSON payload string
//    String payload = "{";
//  payload += "\"Humidityuyt\":";payload += data.substring(4,8); 
//   payload += "}";
    String payload = "{";
 payload += "\"FireTest\":";payload += data.substring(0,4); payload += ",";
 payload += "\"TempTest\":";payload +=data.substring(4,8); 
  payload += "}";

  char attributes[1000];
  payload.toCharArray( attributes, 1000 );
  client.publish( "v1/devices/me/telemetry",attributes);
  Serial.println( attributes );
   
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
    if ( client.connect("Esp8266", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 500 );
    }
  }
}