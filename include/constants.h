#pragma once

// APDU
#define CLA_HONEY 0xE0

// Instructions
#define INS_GET_VERSION 0x01
#define INS_PING        0x02

// Status words
#define SW_OK                   0x9000
#define SW_INS_NOT_SUPPORTED    0x6D00
#define SW_CLA_NOT_SUPPORTED    0x6E00
#define SW_WRONG_LENGTH         0x6700
