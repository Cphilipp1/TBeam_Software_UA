#include <LoRa.h>
#include "boards.h"
#include <map>

void updateDisplay(String msg) {
    u8g2->clearBuffer();
    char buf[256];
    snprintf(buf, sizeof(buf), "%s", msg);
    u8g2->drawStr(0, 40, buf);
    u8g2->sendBuffer();
}

void receiveTransmitterData();
void printTransmitterDataToSerial();
void parseTransmitterDataMessage(const String& message);

struct TransmitterData {
    int rssi;
    float snr;
};

std::map<int, TransmitterData> transmitterDataMap;

void setup() {
    initBoard();
    delay(1500);
    Serial.begin(115200);
    Serial.println("LoRa GUI Receiver Initialization");
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
    }
    updateDisplay("GUI");
}

void loop() {
    receiveTransmitterData();
    printTransmitterDataToSerial();
}

void receiveTransmitterData() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String receivedData = "";
        while (LoRa.available()) {
            receivedData += static_cast<char>(LoRa.read());
        }

        if (receivedData.isEmpty()) {
            Serial.println("Received data is empty.");
            return;
        }

        parseTransmitterDataMessage(receivedData);
    }
}

void parseTransmitterDataMessage(const String& message) {
    transmitterDataMap.clear();

    int startIndex = 0;
    int endIndex;
    while ((endIndex = message.indexOf(';', startIndex)) != -1) {
        String entry = message.substring(startIndex, endIndex);
        int commaIndex1 = entry.indexOf(',');
        int commaIndex2 = entry.indexOf(',', commaIndex1 + 1);
        if (commaIndex1 != -1 && commaIndex2 != -1) {
            int transmitterId = entry.substring(0, commaIndex1).toInt();
            int rssi = entry.substring(commaIndex1 + 1, commaIndex2).toInt();
            float snr = entry.substring(commaIndex2 + 1).toFloat();
            transmitterDataMap[transmitterId] = {rssi, snr};
        }
        startIndex = endIndex + 1;
    }

    // Process the last entry if it doesn't have a trailing semicolon
    if (startIndex < message.length()) {
        String entry = message.substring(startIndex);
        int commaIndex1 = entry.indexOf(',');
        int commaIndex2 = entry.indexOf(',', commaIndex1 + 1);
        if (commaIndex1 != -1 && commaIndex2 != -1) {
            int transmitterId = entry.substring(0, commaIndex1).toInt();
            int rssi = entry.substring(commaIndex1 + 1, commaIndex2).toInt();
            float snr = entry.substring(commaIndex2 + 1).toFloat();
            transmitterDataMap[transmitterId] = {rssi, snr};
        }
    }
}

void printTransmitterDataToSerial() {
    if (!transmitterDataMap.empty()) {
        for (const auto& entry : transmitterDataMap) {
            int transmitterId = entry.first;
            const TransmitterData& data = entry.second;
            Serial.print(transmitterId);
            Serial.print(",");
            Serial.print(data.rssi);
            Serial.print(",");
            Serial.println(data.snr);
        }
        Serial.println();
        delay(1000);
    }
}