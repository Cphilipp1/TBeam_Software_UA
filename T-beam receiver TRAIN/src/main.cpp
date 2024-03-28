#include <LoRa.h>
#include "boards.h"
#include "TinyGPSPlus.h"
#include <vector>

#ifdef HAS_DISPLAY
void displayPacketDetails(int spreadingFactor, long bandwidth) {
    u8g2->clearBuffer();
    char buf[256];
    snprintf(buf, sizeof(buf), "RSSI: %i SNR: %li", spreadingFactor, bandwidth);
    u8g2->drawStr(0, 40, buf);
    u8g2->sendBuffer();
}
#endif


void processGPSAndLoRaData();
void updateGPSLocation(double &lat, double &lon);
void handleReceivedPacket(double receiverLat, double receiverLon);
void parseCoordinates(String data, double &lat, double &lon);
void logPacketDetails(double distance, double receiverLat, double receiverLon, double transmitterLat, double transmitterLon);
void processTestPacket();

TinyGPSPlus gps;

int numRec;
const bool test = false; // Flag to toggle test mode

const int windowSize = 10;

std::vector<int> rssiValues(windowSize, 0); // Stores recent RSSI values
std::vector<float> snrValues(windowSize, 0.0); // Stores recent SNR values
int currentIndex = 0; // Index for the current value in the window
bool windowFilled = false; // Indicates if the window has been filled once

// Constants for distance estimation and Earth's radius
const float PathLossExponent = 3.0; // Path loss exponent, adjust for your environment
const double earthRadiusKm = 6371.0; // Earth's radius in kilometers

// Function to estimate distance based on RSSI
double estimateDistance(int rssi) {
    // Constants for the Log-distance path loss model
    const double pathLossExponent = 2.7; // Typical urban area value; adjust based on your environment
    const double rssiAtOneMeter = -19;   // Example value; you need to measure this for your setup

    double distance = pow(10.0, (rssiAtOneMeter - rssi) / (10.0 * pathLossExponent));
    return distance;
}

// Converts degrees to radians
double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// Calculates the distance between two points on Earth using the Haversine formula
double haversineMeters(double lat1, double lon1, double lat2, double lon2) {
    double dLat = degreesToRadians(lat2 - lat1);
    double dLon = degreesToRadians(lon2 - lon1);
    lat1 = degreesToRadians(lat1);
    lat2 = degreesToRadians(lat2);

    double a = sin(dLat/2) * sin(dLat/2) +
               sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2); 
    double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
    return earthRadiusKm * c * 1000; // Convert distance to meters
}


void setup() {
    initBoard();
    delay(1500); // Delay for stability after power-on

    numRec = 0;

    Serial.begin(115200);
    Serial.println("LoRa Receiver Initialization");
    Serial.println("Timestamp,RSSI,SNR,SpreadingFactor,Bandwidth,Coding Rage,Frequency,PreambleLength,PacketLength,TransmitterLat,TransmitterLon,ReceiverLat,ReceiverLon,GPSDistance,DistanceEstimate");

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
    }
}

void loop() {
    if (test) {
        // Test mode operations
        processTestPacket();
    } else {
        // Operational mode with GPS data handling
        processGPSAndLoRaData();
    }
}

void processTestPacket() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        numRec++;
        String receivedData = "";
        
        while (LoRa.available()) {
            receivedData += (char)LoRa.read();
        }

        // Read RSSI and SNR values
        int rssi = LoRa.packetRssi();
        float snr = LoRa.packetSnr();

        // Update the arrays with the latest values
        rssiValues[currentIndex] = rssi;
        snrValues[currentIndex] = snr;

        // Calculate the moving average
        int sumRssi = 0;
        float sumSnr = 0.0;
        int validEntries = windowFilled ? windowSize : currentIndex + 1;
        for (int i = 0; i < validEntries; ++i) {
            sumRssi += rssiValues[i];
            sumSnr += snrValues[i];
        }
        int averageRssi = sumRssi / validEntries;
        float averageSnr = sumSnr / validEntries;

        // Update the current index and check if the window has been filled
        currentIndex = (currentIndex + 1) % windowSize;
        if (currentIndex == 0) windowFilled = true;

        displayPacketDetails(averageRssi, averageSnr);
        Serial.println(rssi);
        Serial.println(snr);
        Serial.flush();
    }
}

void processGPSAndLoRaData() {
    double receiverLat = 0, receiverLon = 0;
    while (Serial1.available() > 0) {
        char c = Serial1.read();
        if(gps.encode(c)) {
            if (gps.location.isUpdated() && gps.location.isValid()) {
                receiverLat = gps.location.lat();
                receiverLon = gps.location.lng();


        int packetSize = LoRa.parsePacket();
        if (packetSize) {
            handleReceivedPacket(receiverLat, receiverLon);
        }            
        } 
        }
    }
}

void handleReceivedPacket(double receiverLat, double receiverLon) {
    
    String receivedData = "";
    while (LoRa.available()) {
        receivedData += (char)LoRa.read();
    }

    // Assuming the packet format is "lat,lon"
    double transmitterLat, transmitterLon;
    parseCoordinates(receivedData, transmitterLat, transmitterLon);

    if (transmitterLat == 0 || transmitterLon == 0) {
        Serial.println("Invalid coordinates received.");
        return;
    }

// Latitude: 32.231659 Longitude: -110.953139

// 32.231733176713156, -110.9530524010333 1km old main

    transmitterLat = 32.231733176713156; // old main 
    transmitterLon = -110.9530524010333;
    double distanceMeter = haversineMeters(receiverLat, receiverLon, transmitterLat, transmitterLon);
    logPacketDetails(distanceMeter, receiverLat, receiverLon, transmitterLat, transmitterLon);
}

void parseCoordinates(String data, double &lat, double &lon) {
    int commaIndex = data.indexOf(',');
    if (commaIndex != -1) {
        lat = data.substring(0, commaIndex).toDouble();
        lon = data.substring(commaIndex + 1).toDouble();
    }
}

void logPacketDetails(double distance, double receiverLat, double receiverLon, double transmitterLat, double transmitterLon) {
    // Read RSSI and SNR values
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    // Update the arrays with the latest values
    rssiValues[currentIndex] = rssi;
    snrValues[currentIndex] = snr;

    // Calculate the moving average
    int sumRssi = 0;
    float sumSnr = 0.0;
    int validEntries = windowFilled ? windowSize : currentIndex + 1;
    for (int i = 0; i < validEntries; ++i) {
        sumRssi += rssiValues[i];
        sumSnr += snrValues[i];
    }
    int averageRssi = sumRssi / validEntries;
    float averageSnr = sumSnr / validEntries;

    // Update the current index and check if the window has been filled
    currentIndex = (currentIndex + 1) % windowSize;
    if (currentIndex == 0) windowFilled = true;

    int spreadingFactor = LoRa.getSpreadingFactor();
    long signalBandwidth = LoRa.getSignalBandwidth();
    int codingRate = LoRa.getCodingRate();
    long frequency = LoRa.getFrequency();
    long preambleLength = LoRa.getPreambleLength();
    int packetLength = LoRa.parsePacket(0); // Assuming you call this at the right time to get packet length
    double distanceEstimate = estimateDistance(rssi);

    // Serial.println("Timestamp,RSSI,SNR,SpreadingFactor,Bandwidth,Coding Rage,Frequency,PreambleLength,PacketLength,TransmitterLat,TransmitterLon,ReceiverLat,ReceiverLon,Distance,DistanceEstimate");
    Serial.print(millis());
    Serial.print(",");
    Serial.print(averageRssi);
    Serial.print(",");
    Serial.print(averageSnr);
    Serial.print(",");
    Serial.print(spreadingFactor);
    Serial.print(",");
    Serial.print(signalBandwidth);
    Serial.print(",");
    Serial.print(codingRate);
    Serial.print(",");
    Serial.print(frequency);
    Serial.print(",");
    Serial.print(preambleLength);
    Serial.print(",");
    Serial.print(packetLength);
    Serial.print(",");
    Serial.print(transmitterLat, 6);
    Serial.print(",");
    Serial.print(transmitterLon, 6);
    Serial.print(",");
    Serial.print(receiverLat, 6);
    Serial.print(",");
    Serial.print(receiverLon, 6);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(",");
    Serial.print(distanceEstimate);
    Serial.println();
}


