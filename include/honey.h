/*
 * Honey Monetary Definitions
 *
 * Base unit: CONE
 * Display unit: HONEY (HNY)
 *
 * 1 HNY = 10^18 CONES
 *
 * - All cryptographic signing uses CONES
 * - UI formatting converts CONES → HNY
 * - No floating-point math is used
 *
 * This model is fixed and protocol-level.
 */

#pragma once

#include <stdint.h>

#define CLA_HONEY              0xE0

#define INS_GET_PUBLIC_KEY     0x02
#define INS_GET_ADDRESS        0x03
#define INS_SIGN_TX            0x04

#define SW_OK                  0x9000
#define SW_INS_NOT_SUPPORTED   0x6D00
#define SW_WRONG_LENGTH        0x6700

#define PUBKEY_LEN             65
#define HONEY_ADDR_LEN         43

#define HONEY_BIP32_PATH_LEN 5

static const uint32_t HONEY_BIP32_PATH[HONEY_BIP32_PATH_LEN] = {
    44  | 0x80000000,
    118 | 0x80000000,
    0   | 0x80000000,
    0,
    0
};
