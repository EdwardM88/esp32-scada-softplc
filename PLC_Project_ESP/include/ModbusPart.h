#ifndef MODBUSPART_H
#define MODBUSPART_H

#include <ModbusIP_ESP8266.h>
#include <stdint.h>

#define Reg_TEMP 100
#define Reg_HUM 101
#define Reg_DIST 102
#define Reg_LDR 103

void initModbusPart();
void handleModbusRequest();
void updateModbusRegisters(float temperature, float humidity, float distance,float ldr);

#endif // MODBUSPART_H
