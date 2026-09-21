#ifndef HEADER_H_
#define HEADER_H_

#include "DHT11.h"
#include "HCSensor.h"
#include "Presistor.h"
#include "WiFiPart.h"
#include "Outputs.h"
#include "UARTPart.h"
#include "ModbusPart.h"
#include "functionPart.h"
#include <Arduino.h>

#define UART_PORT UART_NUM_2

// al opcodes for modbus
#define PKT_HEADER        0xAA
#define CMD_SERVO_ANGLE   0x01
#define CMD_STEPPER_RUN   0x02
#define CMD_STEPPER_STOP  0x03
#define CMD_EMERGENCY     0x04
#define CMD_ACK_DONE      0x06


// global declaration for states of process
typedef enum {
    STATE_IDLE = 0,
    STATE_EMERGENCY,
    STATE_SORT,
    STATE_TRANSPORT_TO_SORT,
    STATE_TRANSPORT_TO_QC,
    STATE_QC_INSPECTION
} lineState;

extern lineState currentState;
extern volatile bool mega_reported_done;

#pragma pack(push, 1)
typedef struct {
    uint8_t sys_run_stop; // Coil 00001
} coils_reg_t;

typedef struct {
    uint8_t piece_detected; // Discrete 10001
    uint8_t qc_status;      // Discrete 10002
} discrete_reg_t;

typedef struct {
    uint16_t dist_hc_sr04;     // Input 30001
    uint16_t ldr_intensity;    // Input 30002
    int16_t  dht11_temp;       // Input 30003
    uint16_t dht11_hum;        // Input 30004
    uint16_t total_ok_pieces;  // Input 30005
    uint16_t total_rejects;    // Input 30006
} input_reg_t;

typedef struct {
    uint16_t operation_mode;       // Holding 40001 (0=Manual, 1=Auto)
    uint16_t temp_threshold;      // Holding 40002 (Default 25C)
    uint16_t ldr_threshold;       // Holding 40003 (Cutoff ADC)
    uint16_t hum_threshold;       // Holding 40004    
    uint16_t manual_servo_angle;  // Holding 40005
    uint16_t stepper_manual_steps;// Holding 40006
} holding_reg_t;
#pragma pack(pop)

static coils_reg_t     mb_coils     = {.sys_run_stop = 1};
static discrete_reg_t  mb_discrete  = {0};
static input_reg_t     mb_inputs    = {0};
static holding_reg_t   mb_holding   = {
    .operation_mode = 1,
    .temp_threshold = 25,
    .ldr_threshold = 1500,
    .manual_servo_angle = 0,
    .stepper_manual_steps = 0
};

void sensors(void *pvParameters);

#endif