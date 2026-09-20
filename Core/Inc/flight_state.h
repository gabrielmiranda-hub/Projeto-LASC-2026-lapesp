#pragma once

#include "cmsis_os.h"
#include "lapesp-types.h"
#include "stm32f4xx_hal.h"

extern LapespDTO lapesp_dto;
extern osMutexId_t lapesp_mutex;

typedef enum 
{
    FLIGHT_STAGE_LAUNCH = 0,     // em terra, na plataforma (estado inicial)
    FLIGHT_STAGE_POWERED_ASCENT, // motor em combustao
    FLIGHT_STAGE_COASTING,       // pos-queima, subindo por inercia
    FLIGHT_STAGE_DESCENT,        // pos-apogeu, descendo
    FLIGHT_STAGE_RECOVERY,       // em solo, repouso confirmado (terminal)
} FlightStage_t;

/**
 * Task FreeRTOS da maquina de estados de voo.
 *
 * Roda em prioridade abaixo das tasks de sensoriamento. A cada iteracao le
 * BME_PRESSURE e BME_TEMP do DTO global, deriva altitude e velocidade vertical,
 * e avalia a condicao de transicao do estado atual.
 */
void task_flight_state(void  *argument);

FlightStage_t FlightState_Get(void);
double FlightState_GetAltitude(void);
double FlightState_GetVerticalSpeed(void);
double FlightState_GetApogeeAltitude(void);
uint32_t FlightState_GetApogeeTick(void);
const char *FlightState_Name(FlightStage_t stage);
