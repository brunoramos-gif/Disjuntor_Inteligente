#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "telemetry.h"
#include "medidas.h"


const char* endpoint =
"https://iothub-homolog.eletromidia.com.br/api/v1/teste-energia-led";

unsigned long lastSend = 0;

void sendTelemetry()
{
    if(millis() - lastSend < 30000)
        return;

    lastSend = millis();

    if(WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        http.begin(endpoint);
        http.addHeader("Content-Type","application/json");

        JsonDocument doc;

        //--------------------------------
        // EXEMPLO JSON
        //--------------------------------

        doc["device"] = "R0_01";

        JsonObject dados = doc["dados"].to<JsonObject>();

        dados["freq"] = medidas.freq;
        dados["tensao"] = medidas.v1;
        dados["corrente"] = medidas.i1;
        dados["potencia_total"] = medidas.pTotal;
        dados["potencia_a"] = medidas.pA;
        dados["temperatura"] = medidas.tempA;


        String json;
        serializeJson(doc,json);

        Serial.println();
        Serial.println("JSON:");
        Serial.println(json);

        int httpCode = http.POST(json);

        Serial.print("HTTP code: ");
        Serial.println(httpCode);

        http.end();
    }
    else
    {
        Serial.println("WiFi desconectado");
    }
}