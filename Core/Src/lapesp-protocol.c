// lapesp-protocol.c
#include "lapesp-protocol.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

//static const char *TAG = "LAPESP_PROTOCOL";

uint16_t crc16_ccitt(const uint8_t *data, size_t len, uint16_t crc) {
    for (size_t i = 0; i < len; i++) {     // Process each byte
        crc ^= (uint16_t)data[i] << 8;     // shifts the byte into the high 8 bits of the 16-bit CRC register
        for (int j = 0; j < 8; j++) {      // Process each bit
            if (crc & 0x8000)              // If the leftmost bit is set
                crc = (crc << 1) ^ 0x1021; // Shift left and XOR with the polynomial
            else
                crc <<= 1; // Just shift left
        }
    }
    return crc;
}

LapespPacket encode_packet(uint8_t type, const uint8_t *payload, size_t payload_len) {
    static uint8_t out_buffer[BUF_SIZE];
    size_t out_len = 0;
    LapespPacket packet;

    if (payload_len + LAPESP_PROTOCOL_METADATA_SIZE > BUF_SIZE) { // 7 bytes for header + type + length + CRC
        //ESP_LOGE(TAG, "Payload too large to encode");
        packet.out_buffer = NULL;
        packet.out_len = 0;
        packet.type = 0;
        return packet;
    }

    size_t idx = 0;

    // Header
    out_buffer[idx++] = FRAME_HEADER_1;
    out_buffer[idx++] = FRAME_HEADER_2;

    // Type
    out_buffer[idx++] = type;

    // Length - Use 2 bytes for length to support larger payloads - Leave at little-endian for simplicity
    out_buffer[idx++] = payload_len & 0xFF;        // Keep only the least significant byte
    out_buffer[idx++] = (payload_len >> 8) & 0xFF; // Shift right to get the most significant byte

    // Payload
    memcpy(&out_buffer[idx], payload, payload_len);
    idx += payload_len;

    // CRC
    uint16_t crc = crc16_ccitt(payload, payload_len, 0xFFFF);
    memcpy(&out_buffer[idx], &crc, sizeof(uint16_t));
    idx += sizeof(uint16_t); // Size of CRC16

    // Fill packet struct
    packet.out_buffer = out_buffer;
    packet.out_len = idx;
    packet.type = type;

    return packet;
}