#include <SPI.h>
#include "ESP8266WiFi.h"

void initWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Print WiFi MAC address:
    printMacAddress();
}

void printMacAddress()
{
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC: ");
    Serial.print(mac[5], HEX);
    Serial.print(":");
    Serial.print(mac[4], HEX);
    Serial.print(":");
    Serial.print(mac[3], HEX);
    Serial.print(":");
    Serial.print(mac[2], HEX);
    Serial.print(":");
    Serial.print(mac[1], HEX);
    Serial.print(":");
    Serial.println(mac[0], HEX);
}

void scanSSIDs(char msg[])
{
    // scan for nearby networks:
    Serial.println("** Scan Networks **");
    int numSsid = WiFi.scanNetworks();
    if (numSsid == -1)
    {
        Serial.println("Couldn't get a wifi connection");
        return 0;
    }
    else if (numSsid > 5)
    {
        numSsid = 5;
    }
    uint8_t msgPointer = 0;

    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
    uint8_t *bssid;
    int32_t channel;
    bool hidden;
    int scanResult;

    // print the network number and name for each network found:
    for (int i = 0; i < numSsid; i++)
    {

        WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, hidden);

        // print out each network
        Serial.printf(PSTR("[%02X:%02X:%02X:%02X:%02X:%02X] %ddBm\n"),
                      bssid[0], bssid[1], bssid[2],
                      bssid[3], bssid[4], bssid[5],
                      rssi);
        // copy mac 1byte by 1byte
        for (int j = 0; j < 6; j++)
        {
            msg[msgPointer++] = (char)bssid[j];
        }
        // copy signal strength
        msg[msgPointer++] = (char)rssi;
    }
    // print full stack msg
    for (size_t i = 0; i < 35; i++)
    {
        Serial.print((char)msg[i], HEX);
    }
    Serial.println();

    return msgPointer;
}
