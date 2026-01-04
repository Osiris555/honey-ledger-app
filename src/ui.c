#include "ui.h"
#include "ux.h"
#include "os.h"

/* ---------- IDLE SCREEN ---------- */

UX_STEP_NOCB(
    ux_idle_step,
    pnn,
    {
        &C_icon_dashboard,
        "Honey",
        "Ready",
    }
);

UX_FLOW(
    ux_idle_flow,
    &ux_idle_step
);

void ui_idle(void) {
    ux_flow_init(0, ux_idle_flow, NULL);
}

/* ---------- APPROVE TX ---------- */

UX_STEP_NOCB(
    ux_sign_tx_step,
    pnn,
    {
        &C_icon_eye,
        "Sign Honey",
        "transaction?",
    }
);

UX_STEP_CB(
    ux_sign_tx_approve,
    pb,
    {
        &C_icon_validate_14,
        "Approve",
    },
    {
        // Continue signing
    }
);

UX_STEP_CB(
    ux_sign_tx_reject,
    pb,
    {
        &C_icon_crossmark,
        "Reject",
    },
    {
        THROW(0x6985);
    }
);

UX_FLOW(
    ux_sign_tx_flow,
    &ux_sign_tx_step,
    &ux_sign_tx_approve,
    &ux_sign_tx_reject
);

void ui_approve_tx(void) {
    ux_flow_init(0, ux_sign_tx_flow, NULL);
}
