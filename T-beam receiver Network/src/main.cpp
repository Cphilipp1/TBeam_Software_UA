#include <LoRa.h>
#include "boards.h"
#include <map>
#include <queue>

bool parseTransmitterData(const String& data, int& transmitterId);
void processLoRaData();
void sendTransmitterDataToGUI();
void handleReceivedPacket();
void updateTransmitterData(int transmitterId, int rssi, float snr);
void printTransmitterDataMap();

void updateDisplay(String msg) {
    u8g2->clearBuffer();
    char buf[256];
    snprintf(buf, sizeof(buf), "%s", msg);
    u8g2->drawStr(0, 40, buf);
    u8g2->sendBuffer();
}

int currentIndex = 0; // Index for the current value in the window
bool windowFilled = false; // Indicates if the window has been filled once

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // Send data to GUI every 1000ms (1 second)

struct TransmitterData {
    const int windowSize = 10;
    std::queue<int> rssiWindow;
    std::queue<float> snrWindow;
    int rssiAvg;
    float snrAvg;
    int currentRssi; // Added variable for current RSSI value
};

std::map<int, TransmitterData> transmitterDataMap;

void setup() {
    initBoard();
    delay(1500);
    Serial.begin(115200);
    Serial.println("LoRa Receiver Initialization");
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
    }
    updateDisplay("Receiver");
}

void loop() {
    processLoRaData();

    if (millis() - lastSendTime >= sendInterval) {
        sendTransmitterDataToGUI();
        lastSendTime = millis();
        Serial.println("Sent Transmission!");
    }
}

void processLoRaData() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        handleReceivedPacket();
    }
}

void handleReceivedPacket() {
    String receivedData = "";
    while (LoRa.available()) {
        receivedData += static_cast<char>(LoRa.read());
    }

    if (receivedData.isEmpty()) {
        Serial.println("Received data is empty.");
        return;
    }

    Serial.print("Received data: ");
    Serial.println(receivedData);

    int transmitterId;
    if (parseTransmitterData(receivedData, transmitterId)) {
        int rssi = LoRa.packetRssi();
        float snr = LoRa.packetSnr();
        updateTransmitterData(transmitterId, rssi, snr);
        printTransmitterDataMap();
    } else {
        Serial.println("Failed to parse transmitter data.");
    }
}

bool parseTransmitterData(const String& data, int& transmitterId) {
    transmitterId = data.toInt();
    return true;
}

void updateTransmitterData(int transmitterId, int rssi, float snr) {
    if (transmitterId > 0 && transmitterId < 5) {
        TransmitterData& data = transmitterDataMap[transmitterId];

        data.currentRssi = rssi; // Store the current RSSI value

        // Update RSSI moving window average
        data.rssiWindow.push(rssi);
        if (data.rssiWindow.size() > data.windowSize) {
            data.rssiWindow.pop();
        }
        long long rssiSum = 0; // Change data type to long long
        std::queue<int> tempRssiWindow = data.rssiWindow;
        while (!tempRssiWindow.empty()) {
            rssiSum += tempRssiWindow.front();
            tempRssiWindow.pop();
        }
        data.rssiAvg = rssiSum / data.rssiWindow.size();

        // Update SNR moving window average
        data.snrWindow.push(snr);
        if (data.snrWindow.size() > data.windowSize) {
            data.snrWindow.pop();
        }
        float snrSum = 0;
        std::queue<float> tempSnrWindow = data.snrWindow;
        while (!tempSnrWindow.empty()) {
            snrSum += tempSnrWindow.front();
            tempSnrWindow.pop();
        }
        data.snrAvg = snrSum / data.snrWindow.size();
    }
}

void printTransmitterDataMap() {
    Serial.println("Transmitter Data Map:");
    for (const auto& entry : transmitterDataMap) {
        int transmitterId = entry.first;
        const TransmitterData& data = entry.second;
        Serial.print("Transmitter ID: ");
        Serial.print(transmitterId);
        Serial.print(", Current RSSI: ");
        Serial.print(data.currentRssi);
        Serial.print(", RSSI Avg: ");
        Serial.print(data.rssiAvg);
        Serial.print(", SNR Avg: ");
        Serial.println(data.snrAvg);
    }
    Serial.println();
}

void sendTransmitterDataToGUI() {
    if (!transmitterDataMap.empty()) {
        String message = "";
        for (const auto& entry : transmitterDataMap) {
            int transmitterId = entry.first;
            const TransmitterData& data = entry.second;
            message += String(transmitterId) + "," + String(data.rssiAvg) + "," + String(data.snrAvg) + ";";
        }
        message.remove(message.length() - 1); // Remove the last semicolon
        Serial.println(message);
        LoRa.beginPacket();
        LoRa.print(message);
        LoRa.endPacket();
        
        // Add a small delay to allow the device to switch back to receiver mode
        delay(10);
        // Switch the LoRa module back to receiver mode
        LoRa.receive();
    }
}


// #include <LoRa.h>
// #include "boards.h"
// #include <map>
// #include <deque>
// #include <numeric> // For std::accumulate

// bool parseTransmitterData(const String& data, int& transmitterId);
// void processLoRaData();
// void sendTransmitterDataToGUI();
// void handleReceivedPacket();
// void updateTransmitterData(int transmitterId, int rssi, float snr);
// void printTransmitterDataMap();

// void updateDisplay(String msg) {
//     u8g2->clearBuffer();
//     char buf[256];
//     snprintf(buf, sizeof(buf), "%s", msg);
//     u8g2->drawStr(0, 40, buf);
//     u8g2->sendBuffer();
// }

// unsigned long lastSendTime = 0;
// const unsigned long sendInterval = 1000; // Send data to GUI every 1000ms (1 second)

// struct TransmitterData {
//     int rssi;
//     float snr;
// };

// std::map<int, TransmitterData> transmitterDataMap;

// void setup() {
//     initBoard();
//     delay(1500);
//     Serial.begin(115200);
//     Serial.println("LoRa Receiver Initialization");
//     LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
//     if (!LoRa.begin(LoRa_frequency)) {
//         Serial.println("Starting LoRa failed!");
//     }
//     updateDisplay("Receiver");
// }

// void loop() {
//     processLoRaData();

//     if (millis() - lastSendTime >= sendInterval) {
//         sendTransmitterDataToGUI();
//         lastSendTime = millis();
//         Serial.println("Sent Transmission!");
//     }
// }

// void processLoRaData() {
//     int packetSize = LoRa.parsePacket();
//     if (packetSize) {
//         handleReceivedPacket();
//     }
// }

// void handleReceivedPacket() {
//     String receivedData = "";
//     while (LoRa.available()) {
//         receivedData += static_cast<char>(LoRa.read());
//     }

//     if (receivedData.isEmpty()) {
//         Serial.println("Received data is empty.");
//         return;
//     }

//     Serial.print("Received data: ");
//     Serial.println(receivedData);

//     int transmitterId;
//     if (parseTransmitterData(receivedData, transmitterId)) {
//         int rssi = LoRa.packetRssi();
//         float snr = LoRa.packetSnr();
//         updateTransmitterData(transmitterId, rssi, snr);
//         printTransmitterDataMap();
//     } else {
//         Serial.println("Failed to parse transmitter data.");
//     }
// }

// bool parseTransmitterData(const String& data, int& transmitterId) {
//     transmitterId = data.toInt();
//     return true;
// }

// void updateTransmitterData(int transmitterId, int rssi, float snr) {
//     if(transmitterId > 0 && transmitterId < 5){
//         transmitterDataMap[transmitterId] = {rssi, snr};
//     }
// }

// void printTransmitterDataMap() {
//     Serial.println("Transmitter Data Map:");
//     for (const auto& entry : transmitterDataMap) {
//         int transmitterId = entry.first;
//         const TransmitterData& data = entry.second;
//         Serial.print("Transmitter ID: ");
//         Serial.print(transmitterId);
//         Serial.print(", RSSI: ");
//         Serial.print(data.rssi);
//         Serial.print(", SNR: ");
//         Serial.println(data.snr);
//     }
//     Serial.println();
// }

// void sendTransmitterDataToGUI() {
// if (!transmitterDataMap.empty()) {
//         String message = "";
//         for (const auto& entry : transmitterDataMap) {
//             int transmitterId = entry.first;
//             const TransmitterData& data = entry.second;
//             message += String(transmitterId) + "," + String(data.rssi) + "," + String(data.snr) + ";";
//         }
//         message.remove(message.length() - 1); // Remove the last semicolon
//         Serial.println(message);
//         LoRa.beginPacket();
//         LoRa.print(message);
//         LoRa.endPacket();
        
//         // Add a small delay to allow the device to switch back to receiver mode
//         delay(10);
//         // Switch the LoRa module back to receiver mode
//         LoRa.receive();
//     }
// }
