# wb-snmp
SNMP subaget for WirenBoard
snmptable -c public -v2c localhost NET-SNMP-WB-NNM::netSnmpMQTTTable

The table structure:

 nsMQTTName   - name if param DeviceName/Control
 nsMQTTValue  - String Value
 nsMQTTDValue - Integer value
 nsMQTTDPow 
 nsMQTTType -  Type (Temperature, Range, etc.)
 nsMQTTReadonly  - Readonly
 nsMQTTOrder - Order
 nsMQTTTs  - Timestamp in MQTT
 nsMQTTTOC - Timestamp in Table
 
