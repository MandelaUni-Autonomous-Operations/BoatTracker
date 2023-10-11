#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>

#define NSS 4      //18
#define RST 5      //14
#define DI0 2      //26
#define SWITCH_PIN 26   //25

TinyGPSPlus gps;
String senderID;
float lat = 25.5772156;
float lng = -33.9179985;
String dataToSend;
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 17, 16); // Start serial communication with GPS module on pins 17 (RX2) and 16 (TX2)
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  LoRa.setPins(NSS, RST, DI0);
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF1);
  Serial.println("LoRa Initializing Successful!");
  
  // Set the sender identifier based on the sender device
  // Modify this line for each sender device
  senderID = "0";
}

void loop() {
  
  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
     // if (gps.location.isValid()) {


        lat = gps.location.lat();   //Update Lattitude from GPS
        lng = gps.location.lng();   //Update Longitude from GPS
        dataToSend = senderID +String(lng, 6) + "," + String(lat, 6) ;
        // Serial.print("Latitude: ");
        // Serial.print(gps.location.lat(), 6); // 6 decimal places for better accuracy
        // Serial.print(", Longitude: ");
        // Serial.print(gps.location.lng(), 6); // 6 decimal places for better accuracy
        // Serial.print(", Speed: ");
        // Serial.print(gps.speed.kmph()); // Speed in km/h
        // Serial.print(" km/h, Altitude: ");
        // Serial.print(gps.altitude.meters()); // Altitude in meters
        // Serial.println(" meters");
     // } else {
        //Serial.println("No GPS data available.");
      //}
      
  // Send the data via LoRa
  LoRa.beginPacket();
  LoRa.print(dataToSend);
  LoRa.endPacket();

  Serial.println("Sent:" +dataToSend);
  
    }


 
  }

 delay(1000); // Adjust the delay as needed to control the data sending rate
  // Prepare the data to be sent including the sender identifier
//(switchState ? String(lng, 6) : String(lat, 6));




}
