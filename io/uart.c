/******************************************************************************
 * File:       /vv4os/io/rs232.c                                              *
 * Author:     vodozhaba                                                      *
 * Created on: Jun 16, 2017                                                   *
 * Purpose:    Allows working with serial ports.                              *
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "io/ports.h"

#define BASE_FREQ 115200

#define PORT(n) (0x3F8 + (n))
#define DATA (PORT(0))
#define INT_ENABLE (PORT(1))
#define FREQ_DIV (PORT(0))
#define INT_IDENT_AND_FIFO_CTRL (PORT(2))
#define LINE_CONTROL (PORT(3))
#define MODEM_CONTROL (PORT(4))
#define LINE_STATUS (PORT(5))
#define MODEM_STATUS (PORT(6))
#define SCRATCH (PORT(7))

#define DATA_BITS(n) ((n) - 5)

#define PARITY_NONE 0x00
#define PARITY_ODD 0x01
#define PARITY_EVEN 0x03
#define PARITY_MARK 0x05
#define PARITY_SPACE 0x07

typedef union {
	struct {
		uint8_t data_bits : 2;
		bool more_stop_bits : 1;
		uint8_t parity : 3;
		uint8_t unused : 1;
		bool dlab : 1;
	} __attribute__((packed)) as_struct;
	uint8_t as_byte;
} LineControlReg;

typedef union {
	struct {
		bool data_avail : 1;
		bool tx_empty : 1;
		bool break_or_error : 1;
		bool status_change : 1;
		uint8_t unused : 4;
	} __attribute__((packed)) as_struct;
	uint8_t as_byte;
} IntEnableReg;

static void SetDivisor(int freq) {
	uint16_t divisor = BASE_FREQ / freq;
	LineControlReg lcr;
	lcr.as_byte = PortRead8(LINE_CONTROL);
	lcr.as_struct.dlab = true;
	PortWrite8(LINE_CONTROL, lcr.as_byte);
	PortWrite16(FREQ_DIV, divisor);
	lcr.as_struct.dlab = false;
	PortWrite8(LINE_CONTROL, lcr.as_byte);
}

void Rs232Init(int freq) {
	SetDivisor(freq);
	// 8N1 mode
	LineControlReg lcr;
	lcr.as_struct.data_bits = DATA_BITS(8); // 8 bits
	lcr.as_struct.more_stop_bits = false;
	lcr.as_struct.parity = PARITY_NONE;
	lcr.as_struct.unused = 0x00;
	lcr.as_struct.dlab = false;
	PortWrite8(LINE_CONTROL, lcr.as_byte);
	PortWrite8(INT_IDENT_AND_FIFO_CTRL, 0xC7); // FIXME: magic number
	IntEnableReg ier;
	ier.as_struct.data_avail = true;
	ier.as_struct.tx_empty = true;
	ier.as_struct.break_or_error = false;
	ier.as_struct.status_change = true;
	ier.as_struct.unused = 0;
	PortWrite8(INT_ENABLE, ier.as_byte);
}
