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

#include "libOpcTrigaPLC.h"

void libOpcTrigaPLC_license()
{
    std::cout << "libOpcTrigaPLC    Copyright (C) 2024 Thalles Campagnani" << std::endl;
    std::cout << "This program comes with ABSOLUTELY NO WARRANTY;" << std::endl;
    std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
    std::cout << "under certain conditions; For more details read the file LICENSE" << std::endl;
    std::cout << "that came together with the library." << std::endl << std::endl;
}

bool libOpcTrigaPLC::tryConnect()
{
    try
    {
        client.connect(serverAddress);
    }
    catch (const std::exception& e)
    {
        std::cerr << stdErrorMsg("tryConnect()", "Erro ao tentar conectar", e.what());
        return 1;
    }
    return 0;
}

std::string libOpcTrigaPLC::stdErrorMsg(std::string functionName, std::string errorMsg, std::string exptionMsg)
{
    std::string msg =   "ERROR in libOpcTrigaPLC::" + 
                        functionName + 
                        "\n\tError type: " + 
                        errorMsg;
    if (exptionMsg!="") 
    {
        msg +=          "\n\tError code: " + 
                        exptionMsg;
    }
    msg += "\n";
    return msg;
}

libOpcTrigaPLC::libOpcTrigaPLC(std::string address)
{
    serverAddress = "opc.tcp://" + address;
    tryConnect();
}

libOpcTrigaPLC::~libOpcTrigaPLC()
{
    client.disconnect();
}

PLC_DATA libOpcTrigaPLC::get_all()
{
    try
    {
        plcData.BarraReg        = client.getNode({2, "IoConfig_Globals_Mapping.inBarraReg (%IW5)"})   .readValueScalar<int16_t>();
        plcData.BarraCon        = client.getNode({2, "IoConfig_Globals_Mapping.inBarraCon (%IW6)"})   .readValueScalar<int16_t>();
        plcData.BarraSeg        = client.getNode({2, "IoConfig_Globals_Mapping.inBarraSeg (%IW7)"})   .readValueScalar<int16_t>();
        plcData.CLogALog        = client.getNode({2, "IoConfig_Globals_Mapping.inCLogALog (%IW17)"})  .readValueScalar<int16_t>();
        plcData.CLogALin        = client.getNode({2, "IoConfig_Globals_Mapping.inCLogALin (%IW18)"})  .readValueScalar<int16_t>();
        plcData.CLogAPer        = client.getNode({2, "IoConfig_Globals_Mapping.inCLogAPer (%IW19)"})  .readValueScalar<int16_t>();
        plcData.CParALin        = client.getNode({2, "IoConfig_Globals_Mapping.inCParALin (%IW14)"})  .readValueScalar<int16_t>();
        plcData.CParALog        = client.getNode({2, "IoConfig_Globals_Mapping.inCParALog (%IW15)"})  .readValueScalar<int16_t>();
        plcData.CParAPer        = client.getNode({2, "IoConfig_Globals_Mapping.inCParAPer (%IW16)"})  .readValueScalar<int16_t>();
        plcData.CLogARea        = client.getNode({2, "IoConfig_Globals_Mapping.inCLogARea (%IW20)"})  .readValueScalar<int16_t>();
        plcData.CLin            = client.getNode({2, "IoConfig_Globals_Mapping.inCLin (%IW25)"})      .readValueScalar<int16_t>();
        plcData.CLinScale       = client.getNode({2, "IoConfig_Globals_Mapping.inModule_D0 (%IW0)"})  .readValueScalar<u_int16_t>() & 0b00000111 - 2;
        plcData.CPer            = client.getNode({2, "IoConfig_Globals_Mapping.inCPer (%IW26)"})      .readValueScalar<int16_t>();
        plcData.SRadAre         = client.getNode({2, "IoConfig_Globals_Mapping.inSRadAre (%IW27)"})   .readValueScalar<int16_t>();
        plcData.SRadEntPri      = client.getNode({2, "IoConfig_Globals_Mapping.inSRadEntPri (%IW28)"}).readValueScalar<int16_t>();
        plcData.SRadPoc         = client.getNode({2, "IoConfig_Globals_Mapping.inSRadPoc (%IW29)"})   .readValueScalar<int16_t>();
        plcData.SRadRes         = client.getNode({2, "IoConfig_Globals_Mapping.inSRadRes (%IW30)"})   .readValueScalar<int16_t>();
        plcData.SRadSaiSec      = client.getNode({2, "IoConfig_Globals_Mapping.inSRadSaiSec (%IW31)"}).readValueScalar<int16_t>();
        plcData.SRadAer         = client.getNode({2, "IoConfig_Globals_Mapping.inSRadAer (%IW32)"})   .readValueScalar<int16_t>();
        plcData.SVasPri         = client.getNode({2, "IoConfig_Globals_Mapping.inSVasPri (%IW49)"})   .readValueScalar<int16_t>();
        plcData.SPt100Poco      = client.getNode({2, "GVL.SPt100Poco (%MD1)"})                        .readValueScalar<float>();
        plcData.SPt100EntPri    = client.getNode({2, "GVL.SPt100EntPri (%MD2)"})                      .readValueScalar<float>();
        plcData.SPt100SaiPri    = client.getNode({2, "GVL.SPt100SaiPri (%MD3)"})                      .readValueScalar<float>();
        plcData.SPt100EntSec    = client.getNode({2, "GVL.SPt100EntSec (%MD4)"})                      .readValueScalar<float>();
        plcData.SPt100SaiSec    = client.getNode({2, "GVL.SPt100SaiSec (%MD5)"})                      .readValueScalar<float>();
        plcData.STpPoc1         = client.getNode({2, "GVL.STpPoc1 (%MD6)"})                           .readValueScalar<float>();
        plcData.STpPoc2         = client.getNode({2, "GVL.STpPoc2 (%MD7)"})                           .readValueScalar<float>();
        plcData.STpLen          = client.getNode({2, "GVL.STpLen (%MD8)"})                            .readValueScalar<float>();
        plcData.SConPoc         = client.getNode({2, "GVL.SConPoc (%MD9)"})                           .readValueScalar<float>();
        plcData.SConSaiPri      = client.getNode({2, "GVL.SConSaiPri (%MD10)"})                       .readValueScalar<float>();
        plcData.STATE = 0;
    }
    catch (const std::exception& e)
    {
        if (client.isConnected())
        {
            std::cerr << stdErrorMsg("get_all()", "Cliente conectado, porém erro ao adquirir dados", e.what());
            plcData.STATE = 1;
        }
        else
        {
            std::cerr << stdErrorMsg("get_all()", "Cliente desconectado", e.what());
            plcData.STATE = 2;
        }
    }

    return plcData;
}

