#include "rs485.h"
#include <ModbusMaster.h>

// ------------------------
// UART2 ESP32
// ------------------------
#define RX_PIN     26
#define TX_PIN     27

// ------------------------
// Controle RS485
// ------------------------
#define RS485_DE   32
#define RS485_RE   33

HardwareSerial RS485Serial(2);

ModbusMaster node;

//---------------------------------
// RS485 -> transmitir
//---------------------------------
void preTransmission()
{
    digitalWrite(RS485_DE,HIGH);
    digitalWrite(RS485_RE,HIGH);
}

//---------------------------------
// RS485 -> receber
//---------------------------------
void postTransmission()
{
    digitalWrite(RS485_DE,LOW);
    digitalWrite(RS485_RE,LOW);
}

void initRS485()
{
    pinMode(RS485_DE,OUTPUT);
    pinMode(RS485_RE,OUTPUT);

    postTransmission();

    RS485Serial.begin(
        9600,
        SERIAL_8N1,
        RX_PIN,
        TX_PIN
    );

    node.begin(1,RS485Serial);

    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
}