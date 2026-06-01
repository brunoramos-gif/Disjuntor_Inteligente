#include "status.h"
#include "rs485.h"

struct FaultBit
{
    uint8_t bit;
    const char* texto;
};

//-----------------------------------------
// Fault Status 0x5003
//-----------------------------------------

FaultBit faultList[] =
{
    {3,"SUBTENSAO"},
    {6,"SOBRECORRENTE"},
    {12,"FALTA DE FASE"}
};

//-----------------------------------------
// Memoria Falha 0x5303
//-----------------------------------------

const char* memoriaFalha[] =
{
    "SOBRETENSAO",
    "SUBTENSAO",
    "SOBRECORRENTE / CURTO",
    "OVERPOWER",
    "FALTA DE FASE",
    "DESEQUILIBRIO"
};

void readStatus()
{
    uint8_t result;

    //-----------------------------------------
    // BLOCO 1
    // Estado disjuntor 0x5000
    //-----------------------------------------

    delay(50);

    result = node.readHoldingRegisters(20480,1);

    uint16_t estado = 999;

    if(result == node.ku8MBSuccess)
    {
        estado = node.getResponseBuffer(0);
    }

    //-----------------------------------------
    // BLOCO 2
    // Fault Status 0x5003
    //-----------------------------------------

    delay(50);

    result = node.readHoldingRegisters(20483,1);

    uint16_t falha = 0;

    if(result == node.ku8MBSuccess)
    {
        falha = node.getResponseBuffer(0);
    }

    //-----------------------------------------
    // BLOCO 3
    // Short Alarm 0x5006
    //-----------------------------------------

    delay(50);

    result = node.readHoldingRegisters(20486,1);

    uint16_t curto = 0;

    if(result == node.ku8MBSuccess)
    {
        curto = node.getResponseBuffer(0);
    }

    //-----------------------------------------
    // BLOCO 4
    // Memoria falha 0x5303
    //-----------------------------------------

    delay(50);

    result = node.readHoldingRegisters(21251,1);

    uint16_t causa = 999;

    if(result == node.ku8MBSuccess)
    {
        causa = node.getResponseBuffer(0);
    }

    //-----------------------------------------
    // BLOCO 4.1
    // Snapshot falha
    //-----------------------------------------

    delay(50);

    uint16_t vA = 0;
    uint16_t iA = 0;
    uint16_t power = 0;

    result = node.readHoldingRegisters(21252,7);

    if(result == node.ku8MBSuccess)
    {
        vA = node.getResponseBuffer(0);
        iA = node.getResponseBuffer(3);
        power = node.getResponseBuffer(6);
    }

    //-----------------------------------------
    // PRINT
    //-----------------------------------------

    Serial.println();
    Serial.println("===== STATUS =====");

    //-----------------------------------------
    // Estado
    //-----------------------------------------

    Serial.print("Estado 0x5000: ");
    Serial.println(estado);

    //-----------------------------------------
    // 5006
    //-----------------------------------------

    if(curto == 1)
    {
        Serial.println("ALERTA: CURTO INSTANTANEO 5006");
    }

    //-----------------------------------------
    // 5003
    //-----------------------------------------

    int totalFaults =
    sizeof(faultList) /
    sizeof(faultList[0]);

    for(int i = 0; i < totalFaults; i++)
    {
        if(falha & (1UL << faultList[i].bit))
        {
            Serial.print("ALERTA: ");
            Serial.println(faultList[i].texto);
        }
    }

    //-----------------------------------------
    // 5303
    //-----------------------------------------

    Serial.print("Memoria Falha 5303: ");
    Serial.println(causa);

    if(causa <= 5)
    {
        Serial.print("Memoria: ");
        Serial.println(memoriaFalha[causa]);
    }
    else
    {
        Serial.println("Memoria: SEM EVENTO");
    }

    //-----------------------------------------
    // Snapshot evento
    //-----------------------------------------

    if(causa != 999)
    {
        Serial.println();
        Serial.println("=== ULTIMO EVENTO ===");

        Serial.print("VA antes falha: ");
        Serial.println(vA * 0.1);

        Serial.print("IA antes falha: ");
        Serial.println(iA * 0.01);

        Serial.print("Potencia antes falha: ");
        Serial.println(power);
    }

    Serial.println("==================");
}