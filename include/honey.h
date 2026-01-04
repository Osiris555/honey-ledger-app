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
#include "cx.h"

/* ================================
 * App
 * ================================ */

#define APPNAME        "Honey"
#define APPVERSION     "0.5.0"

/* ================================
 * Chain
 * ================================ */

#define HONEY_CHAIN_ID "honey-mainnet-1"

/* ================================
 * APDU
 * ================================ */

#define CLA_HONEY        0xE0
#define INS_GET_VERSION  0x01
#define INS_GET_ADDRESS  0x02
#define INS_SIGN_TX      0x03

/* ================================
 * Token
 * ================================ */

#define HONEY_DECIMALS   18
#define HONEY_TICKER     "HNY"

/* ================================
 * Address
 * ================================ */

#define HONEY_HRP        "hny"
#define HONEY_ADDR_LEN   45

/* ================================
 * Transaction
 * ================================ */

extern char     G_tx_recipient[HONEY_ADDR_LEN];
extern char     G_tx_amount[32];
extern uint32_t G_tx_nonce;
extern bool     G_tx_approved;

/* ================================
 * Crypto
 * ================================ */

extern cx_ecfp_private_key_t G_private_key;
extern cx_ecfp_public_key_t  G_public_key;

#endif
