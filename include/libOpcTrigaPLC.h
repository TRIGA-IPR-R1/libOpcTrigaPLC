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

struct PLC_DATA
{
  int STATE = -1; // Status do PLC:
                  // -1 = Nenhuma tentativa de conexão realizada ainda.
                  //  0 = Valores lidos com sucesso.
                  //  1 = Erro de leitura
                  //  2 = Servidor desconectado
  std::chrono::system_clock::time_point TIME;
  // 
  float BarraReg      = -1; //ns=2;s=IoConfig_Globals_Mapping.inBarraReg (%IW5)   //Barra de Regulação
  float BarraCon      = -1; //ns=2;s=IoConfig_Globals_Mapping.inBarraCon (%IW6)   //Barra de Controle
  float BarraSeg      = -1; //ns=2;s=IoConfig_Globals_Mapping.inBarraSeg (%IW7)   //Barra de Segurança
  int   CLinScale     = -1; //ns=2;IoConfig_Globals_Mapping.inModulo_D0  (%IW0)   //Canal Linear - Escala (Bit 0, 1 e 3)
  float CLin          = -1; //ns=2;IoConfig_Globals_Mapping.inCLin (%IW8)         //Canal Linear
  float CPer          = -1; //ns=2;IoConfig_Globals_Mapping.inCPer (%IW13)        //Canal Percentual
  float CLogARea      = -1; //ns=2;IoConfig_Globals_Mapping.inCLogARea (%IW14)    //Canal Logarítimo - Aquisição Reativímetro
  float CLogALin      = -1; //ns=2;IoConfig_Globals_Mapping.inCLogALin (%IW15)    //Canal Logaritico - Aquisição Linear
  float CLogALog      = -1; //ns=2;IoConfig_Globals_Mapping.inCLogALog (%IW16)    //Canal Logaritico - Aquisição Logarítima
  float CLogAPer      = -1; //ns=2;IoConfig_Globals_Mapping.inCLogAPer (%IW17)    //Canal Logarítimo - Aquisição Período
  float CParALin      = -1; //ns=2;IoConfig_Globals_Mapping.inCParALin (%IW18)    //Canal Partida - Aquisição Linear
  float CParALog      = -1; //ns=2;IoConfig_Globals_Mapping.inCParALog (%IW19)    //Canal Partida - Aquisição Log
  float CParAPer      = -1; //ns=2;IoConfig_Globals_Mapping.inCParAPer (%IW20)    //Canal Partida - Aquisição Período
  float SRadAre       = -1; //ns=2;IoConfig_Globals_Mapping.inSRadAre (%IW25)     //Sensor Radiação Área
  float SRadEntPri    = -1; //ns=2;IoConfig_Globals_Mapping.inSRadEntPri (%IW26)  //Sensor Radiação Entrada Primário
  float SRadPoc       = -1; //ns=2;IoConfig_Globals_Mapping.inSRadPoc (%IW27)     //Sensor Radiação Poço
  float SRadRes       = -1; //ns=2;IoConfig_Globals_Mapping.inSRadRes (%IW28)     //Sensor Radiação Resinas
  float SRadSaiSec    = -1; //ns=2;IoConfig_Globals_Mapping.inSRadSaiSec (%IW29)  //Sensor Radiação Saída Secundário
  float SRadAer       = -1; //ns=2;IoConfig_Globals_Mapping.inSRadAer (%IW30)     //Sensor Radiação Aerossois
  float SVasPri       = -1; //ns=2;IoConfig_Globals_Mapping.inSVasPri (%IW49)     //Sensor Vazão Sistema Primário de Refrigeração
};

struct CONV_LIN
{
  float x0 =  0;
  float x1 =  1;
  float y0 = -2;
  float y1 = -2;
};

struct CONV_LOG
{
  double A = -2;
  double B =  0;
};

struct CONV_PER
{
  double K = 4096;//Bit referente +/-inf
  double L = 0;//Multiplicação
  //double M = 0;
  //double N = 0;
};

struct CONV_PLC {
  CONV_LIN BarraReg;
  CONV_LIN BarraCon;
  CONV_LIN BarraSeg;
  CONV_LIN CLogALin;
  CONV_LOG CLogALog;
  CONV_PER CLogAPer;
  CONV_LIN CParALin;
  CONV_LOG CParALog;
  CONV_PER CParAPer;
  CONV_LIN CLogARea;
  CONV_LIN CLin;
  CONV_LIN CPer;
  CONV_LOG SRadAre;
  CONV_LOG SRadEntPri;
  CONV_LOG SRadPoc;
  CONV_LOG SRadRes;
  CONV_LOG SRadSaiSec;
  CONV_LOG SRadAer;
  CONV_LIN SVasPri;
};

void libOpcTrigaPLC_license();

struct libOpcTrigaPLC_private;

class libOpcTrigaPLC {
public:
  libOpcTrigaPLC(std::string address);
  libOpcTrigaPLC(std::string address, std::string filename);
  ~libOpcTrigaPLC();

  CONV_PLC fatorConv;
  CONV_PLC readFatorConvFile(std::string filename);

  PLC_DATA convAllData(PLC_DATA plcOrig);
  PLC_DATA get_all_conv();
  PLC_DATA get_all();
  bool tryConnect();

private:
  libOpcTrigaPLC_private *_p;

  std::string stdErrorMsg(std::string functionName, std::string errorMsg,
                          std::string exptionMsg);

  float convLin(float  x, CONV_LIN conv);
  float convLog(double x, CONV_LOG conv);
  float convPer(double x, CONV_PER conv);  
};
