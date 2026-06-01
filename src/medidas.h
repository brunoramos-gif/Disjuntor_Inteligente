#pragma once
#ifndef MEDIDAS_H
#define MEDIDAS_H

struct Medidas
{
    float freq;
    float v1;
    float i1;
    float pTotal;
    float pA;
    float tempA;
};

extern Medidas medidas;



void readMedidas();

#endif // MEDIDAS_H