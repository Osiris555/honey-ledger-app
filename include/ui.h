#pragma once

#include <stdint.h>

void ui_idle(void);
void ui_show_address(const char *addr);
void ui_confirm_tx(void *tx);
