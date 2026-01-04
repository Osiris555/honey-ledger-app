#include "os.h"
#include "ux.h"
#include "ui.h"

static ux_step_t ux_steps[4];
static ux_flow_t ux_flow;

static char amount_text[32];
static bool approved = false;

/* ---------- UI Steps ---------- */

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
        amount_text
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

/* ---------- Flow ---------- */

void ui_sign_tx_start(uint64_t amount) {
    snprintf(amount_text, sizeof(amount_text), "%llu HNY", amount);
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
