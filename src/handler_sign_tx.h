#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_TX_SIZE 256

typedef struct {
    uint8_t buffer[MAX_TX_SIZE];
    uint16_t length;
    bool active;
} tx_context_t;

void handle_sign_tx_chunk(uint8_t p1, uint8_t *data, uint16_t len);
