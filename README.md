# ⚡ ESP32 Modbus - Smart Circuit Breaker R0 Series

Este projeto implementa a comunicação, monitoramento e controle de um Disjuntor Inteligente (Série R0) utilizando um ESP32 e o protocolo Modbus RTU via módulo RS-485 (MAX485). 

O código foi projetado de forma modular, permitindo a leitura contínua (polling em lote) e a atuação em tempo real através de comandos seriais, garantindo a estabilidade do barramento.

---

## 📂 Estrutura dos Arquivos

A arquitetura do projeto foi dividida para separar a camada de hardware das regras de negócio:

* **`main.cpp`**: Arquivo principal. Gerencia a máquina de estados, o tempo de polling (leitura contínua) e a interface interativa de testes via Monitor Serial.
* **`rs485.h` / `rs485.cpp`**: Módulo de Comunicação. Isola a configuração do hardware (pinos RX/TX, controle Half-Duplex DE/RE) e a integração com a biblioteca `ModbusMaster`.
* **`test.h` / `test.cpp`**: Regras de Negócio e Lógica. Contém as funções de leitura de telemetria, conversão de escalas (Hex para Decimal), comandos remotos e injeção de falhas simuladas por software.

---

## 📊 Funcionalidades Validadas (Mapeamento Modbus)

Abaixo estão listados todos os registradores mapeados. **Atenção aos itens marcados como "NÃO" na coluna de Validação**, pois indicam funcionalidades que não responderam neste modelo específico de hardware/firmware, possivelmente por exigirem módulos físicos adicionais (como DR interno de fuga).

### 1. Telemetria e Grandezas Elétricas
*Todas as leituras de telemetria abaixo foram testadas e retornaram `OK`.*

| Parâmetro | Endereço (Hex) | Multiplicador / Resolução | Validação |
| :--- | :---: | :--- | :---: |
| Tensão (Fases A, B, C) | `0x0004` a `0x0006` | 0.1 V | ✅ OK |
| Corrente (Fases A, B, C) | `0x000A` a `0x000C` | 0.01 A | ✅ OK |
| Corrente Neutro | `0x000D` | 0.01 A | ✅ OK |
| Potência Ativa (Total, Fases A, B, C) | `0x000E` a `0x0011` | 0.01 kW | ✅ OK |
| Fator de Potência (Total, Fases A, B, C) | `0x001A` a `0x001D` | 1 (Ex: 985 = 0.985) | ✅ OK |
| Temperatura (Geral, Fases A, B, C, Neutro) | `0x001E` a `0x0023` | 0.1 °C | ✅ OK |
| Energia Ativa Total (Acumulada) | `0x1000` | 0.01 kWh (DWORD/2 Regs) | ✅ OK |
| Energia Consumida | `0x1014` | 0.01 kWh (DWORD/2 Regs) | ✅ OK |
| Energia Exportada | `0x101E` | 0.01 kWh (DWORD/2 Regs) | ✅ OK |

### 2. Status do Equipamento (`0x5000`)
*Leitura de mapa de bits (Bit Status) no registrador 0x5000.*

| Parâmetro (Bit Status) | Descrição do Estado | Validação |
| :--- | :--- | :---: |
| `Bit 0` a `Bit 2` | Abertura (Manual, por Comando, por Falha) | ✅ OK |
| `Bit 3` | Abertura por Falta de Energia | ✅ OK |
| `Bit 4` a `Bit 6` | Fechamento (Manual, por Comando, Automático) | ✅ OK |
| `Bit 7` e `Bit 8` | Bloqueio (por Comando, por Falha) | ✅ OK |
| `Bit 9` | Bloqueio Mecânico (Mechanism Padlock) | ✅ OK |
| `Bit 10` e `Bit 11` | Abertura e Fechamento Programado (Timing) | ✅ OK |
| `Bit 12` | Abertura por Fuga de Corrente (Leakage Switching) | ❌ **NÃO** |

### 3. Alarmes e Proteções
*Mapas de bits lidos nos registradores `0x5003` (Elétricos), `0x5004` (Térmicos), e booleanos `0x5005`/`0x5006`.*

| Parâmetro | Endereço (Hex) | Descrição do Alarme | Validação |
| :--- | :---: | :--- | :---: |
| Sobretensão (Fases A, B, C) | `0x5003` (Bits 0 a 2) | Disparo por alta tensão (OV) | ✅ OK |
| Subtensão (Fases A, B, C) | `0x5003` (Bits 3 a 5) | Disparo por baixa tensão (UV) | ✅ OK |
| Sobrecorrente (Fases A, B, C) | `0x5003` (Bits 6 a 8) | Disparo por alta corrente (OC) | ✅ OK |
| Sobrepotência (Fases A, B, C) | `0x5003` (Bits 9 a 11) | Disparo por alta potência (OP) | ✅ OK |
| Falta de Fase (Fases A, B, C) | `0x5003` (Bits 12 a 14) | Disparo por perda de fase (LOSS) | ✅ OK |
| Desequilíbrio | `0x5003` (Bit 15) | Disparo por desequilíbrio entre fases | ✅ OK |
| Temperatura Alta (A, B, C, Neutro) | `0x5004` (Bits 0 a 3) | Disparo por superaquecimento | ✅ OK |
| Fuga / Corrente Residual | `0x5005` | Alarme de Fuga Elétrica | ❌ **NÃO** |
| Curto-Circuito | `0x5006` | Alarme de Curto | ❌ **NÃO** |

### 4. Comandos Remotos e Travas (`Write`)

| Categoria | Ação / Comando | Endereço (Hex) | Valor a Escrever | Validação |
| :--- | :--- | :---: | :---: | :---: |
| **Comando** | Bloquear Disjuntor (Break lock) | `0x6802` | `0x0003` | ✅ OK |
| **Comando** | Desligar sem Travar | `0x6802` | `0x0013` | ✅ OK |
| **Comando** | Desligar e Destravar (Ligar) | `0x6802` | `0x0023` | ✅ OK |
| **Comando** | Desligar Padrão | `0x6802` | `0x0033` | ✅ OK |
| **Config.** | Religamento Automático (On/Off) | `0x6801` | `1` / `0` | ✅ OK |
| **Config.** | Trava Remota (Lock/Unlock) | `0x6804` | `1` / `0` | ✅ OK |

### 5. Parametrização de Proteções (Ajuste de Limites)

A configuração das proteções via software permite alterar o comportamento de desarme preventivo do equipamento.

| Parâmetro de Proteção | Endereço (Hex) | Ação / Valores de Exemplo |
| :--- | :---: | :--- |
| Ação de Sobretensão | `0x6600` | `1` (Apenas Alarme) / `3` (Alarme + Desarme Mecânico) |
| Limite de Sobretensão | `0x6601` | Resolução de 0.1 V (Ex: `2800` = 280.0 V) |
| Tempo de Julg. Sobretensão | `0x6603` | Resolução em Segundos (Ex: `5` = 5 segundos) |
| Ação de Subtensão | `0x6605` | `1` (Apenas Alarme) / `3` (Alarme + Desarme Mecânico) |
| Limite de Subtensão | `0x6606` | Resolução de 0.1 V (Ex: `1150` = 115.0 V) |
| Tempo de Julg. Subtensão | `0x6608` | Resolução em Segundos |
| Ação de Sobrecorrente | `0x660A` | `1` (Apenas Alarme) / `3` (Alarme + Desarme Mecânico) |
| Limite de Sobrecorrente | `0x660B` | Resolução de 0.01 A (Ex: `10000` = 100.00 A) |
| Tempo de Julg. Sobrecorrente | `0x660C` | Resolução em Segundos |

> **Nota:** Estes limites foram validados utilizando uma rotina própria de "Injeção de Falhas" (ex: `simularSobretensao()`), que escreve limites propositalmente baixos/altos no disjuntor para forçar o disparo dos alarmes na bancada, validando a assertividade da leitura da nossa aplicação.
