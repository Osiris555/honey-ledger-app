#include "os.h"
#include "ux.h"
#include "ui.h"

static char address_buffer[50];

UX_STEP_NOCB(
    ux_idle_flow_1_step,
    pnn,
    {
        &C_icon_dashboard,
        "Honey",
        "Ready",
    });

UX_FLOW(
    ux_idle_flow,
    &ux_idle_flow_1_step
);

UX_STEP_NOCB(
    ux_addr_step,
    bnnn,
    {
        "Honey",
        "Address",
        address_buffer,
    });

UX_FLOW(
    ux_address_flow,
    &ux_addr_step
);

void ui_idle(void) {
    ux_flow_init(0, ux_idle_flow, NULL);
}

void ui_show_address(const char *addr) {
    os_strcpy(address_buffer, addr);
    ux_flow_init(0, ux_address_flow, NULL);
}

void ui_confirm_tx(void *tx) {
    (void)tx;
    // Placeholder — expanded in later step
}
