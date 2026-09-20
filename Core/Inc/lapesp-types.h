#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <time.h>

#define TYPE_LAPESP_DTO 0x01
#define TYPE_FLASH_TOGGLE_DTO 0x02

//esse atributo packed remove qualquer byte de alinhamento e garante que a estrutura tenha exatamente o numero de bytes
//double = 8bytes
//int    = 4bytes
//time_t = 8bytes
typedef struct __attribute__((packed)) {
  // BME280  = 24 bytes
  double BME_temperatura;
  double BME_pressao;
  double BME_humidade;

 // BNO085 (orientação e movimento) 32 + 24 + 24 = 80 bytes
  double BNO_q0, BNO_q1, BNO_q2, BNO_q3;        // quatérnio (32 bytes)
  double BNO_accel_x, BNO_accel_y, BNO_accel_z; // aceleração linear (24 bytes)
  double BNO_gyro_x, BNO_gyro_y, BNO_gyro_z;    // velocidade angular (24 bytes)

  // ADXL375 (alta-g) --aceleração bruta (±200g) = 24 bytes
  double ADXL_acelg_x;
  double ADXL_acelg_y;
  double ADXL_acelg_z;

  // GPS = 44 bytes
  double latitude;
  double longitude;
  double altitude;
  int GPS_count;
  time_t timestamp;
  double GPS_PREC;

  // MOSFET (carga)
  uint8_t mosfet_state; // 1 byte

  //EPS = 16 bytes
  double CURRENT;
  double VOLTAGE;
} LapespDTO; //24 + 80 + 24 + 44 + 1 + 16 = 189 bytes

typedef struct __attribute__((packed)) {
    uint8_t state;
} FlashToggleDTO; // 1 byte // can it be 8 bits?

// Message types for queue communication
typedef enum {
  MSG_TYPE_LAPESP_DTO,
  MSG_TYPE_FLASH_TOGGLE
} MessageType;

typedef struct __attribute__((packed)) {
    MessageType type;
    union {
        LapespDTO lapesp_data;
        FlashToggleDTO flash_toggle_data;
    } data;
} QueueMessage;

#ifdef __cplusplus
}
#endif