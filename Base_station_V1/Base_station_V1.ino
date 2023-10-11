#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <LoRa.h>

const char* ssid = "WilsonPro";
const char* password = "123456789";
WebServer server(80);
IPAddress ip;
#define NSS 4
#define RST 5
#define DI0 2
#define LED 26

String LoRaData;
float lng;
float lat;
// GPS data and device IDs
struct GPSData {
  float latitude;
  float longitude;
};

GPSData devices[1];  // Store GPS data for up to 4 devices
int numDevices = 1;  // Assuming all 4 devices are active
int senderID;

void setup() {
  Serial.begin(115200);
  setupWiFi();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/getGPSData", HTTP_GET, handleGPSData);
  server.begin();

   LoRa.setPins(NSS, RST, DI0);
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF1);
  Serial.println("LoRa Initializing Successful!");
}

void loop() {

int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Clear the previous LoRaData before receiving new data
    LoRaData = "";

    // received a packet
    Serial.print("Received packet: ");

    // Identify the sender
    senderID = LoRa.read();
    senderID =atoi((char*)&senderID);
    //char senderID = (char)LoRa.read(); // The sender ID is the first byte received
    Serial.print("From Sender:");
    Serial.print(senderID);
    Serial.println(", Data: ");

    // Receiving the data from LoRa sender
    while (LoRa.available()) {
      LoRaData += (char)LoRa.read(); // Append the received characters to LoRaData
    }

 // Split the string into longitude and latitude
    //char* long_str = strtok((char*)LoRaData.c_str(), ",");
    //char* lat_str = strtok(NULL, ",");

    // Convert longitude and latitude strings to floating-point numbers
   // float longitude = atof(long_str);
   // float latitude = atof(lat_str);

    // Now you can use the "longitude" and "latitude" variables as needed
    // Serial.print("Longitude: ");
     //Serial.println(longitude);
    // Serial.print("Latitude: ");
    // Serial.println(latitude);

int commaPos = LoRaData.indexOf(',');

    if (commaPos != -1) {
      // Extract longitude and latitude substrings
      String long_str = LoRaData.substring(0, commaPos);
      String lat_str = LoRaData.substring(commaPos + 1);

      // Convert longitude and latitude strings to floating-point numbers
       lng = long_str.toFloat();
      lat = lat_str.toFloat();

      // Now you can use the "longitude" and "latitude" variables as needed
      Serial.print("Longitude: ");
      Serial.println(lng, 6); // Display with 6 decimal places
      Serial.print("Latitude: ");
      Serial.println(lat, 6); // Display with 6 decimal places
    }
    //Serial.println(LoRaData);
  }

  // Simulate random GPS data for each device
    //for (int i = 0; i < numDevices; i++) {

      if(lng!=0 && lat!=0){
    devices[senderID].latitude = lat;//-34.0 + random(0, 100) / 1000.0;  // Random latitude between 30.00 and 40.00
    devices[senderID].longitude =lng; //25.5 + random(0, 220) / 1000.0; // Random longitude between -80.00 and -70.00
      }
   // Serial.println( devices[senderID].latitude,6);
   // Serial.println( devices[senderID].longitude,6);
 // }

  //delay(1000);  // Simulate delay between updates
  server.handleClient();
}
