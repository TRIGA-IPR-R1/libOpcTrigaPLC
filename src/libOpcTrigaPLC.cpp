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

#include <libOpcTrigaPLC.h>
#include <cmath>
#include <open62541pp/open62541pp.h>

struct libOpcTrigaPLC_private {
    opcua::Client client;
    PLC_DATA plcData;
    std::string serverAddress;
};

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
        this->_p->client.connect(this->_p->serverAddress);
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
    this->_p = new libOpcTrigaPLC_private;
    this->_p->serverAddress = "opc.tcp://" + address;
    tryConnect();
}

libOpcTrigaPLC::libOpcTrigaPLC(std::string address, std::string filename)
{
    this->_p = new libOpcTrigaPLC_private;
    this->_p->serverAddress = "opc.tcp://" + address;
    tryConnect();
    this->fatorConv = this->readFatorConvFile(filename);
}

libOpcTrigaPLC::~libOpcTrigaPLC()
{
    this->_p->client.disconnect();
    delete this->_p;
}

//Função para converter o valor de x através de 2 pontos conhecidos. 
float libOpcTrigaPLC::convLin(float x, CONV_LIN conv)
{
    return x * (conv.y1-conv.y0) / (conv.x1-conv.x0) + (conv.y0*conv.x1-conv.y1*conv.x0) / (conv.x1-conv.x0);
}

//Função para converter o valor de x, sendo x logarítimo de base 10. 
float libOpcTrigaPLC::convLog(double x, CONV_LOG conv)
{
    return conv.A*std::pow(10,conv.B*x);
}

//Função para converter o valor de x, sendo x sinal de período. 
float libOpcTrigaPLC::convPer(float x, CONV_PER conv)
{
    return x;
}

float libOpcTrigaPLC::convRea(float x, CONV_REA conv)
{
    return x;
}

//Função para converter os dados brutos do PLC
PLC_DATA libOpcTrigaPLC::convAllData(PLC_DATA plcOrig, CONV_PLC fatorConv)
{
    PLC_DATA plcConv   = plcOrig;
    plcConv.BarraReg   = convLin(plcOrig.BarraReg,   fatorConv.BarraReg);//Converter bits para "posições de barra"
    plcConv.BarraCon   = convLin(plcOrig.BarraCon,   fatorConv.BarraCon);
    plcConv.BarraSeg   = convLin(plcOrig.BarraSeg,   fatorConv.BarraSeg);
    plcConv.CLogALin   = convLin(plcOrig.CLogALin,   fatorConv.CLogALin);//Converter bits para W
    plcConv.CLogALog   = convLog(plcOrig.CLogALog,   fatorConv.CLogALog);
    plcConv.CLogAPer   = convPer(plcOrig.CLogAPer,   fatorConv.CLogAPer);
    plcConv.CParALin   = convLin(plcOrig.CParALin,   fatorConv.CParALin);
    plcConv.CParALog   = convLog(plcOrig.CParALog,   fatorConv.CParALog);
    plcConv.CParAPer   = convPer(plcOrig.CParAPer,   fatorConv.CParAPer);
    plcConv.CLogARea   = convRea(plcOrig.CLogARea,   fatorConv.CLogARea);
    plcConv.CLin       = convLin(plcOrig.CLin,       fatorConv.CLin);
    plcConv.CPer       = convLin(plcOrig.CPer,       fatorConv.CPer);
    plcConv.SRadAre    = convLog(plcOrig.SRadAre,    fatorConv.SRadAer);
    plcConv.SRadEntPri = convLog(plcOrig.SRadEntPri, fatorConv.SRadEntPri);
    plcConv.SRadPoc    = convLog(plcOrig.SRadPoc,    fatorConv.SRadPoc);
    plcConv.SRadRes    = convLog(plcOrig.SRadRes,    fatorConv.SRadRes);
    plcConv.SRadSaiSec = convLog(plcOrig.SRadSaiSec, fatorConv.SRadSaiSec);
    plcConv.SRadAer    = convLog(plcOrig.SRadAer,    fatorConv.SRadAer);
    plcConv.SVasPri    = convLin(plcOrig.SVasPri,    fatorConv.SVasPri);//Converter bits para m^3/h
    return plcConv;
}

PLC_DATA libOpcTrigaPLC::get_all_conv()
{
    this->get_all();
    this->_p->plcData.BarraReg   = convLin(this->_p->plcData.BarraReg,   this->fatorConv.BarraReg);//Converter bits para "posições de barra"
    this->_p->plcData.BarraCon   = convLin(this->_p->plcData.BarraCon,   this->fatorConv.BarraCon);
    this->_p->plcData.BarraSeg   = convLin(this->_p->plcData.BarraSeg,   this->fatorConv.BarraSeg);
    this->_p->plcData.CLogALin   = convLin(this->_p->plcData.CLogALin,   this->fatorConv.CLogALin);//Converter bits para W
    this->_p->plcData.CLogALog   = convLog(this->_p->plcData.CLogALog,   this->fatorConv.CLogALog);
    this->_p->plcData.CLogAPer   = convPer(this->_p->plcData.CLogAPer,   this->fatorConv.CLogAPer);
    this->_p->plcData.CParALin   = convLin(this->_p->plcData.CParALin,   this->fatorConv.CParALin);
    this->_p->plcData.CParALog   = convLog(this->_p->plcData.CParALog,   this->fatorConv.CParALog);
    this->_p->plcData.CParAPer   = convPer(this->_p->plcData.CParAPer,   this->fatorConv.CParAPer);
    this->_p->plcData.CLogARea   = convRea(this->_p->plcData.CLogARea,   this->fatorConv.CLogARea);
    this->_p->plcData.CLin       = convLin(this->_p->plcData.CLin,       this->fatorConv.CLin);
    this->_p->plcData.CPer       = convLin(this->_p->plcData.CPer,       this->fatorConv.CPer);
    this->_p->plcData.SRadAre    = convLog(this->_p->plcData.SRadAre,    this->fatorConv.SRadAer);
    this->_p->plcData.SRadEntPri = convLog(this->_p->plcData.SRadEntPri, this->fatorConv.SRadEntPri);
    this->_p->plcData.SRadPoc    = convLog(this->_p->plcData.SRadPoc,    this->fatorConv.SRadPoc);
    this->_p->plcData.SRadRes    = convLog(this->_p->plcData.SRadRes,    this->fatorConv.SRadRes);
    this->_p->plcData.SRadSaiSec = convLog(this->_p->plcData.SRadSaiSec, this->fatorConv.SRadSaiSec);
    this->_p->plcData.SRadAer    = convLog(this->_p->plcData.SRadAer,    this->fatorConv.SRadAer);
    this->_p->plcData.SVasPri    = convLin(this->_p->plcData.SVasPri,    this->fatorConv.SVasPri);//Converter bits para m^3/h
    return this->_p->plcData;
}

PLC_DATA libOpcTrigaPLC::get_all()
{
    try
    {
        this->_p->plcData.BarraReg        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inBarraReg (%IW5)"})   .readValueScalar<int16_t>();
        this->_p->plcData.BarraCon        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inBarraCon (%IW6)"})   .readValueScalar<int16_t>();
        this->_p->plcData.BarraSeg        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inBarraSeg (%IW7)"})   .readValueScalar<int16_t>();
        this->_p->plcData.CLogALog        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCLogALog (%IW17)"})  .readValueScalar<int16_t>();
        this->_p->plcData.CLogALin        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCLogALin (%IW18)"})  .readValueScalar<int16_t>();
        this->_p->plcData.CLogAPer        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCLogAPer (%IW19)"})  .readValueScalar<int16_t>();
        this->_p->plcData.CParALin        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCParALin (%IW14)"})  .readValueScalar<int16_t>();
        this->_p->plcData.CParALog        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCParALog (%IW15)"})  .readValueScalar<int16_t>();
        this->_p->plcData.CParAPer        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCParAPer (%IW16)"})  .readValueScalar<int16_t>();
        this->_p->plcData.CLogARea        = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCLogARea (%IW20)"})  .readValueScalar<int16_t>();
        this->_p->plcData.CLin            = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCLin (%IW25)"})      .readValueScalar<int16_t>();
        this->_p->plcData.CLinScale       = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inModule_D0 (%IW0)"})  .readValueScalar<u_int16_t>() & 0b00000111 - 2;
        this->_p->plcData.CPer            = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inCPer (%IW26)"})      .readValueScalar<int16_t>();
        this->_p->plcData.SRadAre         = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inSRadAre (%IW27)"})   .readValueScalar<int16_t>();
        this->_p->plcData.SRadEntPri      = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inSRadEntPri (%IW28)"}).readValueScalar<int16_t>();
        this->_p->plcData.SRadPoc         = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inSRadPoc (%IW29)"})   .readValueScalar<int16_t>();
        this->_p->plcData.SRadRes         = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inSRadRes (%IW30)"})   .readValueScalar<int16_t>();
        this->_p->plcData.SRadSaiSec      = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inSRadSaiSec (%IW31)"}).readValueScalar<int16_t>();
        this->_p->plcData.SRadAer         = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inSRadAer (%IW32)"})   .readValueScalar<int16_t>();
        this->_p->plcData.SVasPri         = this->_p->client.getNode({2, "IoConfig_Globals_Mapping.inSVasPri (%IW49)"})   .readValueScalar<int16_t>();
        this->_p->plcData.SPt100Poco      = this->_p->client.getNode({2, "GVL.SPt100Poco (%MD1)"})                        .readValueScalar<float>();
        this->_p->plcData.SPt100EntPri    = this->_p->client.getNode({2, "GVL.SPt100EntPri (%MD2)"})                      .readValueScalar<float>();
        this->_p->plcData.SPt100SaiPri    = this->_p->client.getNode({2, "GVL.SPt100SaiPri (%MD3)"})                      .readValueScalar<float>();
        this->_p->plcData.SPt100EntSec    = this->_p->client.getNode({2, "GVL.SPt100EntSec (%MD4)"})                      .readValueScalar<float>();
        this->_p->plcData.SPt100SaiSec    = this->_p->client.getNode({2, "GVL.SPt100SaiSec (%MD5)"})                      .readValueScalar<float>();
        this->_p->plcData.STpPoc1         = this->_p->client.getNode({2, "GVL.STpPoc1 (%MD6)"})                           .readValueScalar<float>();
        this->_p->plcData.STpPoc2         = this->_p->client.getNode({2, "GVL.STpPoc2 (%MD7)"})                           .readValueScalar<float>();
        this->_p->plcData.STpLen          = this->_p->client.getNode({2, "GVL.STpLen (%MD8)"})                            .readValueScalar<float>();
        this->_p->plcData.SConPoc         = this->_p->client.getNode({2, "GVL.SConPoc (%MD9)"})                           .readValueScalar<float>();
        this->_p->plcData.SConSaiPri      = this->_p->client.getNode({2, "GVL.SConSaiPri (%MD10)"})                       .readValueScalar<float>();
        this->_p->plcData.STATE = 0;
    }
    catch (const std::exception& e)
    {
        if (this->_p->client.isConnected())
        {
            std::cerr << stdErrorMsg("get_all()", "Cliente conectado, porém erro ao adquirir dados", e.what());
            this->_p->plcData.STATE = 1;
        }
        else
        {
            std::cerr << stdErrorMsg("get_all()", "Cliente desconectado", e.what());
            this->_p->plcData.STATE = 2;
        }
    }

    this->_p->plcData.TIME = std::chrono::system_clock::now();
    return this->_p->plcData;
}

CONV_PLC libOpcTrigaPLC::readFatorConvFile(std::string filename)
{
    CONV_PLC fatorConv;
    //Temporariamente para testes
    fatorConv.BarraReg.x0 = 262;
    fatorConv.BarraReg.x1 = 1580;
    fatorConv.BarraReg.y0 = 151;
    fatorConv.BarraReg.y1 = 902;

    fatorConv.BarraCon.x0 = 312;
    fatorConv.BarraCon.x1 = 1739;
    fatorConv.BarraCon.y0 = 162;
    fatorConv.BarraCon.y1 = 900;

    fatorConv.BarraSeg.x0 = 301;
    fatorConv.BarraSeg.x1 = 1593;
    fatorConv.BarraSeg.y0 = 172;
    fatorConv.BarraSeg.y1 = 900;

    fatorConv.CLogALin.x0 = 820;
    fatorConv.CLogALin.x1 = 4098;
    fatorConv.CLogALin.y0 = 10000;
    fatorConv.CLogALin.y1 = 100000;

    fatorConv.CLogALog.A = 0.0025;
    fatorConv.CLogALog.B = 0.000990098877;

    fatorConv.CLin.x0 = 14;
    fatorConv.CLin.x1 = 8145;
    fatorConv.CLin.y0 = 0;
    fatorConv.CLin.y1 = 2.375;
    return fatorConv;
}
