cmake_minimum_required(VERSION 3.5)
project(wb_snmp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp wb-snmp.h netSnmpMQTTTable.cpp netSnmpMQTTTable.h netSnmpMQTTTable_entry.cpp netSnmpMQTTTable_entry.h netSnmpMQTTTableHandler.cpp TMQTTSNMPHandler.cpp TMQTTSNMPHandler.h)
set(LIBRARIES netsnmp wbmqtt netsnmpagent mosquittopp)
add_executable(wb_snmp ${SOURCE_FILES})
target_link_libraries(wb_snmp ${LIBRARIES})