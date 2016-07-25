//
// Created by zlobec on 24.07.16.
//

#ifndef WB_SNMP_NETSNMPMQTTTABLE_ENTRY_H
#define WB_SNMP_NETSNMPMQTTTABLE_ENTRY_H
#define NNN 255
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
namespace netSnmpMQTT
{
    struct netSnmpMQTTTable_entry {
         /* Index values */
            u_long nsMQTTIndex;

        /* Column values */
            char nsMQTTName[NNN];
            size_t nsMQTTName_len;
            char old_nsMQTTName[NNN];
            size_t old_nsMQTTName_len;
            char nsMQTTValue[NNN];
            size_t nsMQTTValue_len;
            char old_nsMQTTValue[NNN];
            size_t old_nsMQTTValue_len;
            long nsMQTTDValue;
            long old_nsMQTTDValue;
            long nsMQTTDPow;
            long old_nsMQTTDPow;
            char nsMQTTType[NNN];
            size_t nsMQTTType_len;
            char old_nsMQTTType[NNN];
            size_t old_nsMQTTType_len;
            long nsMQTTReadonly;
            long old_nsMQTTReadonly;
            long nsMQTTOrder;
            long old_nsMQTTOrder;
            u_long nsMQTTTs;
            u_long old_nsMQTTTs;
            u_long nsMQTTTOC;
            u_long old_nsMQTTTOC;

            int   valid;
    };
}

#endif //WB_SNMP_NETSNMPMQTTTABLE_ENTRY_H
