/******************************************************************************
 * File:       /vv4os/debug/gdb_stub.c                                        *
 * Author:     vodozhaba                                                      *
 * Created on: Jun 20, 2017                                                   *
 * Purpose:    Allows debugging VV4OS with GDB without external tools.        *
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include "arch/x86/dt/idt_x86.h"
#include "core/config.h"
#include "io/uart.h"
#include "stdlib/stdio.h"
#include "stdlib/string.h"

static const char* alphabet = "0123456789abcdef";

static char ReadDebugChar() {
#if defined(UART_DEBUGGING)
	return UartRead();
#endif
}

__attribute__((unused)) static void SendDebugChar(char c) {
#if defined(UART_DEBUGGING)
	UartSend(c);
#endif
}

__attribute__((unused)) static char* ReadPacket(char* buf, size_t size) {
	char first = ReadDebugChar();
	if(first != '$') {
		return NULL;
	}
	char c;
	uint8_t checksum = 0;
	size_t i;
	for(i = 0; (c = ReadDebugChar()) != '#'; i++) {
		if(i == size - 1) {
			return NULL;
		}
		buf[i] = c;
		checksum += c;
	}
	buf[i] = '\0';
	uint8_t nibble_1 = (size_t)(strchr(alphabet, ReadDebugChar()) - alphabet);
	uint8_t nibble_2 = (size_t)(strchr(alphabet, ReadDebugChar()) - alphabet);
	if(nibble_1 * 16 + nibble_2 == checksum) {
		return buf;
	} else {
		return NULL;
	}
}

static void Breakpoint(__attribute__((unused)) volatile InterruptedCpuState state) {
	while(true);
}

void GdbStubInit() {
#if defined(__X86__)
	X86RegisterIsrHandler(3, Breakpoint);
#endif
#if defined(WAIT_FOR_GDB)
	printf("Waiting for GDB remote connection...\n");
	__asm("int3");
#endif
}
