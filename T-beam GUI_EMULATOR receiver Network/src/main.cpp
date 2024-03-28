#include <LoRa.h>
#include "boards.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <SD.h>
#include <Math.h>
#include "LookupTables.h"

void simulateObjectMovementAndSignalStrength();
float calculateDistance(float x1, float y1, float x2, float y2);
void findClosestRSSIandSNR(float distance, int& rssi, float& snr);

void updateDisplay(String msg) {
    u8g2->clearBuffer();
    char buf[256];
    snprintf(buf, sizeof(buf), "%s", msg);
    u8g2->drawStr(0, 40, buf);
    u8g2->sendBuffer();
}

void setup() {
    Serial.begin(115200);
    initBoard();
    delay(1500);
    // Initial setup message
    updateDisplay("EMULATOR");
}

void loop() {
    simulateObjectMovementAndSignalStrength();
    delay(500); // Update every second to simulate the movement
}

void simulateObjectMovementAndSignalStrength() {
    // Object's position calculation based on a simple time-dependent circular movement
    const float networkWidth = 400.0;
    const float networkHeight = 400.0;
    
    // Object's position calculation based on a simple time-dependent circular movement
    float objectX = networkWidth / 2 + networkWidth / 4 * cos(millis() / 10000.0);
    float objectY = networkHeight / 2 + networkHeight / 4 * sin(millis() / 10000.0);
    
    // Transmitter positions
    float txPositions[8][2] = {
        {0, 0},
        {networkWidth / 2, 0},
        {networkWidth, 0},
        {networkWidth, networkHeight / 2},
        {networkWidth, networkHeight},
        {networkWidth / 2, networkHeight},
        {0, networkHeight},
        {0, networkHeight / 2}
    };
    
    for(int i = 0; i < sizeof(txPositions) / sizeof(txPositions[0]); i++) {
        float distance = calculateDistance(objectX, objectY, txPositions[i][0], txPositions[i][1]);
        int rssi;
        float snr;
        findClosestRSSIandSNR(distance, rssi, snr);
        
        // Print the selected RSSI and SNR values for each transmitter
        Serial.print(i+1);
        Serial.print(",");
        Serial.print(rssi);
        Serial.print(",");
        Serial.print(snr);
        Serial.print(",");
        Serial.print(objectX);
        Serial.print(",");
        Serial.print(objectY);
        Serial.print(",");
        Serial.print(distance);
        Serial.print("\n");
    }
}

float calculateDistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void findClosestRSSIandSNR(float distance, int& rssi, float& snr) {
    // Initialize with the first entry
    float closestDistanceDiff = fabs(lookupTable800[0].distance - distance);
    int closestIndex = 0;
    
    // Search for the closest distance entry
    for (int i = 1; i < lookupTableSize800; i++) {
        float currentDistanceDiff = fabs(lookupTable800[i].distance - distance);
        if (currentDistanceDiff < closestDistanceDiff) {
            closestDistanceDiff = currentDistanceDiff;
            closestIndex = i;
        }
    }
    
    // Return the RSSI and SNR values for the closest entry
    rssi = lookupTable800[closestIndex].rssi;
    snr = lookupTable800[closestIndex].snr;
}

// #include <LoRa.h>
// #include "boards.h"
// #include <map>
// #include <vector>
// #include <string>
// #include <sstream>
// #include <SD.h>
// #include <Math.h>
// #include "LookupTables.h"


// void simulateObjectMovementAndSignalStrength();
// float calculateDistance(float x1, float y1, float x2, float y2);
// void findClosestRSSIandSNR(float distance, int& rssi, float& snr);

// void updateDisplay(String msg) {
//     u8g2->clearBuffer();
//     char buf[256];
//     snprintf(buf, sizeof(buf), "%s", msg);
//     u8g2->drawStr(0, 40, buf);
//     u8g2->sendBuffer();
// }

// void setup() {
//     Serial.begin(115200);
//     initBoard();
//     delay(1500);
//     // Initial setup message
//     updateDisplay("EMULATOR");
// }

// void loop() {
//     simulateObjectMovementAndSignalStrength();
//     delay(500); // Update every second to simulate the movement
// }

// void simulateObjectMovementAndSignalStrength() {
//     // Object's position calculation based on a simple time-dependent circular movement
//     const float networkWidth = 100.0;
//     const float networkHeight = 100.0;
    
//     // Object's position calculation based on a simple time-dependent circular movement
//     float objectX = networkWidth / 2 + networkWidth / 4 * cos(millis() / 10000.0);
//     float objectY = networkHeight / 2 + networkHeight / 4 * sin(millis() / 10000.0);

//     // Transmitter positions
//     float txPositions[4][2] = {{0, 0}, {networkWidth, 0}, {networkWidth, networkHeight}, {0, networkHeight}};
    
    
//     for(int i = 0; i < sizeof(txPositions) / sizeof(txPositions[0]); i++) {
//         float distance = calculateDistance(objectX, objectY, txPositions[i][0], txPositions[i][1]);
//         int rssi;
//         float snr;
//         findClosestRSSIandSNR(distance, rssi, snr);
        
//         // Print the selected RSSI and SNR values for each transmitter
//         Serial.print(i+1);
//         Serial.print(",");
//         Serial.print(rssi);
//         Serial.print(",");
//         Serial.print(snr);
//         Serial.print(",");
//         Serial.print(objectX);
//         Serial.print(",");
//         Serial.print(objectY);
//         Serial.print(",");
//         Serial.print(distance);
//         Serial.print("\n");
//     }
// }

// float calculateDistance(float x1, float y1, float x2, float y2) {
//     return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
// }

// void findClosestRSSIandSNR(float distance, int& rssi, float& snr) {
//     // Initialize with the first entry
//     float closestDistanceDiff = fabs(lookupTable800[0].distance - distance);
//     int closestIndex = 0;
    
//     // Search for the closest distance entry
//     for (int i = 1; i < lookupTableSize800; i++) {
//         float currentDistanceDiff = fabs(lookupTable800[i].distance - distance);
//         if (currentDistanceDiff < closestDistanceDiff) {
//             closestDistanceDiff = currentDistanceDiff;
//             closestIndex = i;
//         }
//     }

//     // Return the RSSI and SNR values for the closest entry
//     rssi = lookupTable800[closestIndex].rssi;
//     snr = lookupTable800[closestIndex].snr;
// }
