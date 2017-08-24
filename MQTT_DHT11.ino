//Required for VCC
ADC_MODE(ADC_VCC);

//Import Libraries
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>

// WiFi parameters
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

//Create client
WiFiClient client;

// DHT11 Temperature chip i/o
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

//MQTT Configuration
PubSubClient mosq(client);
char tmp[50];
char hum[50];

void setup() {
  // Start Serial
  Serial.begin(115200);
  
  // Connect to WiFi  
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  //Connect to Mosquitto Server
  mosq.setServer("192.168.5.3",1883);
}

void loop() {
    if(!mosq.connected()){
      mosq.connect("client2");
      getTemperature();
      getVoltage();
    }
    
    delay(60000);
}

void getTemperature() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Temp as string
  itoa(t,tmp,10);
  mosq.publish("hab/bathroom/temperature",tmp);
  Serial.println(tmp);

  //Humidity as string
  itoa(h,hum,10);
  mosq.publish("hab/bathroom/humidity",hum);
  Serial.println(hum);
}

void getVoltage() {
  int iVcc = ESP.getVcc();
  float fVcc = (float)ESP.getVcc() / 1000;
  char cVcc[5];
  dtostrf(fVcc,5, 3, cVcc);
  
  mosq.publish("hab/bathroom/voltage",cVcc);
  Serial.println(cVcc);
}

