/******************************************************************************
 * File:       /vv4os/arch/x86/gdt.c                                          *
 * Author:     velikiyv4                                                      *
 * Created on: Sep 24, 2016                                                   *
 * Purpose:    Allows working with the IA32 Global Descriptor Table.          *
 ******************************************************************************/

#include "gdt.h"

#include <stdint.h>
#include <stdbool.h>

static GdtDesc gdt[3];

static GdtAccessByte ConstructGdtAccessByte(bool opt_rights, bool dc,
        bool is_code, uint8_t dpl) {
    GdtAccessByte ret;
    ret.opt_rights = opt_rights;
    ret.dc = dc;
    ret.is_code = is_code;
    ret.dpl = dpl;
    ret.accessed = false;
    ret.one = 1;
    ret.present = true;
    return ret;
}

static GdtDesc ConstructGdtEntry(uint32_t limit, uint32_t base,
        GdtAccessByte access_byte, bool is_32bit, bool page_gr) {
    GdtDesc ret;
    ret.limit_low = limit;
    ret.limit_high = limit >> 16;
    ret.base_low = base;
    ret.base_high = base >> 24;
    ret.access_byte = access_byte;
    ret.is_32bit = is_32bit;
    ret.page_gr = page_gr;
    ret.zero = 0;
    return ret;
}

void InitGdt() {
    GdtDesc desc = { };
    gdt[0] = desc;
    GdtAccessByte access_byte = ConstructGdtAccessByte(false, false, true, 0);
    desc = ConstructGdtEntry(0xFFFFF, 0, access_byte, true, true);
    gdt[1] = desc;
    access_byte = ConstructGdtAccessByte(true, false, false, 0);
    desc = ConstructGdtEntry(0xFFFFF, 0, access_byte, true, true);
    gdt[2] = desc;
    SetGdt(gdt, sizeof(gdt) / sizeof(GdtDesc));
}
