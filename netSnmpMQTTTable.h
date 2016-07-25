//
// Created by zlobec on 24.07.16.
//

#ifndef WB_SNMP_NETSNMPMQTTTABLE_H
#define WB_SNMP_NETSNMPMQTTTABLE_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <math.h>
// column number definitions for table netSnmpMQTTTable
#define COLUMN_NSMQTTINDEX        1
#define COLUMN_NSMQTTNAME        2
#define COLUMN_NSMQTTVALUE        3
#define COLUMN_NSMQTTDVALUE        4
#define COLUMN_NSMQTTDPOW        5
#define COLUMN_NSMQTTTYPE        6
#define COLUMN_NSMQTTREADONLY    7
#define COLUMN_NSMQTTORDER        8
#define COLUMN_NSMQTTTS            9
#define COLUMN_NSMQTTTOC        10


namespace netSnmpMQTT {


    class netSnmpMQTTTable {
        const oid netSnmpMQTTTable_oid[10] = {1, 3, 6, 1, 4, 1, 8072, 9997, 1, 1};
        const size_t netSnmpMQTTTable_oid_len = OID_LENGTH(netSnmpMQTTTable_oid);
        netsnmp_handler_registration *reg;
        netsnmp_tdata *table_data;
        netsnmp_table_registration_info *table_info;

        netsnmp_tdata_row *_find_row(u_long);

        netsnmp_tdata_row *createEntry(long);

        void removeEntry(netsnmp_tdata_row *row);


    public:

         int  netSnmpMQTTTable_handler(
                netsnmp_mib_handler *handler,
                netsnmp_handler_registration *reginfo,
                netsnmp_agent_request_info *reqinfo,
                netsnmp_request_info *requests);

        void remove(long idx);

        void add(netsnmp_tdata_row *row, u_long idx, const char *nsMQTTName = "Noname", const char *nsMQTTValue = "0",
                 long nsMQTTDValue = 0, long nsMQTTDPow = 0, const char *nsMQTTType = "range", long nsMQTTReadonly = 1,
                 long nsMQTTOrder = 0, u_long nsMQTTTs = 0, u_long nsMQTTTOC = 0);

        void addUpdate(u_long idx, const char *nsMQTTName, const char *nsMQTTValue, const char *nsMQTTTOC);

        void addUpdateType(u_long idx, const char *nsMQTTType, const char *nsMQTTName);

        netSnmpMQTTTable();

        ~netSnmpMQTTTable();

        void addupdateOrder(u_long idx, const char *nsMQTTOrder);

        void addUpdateReadonly(u_long idx, const char *nsMQTTReadonly);

        void addUpdateTs(u_long idx, const char *nsMQTTTs);

        void remove(u_long idx);
    };


}

#endif //WB_SNMP_NETSNMPMQTTTABLE_H
