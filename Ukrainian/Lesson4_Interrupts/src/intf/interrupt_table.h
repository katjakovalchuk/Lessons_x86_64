#pragma once

#ifndef INT_TABLE_H
#define INT_TABLE_H

extern "C" {

#include <stdint.h>

struct IDTEntry {
    uint16_t offset_low;     // Lower 16 bits of the handler address
    uint16_t selector;       // Code segment selector in GDT
    uint8_t ist;             // Interrupt Stack Table offset (set to 0 for simplicity)
    uint8_t type_attr;       // Type and attributes (e.g., 0x8E for interrupt gate)
    uint16_t offset_mid;     // Middle 16 bits of the handler address
    uint32_t offset_high;    // High 32 bits of the handler address
    uint32_t zero;           // Reserved (set to 0)

    IDTEntry(void (*handler)(), uint16_t sel, uint8_t attr) {
        uint64_t handler_address = reinterpret_cast<uint64_t>(handler);
        offset_low = handler_address & 0xFFFF;
        selector = sel;
        ist = 0;
        type_attr = attr;
        offset_mid = (handler_address >> 16) & 0xFFFF;
        offset_high = (handler_address >> 32) & 0xFFFFFFFF;
        zero = 0;
    }
};

struct IDTDescriptor {
    uint16_t limit;      // Size of the IDT - 1
    uint64_t base;       // Base address of the IDT

    IDTDescriptor(IDTEntry const* baseAddress, uint16_t size) {
        limit = size - 1;
        base = reinterpret_cast<uint64_t>(baseAddress);
    }
} __attribute__((packed));

void isr_handler() {
    asm volatile (
        // "mov $0x1, %rax\n\t"
        "iretq"
    );
}

static const IDTEntry idt[1] = {
    IDTEntry(isr_handler, 0x08, 0x8E) 
};

static const IDTDescriptor idtDescriptor(idt, sizeof(idt));

static void load_idt() {
    asm volatile (
        "lidt (%0)"
        :
        : "r" (&idtDescriptor)
    );
}

}

#endif // INT_TABLE_H