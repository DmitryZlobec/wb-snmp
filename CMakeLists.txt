cmake_minimum_required(VERSION 2.2)
project(wb_snmp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive")

set(SOURCE_FILES main.cpp netSnmpMQTTTable.cpp netSnmpMQTTTable.h netSnmpMQTTTable_entry.cpp netSnmpMQTTTable_entry.h netSnmpMQTTTableHandler.cpp TMQTTSNMPHandler.cpp TMQTTSNMPHandler.h)
set(LIBRARIES netsnmpagent netsnmphelpers netsnmpmibs netsnmp  wbmqtt mosquittopp)
add_executable(wb_snmp ${SOURCE_FILES})
target_link_libraries(wb_snmp ${LIBRARIES})
