/******************************************************************************
 * File:       /vv4os/debug/gdb_stub.c                                        *
 * Author:     vodozhaba                                                      *
 * Created on: Jun 20, 2017                                                   *
 * Purpose:    Allows debugging VV4OS with GDB without external tools.        *
 ******************************************************************************/

#ifdef DEBUG

#include <stddef.h>
#include <stdint.h>
#include "arch/x86/dt/idt_x86.h"
#include "core/config.h"
#include "io/uart.h"
#include "stdlib/stdio.h"
#include "stdlib/stdlib.h"
#include "stdlib/string.h"

static const char* alphabet = "0123456789abcdef";
static char read_buf[GDB_PACKET_BUF_SIZE];
static char send_buf[GDB_PACKET_BUF_SIZE];

static char ReadDebugChar() {
#if defined(UART_DEBUGGING)
	return UartRead();
#endif
}

static void SendDebugChar(char c) {
#if defined(UART_DEBUGGING)
	UartSend(c);
#endif
}

static char* ReadPacket(char* buf, size_t size) {
	while(ReadDebugChar() != '$');
	char c;
	uint8_t checksum = 0;
	size_t i;
	for(i = 0; (c = ReadDebugChar()) != '#'; i++) {
		if(i == size - 1) {
			SendDebugChar('-');
			return NULL;
		}
		if(c == '}') {
			buf[i] = (c = ReadDebugChar());
		} else {
			buf[i] = c;
		}
		checksum += c;
	}
	buf[i] = '\0';
	uint8_t nibble_1 = (size_t)(strchr(alphabet, ReadDebugChar()) - alphabet);
	uint8_t nibble_2 = (size_t)(strchr(alphabet, ReadDebugChar()) - alphabet);
	if(nibble_1 * 16 + nibble_2 == checksum) {
		SendDebugChar('+');
		return buf;
	} else {
		SendDebugChar('-');
		return NULL;
	}
}

static void SendPacket(char* data) {
#if defined(GDB_MAX_SEND_ATTEMPTS)
	for(size_t a = 0; a < GDB_MAX_SEND_ATTEMPTS; a++)
#else
	while(true)
#endif
	{
		SendDebugChar('$');
		uint8_t checksum;
		char c;
		for(size_t i = 0; (c = data[i]) != '\0'; i++, checksum += c) {
			if(c == '#' || c == '$' || c == '}' || c == '*') {
				SendDebugChar('}');
			}
			SendDebugChar(c);
		}
		SendDebugChar('#');
		char nibble_1 = alphabet[checksum / 16];
		char nibble_2 = alphabet[checksum % 16];
		SendDebugChar(nibble_1);
		SendDebugChar(nibble_2);
		if(ReadDebugChar() == '+') {
			break;
		}
	}
}

static void Breakpoint(__attribute__((unused)) InterruptedCpuState* state) {
	while(true) {
		while(ReadPacket(read_buf, GDB_PACKET_BUF_SIZE) == NULL);
		printf("GDB: \"%s\"\n", read_buf);
		char* seek = read_buf;
		char cmd = *(seek++);
		switch(cmd) {
		case 'g':
			sprintf(send_buf, "%08x%08x%08x%08x%08x%08x%08x0000%04x0000%04x0000%04x0000%04x",
					state->eax, state->ecx, state->edx, state->ebx,
					state->ebp, state->esi, state->edi,
					state->ds, state->ds, state->ds, state->ds);
			break;
		}
		SendPacket(send_buf);
		printf("GDB stub: \"%s\"\n", send_buf);
	}
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

#endif
