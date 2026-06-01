#include "rs485.h"
#include "medidas.h"
#include "status.h"
#include "comandos.h"
#include "wifi_conn.h"
#include "telemetry.h"

void setup()
{
    Serial.begin(115200);

    initRS485();

    setupWiFi();

    Serial.println();
    Serial.println("===== EKR0 ONLINE =====");
    Serial.println();
}

void loop()
{
    Serial.println();
    Serial.println("--------------------");

    readMedidas();

    sendTelemetry();

    delay(2000);

    //processCommands();

    //readStatus();

    Serial.println("--------------------");

    delay(2000);
}