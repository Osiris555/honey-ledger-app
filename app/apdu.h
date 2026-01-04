#ifndef HONEY_APDU_H
#define HONEY_APDU_H

#include <stdint.h>

void handle_get_version(uint8_t *buffer, uint16_t *tx_len);
void handle_ping(uint8_t *buffer, uint16_t *tx_len);

#endif
