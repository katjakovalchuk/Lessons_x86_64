#include "print.h"

#define VGA_BUFFER 0xB8000

void print_full_row(unsigned char character) {
  for (size_t col = 0; col <= NUM_COLS; col++) {
    print_char(character);
  }
}

void kernel_main() {
  print_clear();
  uint64_t cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0));

  cr0 &= ~(1 << 31); // 0 - disable

  // cr0 |= (1 << 31); // 1 - enable

  // asm volatile("mov %0, %%cr0" ::"r"(cr0));

  int paging_enabled = cr0 & (1 << 31);

  if (paging_enabled) {
    for (unsigned char c = 'z'; c >= 'a'; c--) {
      print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
      print_full_row(c);
    }
  } else {
    cr0 |= (1 << 31); // 1 - enable

    asm volatile("mov %0, %%cr0" ::"r"(cr0));

    for (unsigned char c = 'a'; c <= 'z'; c++) {
      print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
      print_full_row(c);
    }
  }

  asm volatile("hlt");
}
