//
// Created by zlobec on 26.07.16.
//

#ifndef WB_SNMP_TMQTTSNMPHANDLER_H
#define WB_SNMP_TMQTTSNMPHANDLER_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>


#include <mosquittopp.h>
#include <wbmqtt/utils.h>
#include <wbmqtt/mqtt_wrapper.h>
#include<chrono>
#include<thread>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "netSnmpMQTTTable.h"

class TMQTTSNMPHandler  : public TMQTTWrapper {
public:
    TMQTTSNMPHandler(const TMQTTSNMPHandler::TConfig &config, netSnmpMQTT::netSnmpMQTTTable *netSnmpMQTTTable);
    ~TMQTTSNMPHandler();
    void OnConnect(int rc);
    void OnMessage(const struct mosquitto_message *message);
    void OnSubscribe(int mid, int qos_count, const int *granted_qos);
private:
    netSnmpMQTT::netSnmpMQTTTable *netSnmpTable;
};


#endif //WB_SNMP_TMQTTSNMPHANDLER_H
