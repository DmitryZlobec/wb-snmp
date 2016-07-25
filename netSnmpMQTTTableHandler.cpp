//
// Created by zlobec on 24.07.16.
//

#include "netSnmpMQTTTable.h"
#include "netSnmpMQTTTable_entry.h"



using namespace netSnmpMQTT;

extern netSnmpMQTTTable *tbl;

int   handler(
        netsnmp_mib_handler               *handler,
        netsnmp_handler_registration      *reginfo,
        netsnmp_agent_request_info        *reqinfo,
        netsnmp_request_info              *requests)


{
   return  tbl->netSnmpMQTTTable_handler(handler, reginfo, reqinfo, requests);

}

/** handles requests for the netSnmpMQTTTable table */
int  netSnmpMQTTTable::netSnmpMQTTTable_handler(
        netsnmp_mib_handler               *handler,
        netsnmp_handler_registration      *reginfo,
        netsnmp_agent_request_info        *reqinfo,
        netsnmp_request_info              *requests) {

    netsnmp_request_info       *request;
    netsnmp_table_request_info *table_info;
    netsnmp_tdata              *table_data;
    netsnmp_tdata_row          *table_row;
    struct netSnmpMQTTTable_entry          *table_entry;
    int                         ret;

    DEBUGMSGTL(("netSnmpMQTTTable:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
        case MODE_GET:
            for (request=requests; request; request=request->next) {
                if (request->processed)
                    continue;

                table_entry = (struct netSnmpMQTTTable_entry *)
                        netsnmp_tdata_extract_entry(request);
                table_info  =     netsnmp_extract_table_info( request);

                switch (table_info->colnum) {
                    case COLUMN_NSMQTTNAME:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                                  table_entry->nsMQTTName,
                                                  table_entry->nsMQTTName_len);
                        break;
                    case COLUMN_NSMQTTVALUE:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                                  table_entry->nsMQTTValue,
                                                  table_entry->nsMQTTValue_len);
                        break;
                    case COLUMN_NSMQTTDVALUE:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                                    table_entry->nsMQTTDValue);
                        break;
                    case COLUMN_NSMQTTDPOW:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                                    table_entry->nsMQTTDPow);
                        break;
                    case COLUMN_NSMQTTTYPE:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                                  table_entry->nsMQTTType,
                                                  table_entry->nsMQTTType_len);
                        break;
                    case COLUMN_NSMQTTREADONLY:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                                    table_entry->nsMQTTReadonly);
                        break;
                    case COLUMN_NSMQTTORDER:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                                    table_entry->nsMQTTOrder);
                        break;
                    case COLUMN_NSMQTTTS:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                                    table_entry->nsMQTTTs);
                        break;
                    case COLUMN_NSMQTTTOC:
                        if ( !table_entry ) {
                            netsnmp_set_request_error(reqinfo, request,
                                                      SNMP_NOSUCHINSTANCE);
                            continue;
                        }
                        snmp_set_var_typed_integer( request->requestvb, ASN_COUNTER,
                                                    table_entry->nsMQTTTOC);
                        break;
                    default:
                        netsnmp_set_request_error(reqinfo, request,
                                                  SNMP_NOSUCHOBJECT);
                        break;
                }
            }
            break;

            /*
             * Write-support
             */
        case MODE_SET_RESERVE1:
            for (request=requests; request; request=request->next) {
                if (request->processed)
                    continue;

                table_entry = (struct netSnmpMQTTTable_entry *)
                        netsnmp_tdata_extract_entry(request);
                table_info  =     netsnmp_extract_table_info( request);

                switch (table_info->colnum) {
                    case COLUMN_NSMQTTNAME:
                        /* or possibly 'netsnmp_check_vb_type_and_size' */
                        ret = netsnmp_check_vb_type_and_max_size(
                                request->requestvb, ASN_OCTET_STR, sizeof(table_entry->nsMQTTName));
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTVALUE:
                        /* or possibly 'netsnmp_check_vb_type_and_size' */
                        ret = netsnmp_check_vb_type_and_max_size(
                                request->requestvb, ASN_OCTET_STR, sizeof(table_entry->nsMQTTValue));
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTDVALUE:
                        /* or possibly 'netsnmp_check_vb_int_range' */
                        ret = netsnmp_check_vb_int( request->requestvb );
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTDPOW:
                        /* or possibly 'netsnmp_check_vb_int_range' */
                        ret = netsnmp_check_vb_int( request->requestvb );
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTTYPE:
                        /* or possibly 'netsnmp_check_vb_type_and_size' */
                        ret = netsnmp_check_vb_type_and_max_size(
                                request->requestvb, ASN_OCTET_STR, sizeof(table_entry->nsMQTTType));
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTREADONLY:
                        /* or possibly 'netsnmp_check_vb_int_range' */
                        ret = netsnmp_check_vb_int( request->requestvb );
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTORDER:
                        /* or possibly 'netsnmp_check_vb_int_range' */
                        ret = netsnmp_check_vb_int( request->requestvb );
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTTS:
                        /* or possibly 'netsnmp_check_vb_int_range' */
                        ret = netsnmp_check_vb_int( request->requestvb );
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    case COLUMN_NSMQTTTOC:
                        /* or possibly 'netsnmp_check_vb_int_range' */
                        ret = netsnmp_check_vb_int( request->requestvb );
                        if ( ret != SNMP_ERR_NOERROR ) {
                            netsnmp_set_request_error( reqinfo, request, ret );
                            return SNMP_ERR_NOERROR;
                        }
                        break;
                    default:
                        netsnmp_set_request_error( reqinfo, request,
                                                   SNMP_ERR_NOTWRITABLE );
                        return SNMP_ERR_NOERROR;
                }
            }
            break;

        case MODE_SET_RESERVE2:
            for (request=requests; request; request=request->next) {
                if (request->processed)
                    continue;

                table_row  = netsnmp_tdata_extract_row(  request);
                table_data = netsnmp_tdata_extract_table(request);
                table_info = netsnmp_extract_table_info( request);

                switch (table_info->colnum) {
                    case COLUMN_NSMQTTNAME:
                    case COLUMN_NSMQTTVALUE:
                    case COLUMN_NSMQTTDVALUE:
                    case COLUMN_NSMQTTDPOW:
                    case COLUMN_NSMQTTTYPE:
                    case COLUMN_NSMQTTREADONLY:
                    case COLUMN_NSMQTTORDER:
                    case COLUMN_NSMQTTTS:
                    case COLUMN_NSMQTTTOC:
                        if ( !table_row ) {
                            table_row = createEntry(*table_info->indexes->val.integer);
                            if (table_row) {
                                netsnmp_insert_tdata_row( request, table_row );
                            } else {
                                netsnmp_set_request_error( reqinfo, request,
                                                           SNMP_ERR_RESOURCEUNAVAILABLE );
                                return SNMP_ERR_NOERROR;
                            }
                        }
                        break;
                }
            }
            break;

        case MODE_SET_FREE:
            for (request=requests; request; request=request->next) {
                if (request->processed)
                    continue;

                table_entry = (struct netSnmpMQTTTable_entry *)
                        netsnmp_tdata_extract_entry(request);
                table_row   =     netsnmp_tdata_extract_row(  request);
                table_data  =     netsnmp_tdata_extract_table(request);
                table_info  =     netsnmp_extract_table_info( request);

                switch (table_info->colnum) {
                    case COLUMN_NSMQTTNAME:
                    case COLUMN_NSMQTTVALUE:
                    case COLUMN_NSMQTTDVALUE:
                    case COLUMN_NSMQTTDPOW:
                    case COLUMN_NSMQTTTYPE:
                    case COLUMN_NSMQTTREADONLY:
                    case COLUMN_NSMQTTORDER:
                    case COLUMN_NSMQTTTS:
                    case COLUMN_NSMQTTTOC:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        }
                        break;
                }
            }
            break;

        case MODE_SET_ACTION:
            for (request=requests; request; request=request->next) {
                if (request->processed)
                    continue;

                table_entry = (struct netSnmpMQTTTable_entry *)
                        netsnmp_tdata_extract_entry(request);
                table_info  =     netsnmp_extract_table_info( request);

                switch (table_info->colnum) {
                    case COLUMN_NSMQTTNAME:
                        memcpy( table_entry->old_nsMQTTName,
                                table_entry->nsMQTTName,
                                sizeof(table_entry->nsMQTTName));
                        table_entry->old_nsMQTTName_len =
                                table_entry->nsMQTTName_len;
                        memset( table_entry->nsMQTTName, 0,
                                sizeof(table_entry->nsMQTTName));
                        memcpy( table_entry->nsMQTTName,
                                request->requestvb->val.string,
                                request->requestvb->val_len);
                        table_entry->nsMQTTName_len =
                                request->requestvb->val_len;
                        break;
                    case COLUMN_NSMQTTVALUE:
                        memcpy( table_entry->old_nsMQTTValue,
                                table_entry->nsMQTTValue,
                                sizeof(table_entry->nsMQTTValue));
                        table_entry->old_nsMQTTValue_len =
                                table_entry->nsMQTTValue_len;
                        memset( table_entry->nsMQTTValue, 0,
                                sizeof(table_entry->nsMQTTValue));
                        memcpy( table_entry->nsMQTTValue,
                                request->requestvb->val.string,
                                request->requestvb->val_len);
                        table_entry->nsMQTTValue_len =
                                request->requestvb->val_len;
                        break;
                    case COLUMN_NSMQTTDVALUE:
                        table_entry->old_nsMQTTDValue = table_entry->nsMQTTDValue;
                        table_entry->nsMQTTDValue     = *request->requestvb->val.integer;
                        break;
                    case COLUMN_NSMQTTDPOW:
                        table_entry->old_nsMQTTDPow = table_entry->nsMQTTDPow;
                        table_entry->nsMQTTDPow     = *request->requestvb->val.integer;
                        break;
                    case COLUMN_NSMQTTTYPE:
                        memcpy( table_entry->old_nsMQTTType,
                                table_entry->nsMQTTType,
                                sizeof(table_entry->nsMQTTType));
                        table_entry->old_nsMQTTType_len =
                                table_entry->nsMQTTType_len;
                        memset( table_entry->nsMQTTType, 0,
                                sizeof(table_entry->nsMQTTType));
                        memcpy( table_entry->nsMQTTType,
                                request->requestvb->val.string,
                                request->requestvb->val_len);
                        table_entry->nsMQTTType_len =
                                request->requestvb->val_len;
                        break;
                    case COLUMN_NSMQTTREADONLY:
                        table_entry->old_nsMQTTReadonly = table_entry->nsMQTTReadonly;
                        table_entry->nsMQTTReadonly     = *request->requestvb->val.integer;
                        break;
                    case COLUMN_NSMQTTORDER:
                        table_entry->old_nsMQTTOrder = table_entry->nsMQTTOrder;
                        table_entry->nsMQTTOrder     = *request->requestvb->val.integer;
                        break;
                    case COLUMN_NSMQTTTS:
                        table_entry->old_nsMQTTTs = table_entry->nsMQTTTs;
                        table_entry->nsMQTTTs     = *request->requestvb->val.integer;
                        break;
                    case COLUMN_NSMQTTTOC:
                        table_entry->old_nsMQTTTOC = table_entry->nsMQTTTOC;
                        table_entry->nsMQTTTOC     = *request->requestvb->val.integer;
                        break;
                }
            }
            break;

        case MODE_SET_UNDO:
            for (request=requests; request; request=request->next) {
                if (request->processed)
                    continue;

                table_entry = (struct netSnmpMQTTTable_entry *)
                        netsnmp_tdata_extract_entry(request);
                table_row   =     netsnmp_tdata_extract_row(  request);
                table_data  =     netsnmp_tdata_extract_table(request);
                table_info  =     netsnmp_extract_table_info( request);

                switch (table_info->colnum) {
                    case COLUMN_NSMQTTNAME:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            memcpy( table_entry->nsMQTTName,
                                    table_entry->old_nsMQTTName,
                                    sizeof(table_entry->nsMQTTName));
                            memset( table_entry->old_nsMQTTName, 0,
                                    sizeof(table_entry->nsMQTTName));
                            table_entry->nsMQTTName_len =
                                    table_entry->old_nsMQTTName_len;
                        }
                        break;
                    case COLUMN_NSMQTTVALUE:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            memcpy( table_entry->nsMQTTValue,
                                    table_entry->old_nsMQTTValue,
                                    sizeof(table_entry->nsMQTTValue));
                            memset( table_entry->old_nsMQTTValue, 0,
                                    sizeof(table_entry->nsMQTTValue));
                            table_entry->nsMQTTValue_len =
                                    table_entry->old_nsMQTTValue_len;
                        }
                        break;
                    case COLUMN_NSMQTTDVALUE:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            table_entry->nsMQTTDValue     = table_entry->old_nsMQTTDValue;
                            table_entry->old_nsMQTTDValue = 0;
                        }
                        break;
                    case COLUMN_NSMQTTDPOW:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            table_entry->nsMQTTDPow     = table_entry->old_nsMQTTDPow;
                            table_entry->old_nsMQTTDPow = 0;
                        }
                        break;
                    case COLUMN_NSMQTTTYPE:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            memcpy( table_entry->nsMQTTType,
                                    table_entry->old_nsMQTTType,
                                    sizeof(table_entry->nsMQTTType));
                            memset( table_entry->old_nsMQTTType, 0,
                                    sizeof(table_entry->nsMQTTType));
                            table_entry->nsMQTTType_len =
                                    table_entry->old_nsMQTTType_len;
                        }
                        break;
                    case COLUMN_NSMQTTREADONLY:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            table_entry->nsMQTTReadonly     = table_entry->old_nsMQTTReadonly;
                            table_entry->old_nsMQTTReadonly = 0;
                        }
                        break;
                    case COLUMN_NSMQTTORDER:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            table_entry->nsMQTTOrder     = table_entry->old_nsMQTTOrder;
                            table_entry->old_nsMQTTOrder = 0;
                        }
                        break;
                    case COLUMN_NSMQTTTS:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            table_entry->nsMQTTTs     = table_entry->old_nsMQTTTs;
                            table_entry->old_nsMQTTTs = 0;
                        }
                        break;
                    case COLUMN_NSMQTTTOC:
                        if ( table_entry && !table_entry->valid ) {
                            removeEntry(table_row );
                        } else {
                            table_entry->nsMQTTTOC     = table_entry->old_nsMQTTTOC;
                            table_entry->old_nsMQTTTOC = 0;
                        }
                        break;
                }
            }
            break;

        case MODE_SET_COMMIT:
            for (request=requests; request; request=request->next) {
                if (request->processed)
                    continue;

                table_entry = (struct netSnmpMQTTTable_entry *)
                        netsnmp_tdata_extract_entry(request);
                table_info  =     netsnmp_extract_table_info(    request);

                switch (table_info->colnum) {
                    case COLUMN_NSMQTTNAME:
                    case COLUMN_NSMQTTVALUE:
                    case COLUMN_NSMQTTDVALUE:
                    case COLUMN_NSMQTTDPOW:
                    case COLUMN_NSMQTTTYPE:
                    case COLUMN_NSMQTTREADONLY:
                    case COLUMN_NSMQTTORDER:
                    case COLUMN_NSMQTTTS:
                    case COLUMN_NSMQTTTOC:
                        if ( table_entry && !table_entry->valid ) {
                            table_entry->valid = 1;
                        }
                }
            }
            break;
    }
    return SNMP_ERR_NOERROR;
}
