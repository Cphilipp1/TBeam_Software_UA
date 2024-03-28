#include <Arduino.h>
#include <LoRa.h>
#include <Wire.h>
#include <boards.h>

void sendTransmitterId();
void updateDisplayAndSerialMonitor();

const long LORA_FREQUENCY = 915E6;
const bool TEST_MODE = true;
const int TRANSMITTER_ID = 1; // Change this value for each transmitter (1, 2, 3, 4)
const int MIN_INTERVAL = 1000; // Minimum transmission interval (ms)
const int MAX_INTERVAL = 5000; // Maximum transmission interval (ms)

int numSent;

void setup() {
    delay(1000);
    initBoard();
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    numSent = 0;

    if (!LoRa.begin(LORA_FREQUENCY)) {
        Serial.println("Starting LoRa failed for Transmitter!");
    } else {
        Serial.println("Setup Successful for Transmitter");
    }
}

void loop() {
    sendTransmitterId();

    // Generate a random transmission interval within the specified range
    int transmissionInterval = random(MIN_INTERVAL, MAX_INTERVAL);

    // Wait for the random transmission interval
    delay(transmissionInterval);
}

void sendTransmitterId() {
    LoRa.beginPacket();
    LoRa.print(TRANSMITTER_ID);
    LoRa.endPacket();
    numSent++;

    updateDisplayAndSerialMonitor();
}

void updateDisplayAndSerialMonitor() {
    u8g2->clearBuffer();
    char buf[256];
    snprintf(buf, sizeof(buf), "T: %i - Sent: %i", TRANSMITTER_ID, numSent);
    u8g2->drawStr(0, 40, buf);
    u8g2->sendBuffer();

    Serial.print("Transmitter ID: ");
    Serial.println(TRANSMITTER_ID);
    Serial.flush();
}