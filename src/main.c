#include "os.h"
#include "cx.h"

__attribute__((section(".boot")))
int main(void) {
    for (;;) {
        // Idle loop
    }
    return 0;
}
