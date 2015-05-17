#include <ESP8266WiFi.h>
#include <DHT.h>


#define DHTPIN 2
const char* ssid = "Beestje_Open";
const char* passwd = "teldernetwerk29";
char* server = "10.0.0.47";


WiFiClient wifiClient;
DHT dht(DHTPIN, DHT11, 15);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Booting...\r\n");
  delay(1000);
  Serial.print("Connecting to AP...\r\n");
  WiFi.begin(ssid, passwd);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  delay(1000);
  
  String postdata = "{\"ClientID\":\"Woonkamer\",";
  postdata += "\",\"Humidity\":";
  postdata += h;
  postdata += ",\"Temperature\":";
  postdata += t;
  postdata += "}";

  if(wifiClient.connect(server,5001)) {
    Serial.println("Connected to Google");
    wifiClient.println("POST /measure?query=jsondata HTTP/1.1");
    wifiClient.println("Host: **.**.**.**");
    wifiClient.println("Connection: close\r\nContent-Type: application/x-www-form-urlencoded");
    wifiClient.println("User-Agent: ESP8266/1.0");
    //wifiClient.println("Accept: application/json");
    wifiClient.print("Content-Length: ");
    wifiClient.println(postdata.length());
    Serial.println(postdata.length());
    wifiClient.println();
    wifiClient.println(postdata);
    Serial.println(postdata);
  }
    
  delay(100);
  while(wifiClient.available()) {
    char c = wifiClient.read();
    Serial.print(c);
  }
  Serial.println(postdata);
  // put your main code here, to run repeatedly:
  Serial.println("Dit is een test van deep sleep");
  delay(1000);
  ESP.deepSleep(180000000);
}
