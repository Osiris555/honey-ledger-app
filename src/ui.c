#include "format.h"
#include "os.h"
#include "ux.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>

static ux_step_t ux_steps[6];
static ux_flow_t ux_flow;

static char recipient_text[64];
static char amount_text[32];
static char fee_text[32];
static bool approved = false;

/* ---------- UI STEPS ---------- */

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
    ux_recipient_step,
    bnnn_paging,
    {
        "Recipient",
        recipient_text
    }
);

UX_STEP_NOCB(
    ux_amount_step,
    bnnn_paging,
    {
        "Amount",
        amount_text
    }
);

UX_STEP_NOCB(
    ux_fee_step,
    bnnn_paging,
    {
        "Fee",
        fee_text
    }
);

UX_STEP_CB(
    ux_approve_step,
    pbb,
    {
        &C_icon_validate_14,
        "Approve",
        "transaction",
    },
    {
        approved = true;
        ux_flow_exit();
    }
);

UX_STEP_CB(
    ux_reject_step,
    pbb,
    {
        &C_icon_crossmark,
        "Reject",
        "transaction",
    },
    {
        approved = false;
        ux_flow_exit();
    }
);

/* ---------- FLOW ---------- */

void ui_sign_tx_start(
    const char *recipient_raw,
    uint64_t amount,
    uint64_t fee
) {
    format_address(
        (const uint8_t *)recipient_raw,
        recipient_text,
        sizeof(recipient_text)
    );

    format_hny_amount(
        amount,
        amount_text,
        sizeof(amount_text)
    );

    format_hny_amount(
        fee,
        fee_text,
        sizeof(fee_text)
    );

    approved = false;

    ux_flow_init(
        0,
        &ux_flow,
        ux_steps,
        ARRAYLEN(ux_steps)
    );
}

bool ui_sign_tx_approved(void) {
    return approved;
}
