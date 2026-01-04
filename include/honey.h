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
#include <stdbool.h>

/* APDU class */
#define HONEY_CLA 0xE0

/* Instructions */
#define INS_GET_PUBLIC_KEY 0x02
#define INS_SIGN_TX        0x04

/* Status words */
#define SW_OK                 0x9000
#define SW_INS_NOT_SUPPORTED  0x6D00
#define SW_WRONG_LENGTH       0x6700

/* Sizes */
#define PUBKEY_LEN 33   // compressed secp256k1
#define ADDRESS_LEN 20  // Honey address (hash160)
