/******************************************************************************
 * File:       /vv4os/io/uart.h                                              *
 * Author:     vodozhaba                                                      *
 * Created on: Jun 16, 2017                                                   *
 * Purpose:    Allows working with serial ports.                              *
 ******************************************************************************/

#define UART_DEFAULT_FREQ 9600

void UartInit(int freq);
char UartRead();
void UartSend(char c);
