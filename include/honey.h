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

#ifndef HONEY_H
#define HONEY_H

#include <stdint.h>
#include <stdbool.h>

/* ================================
 * App constants
 * ================================ */

#define APPNAME "Honey"
#define APPVERSION "0.1.0"

/* ================================
 * APDU Instructions
 * ================================ */

#define CLA_HONEY        0xE0
#define INS_GET_VERSION  0x01
#define INS_GET_ADDRESS  0x02
#define INS_SIGN_TX      0x03

/* ================================
 * Address format
 * ================================ */

#define HONEY_HRP        "hny"
#define HONEY_ADDR_LEN   45   // typical bech32 length

/* ================================
 * Global buffers
 * ================================ */

extern char G_honey_address[HONEY_ADDR_LEN];
extern bool G_address_approved;

#endif
