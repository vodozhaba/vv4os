/******************************************************************************
 * File:       /vv4os/dt/desc_tables.h                                        *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 9, 2016                                                    *
 * Purpose:    Aliases descriptor table managers for different architectures. *
 ******************************************************************************/

#pragma once

void X86DescTablesInit();

#if !defined(__X86__)
#error "Cannot determine target architecture"
#endif

static inline void DescTablesInit() {
#if defined(__X86__)
    X86DescTablesInit();
#endif
}
