//
// Created by zlobec on 24.07.16.
//

#include "netSnmpMQTTTable.h"
#include "netSnmpMQTTTable_entry.h"

extern Netsnmp_Node_Handler handler;
using namespace netSnmpMQTT;
netSnmpMQTTTable *tbl;
netSnmpMQTTTable::netSnmpMQTTTable() {

    tbl = this;

     reg = netsnmp_create_handler_registration(
            "netSnmpMQTTTable",   handler,
            netSnmpMQTTTable_oid, netSnmpMQTTTable_oid_len,
            HANDLER_CAN_RWRITE);

    table_data = netsnmp_tdata_create_table( "netSnmpMQTTTable", 0 );
    if (NULL == table_data) {
        snmp_log(LOG_ERR,"error creating tdata table for netSnmpMQTTTable\n");
        return;
    }
    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    if (NULL == table_info) {
        snmp_log(LOG_ERR,"error creating table info for netSnmpMQTTTable\n");
        return;
    }
    netsnmp_table_helper_add_indexes(table_info,
                                     ASN_UNSIGNED,  /* index: nsMQTTIndex */
                                     0);

    table_info->min_column = COLUMN_NSMQTTNAME;
    table_info->max_column = COLUMN_NSMQTTTOC;

    netsnmp_tdata_register( reg, table_data, table_info );

}



netSnmpMQTTTable::~netSnmpMQTTTable() {

}

netsnmp_tdata_row *netSnmpMQTTTable::_find_row(u_long idx) {
    netsnmp_variable_list name_var;
    netsnmp_tdata_row *row;
    memset(&name_var, 0, sizeof (netsnmp_variable_list));
    snmp_set_var_typed_integer(&name_var, ASN_UNSIGNED, idx);
    return netsnmp_tdata_row_get_byidx(table_data, &name_var);
}

void netSnmpMQTTTable::remove( u_long idx) {
    netsnmp_tdata_row *row = _find_row( idx);
    if (row) {
        DEBUGMSGTL(("verbose", "Found row with idx, deleting it: %lu\n", idx));
        removeEntry( row);
    } else {
        DEBUGMSGTL(("verbose", "No row found with idx: %lu\n", idx));
    }
}



netsnmp_tdata_row *netSnmpMQTTTable::createEntry(long nsMQTTIndex) {
    struct netSnmpMQTTTable_entry *entry;
    netsnmp_tdata_row *row;

    entry = SNMP_MALLOC_TYPEDEF(struct netSnmpMQTTTable_entry);
    if (!entry)
        return NULL;

    row = netsnmp_tdata_create_row();
    if (!row) {
        SNMP_FREE(entry);
        return NULL;
    }
    row->data = entry;

    DEBUGMSGT(("netSnmpMQTTTable:entry:create", "row 0x%lx\n", (uintptr_t)row));
    entry->nsMQTTIndex = nsMQTTIndex;
    netsnmp_tdata_row_add_index( row, ASN_UNSIGNED,
                                 &(entry->nsMQTTIndex),
                                 sizeof(entry->nsMQTTIndex));
    if (table_data)
        netsnmp_tdata_add_row( table_data, row );
    return row;

}

void netSnmpMQTTTable::removeEntry(netsnmp_tdata_row *row) {
    struct netSnmpMQTTTable_entry *entry;

    if (!row)
        return;    /* Nothing to remove */

    DEBUGMSGT(("netSnmpMQTTTable:entry:remove", "row 0x%lx\n", (uintptr_t)row));

    entry = (struct netSnmpMQTTTable_entry *)row->data;
    SNMP_FREE( entry );   /* XXX - release any other internal resources */

    if (table_data)
        netsnmp_tdata_remove_and_delete_row( table_data, row );
    else
        netsnmp_tdata_delete_row( row );
}

void netSnmpMQTTTable::add(netsnmp_tdata_row *row, u_long idx, const char *nsMQTTName, const char *nsMQTTValue,long  nsMQTTDValue, long nsMQTTDPow, const char *nsMQTTType, long nsMQTTReadonly, long nsMQTTOrder, u_long nsMQTTTs, u_long nsMQTTTOC) {

    struct netSnmpMQTTTable_entry *entry = (struct netSnmpMQTTTable_entry *) row->data;

    if (nsMQTTName) {
        strcpy(entry->nsMQTTName, nsMQTTName);
        entry->nsMQTTName_len = strlen(nsMQTTName);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTName_len = 0;
    }

    if (nsMQTTValue) {
        strcpy(entry->nsMQTTValue, nsMQTTValue);
        entry->nsMQTTValue_len = strlen(nsMQTTValue);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTValue_len = 0;
    }

    if (nsMQTTType) {
        strcpy(entry->nsMQTTType, nsMQTTType);
        entry->nsMQTTType_len = strlen(nsMQTTType);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTType_len = 0;
    }

    entry->nsMQTTDValue = nsMQTTDValue;
    entry->nsMQTTDPow = nsMQTTDValue;
    entry->nsMQTTReadonly = nsMQTTReadonly;
    entry->nsMQTTOrder = nsMQTTOrder;
    entry->nsMQTTTs = nsMQTTTs;
    entry->nsMQTTTOC = nsMQTTTOC;

}



void netSnmpMQTTTable::addUpdate(u_long idx, const char *nsMQTTName, const char *nsMQTTValue, const char *nsMQTTTOC) {

    netsnmp_tdata_row *row = _find_row(idx);
    if (!row) {
        /* Create if it's new, */
        row = createEntry(idx);
        add(row, idx);

    }
    /* then just do the update */
    struct netSnmpMQTTTable_entry *entry = static_cast<struct netSnmpMQTTTable_entry *>(row->data);

    if (nsMQTTName) {
        strcpy(entry->nsMQTTName, nsMQTTName);
        entry->nsMQTTName_len = strlen(nsMQTTName);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTName_len = 0;
    }



    if (nsMQTTValue) {

        strcpy(entry->nsMQTTValue, nsMQTTValue);
        entry->nsMQTTValue_len = strlen(nsMQTTValue);

        double f_value = atof(nsMQTTValue);
        double a,b;
        int i_pow;
        i_pow = 0;

        b = modf(f_value,&a);

        if( b * 100.0 > 0 ) {

            entry->nsMQTTDValue = (long) (f_value *100.0);
            entry->nsMQTTDPow = (long) 100;

        }
        else {
            entry->nsMQTTDValue = (long) f_value;
            entry->nsMQTTDPow = (long) 1;
        }

    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTValue_len = 0;
        entry->nsMQTTDValue = 0;
        entry->nsMQTTDPow = 0;

    }

    if (nsMQTTTOC) {
        entry->nsMQTTTOC = (u_long) atol(nsMQTTTOC);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTTOC = (u_long) 0;
    }

}




void netSnmpMQTTTable::addUpdateType(u_long idx,  const char *nsMQTTType, const char *nsMQTTName) {

    netsnmp_tdata_row *row = _find_row(idx);
    if (!row) {
        /* Create if it's new, */
        row = createEntry(idx);
        add(row, idx);

    }
    /* then just do the update */
    struct netSnmpMQTTTable_entry *entry = static_cast<struct netSnmpMQTTTable_entry *>(row->data);


    if (nsMQTTType) {

        strcpy(entry->nsMQTTType, nsMQTTType);
        entry->nsMQTTType_len = strlen(nsMQTTType);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTType_len = 0;
    }

    if (nsMQTTName) {
        strcpy(entry->nsMQTTName, nsMQTTName);
        entry->nsMQTTName_len = strlen(nsMQTTName);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTName_len = 0;
    }

}


void netSnmpMQTTTable::addupdateOrder(u_long idx,  const char *nsMQTTOrder) {

    netsnmp_tdata_row *row = _find_row(idx);
    if (!row) {
        /* Create if it's new, */
        row = createEntry(idx);
        add(row, idx);

    }
    /* then just do the update */
    struct netSnmpMQTTTable_entry *entry;
    entry = static_cast<struct netSnmpMQTTTable_entry *>(row->data);

    if (nsMQTTOrder) {
        entry->nsMQTTOrder = atol(nsMQTTOrder);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTOrder = 0;
    }

}

void netSnmpMQTTTable::addUpdateReadonly(u_long idx,  const char *nsMQTTReadonly) {

    netsnmp_tdata_row *row = _find_row(idx);
    if (!row) {
        /* Create if it's new, */
        row = createEntry(idx);
        add(row, idx);
    }
    /* then just do the update */
    struct netSnmpMQTTTable_entry *entry;
    entry = static_cast<struct netSnmpMQTTTable_entry *>(row->data);

    if (nsMQTTReadonly) {
        entry->nsMQTTReadonly = atol(nsMQTTReadonly);
    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTReadonly = 0;
    }

}

void netSnmpMQTTTable::addUpdateTs(u_long idx,  const char *nsMQTTTs) {

    netsnmp_tdata_row *row = _find_row(idx);
    if (!row) {
        /* Create if it's new, */
        row = createEntry(idx);
        add(row,idx);

    }
    /* then just do the update */
    struct netSnmpMQTTTable_entry *entry = static_cast<struct netSnmpMQTTTable_entry *>(row->data);

    if (nsMQTTTs) {
        entry->nsMQTTTs= (u_long)atol(nsMQTTTs);

    } else {
        /* don't need to actually zero out the data...*/
        entry->nsMQTTTs = (u_long) 0;
    }


}
