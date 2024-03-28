#include <Arduino.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <boards.h>

TinyGPSPlus gps;
const long frequency = 915E6;  // LoRa Frequency
const bool test = true;


int numSent;


void setup() {
  delay(1000);

  initBoard();  
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  
  numSent = 0;


  // // Initialize LoRa
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed for Transmitter!");
  } else {
    Serial.println("Setup Successful for Transmitter");
  }
}

void loop() {
  if (!test) {
    while (Serial1.available() > 0) {
      char c = Serial1.read();
      if (gps.encode(c)) {

        if (gps.location.isUpdated()) {
          // Transmit location via LoRa
          LoRa.beginPacket();
          // LoRa.print("Latitude: ");
          LoRa.print(gps.location.lat(), 6);
          LoRa.print(",");
          LoRa.print(gps.location.lng(), 6);
          LoRa.endPacket();
          
          numSent++;

          // Latitude: 32.232280 Longitude: -110.953015

          u8g2->clearBuffer();
          char buf[256];
          snprintf(buf, sizeof(buf), "Sent: %i Msgs", numSent);
          u8g2->drawStr(0, 40, buf);
          u8g2->sendBuffer();

          // Also print to serial monitor
          Serial.print("Latitude: ");
          Serial.print(gps.location.lat(), 6);
          Serial.print(" Longitude: ");
          Serial.println(gps.location.lng(), 6);
          Serial.flush();
        }
      }
    }
  } else {
    // Serial.println("Getting here");
    // LoRa.beginPacket();
    // LoRa.print("Test");
    // LoRa.endPacket();

    numSent++;

    u8g2->clearBuffer();
    char buf[256];
    snprintf(buf, sizeof(buf), "Sent: %i Msgs", numSent);
    u8g2->drawStr(0, 40, buf);
    u8g2->sendBuffer();


    // Serial.println("Sent");
    // Serial.flush();
    // delay(1000);

      LoRa.beginPacket();
      // LoRa.print("Latitude: ");
      LoRa.print(32.231733176713156, 6);
      LoRa.print(",");
      LoRa.print(-110.9530524010333, 6);
      LoRa.endPacket();
      delay(100);
  }
  delay(10);
}
