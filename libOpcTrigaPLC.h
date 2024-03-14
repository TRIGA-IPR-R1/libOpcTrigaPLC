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

#ifndef LIB_OPC_TRIGA_PLC
#define LIB_OPC_TRIGA_PLC

#include <stdexcept>
#include <iostream>
#include "open62541pp/open62541pp.h"

struct PLC_DATA
{
    int   STATE     = 0;
    //
    float BarraReg      = 0;//ns=2;s=GVL.BarraReg   //Barra de Regulação
    float BarraCon      = 0;//ns=2;s=GVL.BarraCon   //Barra de Controle
    float BarraSeg      = 0;//ns=2;s=GVL.BarraSeg   //Barra de Segurança
    //
    float CLogALog      = 0;//ns=2;s=GVL.CLogALog   //Canal Logaritico - Aquisição Logarítima
    float CLogALin      = 0;//ns=2;s=GVL.CLogALin   //Canal Logaritico - Aquisição Linear
    float CLogAPer      = 0;//ns=2;s=GVL.CLogAPer   //Canal Logarítimo - Aquisição Período
    //
    /*
    float CParALin      = 0;//ns=2;s=GVL.CParALin   //Canal Partida - Aquisição Linear
    float CParALog      = 0;//ns=2;s=GVL.CParALog   //Canal Partida - Aquisição Log
    float CParAPer      = 0;//ns=2;s=GVL.CParAPer   //Canal Partida - Aquisição Período
    //
    float CLogARea      = 0;//ns=2;s=GVL.CLogARea   //Canal Logarítimo - Aquisição Reativímetro
    float CLin          = 0;//ns=2;s=GVL.CLin       //Canal Linear
    float CPer          = 0;//ns=2;s=GVL.CPer       //Canal Percentual
    //
    float SRadAre       = 0;//ns=2;s=GVL.SRadAre    //Sensor Radiação Área
    float SRadEntPri    = 0;//ns=2;s=GVL.SRadEntPri //Sensor Radiação Entrada Primário
    float SRadPoc       = 0;//ns=2;s=GVL.SRadPoc    //Sensor Radiação Poço
    float SRadRes       = 0;//ns=2;s=GVL.SRadRes    //Sensor Radiação Resinas
    float SRadSaiSec    = 0;//ns=2;s=GVL.SRadSaiSec //Sensor Radiação Saída Secundário
    float SRadAer       = 0;//ns=2;s=GVL.SRadAer    //Sensor Radiação Aerossois
    //
    float VasPri        = 0;//ns=2;s=GVL.VasPri     //Sensor Vazão Sistema Primário de Refrigeração
    //
    float SPt100Poco    = 0;//ns=2;s=GVL.SPt100Poco     //PT100_TEMP_POCO
    float SPt100EntPri  = 0;//ns=2;s=GVL.SPt100EntPri   //PT100_TEMP_ENT_PRIM
    float SPt100SaiPri  = 0;//ns=2;s=GVL.SPt100SaiPri   //PT100_TEMP_SAI_PRIM
    float SPt100EntSec  = 0;//ns=2;s=GVL.SPt100EntSec   //PT100_TEMP_ENT_SEC
    float SPt100SaiSec  = 0;//ns=2;s=GVL.SPt100SaiSec   //PT100_TEMP_SAI_SEC
    float STpPoc1       = 0;//ns=2;s=GVL.STpPoc1        //TERMOPAR_1_POCO
    float STpPoc2       = 0;//ns=2;s=GVL.STpPoc2        //TERMOPAR_2_POCO
    float STpLen        = 0;//ns=2;s=GVL.STpLen         //TERMOPAR_3_LENCOL
    float SConPoc       = 0;//ns=2;s=GVL.SConPoc        //COND_POCO
    float SConSaiPri    = 0;//ns=2;s=GVL.SConSaiPri     //COND_SAI_PRI
    */
};


class libOpcTrigaPLC {
public:
    libOpcTrigaPLC(std::string ipAddress, std::string port);
    ~libOpcTrigaPLC();

    PLC_DATA get_all();

private:
    opcua::Client client;
    PLC_DATA plcData;
};

#endif // LIB_OPC_TRIGA_PLC