#include "ui.h"
#include "handler_sign_tx.h"

#include "os.h"
#include "ux.h"

static char ui_address[21];
static char ui_amount[32];

UX_STEP_NOCB(
    ux_display_address,
    bnnn_paging,
    {
        .title = "Send To",
        .text  = ui_address
    }
);

UX_STEP_NOCB(
    ux_display_amount,
    bnnn_paging,
    {
        .title = "Amount",
        .text  = ui_amount
    }
);

UX_STEP_CB(
    ux_confirm,
    pb,
    sign_transaction(),
    {
        &C_icon_validate_14,
        "Sign",
        "transaction"
    }
);

UX_STEP_CB(
    ux_reject,
    pb,
    os_sched_exit(-1),
    {
        &C_icon_crossmark,
        "Reject",
        "transaction"
    }
);

UX_FLOW(
    ux_sign_flow,
    &ux_display_address,
    &ux_display_amount,
    &ux_confirm,
    &ux_reject
);

void ui_display_tx(const char *address, const char *amount) {
    strncpy(ui_address, address, sizeof(ui_address) - 1);
    strncpy(ui_amount, amount, sizeof(ui_amount) - 1);
    ux_flow_init(0, ux_sign_flow, NULL);
}
