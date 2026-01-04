#include "os.h"
#include "ux.h"
#include "honey.h"
#include <string.h>

/* ================================
 * Globals
 * ================================ */

char G_tx_recipient[HONEY_ADDR_LEN];
char G_tx_amount[32];
bool G_tx_approved = false;

/* ================================
 * Callbacks
 * ================================ */

static void ui_tx_approve(void) {
    G_tx_approved = true;
    ux_flow_exit();
}

static void ui_tx_reject(void) {
    G_tx_approved = false;
    ux_flow_exit();
}

/* ================================
 * UI Steps
 * ================================ */

UX_STEP_NOCB(
    ux_tx_title_step,
    pnn,
    {
        &C_icon_eye,
        "Review",
        "Transaction",
    }
);

UX_STEP_NOCB(
    ux_tx_recipient_step,
    bnnn_paging,
    {
        .title = "To",
        .text = G_tx_recipient,
    }
);

UX_STEP_NOCB(
    ux_tx_amount_step,
    bnnn_paging,
    {
        .title = "Amount (HNY)",
        .text = G_tx_amount,
    }
);

UX_STEP_CB(
    ux_tx_approve_step,
    pb,
    ui_tx_approve(),
    {
        &C_icon_validate_14,
        "Approve",
    }
);

UX_STEP_CB(
    ux_tx_reject_step,
    pb,
    ui_tx_reject(),
    {
        &C_icon_crossmark,
        "Reject",
    }
);

/* ================================
 * Flow
 * ================================ */

UX_FLOW(
    ux_confirm_tx_flow,
    &ux_tx_title_step,
    &ux_tx_recipient_step,
    &ux_tx_amount_step,
    &ux_tx_approve_step,
    &ux_tx_reject_step
);

/* ================================
 * Public entry
 * ================================ */

void ui_confirm_transaction(void) {
    G_tx_approved = false;
    ux_flow_init(0, ux_confirm_tx_flow, NULL);
}
