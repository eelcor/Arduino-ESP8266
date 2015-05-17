#include <PubSubClient.h>
#include <DHT.h>
#include <ESP8266WiFi.h>

#define  DHTPIN 2

const char* ssid = "Beestje_Open";
const char* password = "teldernetwerk29";

char* serverip = "10.0.0.47";
char* topic = "esp8266_arduino_out";

char* outTopic = "sensors/woonkamer_climate/set";
char* clientId = "woonkamer_climate";

WiFiClient wifiClient;
IPAddress server(10,0,0,47);
DHT dht(DHTPIN,DHT11,15);

PubSubClient client(serverip, 1883, callback, wifiClient);
 
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print("Flash size: ");
  Serial.println(ESP.getFlashChipSize());
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  dht.begin(); 
  delay(2000);

  // Generate client name based on MAC address and last 8 bits of microsecond counter
  /*String clientName;
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);*/

  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientId);
  
  if (client.connect("ESP8266")) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(outTopic);
    
    if (client.publish(outTopic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void loop() {
  static int counter = 0;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  String payload = "{\"ClientId\":\"";
  payload += clientId;
  payload += "\",\"Humidity\":";
  payload += h;
  payload += ",\"Temperature\":";
  payload += t;
  payload += "}";
  
  if (client.connected()){
    Serial.print("Sending payload: ");
    Serial.println(payload);
    
    if (client.publish(outTopic, (char*) payload.c_str())) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  ESP.wdtFeed();
  delay(5000);
  ESP.deepSleep(5000000);
  delay(500);
}

