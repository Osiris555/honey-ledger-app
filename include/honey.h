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

/*
 * HONEY Monetary Units
 *
 * Base unit: CONE
 * 1 HONEY (HNY) = 10^18 CONES
 *
 * Ledger UI displays HONEY
 * Ledger signing uses CONES (uint64 / big-int)
 */

#define HNY_DECIMALS 18
#define CONES_PER_HNY 1000000000000000000ULL

// Formatting helpers
void format_hny_amount(uint64_t amount_cones, char *out, uint32_t out_len);
