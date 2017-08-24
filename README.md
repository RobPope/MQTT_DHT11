# MQTT_DHT11
Arduino sketch for ESP8266 to log temperature and humidity from a DHT11

You'll need to have the following libraries installed: 
- ESP8266WiFi 
- PubSubClient
- DHT

Every minute the ESP will log the temperature, humidity and internal voltage to topics on your MQTT Server

**ToDo** 
- Deep Sleep 
- WiFi Manager
