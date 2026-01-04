#include "ui.h"
#include <string.h>

#include "os.h"
#include "ux.h"

static char g_address[65];
static char g_amount[32];
static bool g_approved = false;

/* ---------- UI CALLBACKS ---------- */

static void approve_cb(void) {
    g_approved = true;
    ux_flow_next();
}

static void reject_cb(void) {
    g_approved = false;
    ux_flow_next();
}

/* ---------- UI FLOW ---------- */

UX_STEP_NOCB(
    ux_step_title,
    pnn,
    {
        .title = "Send",
        .text = "HONEY",
    });

UX_STEP_NOCB(
    ux_step_address,
    bnnn_paging,
    {
        .title = "To",
        .text = g_address,
    });

UX_STEP_NOCB(
    ux_step_amount,
    bnnn_paging,
    {
        .title = "Amount",
        .text = g_amount,
    });

UX_STEP_CB(
    ux_step_approve,
    pb,
    approve_cb(),
    {
        .text = "Approve",
    });

UX_STEP_CB(
    ux_step_reject,
    pb,
    reject_cb(),
    {
        .text = "Reject",
    });

UX_FLOW(
    ux_flow_tx,
    &ux_step_title,
    &ux_step_address,
    &ux_step_amount,
    &ux_step_approve,
    &ux_step_reject
);

/* ---------- PUBLIC API ---------- */

void ui_init(void) {
    g_approved = false;
}

void ui_display_tx(const char *address, const char *amount) {
    strncpy(g_address, address, sizeof(g_address) - 1);
    strncpy(g_amount, amount, sizeof(g_amount) - 1);
    ux_flow_init(0, ux_flow_tx, NULL);
}

bool ui_approved(void) {
    return g_approved;
}
