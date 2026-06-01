#pragma once

#include <Arduino.h>
#include <ModbusMaster.h>

extern HardwareSerial RS485Serial;
extern ModbusMaster node;

void initRS485();
void preTransmission();
void postTransmission();