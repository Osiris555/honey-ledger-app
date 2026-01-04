#pragma once

#include <stdint.h>
#include <stdbool.h>

void ui_sign_tx_start(
    const char *recipient,
    uint64_t amount,
    uint64_t fee
);

bool ui_sign_tx_approved(void);
