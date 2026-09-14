#include "ModbusPart.h"

ModbusIP mb;


void initModbusPart()
{
    mb.server(); // Start Modbus TCP server

    mb.addHreg(Reg_TEMP); // Add a holding register for temperature
    mb.addHreg(Reg_HUM); // Add a holding register for humidity
    mb.addHreg(Reg_DIST); // Add a holding register for distance
    mb.addHreg(Reg_LDR); // Add a holding register for LDR

    Serial.println("Load Modbus");

}

void handleModbusRequest()
{
    mb.task(); // Handle Modbus requests
}

void updateModbusRegisters(float temperature, float humidity, float distance,float ldr)
{
    uint16_t tempValue = (uint16_t)(temperature * 10); // Convert to integer representation
    uint16_t humValue = (uint16_t)(humidity * 10); // Convert to integer representation
    uint16_t distValue = (uint16_t)(distance); // Convert to integer representation

    mb.Hreg(Reg_TEMP, tempValue); // Update temperature register
    mb.Hreg(Reg_HUM, humValue); // Update humidity register
    mb.Hreg(Reg_DIST, distValue); // Update distance register
    mb.Hreg(Reg_LDR, (uint16_t)(ldr)); // Update LDR register
}