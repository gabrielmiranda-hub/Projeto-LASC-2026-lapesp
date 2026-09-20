/*
 * lora.c
 *
 *  Created on: 11 de ago. de 2026
 *      Author: eduardo-reichert
 */
#include "lora.h"
#include <string.h>

extern UART_HandleTypeDef huart2;

//verifa o pino auxiliar, se estiver LOW, retorna true
bool AuxisLOW(){
  if(HAL_GPIO_ReadPin(AUX_PORT, PIN_AUX) == 0){
    return true;
  }else{
    return false;
  }
}

//espera o auxiliar ficar LOW, o que indca que o modulo foi configurado
void wait_AuxisLOW(){
  while(!AuxisLOW()){ //enquanto for false
    //continua
  }

  return;
}

void UART_Send(char* mensagem){
    HAL_UART_Transmit(
        &huart2,
        (uint8_t*)mensagem,
        strlen(mensagem),
        HAL_MAX_DELAY
    );
}