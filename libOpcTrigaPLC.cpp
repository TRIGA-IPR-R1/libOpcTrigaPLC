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

libOpcTrigaPLC::libOpcTrigaPLC(std::string ipAddress, std::string port)
{
    serverAddress = "opc.tcp://" + ipAddress + ":" + port;
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
        plcData.BarraReg        = client.getNode({2, "GVL.BarraReg"})    .readValueScalar<float>();
        plcData.BarraCon        = client.getNode({2, "GVL.BarraCon"})    .readValueScalar<float>();
        plcData.BarraSeg        = client.getNode({2, "GVL.BarraSeg"})    .readValueScalar<float>();
        plcData.CLogALog        = client.getNode({2, "GVL.CLogALog"})    .readValueScalar<float>();
        plcData.CLogALin        = client.getNode({2, "GVL.CLogALin"})    .readValueScalar<float>();
        plcData.CLogAPer        = client.getNode({2, "GVL.CLogAPer"})    .readValueScalar<float>();
        plcData.CParALin        = client.getNode({2, "GVL.CParALin"})    .readValueScalar<float>();
        plcData.CParALog        = client.getNode({2, "GVL.CParALog"})    .readValueScalar<float>();
        plcData.CParAPer        = client.getNode({2, "GVL.CParAPer"})    .readValueScalar<float>();
        plcData.CLogARea        = client.getNode({2, "GVL.CLogARea"})    .readValueScalar<float>();
        plcData.CLin            = client.getNode({2, "GVL.CLin"})        .readValueScalar<float>();
        plcData.CPer            = client.getNode({2, "GVL.CPer"})        .readValueScalar<float>();
        plcData.SRadAre         = client.getNode({2, "GVL.SRadAre"})     .readValueScalar<float>();
        plcData.SRadEntPri      = client.getNode({2, "GVL.SRadEntPri"})  .readValueScalar<float>();
        plcData.SRadPoc         = client.getNode({2, "GVL.SRadPoc"})     .readValueScalar<float>();
        plcData.SRadRes         = client.getNode({2, "GVL.SRadRes"})     .readValueScalar<float>();
        plcData.SRadSaiSec      = client.getNode({2, "GVL.SRadSaiSec"})  .readValueScalar<float>();
        plcData.SRadAer         = client.getNode({2, "GVL.SRadAer"})     .readValueScalar<float>();
        plcData.VasPri          = client.getNode({2, "GVL.VasPri"})      .readValueScalar<float>();
        plcData.SPt100Poco      = client.getNode({2, "GVL.SPt100Poco"})  .readValueScalar<float>();
        plcData.SPt100EntPri    = client.getNode({2, "GVL.SPt100EntPri"}).readValueScalar<float>();
        plcData.SPt100SaiPri    = client.getNode({2, "GVL.SPt100SaiPri"}).readValueScalar<float>();
        plcData.SPt100EntSec    = client.getNode({2, "GVL.SPt100EntSec"}).readValueScalar<float>();
        plcData.SPt100SaiSec    = client.getNode({2, "GVL.SPt100SaiSec"}).readValueScalar<float>();
        plcData.STpPoc1         = client.getNode({2, "GVL.STpPoc1"})     .readValueScalar<float>();
        plcData.STpPoc2         = client.getNode({2, "GVL.STpPoc2"})     .readValueScalar<float>();
        plcData.STpLen          = client.getNode({2, "GVL.STpLen"})      .readValueScalar<float>();
        plcData.SConPoc         = client.getNode({2, "GVL.SConPoc"})     .readValueScalar<float>();
        plcData.SConSaiPri      = client.getNode({2, "GVL.SConSaiPri"})  .readValueScalar<float>();
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

