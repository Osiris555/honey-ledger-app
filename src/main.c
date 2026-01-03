#include "os.h"
#include "cx.h"
#include "ux.h"

__attribute__((section(".boot"))) int main(void) {
    os_boot();

    for (;;) {
        // Idle loop
    }
}
