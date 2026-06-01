#include <Arduino.h>
#include <WiFi.h>
#include "wifi_conn.h"

const char* ssid = "ELT-Visitantes";
const char* password = "Eletromidia@2024";

void setupWiFi()
{
    Serial.println("");
    Serial.print("Connecting to WiFi...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}