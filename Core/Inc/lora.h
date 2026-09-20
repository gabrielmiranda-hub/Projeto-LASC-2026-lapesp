/*
 * lora.h
 *
 *  Created on: 11 de ago. de 2026
 *      Author: eduardo-reichert
 */

#ifndef INC_LORA_H_
#define INC_LORA_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define UART_PORT GPIOA
#define PIN_TX    GPIO_PIN_2 //PA2 (TX2)
#define PIN_RX    GPIO_PIN_3 //PA3 (RX2)

#define AUX_PORT  GPIOA
#define PIN_AUX   GPIO_PIN_7 //PA7

#define MODE_PORT GPIOB
#define PIN_M0    GPIO_PIN_0 //PB0
#define PIN_M1    GPIO_PIN_1 //PB1

//verifa o pino auxiliar, se estiver LOW, retorna true
bool AuxisLOW();

//espera o auxiliar ficar LOW, o que indca que o modulo foi configurado ou enviou mensagem
void wait_AuxisLOW();

//envia mensagem via uart2
void UART_Send(char* mensagem);

#endif /* INC_LORA_H_ */