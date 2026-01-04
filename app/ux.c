#include "os.h"
#include "ux.h"
#include "honey.h"
#include <string.h>
#include <stdio.h>

bool tx_approved = false;
honey_tx_t parsed_tx;

// Buffers displayed on screen
static char amount_str[40];
static char address_str[32];

/**
 * Convert uint256 (big endian) → decimal string with 18 decimals
 * Safe for Ledger (no floats)
 */
static void format_amount(void) {
    uint8_t non_zero_found = 0;
    int out = 0;

    strcpy(amount_str, "HNY ");
    out = strlen(amount_str);

    // Print integer part
    for (int i = 0; i < 32; i++) {
        if (parsed_tx.amount[i] != 0 || non_zero_found || i == 31) {
            non_zero_found = 1;
            out += snprintf(
                amount_str + out,
                sizeof(amount_str) - out,
                "%u",
                parsed_tx.amount[i]
            );
        }
    }

    // Decimal point
    strcat(amount_str, ".");
    out = strlen(amount_str);

    // Print 18 decimal places (fixed)
    for (int i = 0; i < HONEY_DECIMALS; i++) {
        amount_str[out++] = '0';
    }

    amount_str[out] = '\0';
}

/**
 * Shortened recipient address
 */
static void format_address(void) {
    snprintf(
        address_str,
        sizeof(address_str),
        "%02X%02X...%02X%02X",
        parsed_tx.recipient[0],
        parsed_tx.recipient[1],
        parsed_tx.recipient[18],
        parsed_tx.recipient[19]
    );
}

/* =========================
 * Ledger UX Steps
 * ========================= */

UX_STEP_NOCB(
    ux_review_step,
    pnn,
    {
        &C_icon_eye,
        "Review",
        "Honey Tx"
    }
);

UX_STEP_NOCB(
    ux_amount_step,
    bnnn_paging,
    {
        "Amount",
        amount_str
    }
);

UX_STEP_NOCB(
    ux_address_step,
    bnnn_paging,
    {
        "To",
        address_str
    }
);

UX_STEP_CB(
    ux_approve_step,
    pb,
    tx_approved = true;
    os_sched_exit(0);
    ,
    {
        &C_icon_validate_14,
        "Approve",
        "Sign"
    }
);

UX_STEP_CB(
    ux_reject_step,
    pb,
    tx_approved = false;
    os_sched_exit(0);
    ,
    {
        &C_icon_crossmark,
        "Reject",
        "Transaction"
    }
);

UX_FLOW(
    ux_tx_flow,
    &ux_review_step,
    &ux_amount_step,
    &ux_address_step,
    &ux_approve_step,
    &ux_reject_step
);

void ux_confirm_transaction(void) {
    tx_approved = false;
    format_amount();
    format_address();
    ux_flow_init(0, ux_tx_flow, NULL);
}
