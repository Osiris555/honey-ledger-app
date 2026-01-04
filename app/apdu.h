#pragma once

#include <stdint.h>

// APDU constants
#define CLA_HONEY  0xE0

#define INS_GET_VERSION 0x01

// Status words
#define SW_OK                 0x9000
#define SW_INS_NOT_SUPPORTED  0x6D00
#define SW_CLA_NOT_SUPPORTED  0x6E00

// Entry point
void handle_apdu(uint8_t cla, uint8_t ins);
