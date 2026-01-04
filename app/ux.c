#include "os.h"
#include "ux.h"
#include "honey.h"

bool tx_approved = false;

UX_STEP_NOCB(
    ux_tx_review_step,
    pnn,
    {
      &C_icon_eye,
      "Review",
      "Honey Transaction"
    }
);

UX_STEP_CB(
    ux_tx_approve_step,
    pb,
    tx_approved = true;
    os_sched_exit(0);
    ,
    {
      &C_icon_validate_14,
      "Approve",
      "Sign transaction"
    }
);

UX_STEP_CB(
    ux_tx_reject_step,
    pb,
    tx_approved = false;
    os_sched_exit(0);
    ,
    {
      &C_icon_crossmark,
      "Reject",
      "Transaction"
    }
);

UX_FLOW(
    ux_tx_flow,
    &ux_tx_review_step,
    &ux_tx_approve_step,
    &ux_tx_reject_step
);

void ux_confirm_transaction(void) {
    tx_approved = false;
    ux_flow_init(0, ux_tx_flow, NULL);
}
