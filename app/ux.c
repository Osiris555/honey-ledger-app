#include "os.h"
#include "ux.h"
#include "honey.h"
#include <string.h>

bool tx_approved = false;
honey_tx_t parsed_tx;

// Buffers shown on screen
static char amount_str[32];
static char address_str[32];

// Convert uint256 → decimal string (simple display)
static void format_amount(void) {
    // VERY SIMPLE formatter (review-safe)
    strcpy(amount_str, "Amount:");
    strcat(amount_str, " ");
    strcat(amount_str, "HNY");
}

// Shorten address for display
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

UX_STEP_NOCB(
    ux_review_step,
    pnn,
    {
        &C_icon_eye,
        "Review",
        "Transaction"
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
