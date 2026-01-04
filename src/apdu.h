#pragma once

#include <stdint.h>

/* CLA */
#define CLA_HONEY 0xE0

/* INS */
#define INS_GET_APP_VERSION 0x01
#define INS_GET_APP_NAME    0x02
#define INS_GET_PUBLIC_KEY  0x03

#define INS_SIGN_TX         0x04
#define INS_SIGN_FINAL      0x05

/* Status words */
#define SW_OK                   0x9000
#define SW_INS_NOT_SUPPORTED    0x6D00
#define SW_WRONG_P1P2           0x6B00
#define SW_WRONG_LENGTH         0x6700
#define SW_SECURITY_STATUS_NOT_SATISFIED 0x6982
