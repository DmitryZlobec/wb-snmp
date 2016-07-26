//
// Created by zlobec on 26.07.16.
//

#include "TMQTTSNMPHandler.h"
static u_long hashData(const char *str)
{
    u_long hash = 5381;
    int c;
    while (c=*str++)
        hash =((hash << 5) + hash) + c;

    return hash;

}

TMQTTSNMPHandler::TMQTTSNMPHandler(const TMQTTSNMPHandler::TConfig &config,
                                   netSnmpMQTT::netSnmpMQTTTable *netSnmpMQTTTable) : TMQTTWrapper(config),netSnmpTable (netSnmpMQTTTable)
{

    Connect();

}




void TMQTTSNMPHandler::OnConnect(int rc) {

    DEBUGMSGTL(("verbose", "Connected with code: %d\n", rc));

    if(rc == 0) {
        string prefix = string("/devices/") + MQTTConfig.Id + "/";
        // Meta
        Publish(NULL, prefix + "meta/name", "SNMP", 0, false);
    }

}

void TMQTTSNMPHandler::OnSubscribe(int mid, int qos_count, const int *granted_qos) {

    DEBUGMSGTL(("verbose", "Subscription succeeded.\n"));

}

void TMQTTSNMPHandler::OnMessage(const struct mosquitto_message *message) {

    string topic = message->topic;
    string payload = static_cast<const char *>(message->payload);


    const vector<string>& tokens = StringSplit(topic, '/');
    if (tokens.size() < 5) {

        DEBUGMSGTL(("snmp-mqtt-wb", "not enough topic sections to care: %s is %d\n", topic, tokens.size()));
        return;

    }

    if ( tokens[1] != "devices" || tokens[0] != "")
    {

        DEBUGMSGTL(("snmp-mqtt-wb", "no devices topic: %s \n", topic));
        return;

    }

    string rowName = tokens[2] + "/" + tokens[4];
    //TODO Make good  cast
    unsigned long int sec = (unsigned long int)time(NULL);

    if (tokens[5] == "meta" && tokens[6] == "type") {

        netSnmpTable->addUpdateType(hashData(rowName.c_str()), rowName.c_str(), payload.c_str());
        return;
    }

    if (tokens[5] == "meta" && tokens[6] == "order") {

        netSnmpTable->addupdateOrder(hashData(rowName.c_str()), payload.c_str());
        return;
    }

    if (tokens[5] == "meta" && tokens[6] == "readonly") {

        netSnmpTable->addUpdateReadonly(hashData(rowName.c_str()), payload.c_str());
        return;
    }

    if (tokens[5] == "meta" && tokens[6] == "ts") {

        netSnmpTable->addUpdateTs(hashData(rowName.c_str()), payload.c_str());
        return;
    }
    if (tokens[3] == "controls" && tokens.size() == 5) {
        std::stringstream ss;
        ss << sec;
        std::string ts = ss.str();
        DEBUGMSGTL(("snmp-mqtt-wb", "Create/Update row: %s with value1: %s, timestamp: %d\n",rowName, payload, sec));
        netSnmpTable->addUpdate(hashData(rowName.c_str()),rowName.c_str(), payload.c_str(), ts.c_str());
        return;
    }
}

TMQTTSNMPHandler::~TMQTTSNMPHandler() {

}







