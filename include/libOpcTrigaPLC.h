/*
libOpcTrigaPLC is a library to communicate with the Triga PLC
using OPC UA client (Ethernet) on a GNU operating system.
Copyright (C) 2024 Thalles Campagnani

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <iostream>
#include <stdexcept>
#include <chrono>

struct PLC_DATA {
  int STATE = -1; // Status do PLC:
                  // -1 = Nenhuma tentativa de conexão realizada ainda.
                  //  0 = Valores lidos com sucesso.
                  //  1 = Erro de leitura
                  //  2 = Servidor desconectado
  std::chrono::system_clock::time_point TIME;
  //
  float BarraReg = -1; // ns=2;IoConfig_Globals_Mapping.inBarraReg (%IW5)
                       // //Barra de Regulação
  float BarraCon = -1; // ns=2;IoConfig_Globals_Mapping.inBarraCon (%IW6)
                       // //Barra de Controle
  float BarraSeg = -1; // ns=2;IoConfig_Globals_Mapping.inBarraSeg (%IW7)
                       // //Barra de Segurança
  float CLogALog = -1; // ns=2;IoConfig_Globals_Mapping.inCLogALog (%IW17)
                       // //Canal Logaritico - Aquisição Logarítima
  float CLogALin = -1; // ns=2;IoConfig_Globals_Mapping.inCLogALin (%IW18)
                       // //Canal Logaritico - Aquisição Linear
  float CLogAPer = -1; // ns=2;IoConfig_Globals_Mapping.inCLogAPer (%IW19)
                       // //Canal Logarítimo - Aquisição Período
  float CParALin = -1; // ns=2;IoConfig_Globals_Mapping.inCParALin (%IW14)
                       // //Canal Partida - Aquisição Linear
  float CParALog = -1; // ns=2;IoConfig_Globals_Mapping.inCParALog (%IW15)
                       // //Canal Partida - Aquisição Log
  float CParAPer = -1; // ns=2;IoConfig_Globals_Mapping.inCParAPer (%IW16)
                       // //Canal Partida - Aquisição Período
  float CLogARea = -1; // ns=2;IoConfig_Globals_Mapping.inCLogARea (%IW20)
                       // //Canal Logarítimo - Aquisição Reativímetro
  float CLin =
      -1; // ns=2;IoConfig_Globals_Mapping.inCLin (%IW25)         //Canal Linear
  int CLinScale = -1; // ns=2;IoConfig_Globals_Mapping.inModulo_D0 (%IW0)
                      // //Canal Linear - Escala (Bit 0, 1 e 3)
  float CPer = -1; // ns=2;IoConfig_Globals_Mapping.inCPer (%IW26) //Canal
                   // Percentual
  float SRadAre = -1; // ns=2;IoConfig_Globals_Mapping.inSRadAre (%IW27)
                      // //Sensor Radiação Área
  float SRadEntPri = -1; // ns=2;IoConfig_Globals_Mapping.inSRadEntPri (%IW28)
                         // //Sensor Radiação Entrada Primário
  float SRadPoc = -1; // ns=2;IoConfig_Globals_Mapping.inSRadPoc (%IW29)
                      // //Sensor Radiação Poço
  float SRadRes = -1; // ns=2;IoConfig_Globals_Mapping.inSRadRes (%IW30)
                      // //Sensor Radiação Resinas
  float SRadSaiSec = -1; // ns=2;IoConfig_Globals_Mapping.inSRadSaiSec (%IW31)
                         // //Sensor Radiação Saída Secundário
  float SRadAer = -1; // ns=2;IoConfig_Globals_Mapping.inSRadAer (%IW32)
                      // //Sensor Radiação Aerossois
  float SVasPri = -1; // ns=2;IoConfig_Globals_Mapping.inSVasPri (%IW49)
                      // //Sensor Vazão Sistema Primário de Refrigeração
  float SPt100Poco = -1; // ns=2;s=GVL.SPt100Poco (%MD1) //PT100_TEMP_POCO
  float SPt100EntPri =
      -1; // ns=2;s=GVL.SPt100EntPri (%MD2) //PT100_TEMP_ENT_PRIM
  float SPt100SaiPri =
      -1; // ns=2;s=GVL.SPt100SaiPri (%MD3) //PT100_TEMP_SAI_PRIM
  float SPt100EntSec =
      -1; // ns=2;s=GVL.SPt100EntSec (%MD4) //PT100_TEMP_ENT_SEC
  float SPt100SaiSec =
      -1;                // ns=2;s=GVL.SPt100SaiSec (%MD5) //PT100_TEMP_SAI_SEC
  float STpPoc1 = -1;    // ns=2;s=GVL.STpPoc1 (%MD6) //TERMOPAR_1_POCO
  float STpPoc2 = -1;    // ns=2;s=GVL.STpPoc2 (%MD7) //TERMOPAR_2_POCO
  float STpLen = -1;     // ns=2;s=GVL.STpLen (%MD8) //TERMOPAR_3_LENCOL
  float SConPoc = -1;    // ns=2;s=GVL.SConPoc (%MD9) //COND_POCO
  float SConSaiPri = -1; // ns=2;s=GVL.SConSaiPri (%MD10) //COND_SAI_PRI
};

void libOpcTrigaPLC_license();

struct libOpcTrigaPLC_private;

class libOpcTrigaPLC {
public:
  libOpcTrigaPLC(std::string address);
  ~libOpcTrigaPLC();

  PLC_DATA get_all();
  bool tryConnect();

private:
  libOpcTrigaPLC_private *_p;

  std::string stdErrorMsg(std::string functionName, std::string errorMsg,
                          std::string exptionMsg);
};
