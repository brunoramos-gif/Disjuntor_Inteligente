#include "medidas.h"
#include "rs485.h"
Medidas medidas;

void readMedidas()
{
    uint8_t result;

    //-----------------------------------------
    // BLOCO 1
    // Medidas principais
    //-----------------------------------------

    delay(50);

    result = node.readHoldingRegisters(3,14);

    if(result == node.ku8MBSuccess)
    {
        //-----------------------------------------
        // Registradores
        //-----------------------------------------

        uint16_t regFreq = node.getResponseBuffer(0);
        uint16_t regV1 = node.getResponseBuffer(1);
        uint16_t regI1 = node.getResponseBuffer(7);
        uint16_t regPtotal = node.getResponseBuffer(11);
        uint16_t regPA = node.getResponseBuffer(12);

        //-----------------------------------------
        // Conversão
        //-----------------------------------------

        medidas.freq = regFreq * 0.01;
        medidas.v1 = regV1 * 0.1;
        medidas.i1 = regI1 * 0.01;
        medidas.pTotal = regPtotal * 0.01;
        medidas.pA = regPA * 0.01;

        //-----------------------------------------
        // BLOCO 2
        // Temperatura
        //-----------------------------------------

        delay(50);

        result = node.readHoldingRegisters(30,5);

        if(result == node.ku8MBSuccess)
        {
            int16_t regTempA =
            node.getResponseBuffer(2);

            medidas.tempA = regTempA * 0.1;

            //-----------------------------------------
            // PRINT
            //-----------------------------------------

            Serial.println();
            Serial.println("===== MEDIDAS =====");

            Serial.print("Freq: ");
            Serial.print(medidas.freq);
            Serial.println(" Hz");

            Serial.print("Tensao A: ");
            Serial.print(medidas.v1);
            Serial.println(" V");

            Serial.print("Corrente A: ");
            Serial.print(medidas.i1);
            Serial.println(" A");

            Serial.print("Potencia Total: ");
            Serial.print(medidas.pTotal);
            Serial.println(" kW");

            Serial.print("Potencia Fase A: ");
            Serial.print(medidas.pA);
            Serial.println(" kW");

            Serial.print("Temp A: ");
            Serial.print(medidas.tempA);
            Serial.println(" C");

            Serial.println("==================");
        }
        else
        {
            Serial.print("Erro temperatura: ");
            Serial.println(result);
        }
    }
    else
    {
        Serial.print("Erro medidas: ");
        Serial.println(result);
    }
}