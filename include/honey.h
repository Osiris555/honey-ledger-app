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
 * App metadata
 * ================================ */

#define APPNAME "Honey"
#define APPVERSION "0.2.0"

/* ================================
 * APDU
 * ================================ */

#define CLA_HONEY        0xE0
#define INS_GET_VERSION  0x01
#define INS_GET_ADDRESS  0x02
#define INS_SIGN_TX      0x03

/* ================================
 * Token constants
 * ================================ */

#define HONEY_DECIMALS   18
#define HONEY_TICKER     "HNY"

/* ================================
 * Address
 * ================================ */

#define HONEY_HRP        "hny"
#define HONEY_ADDR_LEN   45

/* ================================
 * Globals (UI)
 * ================================ */

extern char G_tx_recipient[HONEY_ADDR_LEN];
extern char G_tx_amount[32];
extern bool G_tx_approved;

#endif
