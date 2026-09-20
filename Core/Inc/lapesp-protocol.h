#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BUF_SIZE 1024
#define FRAME_HEADER_1 0xAA
#define FRAME_HEADER_2 0x55
#define LAPESP_PROTOCOL_METADATA_SIZE 7 // 2 (header) + 1 (type) + 2 (length) + 2 (CRC)

uint16_t crc16_ccitt(const uint8_t *data, size_t len, uint16_t crc_init);

typedef struct {
    uint8_t *out_buffer;
    size_t out_len;
    uint8_t type;
} LapespPacket;

LapespPacket encode_packet(uint8_t type, const uint8_t *payload, size_t payload_len);

// Decoder states
typedef enum {
  LAPESP_DECODE_HEADER1,
  LAPESP_DECODE_HEADER2,
  LAPESP_DECODE_TYPE,
  LAPESP_DECODE_LEN_LOW,
  LAPESP_DECODE_LEN_HIGH,
  LAPESP_DECODE_PAYLOAD,
  LAPESP_DECODE_CRC_LOW,
  LAPESP_DECODE_CRC_HIGH
} LapespDecodeState;

// Decoder structure
typedef struct {
    LapespDecodeState state;
    uint8_t type;
    uint16_t payload_len;
    uint16_t crc_received;
    uint16_t crc_computed;
    uint16_t idx;
    uint8_t payload[BUF_SIZE];
} LapespDecoder;

void lapesp_decoder_reset(LapespDecoder *decoder);
bool lapesp_decoder_feed(LapespDecoder *decoder, uint8_t byte);

#ifdef __cplusplus
}
#endif