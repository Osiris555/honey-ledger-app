#include "apdu.h"

void handle_get_version(uint8_t *buffer, uint16_t *tx_len) {
    buffer[0] = 0x01; // major
    buffer[1] = 0x00; // minor
    buffer[2] = 0x00; // patch
    *tx_len = 3;
}

void handle_ping(uint8_t *buffer, uint16_t *tx_len) {
    buffer[0] = 0xAA;
    buffer[1] = 0x55;
    *tx_len = 2;
}
