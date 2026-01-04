#include "os.h"
#include "ux.h"
#include "honey.h"
#include "nbgl_use_case.h"

void ui_display_tx(void) {
    nbgl_useCaseReview(
        "Sign Honey Tx",
        "Structured transaction",
        "Safe signing",
        "Approve",
        ui_approve_tx,
        "Reject",
        ui_reject_tx
    );
}

void ui_show_blind_status(void) {
    nbgl_useCaseChoice(
        "Blind signing",
        "You are signing unknown data",
        "This may be dangerous",
        "Continue",
        ui_approve_tx,
        "Reject",
        ui_reject_tx
    );
}

void ui_approve_tx(void) {
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x9000);
}

void ui_reject_tx(void) {
    io_exchange(CHANNEL_APDU | IO_RETURN_AFTER_TX, 0x6985);
}
