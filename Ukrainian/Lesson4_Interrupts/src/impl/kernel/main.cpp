#include "print.h"
#include "interrupt_table.h"

extern "C" {

void kernel_main() {
    static const char *msg = "Welcome to our 64-bit kernel!";
    load_idt();
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str(msg);

    asm volatile (
        "int $0x0\n\t"
        "hlt\n\t"
        "hlt\n\t"
    );
}

}