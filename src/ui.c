#include "os.h"
#include "ux.h"
#include "honey.h"
#include <string.h>

/* ================================
 * Globals
 * ================================ */

char G_honey_address[HONEY_ADDR_LEN];
bool G_address_approved = false;

/* ================================
 * UI Callbacks
 * ================================ */

static void ui_confirm_address_ok(void) {
    G_address_approved = true;
    ux_flow_exit();
}

static void ui_confirm_address_cancel(void) {
    G_address_approved = false;
    ux_flow_exit();
}

/* ================================
 * Address confirmation flow
 * ================================ */

UX_STEP_NOCB(
    ux_address_title_step,
    pnn,
    {
        &C_icon_eye,
        "Confirm",
        "Address",
    }
);

UX_STEP_NOCB(
    ux_address_value_step,
    bnnn_paging,
    {
        .title = "Honey",
        .text = G_honey_address,
    }
);

UX_STEP_CB(
    ux_address_confirm_step,
    pb,
    ui_confirm_address_ok(),
    {
        &C_icon_validate_14,
        "Approve",
    }
);

UX_STEP_CB(
    ux_address_reject_step,
    pb,
    ui_confirm_address_cancel(),
    {
        &C_icon_crossmark,
        "Reject",
    }
);

UX_FLOW(
    ux_confirm_address_flow,
    &ux_address_title_step,
    &ux_address_value_step,
    &ux_address_confirm_step,
    &ux_address_reject_step
);

/* ================================
 * Public entry point
 * ================================ */

void ui_display_address(void) {
    G_address_approved = false;
    ux_flow_init(0, ux_confirm_address_flow, NULL);
}
