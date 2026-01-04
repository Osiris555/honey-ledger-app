#pragma once

#include <stdint.h>
#include "tx.h"

/* Idle screen */
void ui_idle(void);

/* Address verification */
void ui_verify_address(const char *addr);

/* Transaction confirmation */
void ui_confirm_tx(const honey_tx_t *tx);
