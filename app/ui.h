#pragma once

#include <stdint.h>
#include "tx.h"

void ui_idle(void);
void ui_show_address(const char *addr);
void ui_confirm_tx(const honey_tx_t *tx);
