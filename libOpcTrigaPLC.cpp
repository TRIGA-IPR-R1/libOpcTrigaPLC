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

libOpcTrigaPLC::libOpcTrigaPLC(std::string ipAddress, std::string port)
{
    try
    {
        client.connect("opc.tcp://" + ipAddress + ":" + port);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
}

libOpcTrigaPLC::~libOpcTrigaPLC()
{
    client.disconnect();
}

PLC_DATA libOpcTrigaPLC::get_all()
{
    try
    {
        plcData.BarraReg = client.getNode(opcua::NodeId("ns=2;s=GVL.BarraReg")).readValueScalar<float>();
        plcData.BarraCon = client.getNode(opcua::NodeId("ns=2;s=GVL.BarraCon")).readValueScalar<float>();
        plcData.BarraSeg = client.getNode(opcua::NodeId("ns=2;s=GVL.BarraSeg")).readValueScalar<float>();
        plcData.CLogALog = client.getNode(opcua::NodeId("ns=2;s=GVL.CLogALog")).readValueScalar<float>();
        plcData.CLogALin = client.getNode(opcua::NodeId("ns=2;s=GVL.CLogALin")).readValueScalar<float>();
        plcData.CLogAPer = client.getNode(opcua::NodeId("ns=2;s=GVL.CLogAPer")).readValueScalar<float>();

        plcData.STATE = 0;
    }
    catch (const std::exception& e)
    {
        plcData.STATE = 1;
    }


}

