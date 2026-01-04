#include "os.h"
#include "ux.h"
#include "globals.h"

static ux_layout_t ux;

static const char *review_title = "Review Tx";
static char amount_buffer[32];

void ui_sign_tx_init(uint64_t amount) {
    snprintf(amount_buffer, sizeof(amount_buffer), "%llu HNY", amount);

    ux_layout_paging_init(
        &ux,
        review_title,
        amount_buffer
    );
}

bool ui_sign_tx_approve(void) {
    return true;
}

bool ui_sign_tx_reject(void) {
    return false;
}
