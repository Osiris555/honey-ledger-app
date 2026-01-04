#pragma once

#include <stdint.h>

// CLA
#define CLA_HONEY 0xE0

// INS codes
#define INS_GET_APP_VERSION 0x01
#define INS_GET_PUBLIC_KEY  0x02
#define INS_GET_APP_NAME    0x03

// APDU handler
void handle_apdu(uint8_t *buffer, uint16_t length);
