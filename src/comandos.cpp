#include "comandos.h"
#include "rs485.h"

void processCommands()
{
    uint8_t result;

    if(Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');

        cmd.trim();

        //-----------------------------------------
        // REMOTE LOCK 0x6804
        //-----------------------------------------

        if(cmd == "rl1")
        {
            result = node.writeSingleRegister(26628,1);

            if(result == node.ku8MBSuccess)
            {
                Serial.println("Remote Lock ON");
            }
            else
            {
                Serial.print("Erro RemoteLock: ");
                Serial.println(result);
            }

            return;
        }

        if(cmd == "rl0")
        {
            result = node.writeSingleRegister(26628,0);

            if(result == node.ku8MBSuccess)
            {
                Serial.println("Remote Lock OFF");
            }
            else
            {
                Serial.print("Erro RemoteLock: ");
                Serial.println(result);
            }

            return;
        }

        //-----------------------------------------
        // AUTO RELIGAMENTO 0x6801
        //-----------------------------------------

        if(cmd == "ar1")
        {
            result = node.writeSingleRegister(26625,1);

            if(result == node.ku8MBSuccess)
            {
                Serial.println("Auto Religamento: ON");
            }
            else
            {
                Serial.print("Erro AutoRelig: ");
                Serial.println(result);
            }

            return;
        }

        if(cmd == "ar0")
        {
            result = node.writeSingleRegister(26625,0);

            if(result == node.ku8MBSuccess)
            {
                Serial.println("Auto Religamento: OFF");
            }
            else
            {
                Serial.print("Erro AutoRelig: ");
                Serial.println(result);
            }

            return;
        }

        //-----------------------------------------
        // COMANDO HEX -> 0x6802
        //-----------------------------------------

        uint16_t valor = strtol(
            cmd.c_str(),
            NULL,
            16
        );

        Serial.print("Enviando 0x");
        Serial.println(valor,HEX);

        result = node.writeSingleRegister(
            26626,
            valor
        );

        if(result == node.ku8MBSuccess)
        {
            Serial.println("Comando OK");
        }
        else
        {
            Serial.print("Erro comando: ");
            Serial.println(result);
        }
    }
}