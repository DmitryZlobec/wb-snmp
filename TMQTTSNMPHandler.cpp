//
// Created by zlobec on 26.07.16.
//

#include "TMQTTSNMPHandler.h"

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */

// https://en.wikipedia.org/wiki/B%C3%A9zout's_identity
unsigned hashData(const char* s)
{
    unsigned h = 31 /* also prime */;
    while (*s) {
        h = (h * A) ^ (s[0] * B);
        s++;
    }
    return h; // or return h % C;
}




TMQTTSNMPHandler::TMQTTSNMPHandler(const TMQTTSNMPHandler::TConfig &config,
                                   netSnmpMQTT::netSnmpMQTTTable *netSnmpMQTTTable) : TMQTTWrapper(config),netSnmpTable (netSnmpMQTTTable)
{


    Connect();

}




void TMQTTSNMPHandler::OnConnect(int rc) {


    Subscribe(NULL, "/devices/#");
    DEBUGMSGTL(("verbose", "Connected with code: %d\n", rc));

  //  if(rc == 0) {
  //      string prefix = string("/devices/") + MQTTConfig.Id + "/";
        // Meta
  //      Publish(NULL, prefix + "meta/name", "SNMP", 0, false);
  //  }

}

void TMQTTSNMPHandler::OnSubscribe(int mid, int qos_count, const int *granted_qos) {

    DEBUGMSGTL(("verbose", "Subscription succeeded.\n"));


}

void TMQTTSNMPHandler::OnMessage(const struct mosquitto_message *message) {

    string topic = message->topic;
    string payload = std::string(message->payload ? message->payload : "");


    const vector<string>& tokens = StringSplit(topic, '/');

    if (tokens.size() < 5) {

        DEBUGMSGTL(("snmp-mqtt-wb", "not enough topic sections to care: %s is %zd\n", topic.c_str(), tokens.size()));
        return;

    }

    if ( tokens[1] != "devices" || tokens[0] != "")
    {

        DEBUGMSGTL(("snmp-mqtt-wb", "no devices topic: %s \n", topic.c_str()));
        return;

    }

    string rowName = tokens[2] + "/" + tokens[4];
    //TODO Make good  cast
    unsigned long int sec = (unsigned long int)time(NULL);
    if ((tokens.size() > 5) && (tokens[5] == "meta")) {
        if (tokens[6] == "type") {
            netSnmpTable->addUpdateType(hashData(rowName.c_str()), payload.c_str(), rowName.c_str());
            return;
        } else  if (tokens[6] == "order") {
            netSnmpTable->addupdateOrder(hashData(rowName.c_str()), payload.c_str());
            return;
        } else if (tokens[6] == "readonly") {
            netSnmpTable->addUpdateReadonly(hashData(rowName.c_str()), payload.c_str());
            return;
        } else if (tokens[6] == "ts") {
            netSnmpTable->addUpdateTs(hashData(rowName.c_str()), payload.c_str());
            return;
        }
    }
    if (tokens[3] == "controls" && tokens.size() == 5) {
        std::stringstream ss;
        ss << sec;
        std::string ts = ss.str();
        DEBUGMSGTL(("snmp-mqtt-wb", "Create/Update row: %s with value1: %s, timestamp: %lu\n",rowName.c_str(), payload.c_str(), sec));
        netSnmpTable->addUpdate(hashData(rowName.c_str()),rowName.c_str(), payload.c_str(), ts.c_str());
        return;
    }
}

TMQTTSNMPHandler::~TMQTTSNMPHandler() {

}







